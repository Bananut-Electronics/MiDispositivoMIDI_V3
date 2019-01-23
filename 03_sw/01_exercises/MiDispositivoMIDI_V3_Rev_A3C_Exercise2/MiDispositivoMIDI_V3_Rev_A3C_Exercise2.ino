/*
    MiDispositivoMIDI V3 Rev.A3C
    --------EXERCISE 2--------

    Goal: Create animations with the LEDs, using the function delay() and the for
    structure.

    Exercise proposed: Modify the i of the for structure, seeing how it changes the
    animation.
    
    Hardware: You will need a MiDispositivoMIDI V3 Rev A3C, without any expansions.
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

  // This is a for bucle. This structure will run while
  // i is under 16. The value of i will be the previous plus
  // one. So it will takes values from 0 to 15.
  for (int i = 0; i < 16; i++) {
    // Light up the i LED. First 0, then 1, then 2, and so on.
    ledPad[i][0] = 0;
    ledPad[i][1] = 255;
    ledPad[i][2] = 0;

    // Waits 500 ms.
    delay(500);

    // After a change, it is very important to update
    updateLEDs();
  }

  // Once finished, turn off each LED
  for (int i = 0; i < 16; i++) {
    ledPad[i][0] = 0;
    ledPad[i][1] = 0;
    ledPad[i][2] = 0;
  }

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

