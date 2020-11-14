#include "low_level_stuff.h"

//settings
volatile int maxQueueRemoves = 10; //max number of queue removes for every cycle, tune it to see how affects speed
volatile int maxMidiQueueRemoves = 5; //see above



//mpr121 (BUTTONS)
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
uint16_t lasttouched1 = 0; //variabili che tengono i valori degli elettrodi toccati
uint16_t currtouched1 = 0;
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;

//BUTTONS
// list of buttons
Button t0;
Button t1;
Button t2;
Button t3;
Button t4;
Button t5;
Button t6;
Button t7;
Button t8;
Button t9;
Button t10;
Button t11;
Button t12;
Button t13;
Button t14;
Button t15;
Button t16;
Button t17;
Button t18;
Button t19;
Button t20;
Button t21;
Button t22;
Button t23;
Button sw0;
Button sw1;
Button sw2;
Button sw3;
//data structures for buttons // sugli array per ottnere il valore fai: "nomearray[indice]->valore"
Button *keyboard[VALUES] = {&t0, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9, &t10, &t11, &t12, &t13, &t14, &t15, &t16, &t17, &t19, &t20, &sw0, &sw1, &sw2, &sw3, &t22, &t23};               //mancano t18 e t21 che sono SHIFT e CHANGE
int keyboardVal[VALUES] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};                                //tastiera con lettere
int keyboardValShift[VALUES] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '_', '-', EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}; //force to int, actually chars
int keyboardValShiftChange[VALUES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};                     //force to int, actually chars
Button *numPad[NUMBERS] = {&t0, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9};                                                                                                                   //NUMPAD
int numPadVal[NUMBERS] = {0,  1, 2, 3, 4, 5, 6, 7, 8, 9};
Button *noteButtons[NOTE_BUTTONS] = {&t0, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9, &t10, &t11, &t12, &t13, &t14, &t15, &t16, &t17, &t18, &t19, &t20, &t21, &t22, &t23};
byte noteKeys[NOTE_BUTTONS] = {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77}; //note della tastiera sotto
Button *allButtons[ALL_BUTTONS] = {&t0, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8, &t9, &t10, &t11, &t12, &t13, &t14, &t15, &t16, &t17, &t18, &t19, &t20, &t21, &sw0, &sw1, &sw2, &sw3, &t22, &t23};
//da completare e/o aggiornare
int midiController[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int midiControllerShift[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int sequencer[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int sequencerShift[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int PcControl[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int PcControlShift[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int synth[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int synthShift[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int settings[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int settingShift[ALL_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int midi0[VALUES] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60}; //comandi midi, note o cc
int midi1[VALUES] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
int midi2[VALUES] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
int midi3[VALUES] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};

//ROTARY ENCODERS
RotaryEncoder r0;
RotaryEncoder r1;
RotaryEncoder r2;
RotaryEncoder r3;

//LEDS
volatile uint16_t led_status = 0xFFFF; 

//QUEUES
//queues
const int queueSize = 256; //deve essere multiplo di 256 //per la communicate
QueueHandle_t midiQueue;
QueueHandle_t sequencerQueue;
QueueHandle_t bigQueue;

myQueue qPlayer; // midi player input queue
myQueue qSeq; // sequencer input queue
myQueue bigQ;  //queue that manages all inputs and redirects to the other outputs

void myQueue::newQueue(QueueHandle_t _queueHandle, int _size)
{
    queueHandle = _queueHandle;
    size = _size;
    elements = 0;
}
bool myQueue::isEmpty()
{
    return elements == 0;
}
bool myQueue::addQ(byte _pktToSend[PKT_LENGTH])
{
    if (elements >= size)
    {
        UI.println("QUEUE IS FULL, FAILED ADDING");
        return false;
    }
    //else
    xQueueSend(queueHandle, _pktToSend, 0);
    elements++;
    return true;
}
bool myQueue::getQ(byte _pktToGet[PKT_LENGTH])
{
    if (isEmpty())
    {
        UI.println("QUEUE EMPTY, FAILED GETTING FROM QUEUE");
        return false;
    }
    //else
    xQueueReceive(queueHandle, _pktToGet, 0);
    elements--;
    return true;
}
void initializeQueues()
{
    qPlayer.newQueue(midiQueue, queueSize);
    qSeq.newQueue(sequencerQueue, queueSize);
    bigQ.newQueue(bigQueue, 4 * queueSize);
}

void manageBigQ() //send all the packages to their respectives queues
{

}

//SENSORS
void updatePots()
{
}

void updateRotaryEncoders()
{
}

void updateButtons()
{
}

//SCREEN AND LEDS
void updateScreen() // takes the screen parameters variable and updates the screen based on that
{
}
void updateLeds() // takes the led_status variable and light the leds accordingly
{
}


//BUTTONS
bool Button::isPressed()
{
    return pressed;
}
void Button::resetButton()
{
    counter = 0;
}
bool Button::updateButton()
{ //restituisce true se ha "appena switchato"
    bool toReturn = false;
    if (touch)
    { //nel caso dei tasti touch il comando andrebbe aggiornato fuori usando capx.touched(), adesso provo senza vediamo se da' problemi di velocita'
        if (pin < 12)
        {                                  //primo MPR121
            currtouched1 = cap1.touched(); //540 micros        ********************************************************************vedi sopra
            if ((currtouched1 & _BV(pin)) && !(lasttouched1 & _BV(pin)))
            {
                toReturn = true;
                pressed = true;
                counter++;
                Serial.print(pin);
                Serial.println(" touched");
            }
            if (!(currtouched1 & _BV(pin)) && (lasttouched1 & _BV(pin)))
            {
                Serial.print(pin);
                Serial.println(" released");
                toReturn = true;
                pressed = false;
            }
            lasttouched1 = currtouched1;
        }
        else
        { //secondo MPR121
            int tmpIndex = pin - 12;
            currtouched2 = cap2.touched(); //540 micros        ********************************************************************vedi sopra
            if ((currtouched2 & _BV(tmpIndex)) && !(lasttouched2 & _BV(tmpIndex)))
            {
                Serial.print(pin);
                Serial.println(" touched");
                toReturn = true;
                pressed = true;
            }
            if (!(currtouched2 & _BV(tmpIndex)) && (lasttouched2 & _BV(tmpIndex)))
            {
                Serial.print(pin);
                Serial.println(" released");
                toReturn = true;
                pressed = false;
            }
            lasttouched2 = currtouched2;
        }
    }
    else
    {                                        //switch dei rotary encoders
        bool tmpPressed = !digitalRead(pin); //pull up
        if (tmpPressed && !pressed)
        {
            toReturn = true;
            counter++;
            Serial.print(pin);
            Serial.println(" touched");
        }
        if (!tmpPressed && pressed)
        {
            toReturn = true;
            Serial.print(pin);
            Serial.println(" released");
        }
        pressed = tmpPressed;
    }
    return toReturn;
}
void Button::newButton(int _pin, bool _isTouch)
{
    pin = _pin;
    touch = _isTouch;
    pressed = false;
    lastSwitchTime = millis();
    updateButton();
}
void initializeButtons()
{
    if (true)
    { //inizializza i buttons
        t0.newButton(0, true);
        t1.newButton(1, true);
        t2.newButton(2, true);
        t3.newButton(3, true);
        t4.newButton(4, true);
        t5.newButton(5, true);
        t6.newButton(6, true);
        t7.newButton(7, true);
        t8.newButton(8, true);
        t9.newButton(9, true);
        t10.newButton(10, true);
        t11.newButton(11, true);
        t12.newButton(12, true);
        t13.newButton(13, true);
        t14.newButton(14, true);
        t15.newButton(15, true);
        t16.newButton(16, true);
        t17.newButton(17, true);
        t18.newButton(18, true);
        t19.newButton(19, true);
        t20.newButton(20, true);
        t21.newButton(21, true);
        t22.newButton(22, true);
        t23.newButton(23, true);
        sw0.newButton(36, false);
        sw1.newButton(35, false);
        sw2.newButton(25, false);
        sw3.newButton(14, false);
    }
    //da completare o aggiornare
    for (int i = 0; i < ALL_BUTTONS; i++)
    {
        allButtons[i]->mc = midiController[i];
        allButtons[i]->mcS = midiControllerShift[i];
        allButtons[i]->sq = sequencer[i];
        allButtons[i]->sqS = sequencerShift[i];
        allButtons[i]->pc = midiController[i];
        allButtons[i]->pcS = midiController[i];
        allButtons[i]->sy = midiController[i];
        allButtons[i]->syS = midiController[i];
        allButtons[i]->st = midiController[i];
    }
    for (int i = 0; i < VALUES; i++)
    {
        keyboard[i]->midi0 = midi0[i];
        keyboard[i]->midi1 = midi1[i];
        keyboard[i]->midi2 = midi2[i];
        keyboard[i]->midi3 = midi3[i];
    }
    for (int i = 0; i < NUMBERS; i++)
    {
        numPad[i]->num = numPadVal[i];
    }
    for (int i = 0; i < NOTE_BUTTONS; i++)
    {
        noteButtons[i]->keyNote = noteKeys[i];
    }
}

//ROTARY ENCODERS //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void RotaryEncoder::newRotaryEncoder(int _pinA, int _pinB)
{
    pinA = _pinA;
    pinB = _pinB;
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    prevA = digitalRead(pinA);
    currA = prevA;
    prevB = digitalRead(pinB);
    currB = prevB;
}
bool RotaryEncoder::hasChanged()//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
{
    bool hasChanged = false;
    unsigned long currCallTime = micros();
    if (currCallTime - lastCallTime < 50)
        return false;
    //else
    currA = digitalRead(pinA);
    currB = digitalRead(pinB);
    if (prevA == 0 && currA == 1)
    { //A rising
        if (prevB == currB)
        {
            if (prevB == 1)
            {
                rotEncCnt++; //full steps
                hasChanged = true;
            }
            else
            {
                rotEncCnt--;
                hasChanged = true;
            }
        }
    }
    // if (prevA == 0 && prevB == 1 && currA == 1 && currB == 1) rotEncCnt++;//full step
    // else if (prevA == 0 && prevB == 0 && currA == 1 && currB == 0) rotEncCnt--;//full step
    // else if (prevA == 1 && prevB == 0 && currA == 0 && currB == 0) {} //half step+
    // else if (prevA == 1 && prevB == 1 && currA == 0 && currB == 1) {} //half step-
    lastCallTime = currCallTime;
    prevA = currA;
    prevB = currB;
    return hasChanged;
}
int RotaryEncoder::getValue()
{
    return rotEncCnt;
}

//MEMORY, SD, RAM
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
void createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}
void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}
void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}
void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}
void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}
void testFileIO(fs::FS &fs, const char *path)
{
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file)
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    }
    else
    {
        Serial.println("Failed to open file for reading");
    }
    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}


