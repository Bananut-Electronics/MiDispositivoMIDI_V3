/********************************************************************
 *                       MiDispositivoMIDI V3                       *
 ********************************************************************
 * 2018 BanaNut Electronics                                         *
 * bananutelectronics@gmail.com                                     *
 *                                                                  *
 * See license and documentation in the repository.                 *
 * https://github.com/bananut-electronics                           *
 *                                                                  *
 * Code:     MDM_V3_NoLibrary_StepSequencer.ino                     *
 * Hardware: Four MiDispositivoMIDI V3 connected in a 4x16 config   *
 * Brief:    This code uses four 4x4 MDM V3 connected in a 4x16     *
 *           configuration to create an step sequencer. If you dont *
 *           know what that is, we invite you to read about it on   *
 *           the internet. You can also check this video we made    *
 *           with this code here:                                   *
 *           https://www.youtube.com/watch?v=bYyvVDn-gKs            *
 *                                                                  *
 ********************************************************************/

// Library Import
#include <Adafruit_NeoPixel.h>
#include <MIDIUSB.h>

// Digital Pin Definition
#define LEDPAD_PIN  2
#define MUX_B0_PIN  3
#define MUX_B1_PIN  4
#define MUX_B2_PIN  5
#define MUX_B3_PIN  6
#define SW_0_PIN    7
#define SW_1_PIN    8

// Analog Pin Definition
#define POT_0_PIN   4
#define POT_1_PIN   5
#define PADCH_0_PIN 0
#define PADCH_1_PIN 1
#define PADCH_2_PIN 2
#define PADCH_3_PIN 3

// Defined values
#define LEDPAD_EXT  1    //Number of LED PAD Extension Boards [1:4]
#define LEDPAD_NUM  16   //LEDs per PAD
#define PAD_THR     500  //PAD Value Threhold for Action
#define MAX_PAGE    4    //Maximum number of pages. Must be multiple of 4.

// Pad y Multiplexers Pins
uint8_t padPin[4] = {3, 2, 1, 0}; // PAD Channel Pins
uint8_t muxPin[4] = {6, 5, 4, 3}; // MUX Selector Pins

// Global Variables
Adafruit_NeoPixel ledPadStripe = Adafruit_NeoPixel(LEDPAD_NUM*LEDPAD_EXT, LEDPAD_PIN, NEO_RGB + NEO_KHZ800);

// Pages and notes
uint8_t lastSW_1    = 0;
uint8_t lastSW_2    = 0;
uint8_t currentPage = 0; // Current page [1:MAX_PAGE]
uint8_t eightNote   = 0;
uint8_t contador    = 0;
uint8_t negra       = 1;

// Leds and buttons
int padRead[16] = {0};                     // PAD Results. Analog value
uint8_t ledPad[16][3][MAX_PAGE] = {{{0}}}; // LED PAD RGB Color Values
uint8_t lastRead[16][MAX_PAGE] = {{0}};    // Last value read. Digital value [0:1]
uint8_t stepStatus[16][MAX_PAGE] = {{0}};  // Status of each note [0:1]

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  // Initialize led strip
  ledPadStripe.begin();
  ledPadStripe.show();
  
  // Set Pin Modes
  pinMode(MUX_B0_PIN, OUTPUT);
  pinMode(MUX_B1_PIN, OUTPUT);
  pinMode(MUX_B2_PIN, OUTPUT);
  pinMode(MUX_B3_PIN, OUTPUT);
  pinMode(SW_0_PIN, INPUT);
  pinMode(SW_1_PIN, INPUT);

  // Generate seed for random numbers (may be unused)
  randomSeed(200);
}

void loop() {
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  if (rx.byte1 == 0xF8) {
    if (contador % 24 == 0) {
      Serial.println(negra);
      negra++;
      timerIsr();
      contador = 0;
      if (negra == 5) {
        negra = 1;
      }
    }
    contador++;
  }
  if (rx.byte1 == 0xFC) {
    contador = 0;
    negra = 1;
  }

  readPADs();
  updateLEDs();
  readPage();
}

/*
 * 
 */
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

/*
 * 
 */
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

/* Function void readPADs()
    Reads the PADs through the MUX.
    Reads a number of LEDPAD_EXT Channels starting from 0
*/
void readPADs() {
  for (int i = 0; i < LEDPAD_NUM; i++) { // PAD Button Iterator
    for (int h = 0; h < 4; h++) {
      digitalWrite(muxPin[h], ((i >> (3 - h)) & 0x01)); //MUX SetUp

      delay(1);
    }
    //delay(1); // Time por MUX SetUp
    //delayMicroseconds(1);
    for (int j = 0; j < LEDPAD_EXT; j++) { // PAD Channel Iterator
      padRead[i] = analogRead(padPin[j]); //Assign Reads to PAD matrix
      if (padRead[i] < PAD_THR) {
        if (lastRead[i][currentPage] == 0) {
          stepStatus[i][currentPage] = !(stepStatus[i][currentPage]);
          if (stepStatus[i][currentPage] == 1) {
            ledPad[i][0][currentPage] = 255;
            ledPad[i][1][currentPage] = 0;
            ledPad[i][2][currentPage] = 0;
          } else {
            ledPad[i][0][currentPage] = 0;
            ledPad[i][1][currentPage] = 0;
            ledPad[i][2][currentPage] = 0;
          }
          lastRead[i][currentPage] = 1;
          padRead[i] = 0;
          MidiUSB.flush();
        }
      } else {
        if (lastRead[i][currentPage] == 1) {
          lastRead[i][currentPage] = 0;
        }
      }
    }
  }
}

/* Function updateLEDPADs()
    Updates the LED PAD values
*/
void updateLEDs() {
  for (int i = 0; i < LEDPAD_NUM; i++) { // PAD Button Iterator
    for (int j = 0; j < LEDPAD_EXT; j++) { // PAD Channel Iterator
      ledPadStripe.setPixelColor((i + (j * LEDPAD_NUM)), ledPad[i][0][currentPage], ledPad[i][1][currentPage], ledPad[i][2][currentPage]); // Heavy Stuff
    }
  }
  ledPadStripe.show(); // Update LEDs
}

void readPage() {
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
This is a bit experimental and MUST be refactored.
*/
void timerIsr()
{
  Serial.println("Tick");
  uint8_t page = 0;
  if (eightNote < 15) {
    eightNote++;
  } else {
    eightNote = 0;
  }
  if ((eightNote >= 0 ) && (eightNote <= 3)) {
    page = 0;
  } else if ((eightNote >= 4 ) && (eightNote <= 7)) {
    page = 1;
  } else if ((eightNote >= 8 ) && (eightNote <= 11)) {
    page = 2;
  } else if ((eightNote >= 12 ) && (eightNote <= 15)) {
    page = 3;
  }
  uint8_t tickNote = eightNote % 4;

  if (stepStatus[0 + tickNote][page] == 1) {
    noteOn(0, 45, 127);
    noteOff(0, 45, 127);
  }
  if (stepStatus[4 + tickNote][page] == 1) {
    noteOn(0, 46, 127);
    noteOff(0, 46, 127);
  }
  if (stepStatus[8 + tickNote][page] == 1) {
    noteOn(0, 47, 127);
    noteOff(0, 47, 127);
  }
  if (stepStatus[12 + tickNote][page] == 1) {
    noteOn(0, 48, 127);
    noteOff(0, 48, 127);
  }
  MidiUSB.flush();
}
