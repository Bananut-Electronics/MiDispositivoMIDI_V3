/********************************************************************
 *                       MiDispositivoMIDI V3                       *
 ********************************************************************
 * 2018 BanaNut Electronics                                         *
 * bananutelectronics@gmail.com                                     *
 *                                                                  *
 * See license and documentation in the repository.                 *
 * https://github.com/bananut-electronics                           *
 *                                                                  *
 * Code:     MDM_V3_Library_PotentiometerExtension.ino              *
 * Hardware: One MiDispositivoMIDI V3 4x4 and one 4xe potentiometer *                          
 *           extension.                                             *
 * Brief:    This code configures an extension board with .         *
 *           potentiometers                                         *
 *                                                                  *
 ********************************************************************/
  
// Include the library
#include <MiDispositivoMIDI_V3.h>

// Create a mdm with 4 pages and only 1 expansion
BoardRoles bRoles[2] = {TOUCHPAD, POTENTIOMETER};
MiDispositivoMIDI_V3 mdm = MiDispositivoMIDI_V3(TWO_DEVICES_4X8, bRoles);

void setup() {

  Serial.begin(9600);

  // Configure the colors when the LED is not pressed
  uint8_t offColors[3] = {255, 0, 0};
  mdm.setOffColors( offColors );

  // Configure the colors when the LED is pressed
  uint8_t onColors[3] = {0, 255, 0};
  mdm.setOnColors( onColors );
}

void loop() {

  // Run the loop. This will read all buttons and
  // execute different actions, such as light up the
  // pressed LEDs, and send a MIDI noteOn message.
  mdm.loop();
}
