/*
    MiDispositivoMIDI V3 Rev.A3C
    --------EXERCISE 5--------

    Goal: Use the MIDI controller with a DAW (computer audio program) and modify some basic
    parameters, to undertand how the code works.

    Proposed exercises: Modify colors, notes and velocity.

    Hardware: You will need a MiDispositivoMIDI V3 Rev A3C, without any expansions.

    Note: Check the code section "this following section can be changed
*/
// Library Import
#include <Adafruit_NeoPixel.h>
#include <MIDIUSB.h>
#include <pitchToNote.h>
#include <frequencyToNote.h>

// Digital Pin Definition
#define LEDPAD_PIN  2
#define SW_0_PIN    7
#define SW_1_PIN    8

// Analog Pin Definition
#define PADCH_0_PIN 0
#define PADCH_1_PIN 1
#define PADCH_2_PIN 2
#define PADCH_3_PIN 3

#define MUX_B0_PIN 6
#define MUX_B1_PIN 5
#define MUX_B2_PIN 4
#define MUX_B3_PIN 3

#define MUX_DELAY 50

// Defined values
#define LEDPAD_NUM  16   //LEDs per PAD
#define LEDPAD_EXT  1    //Number of LED PAD Extension Boards [1:4]
#define PAD_THR     500  //PAD Value Threhold for Action
#define MAX_PAGE    4    //Maximum number of pages.
#define MIDI_CHANNEL 0   //Default MIDI channel

// This following section can be change by the user. Further info in doc.
// ---------------------------------------------------------------------//
// RGB LED color Definition                                             //
#define OFF_R  50  // OFF PAD Color Red    [0:255]                      //
#define OFF_G  50  // OFF PAD Color Green  [0:255]                      //
#define OFF_B  50  // OFF PAD Color Blue   [0:255]                      //
                                                                        //
#define ON_R   0   // ON PAD Color Red     [0:255]                      //
#define ON_G   0   // ON PAD Color Green   [0:255]                      //
#define ON_B   255 // ON PAD Color Blue    [0:255]                      //
                                                                        //
// MIDI notes definition                                                //
uint8_t notes[4][4] = {                                                 //
{32,33,34,35},                                                          //
{36,37,38,39},                                                          //
{40,41,42,43},                                                          //
{44,45,46,47},                                                          //
};                                                                      //
uint8_t velocity[4][4] = {                                              //
{127,127,127,127},                                                      //
{127,127,127,127},                                                      //
{127,127,127,127},                                                      //
{127,127,127,127},                                                      //
};                                                                      //
// ---------------------------------------------------------------------//

// Global Variables
Adafruit_NeoPixel ledPadStripe = Adafruit_NeoPixel(16, LEDPAD_PIN, NEO_RGB + NEO_KHZ800);

int ledPad[4][16][3] = {{{0}}}; // LED PAD RGB Color Values
int padRead[4][16] = {{0}};     // PAD Results

int lastSW_1    = 0;
int lastSW_2    = 0;
int lastRead[4][16] = {{0}};

int padPin[4] = {PADCH_0_PIN,PADCH_1_PIN,PADCH_2_PIN,PADCH_3_PIN}; // PAD Channel Pins
int muxPin[4] = {MUX_B0_PIN,MUX_B1_PIN,MUX_B2_PIN,MUX_B3_PIN}; // MUX Selector Pins

int ON_RGB[3]  = {ON_R, ON_G, ON_B}; // ON LED PAD Color
int OFF_RGB[3] = {OFF_R, OFF_G, OFF_B}; // OFF LED PAD Color

uint8_t currentPage = 0; // Current page [1:MAX_PAGE]

void setup() {
  ledPadStripe.begin();
  ledPadStripe.show();
  
  // Set Pin Modes
  pinMode(MUX_B0_PIN, OUTPUT);
  pinMode(MUX_B1_PIN, OUTPUT);
  pinMode(MUX_B2_PIN, OUTPUT);
  pinMode(MUX_B3_PIN, OUTPUT);
  pinMode(SW_0_PIN, INPUT);
  pinMode(SW_1_PIN, INPUT);

  // Welcome animation
  colorWipe(ledPadStripe.Color(255, 0, 0), 50);
  colorWipe(ledPadStripe.Color(0, 255, 0), 50);
  colorWipe(ledPadStripe.Color(0, 0, 255), 50);
  initializeLEDs();
}

void loop() {
  readPADs();   // Read pressed pads
  updateLEDs(); // Update LEDs according to pressed pads
  readPage();   // Read page (lateral buttons)
}

