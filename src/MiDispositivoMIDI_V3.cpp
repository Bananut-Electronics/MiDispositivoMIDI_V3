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

#include "Arduino.h"
#include "MiDispositivoMIDI_V3.h"
#include <Adafruit_NeoPixel.h>
#include <MIDIUSB.h>
#include <TimerOne.h>

// neopixel library to handle all leds
Adafruit_NeoPixel ledPadStripe;

// last value read by the lateral switches
uint8_t lastSwitchRead[NUM_LATERAL_SWITCH] = {0, 0};

// pad channel pins
uint8_t padPin[4] = {PADCH_0_PIN,PADCH_1_PIN,PADCH_2_PIN,PADCH_3_PIN};

// mux selectior pins
uint8_t muxPin[4] = {MUX_B0_PIN,MUX_B1_PIN,MUX_B2_PIN,MUX_B3_PIN};

/*
* Function:    MiDispositivoMIDI_V3::MiDispositivoMIDI_V3
* Description: Constructor.

* Input:
*  -hwConfig  Hardware configuration. If only one board is used, use
              SINGLE_DEVICE_4X4. If you have more than one board connected,
              choose a configuration accordingly
*  -bRoles    Type of each board. Choose between POTENTIOMETER and TOUCHPAD.

* Output:
*  -void
*/
MiDispositivoMIDI_V3::MiDispositivoMIDI_V3(HardwareConfigurations hwConfig, BoardRoles bRoles[MAX_EXTENSIONS])
{
    _currentPage = DEFAULT_PAGE_NUM;
    _hwConfig    = hwConfig;

    switch (hwConfig)
    {
        case SINGLE_DEVICE_4X4:
            _numberExtensions = 1;
            _numOfRows = 4;
            _numOfCols = 4;
            break;
        case TWO_DEVICES_4X8:
            _numberExtensions = 2;
            _numOfRows = 4;
            _numOfCols = 8;
            break;
        case THREE_DEVICES_4X12:
            _numberExtensions = 3;
            _numOfRows = 4;
            _numOfCols = 12;
            break;
        case FOUR_DEVICES_4X16:
            _numberExtensions = 4;
            _numOfRows = 4;
            _numOfCols = 16;
            break;
        case FOUR_DEVICES_8X8:
            _numberExtensions = 4;
            _numOfRows = 8;
            _numOfCols = 8;
            break;
    }

    for (uint8_t i = 0; i < _numberExtensions; i++)
    {
      _bRoles[i] = bRoles[i];
    }

    // set some on/off colors by default
    uint8_t on[RGB_COLORS]  = {DEFAULT_ON_RED, DEFAULT_ON_GREEN, DEFAULT_ON_BLUE};
    uint8_t off[RGB_COLORS] = {DEFAULT_OFF_RED, DEFAULT_OFF_GREEN, DEFAULT_OFF_BLUE};
    setOnColors(on);
    setOffColors(off);

    // set pin modes
    pinMode(MUX_B0_PIN, OUTPUT);
    pinMode(MUX_B1_PIN, OUTPUT);
    pinMode(MUX_B2_PIN, OUTPUT);
    pinMode(MUX_B3_PIN, OUTPUT);
    pinMode(SW_0_PIN, INPUT);
    pinMode(SW_1_PIN, INPUT);

    // init led strip
    ledPadStripe = Adafruit_NeoPixel(LEDPAD_NUM * _numberExtensions, LEDPAD_PIN, NEO_RGB + NEO_KHZ800);
    ledPadStripe.begin();
    
    // set all leds to off color
    initializeLEDs();

    // generate which notes each button will send
    generateNotesVeloc(_numberExtensions);
}

/*
* Function:    MiDispositivoMIDI_V3::~MiDispositivoMIDI_V3
* Description: Destructor.

* Input:
*  -none

* Output:
*  -void
*/
MiDispositivoMIDI_V3::~MiDispositivoMIDI_V3()
{

}

