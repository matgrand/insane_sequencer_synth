#ifndef midi_stuff_h
#define midi_stuff_h


// #include "low_level_stuff.h"
#include "synth.h"

//MIDI
// DESCRIZIONE: l'implementazione del MIDI deve comprendere tutti i vari aspetti del controllore,
// deve avere la possibilita' di selezionare tutte le possibili uscite (OUTUSB, OUTBLE, OUT1, OUT2, SYNTH)
// e analogamente deve gestire tutti gli ingressi (IN1, IN2, INUSB,(INBLE))

// tutto cio' che usa MIDI1 e MIDI2  deve essere qui, copresi i test

void initializeMIDI();
 
void getNextMidiPkt(byte _tmpMidiPkt[PKT_LENGTH]); //place on it the next midiPkt in queue
        
void playMidiPkt(byte _tmpMidiPkt[PKT_LENGTH]); //play the packet to the correct device

void noteOn(byte _device, byte _note, byte _vel, byte _channel);
void noteOff(byte _device, byte _note, byte _vel, byte _channel);
//... //other midi methods for higher classes

void midi_read();


#endif