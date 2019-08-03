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

enum Scale {RUMBITA, BACH, PACO, CHINESE};

class MiDispositivoMIDIAutomelody : public MiDispositivoMIDI_V3 {

public: MiDispositivoMIDIAutomelody(uint8_t numPages, uint8_t numExtensions): MiDispositivoMIDI_V3(numPages, numExtensions)
{
    _transposition = 0;
    _current_octave = _origin_octave;


    setTransModel(BACH);
    initializeLEDs();
}

protected:
    const uint8_t _origin_chroma = 0;
    const uint8_t _origin_octave = 5;
    
    uint8_t _current_octave;
    int16_t _transposition;

    uint8_t _current_note;

    int8_t _note_shift[8] = {-7, -4, -3, -1,
                              7,  4,  3,  1};

    uint8_t _COLOR_SCALE_ON[3] = {128, 0, 128};
    uint8_t _COLOR_SCALE_OFF[3] = {20, 0, 20};

    Scale _scale;

    const uint8_t (*_trans_model)[12];
    const uint8_t *_chroma_probs;

    const uint8_t _trans_model_1[12][12] = {
        {71, 18, 0,  0, 0, 05,  0, 0, 40,  0,  9, 0},
        { 5, 50, 0,  7, 0,  0,  0, 0,  0,  0,  9, 0},
        { 0,  0, 0,  0, 0,  0,  0, 0,  0,  0,  0, 0},
        {13,  5, 0, 77, 0, 10,  6, 0,  0,  0,  0, 0},
        { 0,  0, 0,  0, 0,  0,  0, 0,  0,  0,  0, 0},
        { 3, 23, 0,  2, 0, 79,  0, 0,  0,  0,  2, 0},
        { 0,  0, 0,  0, 0,  5, 83, 0,  5,  0,  0, 0},
        { 0,  0, 0,  0, 0,  0,  0, 0,  0,  0,  0, 0},
        { 0,  0, 0, 13, 0,  0, 11, 0, 50,  0,  0, 0},
        { 0,  0, 0,  2, 0,  0,  0, 0,  0, 50,  0, 0},
        { 8,  3, 0,  0, 0,  2,  0, 0,  5, 50, 81, 0},
        { 0,  0, 0,  0, 0,  0,  0, 0,  0,  0,  0, 0}
        };

    const uint8_t _chroma_probs_1[12] = {22, 8, 0, 22, 0, 15, 6, 0, 7, 0, 16, 0};

    const uint8_t _trans_model_2[12][12] = {
        {54,  6,  2,  2,  4,  9,  0,  4,  7,  1, 11,  0},
        {13, 47,  0,  6,  7,  7,  1,  3,  2,  0, 14,  0},
        { 8,  1, 48, 12,  9, 10,  1,  2,  1,  0,  9,  1},
        { 7, 11,  3, 50,  0,  9,  2,  5,  7,  1,  5,  0},
        {11,  8,  1,  0, 45, 13,  0, 18,  1,  0,  2,  0},
        {11,  5,  1,  6,  4, 50,  2,  6,  6,  2,  7,  0},
        { 1,  2,  1, 12,  1, 18, 49,  0, 10,  1,  6,  0},
        { 8,  2,  0,  3, 10,  7,  0, 49, 10,  1, 11,  0},
        {10,  3,  1,  3,  1, 11,  2,  9, 48,  0, 11,  0},
        {10,  5,  0,  2,  0, 17,  3,  2,  2, 45, 14,  2},
        {14,  6,  2,  3,  2,  5,  1,  9,  7,  2, 49,  0},
        {13,  0,  7,  3,  0,  0,  0, 17,  3,  7,  7, 43}
        };

    const uint8_t _chroma_probs_2[12] = {19, 9, 2, 7, 7, 15, 2, 12, 1, 2, 15, 0};


    const uint8_t _trans_model_3[12][12] = {
        {69,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0, 30},
        { 4, 10, 0, 21,  0, 11,  2,  0, 28,  0,  3, 20},
        { 0, 25, 2, 38,  0,  4,  0,  0,  0,  0, 21, 10},
        { 0, 14, 7,  5,  0, 10,  4,  0, 43,  0, 14,  4},
        { 0,  0, 0, 78,  0, 22,  0,  0,  0,  0,  0,  0},
        { 0,  5, 1, 24,  2,  6, 14,  0,  6,  0, 40,  1},
        { 0,  3, 0, 16,  0, 46,  6,  0, 15,  0,  6,  7},
        { 0,  0, 0,  0,  0,  0, 17,  0, 67,  0, 17,  0},
        { 0, 11, 0, 37,  0,  7, 18,  1,  2, 18,  5,  0},
        { 1,  0, 0,  1,  0,  0,  1,  0,  1,  1, 88,  7},
        { 1,  1, 0, 27,  0, 34,  7,  0, 10,  4,  6, 10},
        {59,  4, 0,  1,  0,  2,  3,  0,  1,  0, 10, 21}};


