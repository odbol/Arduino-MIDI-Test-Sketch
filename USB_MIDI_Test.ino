// this example sends a note-on and note-off every two seconds
// taken from README of https://github.com/rkistner/arcore

#include "MIDI_Shim.h"

#define PIN_DIGIT_0 44

#define MIDI_LED_PIN 13

// mirror the same output to the SerialUSB so we can check if both are sending correctly.
#ifdef MIDI_ENABLED
#  define SERIAL_MIRRORING_TEST 1
#else
#  define SERIAL_MIRRORING_TEST 0
#endif

// for stress-testing serial interface, 
// to make sure it doesn't drop any notes when sending a lot in quick succession
// set to anything <= 79, or set to 1 for basic test
#define MAX_NOTES_TO_SEND_AT_ONCE 1

// starting note pitch
#define START_NOTE 48

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MIDIUSB_write(noteOn);

#if SERIAL_MIRRORING_TEST
  // also send over SerialUsb
  SerialUSB.write((const char*)&noteOn, 4);
#endif  
}

void noteOff(byte channel, byte pitch, byte velocity) {
  MIDIEvent noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MIDIUSB_write(noteOff);
  
#if SERIAL_MIRRORING_TEST
  // also send over SerialUsb
  SerialUSB.write((const char*)&noteOff, 4);
#endif  
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  MIDIEvent event = {0x0B, 0xB0 | channel, control, value};
  MIDIUSB_write(event);
}

int velocity = 70;

void loop() {
  digitalWrite(MIDI_LED_PIN, HIGH);
  
  for (int note = START_NOTE; note < MAX_NOTES_TO_SEND_AT_ONCE + START_NOTE; note++) {
    noteOn(0, note, velocity);   // Channel 0, middle C, normal velocity
  }
  //MIDIUSB_flush();
  delay(500);

  digitalWrite(MIDI_LED_PIN, LOW);
  for (int note = START_NOTE; note < MAX_NOTES_TO_SEND_AT_ONCE + START_NOTE; note++) {
    noteOff(0, note, velocity);  // Channel 0, middle C, normal velocity
  }
  //MIDIUSB_flush();
  delay(1500);

  velocity = (velocity + 1) % 127; 
  // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
}

void setup() {

   pinMode(MIDI_LED_PIN, OUTPUT);  
   
   // this just activates the MIDI_LED_PIN for the DrumPants.
   // it should have no effect on a normal Arduino Due.
   pinMode(PIN_DIGIT_0, OUTPUT); 
   digitalWrite(PIN_DIGIT_0, HIGH);  
}
