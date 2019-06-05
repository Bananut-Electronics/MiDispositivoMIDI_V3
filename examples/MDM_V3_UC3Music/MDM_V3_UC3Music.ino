

// Library Import
#include <Adafruit_NeoPixel.h>
#include <MIDIUSB.h>

// MIDI MSSG VALUES

// First Byte
#define NOTE_ON     0x90
#define NOTE_OFF    0x80
#define CONTROL     0xB0

// Second Byte
#define MODULATION  1
#define VOLUME      7

// SECOND BYTE NOTE VALUES ------------------------------------------------------------------------------------------------------------------------------------
// OCTAVE 00              || OCTAVE 01               || OCTAVE 02               || OCTAVE 03              || OCTAVE 04               || OCTAVE 05
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
const byte NOTE_C0  =  0  ;const byte NOTE_C1  = 12  ;const byte NOTE_C2  = 24  ;const byte NOTE_C3  = 36  ;const byte NOTE_C4  = 48  ;const byte NOTE_C5  = 60;
const byte NOTE_C0s =  1  ;const byte NOTE_C1s = 13  ;const byte NOTE_C2s = 25  ;const byte NOTE_C3s = 37  ;const byte NOTE_C4s = 49  ;const byte NOTE_C5s = 61;
const byte NOTE_D0  =  2  ;const byte NOTE_D1  = 14  ;const byte NOTE_D2  = 26  ;const byte NOTE_D3  = 38  ;const byte NOTE_D4  = 50  ;const byte NOTE_D5  = 62;
const byte NOTE_D0s =  3  ;const byte NOTE_D1s = 15  ;const byte NOTE_D2s = 27  ;const byte NOTE_D3s = 39  ;const byte NOTE_D4s = 51  ;const byte NOTE_D5s = 63;
const byte NOTE_E0  =  4  ;const byte NOTE_E1  = 16  ;const byte NOTE_E2  = 28  ;const byte NOTE_E3  = 40  ;const byte NOTE_E4  = 52  ;const byte NOTE_E5  = 64;
const byte NOTE_F0  =  5  ;const byte NOTE_F1  = 17  ;const byte NOTE_F2  = 29  ;const byte NOTE_F3  = 41  ;const byte NOTE_F4  = 53  ;const byte NOTE_F5  = 65;
const byte NOTE_F0s =  6  ;const byte NOTE_F1s = 18  ;const byte NOTE_F2s = 30  ;const byte NOTE_F3s = 42  ;const byte NOTE_F4s = 54  ;const byte NOTE_F5s = 66;
const byte NOTE_G0  =  7  ;const byte NOTE_G1  = 19  ;const byte NOTE_G2  = 31  ;const byte NOTE_G3  = 43  ;const byte NOTE_G4  = 55  ;const byte NOTE_G5  = 67;
const byte NOTE_G0s =  8  ;const byte NOTE_G1s = 20  ;const byte NOTE_G2s = 32  ;const byte NOTE_G3s = 44  ;const byte NOTE_G4s = 56  ;const byte NOTE_G5s = 68;
const byte NOTE_A0  =  9  ;const byte NOTE_A1  = 21  ;const byte NOTE_A2  = 33  ;const byte NOTE_A3  = 45  ;const byte NOTE_A4  = 57  ;const byte NOTE_A5  = 69;
const byte NOTE_A0s = 10  ;const byte NOTE_A1s = 22  ;const byte NOTE_A2s = 34  ;const byte NOTE_A3s = 46  ;const byte NOTE_A4s = 58  ;const byte NOTE_A5s = 70;
const byte NOTE_B0  = 11  ;const byte NOTE_B1  = 23  ;const byte NOTE_B2  = 35  ;const byte NOTE_B3  = 47  ;const byte NOTE_B4  = 59  ;const byte NOTE_B5  = 71;