    const uint8_t _chroma_probs_3[12] = {31, 4, 1, 11, 0, 8, 5, 0, 9, 2, 10, 15};


    const uint8_t _trans_model_4[12][12] = {
        {19, 0,  44, 0, 3,   6, 0,   9, 0,  16,  3,  0},
        { 0, 0,   0, 0, 0,   0, 0,   0, 0,   0,  0,  0},
        {56, 0,  11, 1, 5,  18, 0,   3, 0,   5,  1,  1},
        {22, 0,  33, 0, 0,  33, 0,  11, 0,   0,  0,  0},
        {14, 0,  27, 0, 7,   4, 0,  38, 0,   7,  0,  4},
        {22, 0,  20, 1, 5,  23, 0,  20, 0,   9,  1,  0},
        { 0, 0,   0, 0, 0, 100, 0,   0, 0,   0,  0,  0},
        {12, 0,  12, 0, 6,  12, 0,  17, 0,  39,  1,  1},
        { 0, 0,   0, 0, 0,   0, 0, 100, 0,   0,  0,  0},
        {28, 0,   7, 0, 3,  10, 0,  26, 0,  15, 11,  0},
        {23, 0,   3, 0, 0,   1, 0,   1, 0,  72,  0,  0},
        { 7, 0,   0, 0, 0,   0, 0,   0, 0,  79,  0, 14}};

    const uint8_t _chroma_probs_4[12] = {27, 0, 20, 0, 4, 11, 0, 14, 0, 17, 2, 0};


    void setTransModel(Scale scale) {
        _scale = scale;

        switch (_scale)
        {
        case RUMBITA:
            _trans_model = _trans_model_1;
            _chroma_probs = _chroma_probs_1;
            break;

        case BACH:
            _trans_model = _trans_model_2;
            _chroma_probs = _chroma_probs_2;
            break;

        case PACO:
            _trans_model = _trans_model_3;
            _chroma_probs = _chroma_probs_3;
            break;

        case CHINESE:
            _trans_model = _trans_model_4;
            _chroma_probs = _chroma_probs_4;
            break;
        
        default:
            break;
        }
    };

    void initializeLEDs()
    {
        // Note  pads
        for(uint8_t i = 0; i < 8; i++)
        {
            for(uint8_t j = 0; j < _numberExtensions; j++)
            {
                setRgbColors(_offColors, _ledPad[j][i]);
            }
        }

        // Octave PADs
        updateOctaveLEDs();
        updateTranspositionLEDs();

        // Scale pads
        for(uint8_t i = 12; i < 16; i++)
        {
            for(uint8_t j = 0; j < _numberExtensions; j++)
            {
                setRgbColors(_COLOR_SCALE_OFF, _ledPad[j][i]);
            }
        }
        setRgbColors(_COLOR_SCALE_ON, _ledPad[0][12 + _scale]);
    }

    void PADaction(PADstatus status, uint8_t extension, uint8_t pad)
    {
        if (status == ONSET)
        {
            if (pad < 8)
            {
                setRgbColors(_onColors, _ledPad[extension][pad]);
                updateNote(extension, pad);
            }
            else if (pad >= 8 && pad < 10)
            {
                /* Octave trainsposition */
                if (pad == 8)
                {
                    _current_note += 12;
                }
                if (pad == 9)
                {
                    _current_note -= 12;
                }
                _current_octave = _current_note / 12;

                updateOctaveLEDs();
                return;
            }
            else if (pad >= 10 && pad < 12)
            {
                /* Semitone Transposition */
                if (pad == 10)
                {
                    if (_transposition < 12)
                    {
                        _transposition++;
                        _current_note++;
                    }
                }
                if (pad == 11)
                {
                    if (_transposition > -12)
                    {
                        _transposition--;
                        _current_note--;
                    }
                }
                _current_octave = _current_note / 12;

                updateTranspositionLEDs();
                return;
            }
            else if (pad >= 12 && pad < 16)
            {
                /* Scale control */
                setRgbColors(_COLOR_SCALE_OFF, _ledPad[extension][12 + _scale]);
                setRgbColors(_COLOR_SCALE_ON, _ledPad[extension][pad]);
                setTransModel(pad - 12);
                return;
            }

        }
        else if (status == OFFSET)
        {
            if (pad < 8)
            {
                setRgbColors(_offColors, _ledPad[extension][pad]);
            }
        }
        else if (status == ON)
        {
            // nothing to do in this case
            return;
        }

        note(status,
             _midiChannel,
             _midiNotes[extension][pad],
             _midiVeloc[extension][pad]);
    }

