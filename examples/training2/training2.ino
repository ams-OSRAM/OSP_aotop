// training2.ino - sketch for exercise 2 of the training - step eeprom
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
#include <aospi.h>
#include <aoosp.h>


/*
(0) Setup
    - Flash/upload it to the ESP (use USB "CMD")
    - Connect OSP32.OUT to OSP32.IN (Loop)

(1) The sketch should step location 40 in EEPROM on the OSP32 board by one
    Complete the sketch below
    - Power the I2C bus
    - Read the current register value from the EEPROM
    - Show value, step value
    - Write the new register value to the EEPROM

(2) Open serial monitor, and press reset (or power cycle)

*/


#define ADDR    0x001 // the address of the OSP node with I2C (SAID OUT on OSP32)
#define DADDR7   0x54 // I2C device address of the I2C EEPROM connected to SAID OUT
#define RADDR    0x40 // some "random" register address in the EEPROM


static void i2ceeprom( ) {
  #define    BUFSIZE 1
  uint8_t    buf[BUFSIZE];
  aoresult_t result;
  uint16_t   last;

  // Reset all nodes (broadcast); all "off"; they also lose their address
  result= aoosp_exec_resetinit(&last); 
  Serial.printf("resetinit() %s %d\n", aoresult_to_str(result), last );
  if( last!=2 ) Serial.printf("ERROR: unexpected topology\n");

  // Power the I2C bus
  ...

  // Read the current register value from the EEPROM
  ...

  // Show value, step value
  Serial.printf("  value %02x\n",buf[0]);
  buf[0]++;

  // Write the new register value to the EEPROM
  ...
}


void setup() {
  Serial.begin(115200); delay(1000);
  Serial.printf("\n\ntraining2.ino - step eeprom\n");

  aospi_init();
  aoosp_init();

  // aoosp_loglevel_set( aoosp_loglevel_tele );
  i2ceeprom();
  // Serial.printf("tx %d rx %d\n", aospi_txcount_get(), aospi_rxcount_get() );
}


void loop() {
  delay(1000);
}