/*
* Function:    MiDispositivoMIDI_V3::readPADs
* Description: Read all buttons and do some actions with them

* Input:
*  -none

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::readPADs()
{
    // iterate all buttons in a pad
    for(int i = 0; i < LEDPAD_NUM; i++)
    {
        // configure the mux
        for(int h = 0; h < 4; h++)
        {
            digitalWrite(muxPin[h], ((i>>(3-h)) & 0x01));
        }
        delayMicroseconds(MUX_DELAY);

        // iterate all extensions
        for(int j = 0; j < _numberExtensions; j++)
        {
            // read the button
            int padRead = analogRead(padPin[j]);

            // If j extension is a TOUCHPAD matrix
            if (_bRoles[j] == TOUCHPAD)
            {
              // if lower than threshold, it has been pressed
              if (padRead < PAD_THR)
              { 
                  // only enter if it was not previously pressed
                  if (_lastRead[j][i] == 0)
                  {
                      _lastRead[j][i] = 1;
                      buttonPressed(j, i);
                  }
                  else
                  {
                      buttonHolded(j, i);
                  }
              }
              else
              {
                  // if it has been released
                  if (_lastRead[j][i] == 1)
                  {
                      _lastRead[j][i] = 0;
                      buttonReleased(j, i);
                  }
              }
              
            }
            // If j extension is a POTENTIOMETER matrix
            else if (_bRoles[j] == POTENTIOMETER)
            {
              potentiometerMoved(j, i, padRead);
            }
        }  
    }
}


/*
 * Function:    MiDispositivoMIDI_V3::buttonPressed
 * Description: Callback function when a button is pressed. It enters
 *              only once when the action happens. This function is
 *              only applicable when the extension is a pad one.

 * Input:
 *  -extension:  Extension that was pressed [0..3]
 *  -pad:        Pad that was pressed [0..15]

 * Output:
 *  -void
 */
void MiDispositivoMIDI_V3::buttonPressed(uint8_t extension, uint8_t pad)
{
    setRgbColors(_onColors, _ledColors[extension][pad]);
    noteOn(DEFAULT_MIDI_CHANNEL,
           _numberExtensions == 1 ? _midiNotes[extension][pad] + _currentPage * LEDPAD_NUM : _midiNotes[extension][pad],
           _midiVeloc[extension][pad]);

    MidiUSB.flush();
}

/*
 * Function:    MiDispositivoMIDI_V3::buttonReleased
 * Description: Callback function when a button is released. This function is
 *              only applicable when the extension is a pad one.

 * Input:
 *  -extension:  Extension that was pressed [0..3]
 *  -pad:        Pad that was pressed [0..15]

 * Output:
 *  -void
 */
void MiDispositivoMIDI_V3::buttonReleased(uint8_t extension, uint8_t pad)
{
    setRgbColors(_offColors, _ledColors[extension][pad]);
    noteOff(DEFAULT_MIDI_CHANNEL,
            _numberExtensions == 1 ? _midiNotes[extension][pad] + _currentPage * LEDPAD_NUM : _midiNotes[extension][pad],
            _midiVeloc[extension][pad]);

    MidiUSB.flush();
}

/*
 * Function:    MiDispositivoMIDI_V3::buttonHolded
 * Description: Callback function when the button is holded. This function
 *              is called while the button is holded on. This function is
 *              only applicable when the extension is a pad one.

 * Input:
 *  -extension:  Extension that was pressed [0..3]
 *  -pad:        Pad that was pressed [0..15]

 * Output:
 *  -void
 */
void MiDispositivoMIDI_V3::buttonHolded(uint8_t extension, uint8_t pad)
{
  /* Do some stuff. Feel free to inherit this function and set up your own callback
    _ledColors[extension][pad][0] > 1 ? _ledColors[extension][pad][0]-- : _ledColors[extension][pad][0] = DEFAULT_ON_RED;
    _ledColors[extension][pad][1] > 1 ? _ledColors[extension][pad][1]-- : _ledColors[extension][pad][1] = DEFAULT_ON_GREEN;
    _ledColors[extension][pad][2] > 1 ? _ledColors[extension][pad][2]-- : _ledColors[extension][pad][2] = DEFAULT_ON_BLUE;
  */
}

