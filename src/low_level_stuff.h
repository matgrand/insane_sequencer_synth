#ifndef low_level_stuff_h
#define low_level_stuff_h

#include "ui.h"

// void noSleep(); 

//QUEUES
void initializeQueues();

void manageBigQ(); //send all the packages to their respectives queues

//SENSORS
void updatePots();

void updateRotaryEncoders();

void updateButtons();

//SCREEN AND LEDS
void updateScreen(); // takes the screen parameters variable and updates the screen based on that

void updateLeds(); // takes the led_status variable and light the leds accordingly 

//buttons
//buttons methods implementations are in this .cpp file
void initializeButtons();

//MEMORY, SD, RAM  //ok
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);  //ok
void createDir(fs::FS &fs, const char *path); //ok
void removeDir(fs::FS &fs, const char *path); //ok
void readFile(fs::FS &fs, const char *path); //ok
void writeFile(fs::FS &fs, const char *path, const char *message); //ok
void appendFile(fs::FS &fs, const char *path, const char *message); //ok
void renameFile(fs::FS &fs, const char *path1, const char *path2); //ok
void deleteFile(fs::FS &fs, const char *path); //ok
void testFileIO(fs::FS &fs, const char *path); //ok






#endif