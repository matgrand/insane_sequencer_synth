#ifndef ui_h
#define ui_h

// Include custom images
#include "midi_stuff.h"
#include "images.h"

#define FPS 60 //fps of the screen

//ui sections
#define JUST_PLAY 1
#define SEQUENCER 2
#define SYNTH 3
#define PC_CONTROL 4

//SCREEN SAVER GAME OF LIFE
#define GRIDX 128 //160
#define GRIDY 64 //128
#define CELLXY 1

//SETTINGS
extern volatile int animationDelay;              //ms
extern volatile int secondsBeforeAimationStarts; //secondi di inattivita' prima che vada in screen saver

// extern unsigned long lastOperationTime; // for screensaver
extern bool firstTimeGof;

extern unsigned long lastOperationTime; // for screensaver

struct myUI {
    bool hasChanged; //methods that change the state of the UI put this to true.
    int currentSection; // JUST_PLAY SEQUENCER SYNTH PC_CONTROL , default JUST_PLAY
    bool settings_window; //if true the settings window of that section is open, default false
    bool debug_window; //true if debug window is open, default open
    int brightness; //screen brightness, 0-255, default 255
    int currLine;

    void print(char *_string);
    void println(char *_string);
    void print(int _num);
    void println(int _num);
    void print(byte _num);
    void println(byte _num);
    void println();

    void setBrightness(byte _brightness);

};

extern myUI UI;

void initializeUI();

void update_UI();  // based on modification on the UI struct made by the other blocks, update screen and operations

void noSleep();


//OLED SMALL SCREEN WITH UI BASED ON LIBRARY SSD1306Wire.h AND OLEDDisplayUi.h

void initializeOledDisplay();

//TESTS
void testOledDisplay(); 

//SCREEN SAVER GAME OF LIFE  //ok
void screenSaver(); //ok





#endif