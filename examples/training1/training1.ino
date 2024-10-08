// training1.ino - sketch for exercise 1 of the training - green/magenta/green
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
(0) Compile and flash
    - Flash/upload it to the ESP (use USB "CMD")
    - Connect the SAIDbasic board in BiDir mode
    - Don’t forget the terminator
    Check that this sketchs turns the first RGBI on the SAIDbasic board to magenta

(1) Check the logging
    - Uncomment aoosp_loglevel_set() 
    - and/or aospi_txcount_get()
    Make sense of the logs

(2) Switch system to Loop mode
    - Replace terminator by cable
    - Use aoosp_send_initloop()
    - Use aospi_dirmux_set_loop()
    Check the Serial output for last
    Check the bidir/loop signaling lEDs

(3) Make the led to the left and right of the magenta RGBI turn green
    - Use aoosp_send_setpwmchn() – pass correct channel
    - Use aoosp_send_goactive() – don’t forget to clear error first
*/


//     001  | 002  003  004  005  006  007  008  |
// MCU SAID | SAID SAID RGBI SAID RGBI RGBI RGBI | terminator
//    OSP32 | SAIDbasic                          | 


static void anim( ) {
  aoresult_t result;
  uint16_t   last;
  uint8_t    temp;
  uint8_t    stat;
  Serial.printf("\n");

  // Reset all nodes (broadcast) in the chain (all "off"; they also lose their address).
  result= aoosp_send_reset(0x000); delayMicroseconds(150);
  Serial.printf("reset(000) %s\n", aoresult_to_str(result) );

  // Assign an address to each node (starting from 1, serialcast).
  aospi_dirmux_set_bidir();
  result= aoosp_send_initbidir(0x001, &last, &temp, &stat);
  Serial.printf("initbidir(001) %s last %03X\n", aoresult_to_str(result), last );

  // Switch the state node 004 (unicast) to active (allowing to switch on LEDs).
  result= aoosp_send_goactive(0x004);
  Serial.printf("goactive(004) %s\n", aoresult_to_str(result) );

  // Set three PWM values of RGBI at 004 (unicast) to dim magenta (all 3 in nightmode)
  result= aoosp_send_setpwm(0x004, 0x04FF/*red*/, 0x0000/*green*/, 0x08FF/*blue*/, 0b000);
  Serial.printf("setpwm(004,magenta) %s\n", aoresult_to_str(result) );
}


void setup() {
  Serial.begin(115200);
  Serial.printf("\n\ntraining1.ino - green/magenta/green\n");

  aospi_init();
  aoosp_init();

  // aoosp_loglevel_set( aoosp_loglevel_tele );
  anim();
  // Serial.printf("tx %d rx %d\n", aospi_txcount_get(), aospi_rxcount_get() );
}


void loop() {
  delay(1000);
}