// SECOND BYTE NOTE VALUES -----------------------------------------------------------------------------------------------------------
// OCTAVE 06              || OCTAVE 07               || OCTAVE 08               || OCTAVE 09               || OCTAVE 10           
//------------------------------------------------------------------------------------------------------------------------------------
const byte NOTE_C6  = 72  ;const byte NOTE_C7  = 84  ;const byte NOTE_C8  = 96  ;const byte NOTE_C9  =108  ;const byte NOTE_C10  =120;
const byte NOTE_C6s = 73  ;const byte NOTE_C7s = 85  ;const byte NOTE_C8s = 97  ;const byte NOTE_C9s =109  ;const byte NOTE_C10s =121;
const byte NOTE_D6  = 74  ;const byte NOTE_D7  = 86  ;const byte NOTE_D8  = 98  ;const byte NOTE_D9  =110  ;const byte NOTE_D10  =122;
const byte NOTE_D6s = 75  ;const byte NOTE_D7s = 87  ;const byte NOTE_D8s = 99  ;const byte NOTE_D9s =111  ;const byte NOTE_D10s =123;
const byte NOTE_E6  = 76  ;const byte NOTE_E7  = 88  ;const byte NOTE_E8  =100  ;const byte NOTE_E9  =112  ;const byte NOTE_E10  =124;
const byte NOTE_F6  = 77  ;const byte NOTE_F7  = 89  ;const byte NOTE_F8  =101  ;const byte NOTE_F9  =113  ;const byte NOTE_F10  =125;
const byte NOTE_F6s = 78  ;const byte NOTE_F7s = 90  ;const byte NOTE_F8s =102  ;const byte NOTE_F9s =114  ;const byte NOTE_F10s =126;
const byte NOTE_G6  = 79  ;const byte NOTE_G7  = 91  ;const byte NOTE_G8  =103  ;const byte NOTE_G9  =115;
const byte NOTE_G6s = 80  ;const byte NOTE_G7s = 92  ;const byte NOTE_G8s =104  ;const byte NOTE_G9s =116;
const byte NOTE_A6  = 81  ;const byte NOTE_A7  = 93  ;const byte NOTE_A8  =105  ;const byte NOTE_A9  =117;
const byte NOTE_A6s = 82  ;const byte NOTE_A7s = 94  ;const byte NOTE_A8s =106  ;const byte NOTE_A9s =118;
const byte NOTE_B6  = 83  ;const byte NOTE_B7  = 95  ;const byte NOTE_B8  =107  ;const byte NOTE_B9  =119;

// PIN DEFINITION
// Digital Pin Definitions
#define LED    2
#define SW_0   7
#define SW_1   8

// Analog Pin Definitions
#define CH_0   0
#define CH_1   1
#define CH_2   2
#define CH_3   3

#define MUX_A  3
#define MUX_B  4
#define MUX_C  5
#define MUX_D  6

// CONSTANT DEFINITION
#define SW_Q   16 // Number of SW_Pads
#define SC_Q   8 // Number of Scales
#define ON     1  // ON Value
#define OFF    0  // OFF Value

//MACROS Templates
#define PREDEF 0
#define DRUM 11

// USER PROGRAMMING INTERFACE --------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

// MIDI Channel Colours
// COLOR RED [0:255]         || COLOR GREEN [0:255]          || COLOR BLUE [0:255]          || Comments
//------------------------------------------------------------------------------------------------------------

//Color de la Base
const byte BCK_R_OFF  = 100   ;const byte BCK_G_OFF  = 100    ;const byte BCK_B_OFF  = 100; // MIDI Channel 15  //Blanco Tenue

