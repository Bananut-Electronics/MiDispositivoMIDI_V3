/*
    MiDispositivoMIDI V3 Rev.A3C
    --------EXERCISE 3--------

    Goal: Understand digitalRead() function, if structure, and the use of the Serial port.

    Proposed exercises: Answer the following questions:
      -Why the sentence Button x is pushed appears so many times?
      -How would you display when button is not pushed?. Hint: else.

    Hardware: You will need a MiDispositivoMIDI V3 Rev A3C, without any expansions.
*/
// Library Import
#include <Adafruit_NeoPixel.h>

// Digital Pin Definition
#define LEDPAD_PIN  2
#define LEDPAD_NUM 16

// Switches pins
#define SW_0_PIN 7
#define SW_1_PIN 8
Adafruit_NeoPixel ledPadStripe = Adafruit_NeoPixel(LEDPAD_NUM, LEDPAD_PIN, NEO_RGB + NEO_KHZ800);

// Variable declaration
uint8_t ledPad[LEDPAD_NUM][3] = {{0}};

void setup() {
  ledPadStripe.begin();
  ledPadStripe.show();

  // Init the serial com
  Serial.begin(9600);
  
  // Set switches as input
  pinMode(SW_0_PIN, INPUT);
  pinMode(SW_1_PIN, INPUT);
}

void loop() {

  // Read the value of the button
  // 0 means not pushed. 1 means pushed
  uint8_t switch0 = digitalRead(SW_0_PIN);
  uint8_t switch1 = digitalRead(SW_1_PIN);

  // If the value is 1, means pushed
  // Note that == is not the same as =
  if (switch0 == 1 ){
    // Write in the Serial port
    Serial.println("Button 0 is pushed");  
  }

  // Same for the other button
  if (switch1 == 1){
    Serial.println("Button 1 is pushed");
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