/*
 * Function: noteOn()
 * Description: Sends a noteOn MIDI message.
 * Input:
 *  -Channel: MIDI Channel. Recommended 0.
 *  -Pith: Note value, from 0 to 255
 *  -Velocity: Note intensity, form 0 to 255
 * Output:
 *  -Void.
 */
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, uint8_t(0x90 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

/*
 * Function: noteOff()
 * Description: Sends a noteOff MIDI message.
 * Input:
 *  -Channel: MIDI Channel. Recommended 0.
 *  -Pith: Note value, from 0 to 255
 *  -Velocity: Note intensity, form 0 to 255
 * Output:
 *  -Void.
 */
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, uint8_t(0x80 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

/*
 * Function: readPADS()
 * Description: Read pushed pads and store it into a matrix.
 * Input:
 *  -None
 * Output:
 *  -Void.
 */
void readPADs(){
  for(int i=0; i<LEDPAD_NUM; i++){ // PAD Button Iterator
    // Mux Setup
    for(int h=0; h<4; h++){
    digitalWrite(muxPin[h], ((i>>(3-h)) & 0x01)); //MUX SetUp
    }
    delayMicroseconds(MUX_DELAY);
    // Pad Read
    for(int j=0; j<LEDPAD_EXT; j++){ // PAD Channel Iterator
      padRead[j][i] = analogRead(padPin[j]); //Assign Reads to PAD matrix
      if (padRead[j][i] < PAD_THR){ //If pushed
        if (lastRead[j][i] == 0){ //If it was not send yet
          for(int n=0; n<2; n++){ // LED PAD ON Color
             ledPad[j][i][n] = ON_RGB[n];
          }
          lastRead[j][i] = 1;
          padRead[j][i]  = 0;
          noteOn(MIDI_CHANNEL, notes[j][i]+(currentPage)*16, velocity[j][i]);
          MidiUSB.flush();
        }
      }else{
        if (lastRead[j][i] == 1){
          for(int n=0; n<3; n++){ // LED PAD OFF Color
             ledPad[j][i][n] = OFF_RGB[n];
          }
          lastRead[j][i] = 0;
          noteOff(MIDI_CHANNEL, notes[j][i]+(currentPage)*16, 0);
          MidiUSB.flush();
        }
      }
    }  
  }
}

/*
 * Function: updateLEDs()
 * Description: Light up each LED with the corresponding value
 * Input:
 *  -None
 * Output:
 *  -Void.
 */
  void updateLEDs(){
    for(int i=0; i<LEDPAD_NUM; i++){ // PAD Button Iterator
      for(int j=0; j<LEDPAD_EXT; j++){ // PAD Channel Iterator
        ledPadStripe.setPixelColor((i+(j*LEDPAD_NUM)),ledPad[j][i][1],ledPad[j][i][0],ledPad[j][i][2]); // Heavy Stuff
      }
    }
    //delay(1);
    ledPadStripe.show(); // Update LEDs
  }

/*
 * Function: readPage()
 * Description: Read if page up or page down button was pressed.
 * Input:
 *  -None
 * Output:
 *  -Void.
 */
void readPage(){
  uint8_t readSW_1 = digitalRead(SW_0_PIN);
  uint8_t readSW_2 = digitalRead(SW_1_PIN);
  if ((readSW_1 == HIGH) && (readSW_1 != lastSW_1)) {
    if (currentPage < (MAX_PAGE - 1)) {
      currentPage++;
    }
  }
  if ((readSW_2 == HIGH) && (readSW_2 != lastSW_2)) {
    if (currentPage > 0) {
      currentPage--;
    }
  }
  lastSW_1 = readSW_1;
  lastSW_2 = readSW_2;
}

/*
 * Function: initializeLEDs()
 * Description: Initialize all LEDs to the OFF color
 * Input:
 *  -None
 * Output:
 *  -Void.
 */
void initializeLEDs(){
  for(int i=0; i<LEDPAD_NUM; i++){ // PAD Button Iterator
    for(int j=0; j<LEDPAD_EXT; j++){ // PAD Channel Iterator
      for(int n=0; n<3; n++){ // LED PAD OFF Color
         ledPad[j][i][n] = OFF_RGB[n];
      }
    }
  }
  ledPadStripe.show(); // Update LEDs
}

/*
 * Function: colorWipe()
 * Description: Funny LED animation to welcome the user
 * Input:
 *  -Color object RGB
 *  -Wait time in ms
 * Output:
 *  -Void.
 */
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<ledPadStripe.numPixels(); i++) {
    ledPadStripe.setPixelColor(i, c);
    ledPadStripe.show();
    delay(wait);
  }
}

