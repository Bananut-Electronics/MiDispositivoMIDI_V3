/*
  2018 BanaNut Electronics (bananutelectronics@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 4.0 license
  (Attribution-NonCommercial-ShareAlike 4.0 International)

  You are free to share, copy and redistribute the material in any medium
  or format, and also to adapt, remix, transform or build upon de provided
  material, always under the following terms:
    * Attribution                — You must give appropriate credit, provide a link
                                   to the license, and indicate if changes were made.
                                   You may do so in any reasonable manner, but not in any way
                                   that suggests the licensor endorses you or your use.
    * NonCommercial              — You may not use the material for commercial purposes.
    * ShareAlike                 — If you remix, transform, or build upon the material, you must
                                   distribute your contributions under the same license as the original.
    * No additional restrictions — You may not apply legal terms or technological measures that
                                   legally restrict others from doing anything the license permits.

  More informatio can be found in https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

#ifndef MiDispositivoMIDI_V3_h
#define MiDispositivoMIDI_V3_h

// digital pin definition
#define LEDPAD_PIN  2
#define SW_0_PIN    7
#define SW_1_PIN    8

// analog pin definition
#define PADCH_0_PIN 0
#define PADCH_1_PIN 1
#define PADCH_2_PIN 2
#define PADCH_3_PIN 3

// mux pin definition 
#define MUX_B0_PIN 6
#define MUX_B1_PIN 5
#define MUX_B2_PIN 4
#define MUX_B3_PIN 3

// some definitions
#define N_ROWS         4
#define N_COLS         4
#define RGB_COLORS     3
#define LEDPAD_NUM     16
#define PAD_THR        500
#define MAX_PAGE       4
#define MAX_EXTENSIONS 4
#define MUX_DELAY      50
#define USEC_IN_SEC    1000000
#define NUM_LATERAL_SWITCH 2

// some default values
#define DEFAULT_MIDI_CHANNEL 0
#define DEFAULT_ON_RED       0
#define DEFAULT_ON_GREEN     255
#define DEFAULT_ON_BLUE      0
#define DEFAULT_OFF_RED      255
#define DEFAULT_OFF_GREEN    255
#define DEFAULT_OFF_BLUE     255
#define DEFAULT_PAGE_NUM     0
#define DEFAULT_VELOCITY     127

enum PADstatus {OFF, ON, ONSET, OFFSET};

class MiDispositivoMIDI_V3
{
public: MiDispositivoMIDI_V3(uint8_t numPages, uint8_t numExtensions);
    
    void loop();
    void initializeLEDs();
    void setMidiNotes(uint8_t notes[][LEDPAD_NUM]);
    void setMidiVeloc(uint8_t veloc[][LEDPAD_NUM]);
    void setOnColors(uint8_t rgbColors[RGB_COLORS]);
    void setOffColors(uint8_t rgbColors[RGB_COLORS]);
    void setPixelColor(uint8_t expansion,
                       uint8_t led,
                       uint8_t rgbColors[RGB_COLORS]);
    void setInterrupt(void (* functionPointer)(), double seconds);
    void setMidiChannel(uint8_t midiChannel);
    
protected:
    // number of hardware extensions [0:MAX_EXTENSIONS]
    uint8_t _numberExtensions;

    // number of pages [0:MAX_PAGE]
    uint8_t _numPages;

    // color per led for all the device
    uint8_t _ledPad[MAX_EXTENSIONS][LEDPAD_NUM][RGB_COLORS] = {{{0}}};

    // last value read by each button
    uint8_t _lastRead[MAX_EXTENSIONS][LEDPAD_NUM]           =  {{0}};

    // midi note sent by each button
    uint8_t _midiNotes[MAX_EXTENSIONS][LEDPAD_NUM]          =  {{0}};

    // midi velocity sent by each button
    uint8_t _midiVeloc[MAX_EXTENSIONS][LEDPAD_NUM]          =  {{0}};

    // default color for all leds when the button is on
    uint8_t _onColors[RGB_COLORS];

    // default color for all leds when the button is off
    uint8_t _offColors[RGB_COLORS];

    // each page sends different notes
    uint8_t _currentPage;

    // midi channel used for midi tx
    uint8_t _midiChannel;

    void readPADs();
    void readPage();
    void updateLEDs();
    void setRgbColors(uint8_t copyFrom[RGB_COLORS],
                      uint8_t copyTo[RGB_COLORS]);

    void generateNotesVeloc(uint8_t numberOfExtensions);

    void note(PADstatus status,
              byte channel,
              byte pitch,
              byte velocity);

    void noteOn(byte channel,
                byte pitch,
                byte velocity);

    void noteOff(byte channel,
                 byte pitch,
                 byte velocity);

    virtual void PADaction(PADstatus status, uint8_t extension, uint8_t pad);
};

#endif