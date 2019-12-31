# Automelody Example
This example generates MIDI melodies based on a probabilistic model. When a `note button` is pressed, a new note with an automatically estimated pitch is produced.  The rest of the buttons on the board are intended to select between different melody models and to change the octave and root key.

## Melody models
The models were created by accumulating note bigrams from the MIDI transcriptions of different music pieces. The most repeated note is considered to be the root and the bigrams are normalized to C. 

There are 4 models:
- **RUMBITA.** From a popular Spanish rumba song.
- **BACH.** From Toccata and Fugue in D minor by J.S. Bach.
- **PACO.** From Paco de Lucia's Entre Dos Aguas.
- **CHINESE.** From a traditional Chinese composition.

On play-time, `note buttons` tell the system to generate a new note that is up to `N` semitones higher or lower than the previous one. `N` depends on which button was pressed. The system is not deterministic because it works by sampling from the probability distribution of all the notes on that range. 

The probability for each note is updated on each iteration depending on 3 factors: 
- **Note probability.** From the note counts from the bigrams.
- **Transition probability.** The probability of the note given the previous one.
- **The controlling factor.** This depends on which button was pressed. It penalizes notes that are far from `N`. Its goal is to make the system more controllable.

## Usage
### Note buttons
When `buttons 0 to 7` are pressed, a new note is generated. Buttons 0 to 3 generate a note that is lower than the previous one. The difference among them is the range of allowed notes that can be produced. 
- 0: [0, -2] semitones
- 1: [0, -4] semitones
- 2: [0, -7] semitones
- 3: [0, -10] semitones

This means that while `button 0` only allows the same, one or two semitones lower, `button 3` may produce almost all the notes in the scale. However, due to the controlling factor, values closer to -10 semitones will have a greater priority. 

`Buttons 4 to 7` have the same behavior for higher notes.

### Transposition buttons
By default, the model operates in C. Use the `buttons 10 to 11` to shift the root note by steps of one semitone up/down.

### Octave shift buttons
Use `buttons 8 to 9` to transpose the pitch one octave up/down. 

### Model buttons
4 different models can be exchanged in real-time. `Buttons 12 to 15` allow exchanging among models:
- 12 -> RUMBITA
- 13 -> BACH
- 14 -> PACO
- 15 -> CHINESE

## Future work
- Improve the models by using several tracks from the same style for each one. 
- Think on new ways of controlling the note generation process.

## Board layout
For the sake of clarity, the following table shows the position of the buttons on the board when the USB connection is pointting downwards.

|               |    |    |    |    |
|       --      | -- | -- | -- | -- |
| Model buttons | 15 | 14 | 13 | 12 |
| Oct. / trans. | 11 | 10 |  9 |  8 |
| Note buttons  | 7  |  6 |  5 |  4 |
| Note buttons  | 3  |  2 |  1 |  0 |