    void updateNote(uint8_t extension, uint8_t pad)
    {
        // update input note and bounds
        uint8_t in_note = _current_note + _note_shift[pad];
        uint8_t in_octave = in_note / 12;
        uint8_t in_chroma = in_note % 12;

        // Compute the cumulative probalities.
        uint32_t cumulative_probs[12] = {0};
        for (size_t i = 0; i < 12; i++)
        {
            cumulative_probs[i] = _chroma_probs[i] * _trans_model[in_chroma][i];

            if (i > 0)
            {
                cumulative_probs[i] += cumulative_probs[i - 1];
            }
        }

        // Random sampling.
        long sample = random(cumulative_probs[11] + 1);

        int min_distance = cumulative_probs[11];
        int distance = cumulative_probs[11];
        uint8_t out_chroma = 0;

        // Check which note was attached to the "winner" bin.
        for (size_t i = 0; i < 12; i++)
        {
            distance = cumulative_probs[i] - sample;
            if ((distance > 0) && (distance < min_distance))
            {
                min_distance = distance;
                out_chroma = i;
            }
        }


        uint8_t out_note = out_chroma + in_octave * 12;

        // Center the note in the target range
        // if we wanted a lower note make sure it is a lower note,
        // other wase octave it
        if (pad < 4)
        {
            if (out_note > in_note)
            {
                out_note -= 12;
            }
        }

        // Same for the higher case.
        if (pad >= 4)
        {
            if (out_note < in_note)
            {
                out_note += 12;
            }
        }

        // update variables
        _current_note = out_note;
        _current_octave = out_note / 12;

        // Save the note with the current transposition value
        _midiNotes[extension][pad] = _current_note + _transposition;

        // update octave LEDs
        updateOctaveLEDs();
    }

    // octave 5 stands for C4
    // going up will increasingly light
    // right octave PAD and goung down
    // will increasingly light up the
    // left octave PAD.
    void updateOctaveLEDs()
    {
        if (_current_octave < 5)
        {
            uint8_t intensity = (4 - _current_octave) * 63.75f;
            uint8_t color[3] = {0, 0, intensity};
            setRgbColors(color, _ledPad[0][9]);
        }
        if (_current_octave > 5)
        {
            // MIDI messages can go up to octave 11.
            // Clip the signal so we don't exceed 
            // uint8 range.
            uint8_t octave_clipped = min(_current_octave, 9);

            uint8_t intensity = (octave_clipped - 5) * 63.75f;
            uint8_t color[3] = {0, 0, intensity};
            setRgbColors(color, _ledPad[0][8]);
        }
        if (_current_octave == 5)
        {
            uint8_t color[3] = {0, 0, 0};
            setRgbColors(color, _ledPad[0][8]);
            setRgbColors(color, _ledPad[0][9]);
        }
    }


    void updateTranspositionLEDs()
    {
        if (_transposition < 0)
        {
            uint8_t intensity = -_transposition * 21.25f;
            uint8_t color[3] = {0, intensity, 0};
            setRgbColors(color, _ledPad[0][11]);
        }
        if (_transposition > 0)
        {
            // MIDI messages can go up to octave 11.
            // Clip the signal so we don't exceed 
            // uint8 range.
            uint8_t intensity = _transposition * 21.25f;
            uint8_t color[3] = {0, intensity, 0};
            setRgbColors(color, _ledPad[0][10]);
        }
        if (_transposition == 0)
        {
            uint8_t color[3] = {0, 0, 0};
            setRgbColors(color, _ledPad[0][10]);
            setRgbColors(color, _ledPad[0][11]);
        }
        
    }
};

MiDispositivoMIDIAutomelody mdma = MiDispositivoMIDIAutomelody(4, 1);

void setup()
{
    Serial.begin(9600);
}

void loop() {
  mdma.loop();
}