//Color de las teclas encendidas 
const byte CH_00_R_ON =   0   ;const byte CH_00_G_ON =   0    ;const byte CH_00_B_ON = 255; // MIDI Channel 00  Azul
const byte CH_01_R_ON =   0   ;const byte CH_01_G_ON = 144    ;const byte CH_01_B_ON = 255; // MIDI Channel 01  Azulito
const byte CH_02_R_ON =  64   ;const byte CH_02_G_ON = 192    ;const byte CH_02_B_ON = 208; // MIDI Channel 02  Azul/Blanco
const byte CH_03_R_ON = 112   ;const byte CH_03_G_ON = 240    ;const byte CH_03_B_ON = 160; // MIDI Channel 03  Celeste
const byte CH_04_R_ON =  00   ;const byte CH_04_G_ON = 255    ;const byte CH_04_B_ON = 130; // MIDI Channel 04  Turquesa
const byte CH_05_R_ON =   0   ;const byte CH_05_G_ON = 255    ;const byte CH_05_B_ON =   0; // MIDI Channel 05  Verde
const byte CH_06_R_ON = 100   ;const byte CH_06_G_ON = 255    ;const byte CH_06_B_ON =   0; // MIDI Channel 06  Lima
const byte CH_07_R_ON = 255   ;const byte CH_07_G_ON = 255    ;const byte CH_07_B_ON =   0; // MIDI Channel 07  Amarillo
const byte CH_08_R_ON = 255   ;const byte CH_08_G_ON = 128    ;const byte CH_08_B_ON =   0; // MIDI Channel 08  Naranjarillo
const byte CH_09_R_ON = 240   ;const byte CH_09_G_ON = 112    ;const byte CH_09_B_ON =  16; // MIDI Channel 09  Naranja
const byte CH_10_R_ON = 150   ;const byte CH_10_G_ON =   0    ;const byte CH_10_B_ON = 255; // MIDI Channel 10  Moradito
const byte CH_11_R_ON = 255   ;const byte CH_11_G_ON = 255    ;const byte CH_11_B_ON = 255; // MIDI Channel 11  Blanco
const byte CH_12_R_ON = 208   ;const byte CH_12_G_ON =  80    ;const byte CH_12_B_ON =  64; // MIDI Channel 12  Rosita
const byte CH_13_R_ON = 255   ;const byte CH_13_G_ON =   0    ;const byte CH_13_B_ON = 150; // MIDI Channel 13  Fucia
const byte CH_14_R_ON = 255   ;const byte CH_14_G_ON =   0    ;const byte CH_14_B_ON =  50; // MIDI Channel 14  Rojosa
const byte CH_15_R_ON = 255   ;const byte CH_15_G_ON =   0    ;const byte CH_15_B_ON =   0; // MIDI Channel 15  Rojo

// SCALE Color Canvas
// COLOR RED [0:255]          || COLOR GREEN [0:255]          || COLOR BLUE [0:255]         || Comments
//------------------------------------------------------------------------------------------------------------
const byte SC_INIT_R  =   0   ;const byte SC_INIT_G  =   0    ;const byte SC_INIT_B  =  50; // INIT from COLOR
const byte SC_END_R   =   0   ;const byte SC_END_G   = 255    ;const byte SC_END_B   = 255; // END to COLOR
const byte SC_ON_R    = 255   ;const byte SC_ON_G    = 255    ;const byte SC_ON_B    = 255; // ON Scale

// OCTAVE Color Canvas
// COLOR RED [0:255]          || COLOR GREEN [0:255]          || COLOR BLUE [0:255]         || Comments
//------------------------------------------------------------------------------------------------------------
const byte OC_INIT_R  =  50   ;const byte OC_INIT_G  =   0    ;const byte OC_INIT_B  =  50; // INIT from COLORº
const byte OC_END_R   =   0   ;const byte OC_END_G   = 255    ;const byte OC_END_B   = 255; // END to COLOR
const byte OC_ON_R    = 255   ;const byte OC_ON_G    = 255    ;const byte OC_ON_B    = 255; // ON Octave

// DEFAULT CONTROL VALUES
int SW_Octave       = 3;   // Starting OCTAVE Value
int SW_Scale        = 0;   // Starting SCALE Value
int SW_MIDIChannel  = 1;   // Starting MIDI_CHANNEL Value
int MDM_MODE        = 0;   // Starting MDM_MODE Value (0 is Notes, 1 is Channel Selection, 2 is Scale/Octave Selection)
int ledEvent        = 1;   // Starting ledEvent Flag

const int PAD_THR   = 500; // SW Push Detection Threshold
const int VELOCITY  = 127; // Default MIDI Note Velocity

bool   SW_CTRL[SW_Q]       = {OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF};
byte   CH_LED[(SW_Q+1)][3] = {{ BCK_R_OFF,  BCK_G_OFF,  BCK_B_OFF},
                              {CH_00_R_ON, CH_00_G_ON, CH_00_B_ON},
                              {CH_01_R_ON, CH_01_G_ON, CH_01_B_ON},
                              {CH_02_R_ON, CH_02_G_ON, CH_02_B_ON},
                              {CH_03_R_ON, CH_03_G_ON, CH_03_B_ON},
                              {CH_04_R_ON, CH_04_G_ON, CH_04_B_ON},
                              {CH_05_R_ON, CH_05_G_ON, CH_05_B_ON},
                              {CH_06_R_ON, CH_06_G_ON, CH_06_B_ON},
                              {CH_07_R_ON, CH_07_G_ON, CH_07_B_ON},
                              {CH_08_R_ON, CH_08_G_ON, CH_08_B_ON},
                              {CH_09_R_ON, CH_09_G_ON, CH_09_B_ON},
                              {CH_10_R_ON, CH_10_G_ON, CH_10_B_ON},
                              {CH_11_R_ON, CH_11_G_ON, CH_11_B_ON}, 
                              {CH_12_R_ON, CH_12_G_ON, CH_12_B_ON},
                              {CH_13_R_ON, CH_13_G_ON, CH_13_B_ON},
                              {CH_14_R_ON, CH_14_G_ON, CH_14_B_ON},
                              {CH_15_R_ON, CH_15_G_ON, CH_15_B_ON}};

