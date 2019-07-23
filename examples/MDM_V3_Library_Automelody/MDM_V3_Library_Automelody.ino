/********************************************************************
 *                       MiDispositivoMIDI V3                       *
 ********************************************************************
 * 2018 BanaNut Electronics                                         *
 * bananutelectronics@gmail.com                                     *
 *                                                                  *
 * See license and documentation in the repository.                 *
 * https://github.com/bananut-electronics                           *
 *                                                                  *
 * Code:     MDM_V3_Library_ConfigureNotes.ino                      *
 * Hardware: One MiDispositivoMIDI V3 4x4 board                     *
 * Brief:    This code allows you to configure which note is going  *
 *           going to be sent when each button is pressed. Note     *
 *           that by default some notes are assigned                *
 *                                                                  *
 ********************************************************************/

// Include the library
#include "MiDispositivoMIDI_V3.h"


class MiDispositivoMIDIAutomelody : public MiDispositivoMIDI_V3 {

public: MiDispositivoMIDIAutomelody(uint8_t numPages, uint8_t numExtensions): MiDispositivoMIDI_V3(numPages, numExtensions) {}

protected:
    const uint8_t _origin_chroma = 0;
    const uint8_t _origin_octave = 5;
    
    uint8_t _current_chroma = _origin_chroma;
    uint8_t _current_octave = _origin_octave;

    uint8_t _current_note = _current_chroma + _current_octave * 12;

    uint8_t _low_bound  = _current_note - 7;
    uint8_t _high_bound = _current_note + 7;

    uint8_t _note_shift[4]  = {-7, -5, 5, 5};
    uint8_t _bound_shift[4] = {-7, 0, 0, 7};

    uint8_t _DEBUG_RED[3] = {1, 0, 0};

    const float _trans_model1[12][12] = {
    {0.71, 0.18, 0.00, 0.00, 0.00, 0.05, 0.00, 0.00, 0.40, 0.00, 0.09, 0.00},
    {0.05, 0.50, 0.00, 0.07, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.09, 0.00},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
    {0.13, 0.05, 0.00, 0.77, 0.00, 0.10, 0.06, 0.00, 0.00, 0.00, 0.00, 0.00},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
    {0.03, 0.23, 0.00, 0.02, 0.00, 0.79, 0.00, 0.00, 0.00, 0.00, 0.02, 0.00},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.05, 0.83, 0.00, 0.05, 0.00, 0.00, 0.00},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
    {0.00, 0.00, 0.00, 0.13, 0.00, 0.00, 0.11, 0.00, 0.50, 0.00, 0.00, 0.00},
    {0.00, 0.00, 0.00, 0.02, 0.00, 0.00, 0.00, 0.00, 0.00, 0.50, 0.00, 0.00},
    {0.08, 0.03, 0.00, 0.00, 0.00, 0.02, 0.00, 0.00, 0.05, 0.50, 0.81, 0.00},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00}
    };

    const float _chroma_probs[12] = {0.083, 0.083, 0.083, 0.083,
                                   0.083, 0.083, 0.083, 0.083,
                                   0.083, 0.083, 0.083, 0.083};

    void PADaction(PADstatus status, uint8_t extension, uint8_t pad)
    {
        if (status == ONSET)
        {

            setRgbColors(_onColors, _ledPad[extension][pad]);
            updateNote(extension, pad);

        }
        else if (status == OFFSET)
        {
            setRgbColors(_offColors, _ledPad[extension][pad]);
        }
        else if (status == ON)
        {            
            // nothing to do in this case
            return;
        }

        note(status,
             _midiChannel,
             _numberExtensions == 1 ? _midiNotes[extension][pad] + _currentPage * LEDPAD_NUM : _midiNotes[extension][pad],
             _midiVeloc[extension][pad]);
    }

    void updateNote(uint8_t extension, uint8_t pad)
    {
        // update input note and bounds
        uint8_t in_note = _current_note + _note_shift[pad];
        uint8_t in_octave = in_note / 12;
        uint8_t in_chroma = in_note % 12;

        _low_bound += _bound_shift[pad];
        _high_bound += _bound_shift[pad];

        // compute new note
        float prev_prob = 0;
        uint8_t out_chroma = 0;

        for (size_t i = 0; i < 12; i++)
        {
            float current_prob = _chroma_probs[in_chroma] * _trans_model1[in_chroma][i];
            if (i != in_chroma)
            {
                if (current_prob > prev_prob)
                {
                    out_chroma = i;
                }
            }
            

            prev_prob = current_prob;
        }

        uint8_t out_note = out_chroma + in_octave * 12;

        // center the note in the target range

        if (out_note < _low_bound)
        {
            out_note += 12;
        }

        if (out_note > _high_bound)
        {
            out_note -= 12;
        }
        
        if (out_note > _low_bound) setRgbColors(_DEBUG_RED, _ledPad[extension][pad]);
        if (out_note < _high_bound) setRgbColors(_DEBUG_RED, _ledPad[extension][pad]);;

        // update variables
        _current_note = out_note;
        _current_octave = out_note / 12;
        _current_chroma = out_note % 12;

        _midiNotes[extension][pad] = _current_note;
    }
};

MiDispositivoMIDIAutomelody mdma = MiDispositivoMIDIAutomelody(4, 1);

void setup() {
  // Serial.begin(9600);  //  debugging output

  // Assign MIDI notes to buttons. Note that if you are using
  // more than one expansion (more than one 4x4 device) you will
  // need to change the size of this array. Always 1 value per button.
  uint8_t midiNotes [1][16] = {{ 30, 34, 50, 33,
                                100, 35, 36, 37,
                                120, 39, 40, 41,
                                 30, 43, 44, 45}};
  mdma.setMidiNotes(midiNotes);

  // Same with the so called velocities of the notes. This means the
  // intensity of each note.
  uint8_t midiVelocities [1][16] = {{30, 30, 30, 30,
                                    127, 35, 36, 37,
                                    120, 39, 40, 41,
                                     30, 43, 44, 45}};

  mdma.setMidiVeloc(midiVelocities);
}

void loop() {

  // Run the loop. This will read which buttons are pressed and
  // execute different actions, such as sending the MIDI messages
  mdma.loop();
}
