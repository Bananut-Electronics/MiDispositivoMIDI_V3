/********************************************************************
 *                       MiDispositivoMIDI V3                       *
 ********************************************************************
 * 2018 BanaNut Electronics                                         *
 * bananutelectronics@gmail.com                                     *
 *                                                                  *
 * See license and documentation in the repository.                 *
 * https://github.com/bananut-electronics                           *
 *                                                                  *
 * Code:     MDM_V3_NoLibrary_Exercise1.ino                         *
 * Hardware: One MiDispositivoMIDI V3 (4x4)                         *
 * Brief:    This is a set of 5 exercises that were thought to      *
 *           teach you that basics of the device works. Note that   *
 *           it does not use the new C++ Arduino library.           *
 *                                                                  *
 *                                                                  *
 ********************************************************************/

/*
 * Goal: Handle RGB LEDs, understanding that each button have three colors,
 * red, green and blue. Each color can be lighted up with an intensity of
 * 0 to 255. Also understand that LEDs are adressed from 0 to 15, beeing the
 * first 0 (element 1,1, top left corner) and 15 the last (element 4,4, bottom righ corner)
 * 
 * Exercise proposed: Modify the colors and LEDs.
 */
// Library Import
#include <Adafruit_NeoPixel.h>

// Digital Pin Definition
#define LEDPAD_PIN  2
#define LEDPAD_NUM 16
Adafruit_NeoPixel ledPadStripe = Adafruit_NeoPixel(LEDPAD_NUM, LEDPAD_PIN, NEO_RGB + NEO_KHZ800);

// Variable declaration
uint8_t ledPad[LEDPAD_NUM][3] = {{0}};

void setup() {
  ledPadStripe.begin();
  ledPadStripe.show();
}

void loop() {
  // Light up the first LED (top left) green
  ledPad[0][0] = 0;    //0 corresponds to the intensity of red color
  ledPad[0][1] = 255;  //1 corresponds to the intensity of green color
  ledPad[0][2] = 0;    //2 corresponds to the intensity of blue color
  // Note that first number indicates the LED (from 0 to 15)
  // The second number indicates the color (0 red, 1 green, 2 blue)

  // After any change in the ledPad matrix, the function updateLEDS()
  // shows the changes in the controler.
  updateLEDs();
}

/* Function updateLEDPADs()
    Updates the LED PAD values
*/
void updateLEDs() {
  for (int i = 0; i < LEDPAD_NUM; i++) { // PAD Button Iterator
    ledPadStripe.setPixelColor(i, ledPad[i][1], ledPad[i][0], ledPad[i][2]); // Heavy Stuff
  }
  ledPadStripe.show(); // Update LEDs
}