byte SW_NOTE[SC_Q][SW_Q] = {{NOTE_C0,  NOTE_D0,   NOTE_E0,   NOTE_F0,   NOTE_G0,   NOTE_A0,   NOTE_B0,  NOTE_C1,  NOTE_D1,   NOTE_E1,   NOTE_F1,   NOTE_G1,   NOTE_A1,   NOTE_B1,  NOTE_C2,  NOTE_D2},   // Escala Mayor  I-II-III-IV-V-VI-VII
                            {NOTE_C1,  NOTE_D1,   NOTE_E1,   NOTE_F1,   NOTE_G1,   NOTE_A1,   NOTE_B1,  NOTE_C2,  NOTE_D2,   NOTE_E2,   NOTE_F2,   NOTE_G2,   NOTE_A2,   NOTE_B2,  NOTE_C3,  NOTE_D3},   // Escala Mayor  I-II-III-IV-V-VI-VII
                            {NOTE_C2,  NOTE_D2,   NOTE_E2,   NOTE_F2,   NOTE_G2,   NOTE_A2,   NOTE_B2,  NOTE_C3,  NOTE_D3,   NOTE_E3,   NOTE_F3,   NOTE_G3,   NOTE_A3,   NOTE_B3,  NOTE_C4,  NOTE_D4},   // Escala Mayor  I-II-III-IV-V-VI-VII
                            {NOTE_C0,  NOTE_D0,   NOTE_E0,   NOTE_F0,   NOTE_G0s,  NOTE_A0,   NOTE_B0,  NOTE_C1,  NOTE_D1,   NOTE_E1,   NOTE_F1,   NOTE_G1s,   NOTE_A1,   NOTE_B1,  NOTE_C2,  NOTE_D2},  // Escala Mayor 5 aumentada  I-II-III-IV-V#-VI-VII
                            {NOTE_C1,  NOTE_D1,   NOTE_E1,   NOTE_F1,   NOTE_G1s,  NOTE_A1,   NOTE_B1,  NOTE_C2,  NOTE_D2,   NOTE_E2,   NOTE_F2,   NOTE_G2,   NOTE_A2,   NOTE_B2,  NOTE_C3,  NOTE_D3},   // Escala Mayor  I-II-III-IV-V-VI-VII
                            {NOTE_C0,  NOTE_D0,   NOTE_E0,   NOTE_G0,   NOTE_A0,   NOTE_C1,   NOTE_D1,  NOTE_E1,  NOTE_G1,   NOTE_A1,   NOTE_C2,   NOTE_D2,   NOTE_E2,   NOTE_G2,  NOTE_A2,  NOTE_C3},   // Pentatonica Mayor  I-II-III-IV-V-VI-VII
                            {NOTE_C0,  NOTE_D0,   NOTE_E0,   NOTE_F0,   NOTE_G0,   NOTE_A0,   NOTE_B0,  NOTE_C1,  NOTE_D1,   NOTE_E1,   NOTE_F1,   NOTE_G1,   NOTE_A2,   NOTE_B2,  NOTE_C2,  NOTE_D2},   // Pentatonica Mayor  I-II-III-IV-V-VI-VII
                            {NOTE_C0,  NOTE_G0,   NOTE_D1,   NOTE_E1,   NOTE_G1,   NOTE_A1,   NOTE_F1,  NOTE_C2,  NOTE_B1,   NOTE_C2,   NOTE_D2,   NOTE_E2,   NOTE_C1,   NOTE_F1,  NOTE_G1,  NOTE_D1}};  // JUMP

int muxSel[4] = {MUX_D, MUX_C, MUX_B, MUX_A}; // MUX Selector Pins
int templateSel = 0; //Selection of templates (0 == predefined)

