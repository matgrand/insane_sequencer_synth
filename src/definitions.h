#ifndef definitions_h
#define definition_h

//all libraries we need

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <MIDI.h>
#include "SPI.h"
#include "FS.h"
#include "SD.h"
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "OLEDDisplayUi.h"
#include "driver/i2s.h"                 // Library of I2S routines, comes with ESP32 standard install

//COMMON DEFINITIONS

//this is a new defintion just to test if github integration works
#define GITHUB 420

#define EMPTY 256

//buttons
#define ALL_BUTTONS 28
#define VALUES 26
#define NUMBERS 10
#define NOTE_BUTTONS 24

#define I2C_FREQ 3400000 //400000 slow //3400000 fast

#define PKT_LENGTH 16
#define MPR1 0x5A         //touch1
#define MPR2 0x5C         //touch2

//////////////
// COMMANDS //
//////////////

//  No command, default command, do nothing
#define noCommand 0

//MIDI
//midi input command
#define midiIN 1 // [midiIN, midi_device, midi_packet]
// midi output command
#define midiOUT 2 // [midiOUT, midi_device, midi_packet]
// midi devices
#define MIDI_USB 3
#define MIDI_SYNTH 4     // [MIDI_SYNTH, midi_command, ]
#define MIDI_1 5
#define MIDI_2 6
#define MIDI_BLE 7

//SEQUENCER
#define startPlay 9 //esp
#define stopPlay 10 //esp

//NANO/SYNTH
//pots
#define POTS 17     //chiedi al nano il valore dei pots, sui byte 1,2,3,4,
//midi commands
#define NOTE_ON 11 //[MIDI_SYNTH, NOTE_ON, note_number, velocity]
#define NOTE_OFF 12 //[MIDI_SYNTH, NOTE_OFF, note_number]

//light leds
#define LEDS 68 //[leds , led_value_1, led_value_2]


//MICRO/USB
#define send_usb_command 29//[send_usb_command , usb_command, val1, val2, val3 ,val4]
//usb_command 
#define media_play_pause 30//[send_usb_command , media_play_pause]
#define volume_up 31       //[send_usb_command , volume_up]
#define volume_down 32     //[send_usb_command , volume_down]
#define media_next 33      //[send_usb_command , media_next]
#define media_previous 34  //[send_usb_command , media_previous]

//SENSORS

//LEDS AND SCREEN
#define audio 11    //nano, I2C, modifiche sull'audio del synth
#define pcUSB 12    //micro , WIFI
#define noAnswer 15 //la manda il nano per i2c all'esp se non ha ancora ricevuto risposta dal micro
#define info 16     //richiede informazioni sul dispositivo



//VARIABLES

//task
extern TaskHandle_t LOOP2;

//MIDI
extern byte channel1;
extern byte channel2;
extern byte vel1;
extern byte vel2;

//LEDS
extern volatile uint16_t led_status; 

extern volatile int maxQueueRemoves;
extern volatile int maxMidiQueueRemoves;

// // extern unsigned long lastOperationTime; // for screensaver
// extern bool firstTimeGof;

//mpr121
extern Adafruit_MPR121 cap1;
extern Adafruit_MPR121 cap2;
extern uint16_t lasttouched1; //variabili che tengono i valori degli elettrodi toccati
extern uint16_t currtouched1;
extern uint16_t lasttouched2;
extern uint16_t currtouched2;

// QUEUES
//implemented in low_level_stuff section
struct myQueue
{
    QueueHandle_t queueHandle;
    int size;     //max size of the queue
    int elements; //current members of the queue number
    void newQueue(QueueHandle_t _queueHandle, int _size);
    bool isEmpty();                         //return true if queue is empty
    bool addQ(byte _pktToSend[PKT_LENGTH]); //add the packet _pkToSend to queue, return true if the adding was succesful
    bool getQ(byte _pktToGet[PKT_LENGTH]);  //remove the first packet from queue and places it on _pktToGet, return true if succesful
};