/*
 * Function:    MiDispositivoMIDI_V3::potentiometerMoved
 * Description: Callback function when a potentiometer is moved. This function is
 *              only applicable when the extension is a potentiometer one.

 * Input:
 *  -extension:  Extension that was pressed [0..3]
 *  -pad:        Pad that was pressed [0..15]

 * Output:
 *  -void
 */
void MiDispositivoMIDI_V3::potentiometerMoved(uint8_t extension, uint8_t pad, int value)
{
  /* TODO This is not tested with hardware yet
    noteOn(DEFAULT_MIDI_CHANNEL,
           _midiNotes[extension][pad],
           (value/1023) * 127);*/
    if (extension == 1)
    {
      if (pad == 0)
      {
        Serial.println(value);
      }
    }
    //MidiUSB.flush();
}

/*
 * Function:    MiDispositivoMIDI_V3::noteOn
 * Description: Sends a noteOn MIDI message.

 * Input:
 *  -channel:   MIDI Channel. Recommended 0.
 *  -pitch:     Note value, from 0 to 255
 *  -velocity:  Note intensity, form 0 to 255

 * Output:
 *  -void
 */

void MiDispositivoMIDI_V3::noteOn(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOn = {0x09, uint8_t(0x90 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
/*
 * Function:    MiDispositivoMIDI_V3::noteOff
 * Description: Sends a noteOff MIDI message.

 * Input:
 *  -channel:   MIDI Channel. Recommended 0.
 *  -pitch:     Note value, from 0 to 255
 *  -velocity:  Note intensity, form 0 to 255

 * Output:
 *  -void
 */
void MiDispositivoMIDI_V3::noteOff(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOff = {0x08, uint8_t(0x80 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

/*
* Function:    MiDispositivoMIDI_V3::updateLEDs
* Description: Light up each LED with the corresponding value

* Input:
*  -none

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::updateLEDs()
{
    for(int i=0; i<LEDPAD_NUM; i++)
    {
        for(int j = 0; j < _numberExtensions; j++)
        {
            ledPadStripe.setPixelColor((i+(j*LEDPAD_NUM)),
                                        _ledColors[j][i][1],
                                        _ledColors[j][i][0],
                                        _ledColors[j][i][2]);
        }
    }
    ledPadStripe.show();
}

/*
* Function:    MiDispositivoMIDI_V3::readPage
* Description: Read the current page, that will only change
*              if a lateral button is pressed.

* Input:
*  -none

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::readPage()
{
    uint8_t readSW_1 = digitalRead(SW_0_PIN);
    uint8_t readSW_2 = digitalRead(SW_1_PIN);
    if ((readSW_1 == HIGH) && (readSW_1 != lastSwitchRead[0]))
    {
        if (_currentPage < (MAX_NUMBER_OF_PAGES - 1))
        {
            _currentPage++;
        }
    }
    if ((readSW_2 == HIGH) && (readSW_2 != lastSwitchRead[1]))
    {
        if (_currentPage > 0)
        {
            _currentPage--;
        }
    }
    lastSwitchRead[0] = readSW_1;
    lastSwitchRead[1] = readSW_2;
}

/*
* Function:    MiDispositivoMIDI_V3::initializeLEDs
* Description: Initialize led matrix to off colors

* Input:
*  -none

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::initializeLEDs()
{
    for(int i = 0; i < LEDPAD_NUM; i++)
    { 
        for(int j = 0; j < _numberExtensions; j++)
        {
            setRgbColors(_offColors, _ledColors[j][i]);
        }
    }
}

/*
* Function:    MiDispositivoMIDI_V3::setMidiNotes
* Description: Set midi notes per button to be used.

* Input:
*  -notes      Contains the midi notes that will be sent per
*              button. Allowed value per note [0:127]

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setMidiNotes(uint8_t notes[MAX_EXTENSIONS][LEDPAD_NUM])
{
    for (int i = 0; i < LEDPAD_NUM; i++)
    {
        for (int j = 0; j < _numberExtensions; j++)
        {
             _midiNotes[j][i] = notes[j][i];
        }
    }
}

/*
* Function:    MiDispositivoMIDI_V3::setMidiVeloc
* Description: Set velocities per button to be used.

* Input:
*  -veloc      Velocities per button to be used. Each note will
*              be sent with the velocity determined by this array
*              Allowed value [0:127]

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setMidiVeloc(uint8_t veloc[MAX_EXTENSIONS][LEDPAD_NUM])
{
    for (uint8_t i = 0; i < LEDPAD_NUM; i++)
    {
        for (uint8_t j = 0; j < _numberExtensions; j++)
        {
             _midiVeloc[j][i] = veloc[j][i];
        }
    }
}

/*
* Function:    MiDispositivoMIDI_V3::setOnColors
* Description: Set which colors are displayed when a button is pressed

* Input:
*  -rgbColors  Array with red, green and blue colors used. Allowed
*              values [0:255]

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setOnColors(uint8_t rgbColors[RGB_COLORS])
{
    setRgbColors(rgbColors, _onColors);
}

/*
* Function:    MiDispositivoMIDI_V3::setOffColors
* Description: Set which colors are displayed when the buttons are off

* Input:
*  -rgbColors  Array with red, green and blue colors used. Allowed
*              values [0:255]

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setOffColors(uint8_t rgbColors[RGB_COLORS])
{
    setRgbColors(rgbColors, _offColors);
    initializeLEDs();
}

/*
* Function:    MiDispositivoMIDI_V3::setInterrupt
* Description: Configure and set an interrupt, with a function handler and
*              a time to call that function.
*              TODO: Explore other libraries to support multiple timers

* Input:
*  -functionPointer  Function that is called each "seconds"
*  -seconds          Time in seconds to call "functionPointer"

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setInterrupt(void (* functionPointer)(), double seconds)
{
    Timer1.initialize(seconds * USEC_IN_SEC);
    Timer1.attachInterrupt(functionPointer);
    interrupts();
}

/*
* Function:    MiDispositivoMIDI_V3::setPixelColor
* Description: Sets the RGB color of a given led in a given expansion.

* Input:
*  -expansion  Expansion to set the color [0:numberExtensions]
*  -led        Led to set the color [0:LEDPAD_NUM]. Note that the led 5 is
*              the first led of the second row.
*  -rgbColors  Array with red, green and blue colors used. Allowed
*              values [0:255]  

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setPixelColor(uint8_t extension, uint8_t led, uint8_t color[RGB_COLORS])
{
    setRgbColors(color, _ledColors[extension][led]);
}

/*
* Function:    MiDispositivoMIDI_V3::loop
* Description: Perform all relevant tasks of the device such as reading
*              which buttons are pushed, sending midi if pushed and updating
*              the leds according to the actions that were performed.

* Input:
*  -none

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::loop()
{
    readPADs();
    readPage();
    updateLEDs();
}

/*
* Function:    MiDispositivoMIDI_V3::setRgbColors
* Description: Copy the color content of one array to the other

* Input:
*  -copyFrom   rgb values that are used and copied into "copyTo", where
*              each element corresponds to red, green and blue.
*  -copyTo     Destination array.

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::setRgbColors(uint8_t copyFrom[RGB_COLORS], uint8_t copyTo[RGB_COLORS])
{
    for (int i = 0; i < RGB_COLORS; i++)
    {
        copyTo[i] = copyFrom[i];
    }
}

/*
* Function:    MiDispositivoMIDI_V3::generateNotesVeloc
* Description: Generates notes and velocities for a given number of
               extensions

* Input:
*  -numberExtensions  Existing number of extensions

* Output:
*  -void
*/
void MiDispositivoMIDI_V3::generateNotesVeloc(uint8_t numberExtensions)
{
    int note = 30;
    for (int i = 0; i < numberExtensions; i++)
    {
        for (int j = 0; j < LEDPAD_NUM; j++)
        {
            _midiNotes[i][j] = note;
            note = note + 1;
        }

    }

    for (int i = 0; i < LEDPAD_NUM; i++)
    {
        for (int j = 0; j < numberExtensions; j++)
        {
             _midiVeloc[j][i] = DEFAULT_VELOCITY;
        }
    }
}
