/*
Based on the following project:
Nano 12 Note Poly MIDI Keyboard
https://diyelectromusic.wordpress.com/2020/06/05/arduino-nano-midi-keyboard/

MIT License

Copyright (c) 2020 diyelectromusic (Kevin)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHERIN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
  Using principles from the following Arduino tutorials:
    Button               - http://www.arduino.cc/en/Tutorial/Button
    Arduino MIDI Library - https://github.com/FortySevenEffects/arduino_midi_library
*/
#include <MIDI.h>
#include "PCF8574.h"

// This is required to set up the MIDI library.
// The default MIDI setup uses the Arduino built-in serial port
// which is pin 1 for transmitting on the Arduino Uno.
MIDI_CREATE_DEFAULT_INSTANCE();

// Define which MIDI channel to transmit on (1 to 16)
int midiChannel = 1;

// Set up the input and output pins to be used for the keys
#define NUM_OCTAVES 5
#define NOTES_PER_OCTAVE 7

// One I/O expander for each octave
// We find each expander using its I2C address, and put them in this array
PCF8574 expanders[NUM_OCTAVES] = {
  -1, // The first octave is directly conencted, so doesn't have an expander
  PCF8574(0x20),
  PCF8574(0x21),
  PCF8574(0x22),
  PCF8574(0x23)
}

// The pins on the I/O expander that correspond to each note index
int octave_pins[NOTES_PER_OCTAVE] = {
  P0, P1, P2, P3, P4, P5, P6
};

// Set the MIDI codes for the notes to be played by each key
int notes[NUM_OCTAVES][NOTES_PER_OCTAVE] = {
  {29, 31, 33, 35, 36, 38, 40}, // F1 to E2 (first octave) (directly connected to Arduino)
  {41, 43, 45, 47, 48, 50, 52}, // F2 to E2
  {53, 55, 57, 59, 60, 62, 64}, // F3 to E3
  {65, 67, 69, 71, 72, 74, 76}, // F4 to E4
  {77, 79, 81, 83, 84, 86, 88}, // F5 to E5
};

// The current octave playing
int octave_playing = -1;
// The current key index within the octave playing
int key_playing = -1;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);

  // This updates speed of the serial channel so that it can be used
  // with a PC rather than a MIDI port.
  // This can be used with software such as the Hairless MIDI Bridge
  // and loopMidi during testing.
  // It is commented out for "real" use.
  //Serial.begin (115200);

  // Initialise the input buttons as piano "keys"
  for (int i = 0; i < NUM_OCTAVES; i++) {
    for (int j = 0; j < NOTES_PER_OCTAVE; j++) {
      // Set up the pin for the key
      int pin = octave_pins[j];
      expanders[i].pinMode(pin, INPUT_PULLUP);
    }
  }
}

void loop() {
  // Check each button and if pressed play the note.
  // Loop through each octave
  for (int i = 0; i < NUM_OCTAVES; i++) {
    // Check each button/key/note in this octave
    for (int j = 0; j < NOTES_PER_OCTAVE; j++) {
      // Read the state of the button
      int pin = octave_pins[j];
      uint8_t value = expanders[i].digitalRead(pin);
      if (value == LOW) {
        // Button IS pressed
        if (octave_playing != i || key_playing != j) {
          // Unpressed-to-pressed transition
          if (octave_playing != -1) {
            // Stop the previous note
            MIDI.sendNoteOff(notes[octave_playing][key_playing], 0, midiChannel);
          }
          // Start the new note
          MIDI.sendNoteOn(notes[i][j], 127, midiChannel);
          octave_playing = i;
          key_playing = j;
        }
      } else {
        // Button is NOT pressed
        if (octave_playing == i && key_playing == j) {
          // Pressed-to-unpressed transition
          MIDI.sendNoteOff(notes[i][j], 0, midiChannel);
          octave_playing = -1;
          key_playing = -1;
        }
      }
    }
  }
}