/********************************************************************
 *                       MiDispositivoMIDI V3                       *
 ********************************************************************
 * 2018 BanaNut Electronics                                         *
 * bananutelectronics@gmail.com                                     *
 *                                                                  *
 * See license and documentation in the repository.                 *
 * https://github.com/bananut-electronics                           *
 *                                                                  *
 * Code:     MDM_V3_Library_ConfigureColors.ino                     *
 * Hardware: One MiDispositivoMIDI V3 4x4                           *
 * Brief:    This code allows you to configure which color each     *
 *           LED will have when pressed and not pressed. For        *
 *           example, if you set the onColor to RED, when you press *
 *           that button, that LED will light up in RED. Note that  *
 *           the colors is given by a RGB array, with [0..255]      *
 *                                                                  *
 ********************************************************************/

// Include the library
#include <MiDispositivoMIDI_V3.h>

// Create a mdm with 4 pages and only 1 expansion
MiDispositivoMIDI_V3 mdm = MiDispositivoMIDI_V3(4, 1);

void setup() {

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
