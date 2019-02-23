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
#include <MiDispositivoMIDI_V3.h>

// Creates a mdm with four pages and only one expansion
MiDispositivoMIDI_V3 mdm = MiDispositivoMIDI_V3(4, 1);

void setup() {

  // Assign MIDI notes to buttons. Note that if you are using
  // more than one expansion (more than one 4x4 device) you will
  // need to change the size of this array. Always 1 value per button.
  uint8_t midiNotes [1][16] = {{ 30, 34, 50, 33,
                                100, 35, 36, 37,
                                120, 39, 40, 41,
                                 30, 43, 44, 45}};
  mdm.setMidiNotes(midiNotes);

  // Same with the so called velocities of the notes. This means the
  // intensity of each note.
  uint8_t midiVelocities [1][16] = {{30, 30, 30, 30,
                                    127, 35, 36, 37,
                                    120, 39, 40, 41,
                                     30, 43, 44, 45}};

  mdm.setMidiVeloc(midiVelocities);
}

void loop() {

  // Run the loop. This will read which buttons are pressed and
  // execute different actions, such as sending the MIDI messages
  mdm.loop();
}
