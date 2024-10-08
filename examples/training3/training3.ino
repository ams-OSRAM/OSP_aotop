// training3.ino - sketch for exercise 1 of the training - commands and topo
/*****************************************************************************
 * Copyright 2024 by ams OSRAM AG                                            *
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
#include <aoresult.h>       // aoresult_t
#include <aospi.h>          // aospi_init()
#include <aoosp.h>          // aoosp_init()
#include <aocmd.h>          // aocmd_register()
#include <aomw.h>           // aomw_topo_settriplet()


/*
(0) Setup
    - Flash/upload it to the ESP (use USB "CMD")
    - Connect OSP32.OUT to OSP32.IN (Loop) or use terminator

(1) Use topo in start and step
    - aomw_topo_build()
    - omw_topo_settriplet()

(2) Code walking yellow animation
    - step 1 CYN CYN YELCYN CYN YEL CYN CYN YEL CYN
    - step 2 YEL CYN CYN YELCYN CYN YEL CYN CYN YEL
    - step 3 CYN YEL CYN CYN YELCYN CYN YEL CYN CYN
    - step 4 CYN CYN YELCYN CYN YEL CYN CYN YEL CYN
    - etc

(3) Every step read colors from EEPROM (daddr 0x54)
    - Foreground color (yellow) 6 bytes at 0x10
    - Background color (cyan) 6 bytes at 0x20
    - Write helper function
        void anim_getcol(uint8_t raddr, aomw_topo_rgb_t * col )

(4) Use serial monitor to configure EEPROM
      said i2c 001 read  54 10 6
      said i2c 001 write 54 10   1F FF   1F FF   00 00 // yellow
      said i2c 001 read  54 20 6
      said i2c 001 write 54 20   00 00   1F FF   1F FF // cyan
*/


#define APP_NAME "Training3 - commands and topo"


// Library aocmd "upcalls" via aocmd_version_app() to allow the application to print its version.
void aocmd_version_app() {
  Serial.printf("%s\n", APP_NAME );
}


#define ADDR      0x001 // the address of the OSP node with I2C (SAID OUT on OSP32)
#define DADDR7     0x54 // I2C device address of the I2C EEPROM connected to SAID OUT
#define RADDR_FG   0x10 // "random" register address in the EEPROM to store 6 bytes for fg color
#define RADDR_BG   0x20 // "random" register address in the EEPROM to store 6 bytes for bg color
#define BUFSIZE       6 // R/G/B each need two bytes


int      anim_ticknum;
uint32_t anim_lastms;


static void anim_start() {
  aoresult_t result = aoosp_exec_resetinit();
  if( result!=aoresult_ok ) Serial.printf("ERROR topo_build %s\n", aoresult_to_str(result) );

  result= aoosp_send_clrerror(0x000); 
  if( result!=aoresult_ok ) Serial.printf("ERROR send_clrerror %s\n", aoresult_to_str(result) );

  result= aoosp_send_goactive(0x000);
  if( result!=aoresult_ok ) Serial.printf("ERROR send_goactive %s\n", aoresult_to_str(result) );

  anim_ticknum = 0;
  anim_lastms = millis();
  Serial.printf("anim: started\n");
}


static void anim_step() {
  if( millis()-anim_lastms < 500 ) return;

  if( anim_ticknum%2 == 0 ) {
    aoresult_t result= aoosp_send_setpwmchn(0x001, 0, 0x1FFF/*red*/, 0x1FFF/*green*/, 0x0000/*blue*/);
    if( result!=aoresult_ok ) Serial.printf("ERROR send_setpwmchn %s\n", aoresult_to_str(result) );
  } else {
    aoresult_t result= aoosp_send_setpwmchn(0x001, 0, 0x0000/*red*/, 0x1FFF/*green*/, 0x1FFF/*blue*/);
    if( result!=aoresult_ok ) Serial.printf("ERROR send_setpwmchn %s\n", aoresult_to_str(result) );
  }
  anim_ticknum++;
  anim_lastms= millis();
}


void setup() {
  Serial.begin(115200); delay(1000);
  Serial.printf("\n\n%s\n",APP_NAME);

  aospi_init();
  aoosp_init();
  aocmd_init();
  aomw_init();
  aocmd_register(); // register all std cmd's
  aomw_topo_cmd_register();
  anim_start();
  Serial.printf("\n");

  aocmd_file_bootcmd_exec_on_por(); 
  Serial.printf( "Type 'help' for help\n" );
  aocmd_cint_prompt();
}


void loop() {
  aocmd_cint_pollserial();
  anim_step();
}

