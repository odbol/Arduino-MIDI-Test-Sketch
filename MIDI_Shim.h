// this is a shim to provide an interface similar to the 
// USB-MIDI library: https://github.com/rkistner/arcore
// Ideally, the USB-MIDI implementation would define these 
// in the Arduino source itself.
#include <arduino.h>

#ifdef MIDI_ENABLED
typedef midiEventPacket_t MIDIEvent;
#else
typedef struct
{
uint8_t type; // this is the header: Cable Number | Code Index Number (from MIDI USB spec)
// these are the data bytes:
uint8_t m1;
uint8_t m2;
uint8_t m3;
} MIDIEvent;

#endif

// this should be replaced with an actual USB MIDI class that inherits from Serial
// for now, to be compatibile with a standard Arduino, we just send over the SerialUSB of the Due.

void MIDIUSB_write(MIDIEvent e) {
#ifdef MIDI_ENABLED
  MidiUSB.write((const uint8_t*)&e, 4);
#else
   SerialUSB.write(e.type);
   SerialUSB.write(e.m1);
   SerialUSB.write(e.m2);
   SerialUSB.write(e.m3);
#endif   
}

void MIDIUSB_flush() {
#ifdef MIDI_ENABLED
  MidiUSB.flush();
#else  
  SerialUSB.flush();
#endif  
}