//queues
extern const int queueSize; //deve essere multiplo di 256 //per la communicate
extern QueueHandle_t midiQueue;
extern QueueHandle_t sequencerQueue;
extern QueueHandle_t bigQueue;

extern myQueue qPlayer; // midi player input queue
extern myQueue qSeq;    // sequencer input queue
extern myQueue bigQ;    //queue that manages all inputs and redirects to the other outputs

//tft
//extern int currLine;

//BUTTONS
//buttons and switches
struct Button //this struct's methods implementations are in "low_level_stuff.cpp"
{
    bool touch;
    int pin;
    bool pressed;
    unsigned long lastSwitchTime;
    int counter;  //tiene il conto delle volte che e' stato premuto.
    byte keyNote; //nota della tastiera sotto
    int num;
    byte midi0; //comando midi associato
    byte midi1;
    byte midi2;
    byte midi3;
    int mc;  //MIDI CONTROLLER
    int mcS; //+shift
    int sq;  //SEQUENCER
    int sqS; //+shift
    int pc;  //PC CONTROL
    int pcS; //+shift
    int sy;  //SYNTH ESP32
    int syS; //+shift
    int st;  //SETTINGS
    bool isPressed();
    void resetButton();
    bool updateButton();
    void newButton(int _pin, bool _isTouch);
};
// list of buttons
extern Button t0;
extern Button t1;
extern Button t2;
extern Button t3;
extern Button t4;
extern Button t5;
extern Button t6;
extern Button t7;
extern Button t8;
extern Button t9;
extern Button t10;
extern Button t11;
extern Button t12;
extern Button t13;
extern Button t14;
extern Button t15;
extern Button t16;
extern Button t17;
extern Button t18;
extern Button t19;
extern Button t20;
extern Button t21;
extern Button t22;
extern Button t23;
extern Button sw0;
extern Button sw1;
extern Button sw2;
extern Button sw3;
//data structures for buttons // sugli array per ottnere il valore fai: "nomearray[indice]->valore"
extern Button *keyboard[VALUES];           //mancano t18 e t21 che sono SHIFT e CHANGE
extern int keyboardVal[VALUES];            //tastiera con lettere
extern int keyboardValShift[VALUES];       //force to int, actually chars
extern int keyboardValShiftChange[VALUES]; //force to int, actually chars
extern Button *numPad[NUMBERS];            //NUMPAD
extern int numPadVal[NUMBERS];
extern Button *noteButtons[NOTE_BUTTONS];
extern byte noteKeys[NOTE_BUTTONS]; //note della tastiera sotto
extern Button *allButtons[ALL_BUTTONS];
//da completare e/o aggiornare
extern int midiController[ALL_BUTTONS];
extern int midiControllerShift[ALL_BUTTONS];
extern int sequencer[ALL_BUTTONS];
extern int sequencerShift[ALL_BUTTONS];
extern int PcControl[ALL_BUTTONS];
extern int PcControlShift[ALL_BUTTONS];
extern int synth[ALL_BUTTONS];
extern int synthShift[ALL_BUTTONS];
extern int settings[ALL_BUTTONS];
extern int settingShift[ALL_BUTTONS];
extern int midi0[VALUES]; //comandi midi, note o cc
extern int midi1[VALUES];
extern int midi2[VALUES];
extern int midi3[VALUES];

//ROTARY ENCODERS
struct RotaryEncoder //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
{
    int pinA;
    int pinB;
    int prevA;
    int prevB;
    int currA;
    int currB;
    int rotEncCnt = 0;
    unsigned long lastCallTime = 0;
    //per usarlo: prima dichiari RotaryEncoder myRotEnc; poi fai myRotEnc.newRotaryEncoder(pin1,pin2);
    void newRotaryEncoder(int _pinA, int _pinB);
    bool hasChanged(); //si usa come myRotEnc.hasChanged(), ritorna se e' cambiato o no
    int getValue();
};
extern RotaryEncoder r0;
extern RotaryEncoder r1;
extern RotaryEncoder r2;
extern RotaryEncoder r3;

#endif

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif