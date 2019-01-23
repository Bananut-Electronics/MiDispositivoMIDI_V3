/*
    MiDispositivoMIDI V3 Rev.A3C
    --------EXERCISE 4--------

    Goal: Experiment with basic MIDI messages such as noteOn and noteOff. You
    will need a program like Ableton live.

    Proposed exercises: Modify the note and velocity of noteOn function

    Hardware: You will need a MiDispositivoMIDI V3 Rev A3C, without any expansions.
*/
// Library Import
#include "MIDIUSB.h"
void setup() {
  // Init the serial com
  Serial.begin(115200);
}

void loop() {
  // Send a note on
  noteOn(0, 48, 127);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  // Waits for 500 ms
  delay(500);
  // Send a note off
  noteOff(0, 48, 0);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(500);

  noteOn(0, 50, 127);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(500);
  noteOff(0, 50, 0);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(500);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