Adafruit_NeoPixel ledPadStripe = Adafruit_NeoPixel(SW_Q, LED, NEO_RGB + NEO_KHZ800);


void setup() {
  // Baudrate Set
  Serial.begin(115200); 
  // LED Stripe Initialization
  ledPadStripe.begin();
  ledPadStripe.show();
  // Set Pin Modes
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(MUX_D, OUTPUT);
  //            G   R   B   G             R              B
  LED_FADE_INIT(0, 0, 0, CH_LED[0][1], CH_LED[0][0], CH_LED[0][2], 10, 80);
}

//LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
void loop() {
  switch(MDM_MODE){
    
    case 1:      // MIDI CHANNEL Mode
      Serial.println("Hey there 1");
      SW_MC_Read();
      SW_MODE_Read();
      break;

    case 2:      // SCALE/OCTAVE Mode
      Serial.println("Hey there 2");
      SW_Scale++;
      delay(300);
      if(SW_Scale>=SC_Q){
        SW_Scale = 0;}
      MDM_MODE = 0;
      //SW_SCOC_Read();
      //SW_MODE_Read();
      //LED_SCOC_Update();
    break;

    default:      // NOTE Mode
      Serial.print("Hey there 0, Scale is ");
      Serial.println(SW_Scale, DEC);
      SW_NOTE_Read();
      SW_MODE_Read();
    break;
  }
}
//FIIIIIIIIIIIIIIIIIIIIIN

void SW_MODE_Read(){
  if(MDM_MODE == 0){           // If in NOTE MODE ('0')
    if(digitalRead(SW_0)){
      MDM_MODE = 1;        // MIDI CHANNEL Mode ('1')
      LED_MC_InitTransition(8, 10);
    }else if (digitalRead(SW_1)){
      MDM_MODE = 2;        // MIDI CHANNEL Mode ('2')
      changeEscale(8, 100);
    }
  }
}


void SW_MC_Read(){
  for (int i = 0; i < SW_Q; i++) {
    SET_MUXGATE(i);
    if (analogRead(CH_0) < PAD_THR) {
      SW_MIDIChannel = (i+1);
      templateSel = i;
      MDM_MODE = 0;             // Return to NOTE MODE ('0')
      LED_MC_EndTransition();
      
    }
  }
}

void changeEscale(int fadeSteps, int fadeDelay){
    for(int i = 0; i< SW_Q; i++){
      ledPadStripe.setPixelColor(i, 100, 50, 100);   // Heavy Stuff
    }
    delay(fadeDelay);
    ledPadStripe.show(); // Update LEDs
    if(templateSel == DRUM){delay(fadeDelay);drum();}
    else{
      for(int i = 0; i< SW_Q; i++){
        ledPadStripe.setPixelColor(i, 100, 100, 100);   // Heavy Stuff
      }
      delay(fadeDelay);
      ledPadStripe.show(); // Update LEDs
    }
}

