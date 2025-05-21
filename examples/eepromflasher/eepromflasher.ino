// eepromflasher.ino - tool to write animation scripts to EEPROMs, for use in aoapps_aniscript
/*****************************************************************************
 * Copyright 2024,2025 by ams OSRAM AG                                       *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************/
#include <aospi.h>
#include <aoosp.h>
#include <aomw.h>


// Macro magic (to join and to stringify)
#define JOIN(x, y)      x ## y
#define _BYTES(name)    JOIN(name, _bytes)
#define STR2(s)         #s
#define STR(s)          STR2(s)


/*
DESCRIPTION
This is a simple tool to write animation scripts, e.g. from 
  ...\OSP_aomw\src\aomw_tscript.cpp
to an EEPROM. 
Such EEPROMs are used by one of the standard apps, namely
  ...\OSP_aoapps\src\aoapps_aniscript.cpp
to read the script from the EEPROM and play the animation on the OSP chain.

An EEPROM needs to be flashed with an animation script. Below in this sketch 
several aspects must be configured before flashing an EEPROM
 - ADDR    is the OSP address of the (SAID) node that has an I2C bridge
 - DADDR7  is the I2C address of the EEPROM device connected to that bridge
 - SCRIPT  is the (name of the function returning the) array of instructions
 - WRITE   is either 1 to perform a write-and-compare or 0 for only compare

There are typically three places where we find an EEPROM. One is on the
OSP32 board. This one is just there for I2C trials; it is not used by
the aniscript app. Secondly, there is an EEPROM on the SAIDbasic board.
This EEPROM is used by the aniscript app if there is no external EEPROM.
Thirdly, the OSP32 and the SAIDbasic board both have an I2C connector
where we can plug-in an external I2C EEPROM stick. This is typically the 
EEPROM that the aniscript app prefers to use. 
To distinguish these three kinds of EEPROMs, they each use a different
I2C device address; see AOMW_EEPROM_XXX_DADDR7 below. DADDR7 stands for I2C
device address (7 bits, so without the read/write flag).

HARDWARE
This sketch runs on the OSP32 board. It has a built-in EEPROM. Typically a 
SAIDbasic board is connected; it also has a built-in EEPROM. Finally, an 
I2C EEPROM stick could be plugged into the I2C slot of either OSP32 or 
SAIDbasic. The output below is from the EEPROM on the SAIDbasic, directly
connected to the OSP32 board.
In Arduino select board "ESP32S3 Dev Module".

BEHAVIOR
Nothing visible 

OUTPUT
Welcome to eepromflasher
spi: init
osp: init
mw: init

flash: ADDR 005 has I2C bridge
flash: DADDR7 50 can be accessed
flash: SCRIPT aomw_tscript_rainbow (214 bytes)
flash: comparing
flash: SUCCESS

Octal dump
00:  007000 007111 007222 007333 007444 007555 007666 007777
10:  011766 011755 011744 011733 011722 011711 011700 022776
20:  022775 022774 022773 022772 022771 022770 033676 033575
30:  033474 033373 033272 033171 033070 044677 044577 044477
40:  044377 044277 044177 044077 055667 055557 055447 055337
50:  055227 055117 055007 066767 066757 066747 066737 066727
60:  066717 066707 000666 000555 000444 000333 000222 000111
70:  000000 011600 011500 011400 011300 011200 011100 011000
80:  022660 022550 022440 022330 022220 022110 022000 033060
90:  033050 033040 033030 033020 033010 033000 044066 044055
a0:  044044 044033 044022 044011 044000 055006 055005 055004
b0:  055003 055002 055001 055000 066606 066505 066404 066303
c0:  066202 066101 066000 077666 077555 077444 077333 077222
d0:  077111 077000 070000 177777 177777 177777 177777 177777
e0:  177777 177777 177777 177777 177777 177777 177777 177777
f0:  177777 177777 177777 177777 177777 177777 177777 177777*/


// === User script ==========================================================


// Option to make one's own animation script
static const uint16_t tscript_user_[] = {
// Octal 0, 0 or 1 for with previous, 0..7 for lower index, 0..7 for upper index, 0..7 for red, 0..7 for green and 0..7 for blue
//oPLURGB

  // frame 0 : 01234567
  0007777, // wwwwwwww
  0111700, // -r------
  0156070, // -----gg-

  // frame 1 : 01234567
  0007777, // wwwwwwww
  0112700, // -rr-----
  0166070, // ------g-

  // frame 2 : End
  0070000,
};
const uint16_t * tscript_user()       { return        tscript_user_ ; }
int              tscript_user_bytes() { return sizeof(tscript_user_); }


// === Config ===============================================================
// 1) Define the following four macros to configure the eepromflasher.
// 2) Run this application (in Arduino select board "ESP32S3 Dev Module").
// 3) Check Serial Monitor for (compare) results

