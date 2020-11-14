#include "midi_stuff.h"


//midi
byte channel1 = 1;
byte channel2 = 2;
byte vel1 = 127;
byte vel2 = 127;

struct Serial1MIDISettings : public midi::DefaultSettings
{
  static const long BaudRate = 31250;
  static const int8_t RxPin = 3;
  static const int8_t TxPin = 1;
};
struct Serial2MIDISettings : public midi::DefaultSettings
{
  static const long BaudRate = 31250;
  static const int8_t RxPin = 16;
  static const int8_t TxPin = 17;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI1, Serial1MIDISettings);
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial2, MIDI2, Serial2MIDISettings);


//MIDI IN HANDLES METHODS
void noteOn1(byte channel, byte pitch, byte velocity) {
  Serial.print(pitch);
  Serial.println(" ON");
}

void noteOff1(byte channel, byte pitch, byte velocity) {
  Serial.print(pitch);
  Serial.println(" OFF");
}

void noteOn2(byte channel, byte pitch, byte velocity) {
  Serial.print(pitch);
  Serial.println(" ON");
}

void noteOff2(byte channel, byte pitch, byte velocity) {
  Serial.print(pitch);
  Serial.println(" OFF");
}


void initializeMIDI() { //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  //MIDI1.begin(MIDI_CHANNEL_OMNI); // o questo o serial monitor
  MIDI2.begin(MIDI_CHANNEL_OMNI);
  MIDI1.setHandleNoteOn(noteOn1);
  MIDI2.setHandleNoteOn(noteOn2);
  MIDI1.setHandleNoteOff(noteOff1);
  MIDI2.setHandleNoteOff(noteOff2);
  //all the other setHandle
}


void getNextMidiPkt(byte _tmpMidiPkt[PKT_LENGTH]) //place on it the next midiPkt in queue
{

}   

void playMidiPkt(byte _tmpMidiPkt[PKT_LENGTH]) //play the packet to the correct device
{

}

void noteOn(byte _device, byte _note, byte _vel, byte _channel) {
  if (_device == 1) {
    MIDI1.sendNoteOn(_note,_vel, _channel);
  }
  else if (_device == 2) {
    MIDI2.sendNoteOn(_note,_vel, _channel);
  }
  else return;
}

void noteOff(byte _device, byte _note, byte _vel, byte _channel) {
  if (_device == 1) {
    MIDI1.sendNoteOff(_note,_vel, _channel);
  }
  else if (_device == 2) {
    MIDI2.sendNoteOff(_note,_vel, _channel);
  }
  else return;
}
//... //other midi methods for higher classes

void midi_read() {
  MIDI1.read();
  MIDI2.read();
  //other midi.read()
}