void LED_MC_InitTransition(int fadeSteps, int fadeDelay){
  //SW_PAD_SCALE control
  for(int h = 0; h < fadeSteps; h++){
    ledPadStripe.setPixelColor(0, ((CH_LED[1][1]/fadeSteps)*h), ((CH_LED[1][0]/fadeSteps)*h), ((CH_LED[1][2]/fadeSteps)*h));   // Heavy Stuff
    delay(fadeDelay);
    ledPadStripe.show(); // Update LEDs 
  }
  for(int h = 0; h < fadeSteps; h++){
    ledPadStripe.setPixelColor(1, ((CH_LED[2][1]/fadeSteps)*h), ((CH_LED[2][0]/fadeSteps)*h), ((CH_LED[2][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(4, ((CH_LED[5][1]/fadeSteps)*h), ((CH_LED[5][0]/fadeSteps)*h), ((CH_LED[5][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(5, ((CH_LED[6][1]/fadeSteps)*h), ((CH_LED[6][0]/fadeSteps)*h), ((CH_LED[6][2]/fadeSteps)*h));   // Heavy Stuff
    delay(fadeDelay);
    ledPadStripe.show(); // Update LEDs 
  }
  for(int h = 0; h < fadeSteps; h++){
    ledPadStripe.setPixelColor(2, ((CH_LED[3][1]/fadeSteps)*h), ((CH_LED[3][0]/fadeSteps)*h), ((CH_LED[3][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(6, ((CH_LED[7][1]/fadeSteps)*h), ((CH_LED[7][0]/fadeSteps)*h), ((CH_LED[7][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(10, ((CH_LED[11][1]/fadeSteps)*h), ((CH_LED[11][0]/fadeSteps)*h), ((CH_LED[11][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(9, ((CH_LED[10][1]/fadeSteps)*h), ((CH_LED[10][0]/fadeSteps)*h), ((CH_LED[10][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(8, ((CH_LED[9][1]/fadeSteps)*h), ((CH_LED[9][0]/fadeSteps)*h), ((CH_LED[9][2]/fadeSteps)*h));   // Heavy Stuff
    delay(fadeDelay);
    ledPadStripe.show(); // Update LEDs 
  }
  for(int h = 0; h < fadeSteps; h++){
    ledPadStripe.setPixelColor( 3, ((CH_LED[ 4][1]/fadeSteps)*h), ((CH_LED[ 4][0]/fadeSteps)*h), ((CH_LED[ 4][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor( 7, ((CH_LED[ 8][1]/fadeSteps)*h), ((CH_LED[ 8][0]/fadeSteps)*h), ((CH_LED[ 8][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(11, ((CH_LED[12][1]/fadeSteps)*h), ((CH_LED[12][0]/fadeSteps)*h), ((CH_LED[12][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(12, ((CH_LED[13][1]/fadeSteps)*h), ((CH_LED[13][0]/fadeSteps)*h), ((CH_LED[13][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(13, ((CH_LED[14][1]/fadeSteps)*h), ((CH_LED[14][0]/fadeSteps)*h), ((CH_LED[14][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(14, ((CH_LED[15][1]/fadeSteps)*h), ((CH_LED[15][0]/fadeSteps)*h), ((CH_LED[15][2]/fadeSteps)*h));   // Heavy Stuff
    ledPadStripe.setPixelColor(15, ((CH_LED[16][1]/fadeSteps)*h), ((CH_LED[16][0]/fadeSteps)*h), ((CH_LED[16][2]/fadeSteps)*h));   // Heavy Stuff
    delay(fadeDelay);
    ledPadStripe.show(); // Update LEDs 
  }
}

void LED_MC_EndTransition(){
  //SW_PAD_SCALE control
  LED_FADE_INIT(CH_LED[0][1], CH_LED[0][0], CH_LED[0][2], CH_LED[SW_MIDIChannel][1], CH_LED[SW_MIDIChannel][0], CH_LED[SW_MIDIChannel][2], 10, 50);
  LED_FADE_INIT(CH_LED[SW_MIDIChannel][1], CH_LED[SW_MIDIChannel][0], CH_LED[SW_MIDIChannel][2], CH_LED[0][1], CH_LED[0][0], CH_LED[0][2], 10, 50);
  if(templateSel == DRUM){drum();};
}

void SW_NOTE_Read(){
  for (int i = 0; i < SW_Q; i++) {
    SET_MUXGATE(i);
    if (analogRead(CH_0) < PAD_THR) {
      if (!SW_CTRL[i]) { // If SW not activated
        SW_CTRL[i] = ON;
        ledPadStripe.setPixelColor(i, CH_LED[SW_MIDIChannel][1], CH_LED[SW_MIDIChannel][0], CH_LED[SW_MIDIChannel][2]);   // Heavy Stuff
        MIDI_TX(SW_MIDIChannel, NOTE_ON,SW_NOTE[SW_Scale][i] + (12*SW_Octave), VELOCITY);   // MIDI USB MSSG
        delay(1);
        ledPadStripe.show(); // Update LEDs
      }
    }else{
      if (SW_CTRL[i]) { // If SW activated
        SW_CTRL[i] = OFF;
        if(templateSel == DRUM){drum();}
        else{ledPadStripe.setPixelColor(i, CH_LED[0][1], CH_LED[0][0], CH_LED[0][2]);};   // Heavy Stuff
        MIDI_TX(SW_MIDIChannel, NOTE_OFF, SW_NOTE[SW_Scale][i] + (12*SW_Octave), 0);        // MIDI USB MSSG
        delay(1);
        ledPadStripe.show(); // Update LEDs
      }
    }
  }
}
//Falla algo... El el orden de los colores real es G R B G R B
void LED_FADE_INIT(int INIT_R, int INIT_G, int INIT_B, int END_R, int END_G, int END_B, int fadeSteps, int fadeDelay){
  bool dir_R, dir_G, dir_B;
  int  step_R, step_G, step_B;
  int  Red, Grn, Blu;
  
  if(INIT_R >= END_R){
    dir_R = 0;
    step_R = ((INIT_R-END_R)/fadeSteps);
  }else{
    dir_R = 1;
    step_R = ((END_R-INIT_R)/fadeSteps);
  };
  
  if(INIT_G >= END_G){dir_G = 0; step_G = ((INIT_G-END_G)/fadeSteps);}else{dir_G = 1; step_G = ((END_G-INIT_G)/fadeSteps);};
  if(INIT_B >= END_B){dir_B = 0; step_R = ((INIT_B-END_B)/fadeSteps);}else{dir_B = 1; step_B = ((END_B-INIT_B)/fadeSteps);};
  for(int h = 0; h <= fadeSteps; h++){
    if(h == fadeSteps){Red = 100; Grn = 100; Blu = 100;}else{
      if(dir_R){Red = (INIT_R + (h* step_R));}else{Red = (INIT_R - (h* step_R));};
      if(dir_G){Grn = (INIT_G + (h* step_G));}else{Grn = (INIT_G - (h* step_G));};
      if(dir_B){Blu = (INIT_B + (h* step_B));}else{Blu = (INIT_B - (h* step_B));};}
    for(int i = 0; i< SW_Q; i++){
      ledPadStripe.setPixelColor(i, Red, Grn, Blu);   // Heavy Stuff
    }
    delay(fadeDelay);
    ledPadStripe.show(); // Update LEDs
  }
}

void SET_MUXGATE(int GATENUM){
  for (int h = 0; h < 4; h++) {
      digitalWrite(muxSel[h], ((GATENUM >> (3 - h)) & 0x01)); //MUX Setup pin for each MUX Pin
  }
  delay(1); // Delay por MUX Setup
}

void MIDI_TX(byte MIDICHANNEL, byte MESSAGE, byte PITCH, byte VELOCITY) {
  //byte status1 = MESSAGE + MIDICHANNEL;
  //Serial.write(MESSAGE + MIDICHANNEL);
  //Serial.write(PITCH);
  //Serial.write(VELOCITY);
  midiEventPacket_t event = {(byte)(MESSAGE >> 4), (byte)(MESSAGE | (MIDICHANNEL)), PITCH, VELOCITY};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}
void drum(){
      for(int i = 0; i< SW_Q; i++){
        if(i==0){ledPadStripe.setPixelColor(i, CH_LED[5][1], CH_LED[5][0], CH_LED[5][2]);};
        if((i==1)||(i==2)){ledPadStripe.setPixelColor(i, CH_LED[2][1], CH_LED[2][0], CH_LED[2][2]);};
        if(i==3){ledPadStripe.setPixelColor(i, CH_LED[3][1], CH_LED[3][0], CH_LED[3][2]);};
        if(i==4){ledPadStripe.setPixelColor(i, CH_LED[6][1], CH_LED[6][0], CH_LED[6][2]);};
        if((i==5)||(i==6)||(i==11)){ledPadStripe.setPixelColor(i, CH_LED[11][1], CH_LED[11][0], CH_LED[11][2]);};
        if(i==7){ledPadStripe.setPixelColor(i, CH_LED[8][1], CH_LED[8][0], CH_LED[8][2]);};
        if(i==8){ledPadStripe.setPixelColor(i, CH_LED[7][1], CH_LED[7][0], CH_LED[7][2]);};
        if((i==9)||(i==10)){ledPadStripe.setPixelColor(i, CH_LED[10][1], CH_LED[10][0], CH_LED[10][2]);};
        if(i==12){ledPadStripe.setPixelColor(i, CH_LED[13][1], CH_LED[13][0], CH_LED[13][2]);};
        if((i==13)||(i==14)){ledPadStripe.setPixelColor(i, CH_LED[16][1], CH_LED[16][0], CH_LED[16][2]);};
        if(i==15){ledPadStripe.setPixelColor(i, CH_LED[15][1], CH_LED[15][0], CH_LED[15][2]);}; 
    }
    delay(1);
    ledPadStripe.show(); // Update LEDs
}