// ADDR is the OSP address of the (SAID) node that has an I2C bridge
//#define ADDR      1  // The first SAID on OSP32 has an I2C bridge wired to a built-in EEPROM and  an I2C header
  #define ADDR      5  // When SAIDbasic is connected to OSP32, SAID5 is wired to a built-in EEPROM and an I2C header

// DADDR7 is the I2C device address of the EEPROM connected to that bridge
//#define DADDR7    AOMW_EEPROM_DADDR7_STICK     // I2C address EEPROM on "I2C EEPROM sticks" (0x51)
//#define DADDR7    AOMW_EEPROM_DADDR7_OSP32     // I2C address EEPROM on OSP32 board (0x54)
  #define DADDR7    AOMW_EEPROM_DADDR7_SAIDBASIC // I2C address EEPROM on SAIDbasic board (0x50)

// SCRIPT is the (name of the function returning) an array of instructions
// Due to macro magic, below #define lines cannot have comments appended. 
//   aomw_tscript_rainbow       is for SAIDbasic EEPROM
//   aomw_tscript_bouncingblock is on EEPROM stick 1
//   aomw_tscript_colormix      is on EEPROM stick 2
//   aomw_tscript_heartbeat     is in the aniscript module (default when no EEPROM)
//   tscript_user               is for own experiments (see top of this file)
  #define SCRIPT    aomw_tscript_rainbow
//#define SCRIPT    aomw_tscript_bouncingblock
//#define SCRIPT    aomw_tscript_colormix
//#define SCRIPT    aomw_tscript_heartbeat
//#define SCRIPT    tscript_user

// WRITE is either 1 to perform a write-and-compare or 0 for only compare
//#define WRITE     1  // Write to EEPROM and read to compare
  #define WRITE     0  // No write, compare only
// ==========================================================================


// Lazy way of error handling
#define PRINT_ERROR(msg) do { if( result!=aoresult_ok ) { Serial.printf("ERROR %s (in %s)\n",aoresult_to_str(result),msg); return; } } while(0)


void eeprom_flash() {
  // (1) initialize the OSP chain
  aoresult_t result;
  result = aoosp_exec_resetinit(); PRINT_ERROR("resetinit");

  // (2patch) In case the SAID does not have the I2C_BRIDGE_EN, we could try to override this in RAM
  // result= aoosp_exec_i2cenable_set(ADDR,1); PRINT_ERROR("i2cenable_set");

  // (2) check if SAID has I2C bridge
  int enable;
  result= aoosp_exec_i2cenable_get(ADDR, &enable); PRINT_ERROR("i2cenable_get");
  if( !enable ) result= aoresult_dev_noi2cbridge; PRINT_ERROR("i2cenable_get");
  Serial.printf("flash: ADDR %03X has I2C bridge\n", ADDR);

  // (3) power the I2C bridge in a SAID
  result= aoosp_exec_i2cpower(ADDR); PRINT_ERROR("i2cpower");

  // (4) check whether there is an EEPROM
  result= aomw_eeprom_present(ADDR,DADDR7); PRINT_ERROR("eeprom_present");
  Serial.printf("flash: DADDR7 %02X can be accessed\n", DADDR7 );

  // (5) get the data
  const uint8_t * buf = (const uint8_t *)SCRIPT();
  int count = _BYTES(SCRIPT)();
  Serial.printf("flash: SCRIPT %s (%d bytes)\n", STR(SCRIPT), count );

  // (6) write data to the EEPROM
  if( WRITE ) {
    Serial.printf("flash: writing\n" );
    result= aomw_eeprom_write(ADDR, DADDR7, 0, buf, count ); PRINT_ERROR("eeprom_compare");  
  }

  // (7) compare data with EEPROM
  Serial.printf("flash: comparing\n" );
  result= aomw_eeprom_compare(ADDR, DADDR7, 0, buf, count ); PRINT_ERROR("eeprom_compare");

  Serial.printf("flash: SUCCESS\n");
}


void eeprom_dump() {
  uint8_t buf[256];
  aoresult_t result= aomw_eeprom_read(ADDR, DADDR7, 0x00, buf, 256 ); PRINT_ERROR("eeprom_read");
  Serial.printf("\nOctal dump\n");
  for( int i=0; i<256; i+=16 ) {
    Serial.printf("%02x: ", i);
    for( int j=i; j<i+16; j+=2 ) Serial.printf(" %06o", buf[j] + buf[j+1]*256);
    Serial.printf("\n");
  }
}


void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nWelcome to eepromflasher\n");

  aospi_init();
  aoosp_init();
  aomw_init();
  Serial.printf("\n");

  eeprom_flash();
  eeprom_dump();
}


void loop() {
  delay(5000);
}

