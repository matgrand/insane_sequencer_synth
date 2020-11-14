//INSANE CONTROLLER V 2.3 MAIN 

#include "sequencer.h"    



void communicate()
{
    manageBigQ();                 //send all the packages to their respectives queues
}

void midi_play()
{
    int j = 0;
    while (!qPlayer.isEmpty())
    {
        j++;
        if (j > maxMidiQueueRemoves)
            break;
        //else
        byte tmpMidiPkt[PKT_LENGTH]; //create a pkt
        getNextMidiPkt(tmpMidiPkt);  //place on it the next midiPkt in queue
        playMidiPkt(tmpMidiPkt);     //play the packet to the correct device
    }
}

void sequence()
{
    //da pensare bene, modello
}

void sensors()
{
    updatePots();           //send to bigQ all updates about Pots values
    updateRotaryEncoders(); //send to bigQ all updates about RotaryEncoders values
    updateButtons();        //send to bigQ all updates about Buttons values
}

void manage_UI()
{
    update_UI();  // based on modification on the UI struct made by the other blocks, update screen and operations
    updateLeds(); // takes the led_status variable and light the leds accordingly 
}

void updateSynth() {

} 

//tests
void testRotEnc0()
{
  static int prevC = 0;
  static int currC = 0;
  currC = r0.hasChanged();
  if (prevC != currC)
  {
    UI.println(currC);
    prevC = currC;
  }
}

void testTouch()
{
  //first mpr121
  currtouched1 = cap1.touched(); //540 micros
  for (byte i = 0; i < 12; i++)
  {
    if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i)))
    {
      //addCom(i);
      Serial.print(i);
      Serial.println(" touched");
      //ledOn(i);
      //runCommand(i);
    }
    if (!(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)))
    {
      Serial.print(i);
      Serial.println(" released");
      //addComOff(i);
      //ledOff(i);
    }
  }
  lasttouched1 = currtouched1;
  //second MPR121
  currtouched2 = cap2.touched(); //540 micros
  for (byte i = 0; i < 12; i++)
  {
    if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i)))
    {
      //addCom(i);
      Serial.print(i + 12);
      Serial.println(" touched");
      //ledOn(i);
      //runCommand(i);
    }
    if (!(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)))
    {
      Serial.print(i + 12);
      Serial.println(" released");
      //addComOff(i);
      //ledOff(i);
    }
  }
  lasttouched2 = currtouched2;
}

void testBrightness()
{
  for (int i = 20; i <= 70; i++)
  {
    // changing the LED brightness with PWM
    UI.setBrightness(i);
    delay(50);
  }

  // decrease the LED brightness
  for (int i = 70; i >= 20; i--)
  {
    // changing the LED brightness with PWM
    UI.setBrightness(i);
    delay(50);
  }
}

void testSD()
{
  Serial.println("SD TEST STARTING");
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  writeFile(SD, "/hello.txt", "Hello ");
  appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/hello.txt");
  deleteFile(SD, "/foo.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  readFile(SD, "/foo.txt");
  testFileIO(SD, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  delay(2000);
}

void testMidiOut()
{
  noSleep();
  byte rnd_tmp = (byte)random(50,80);
  noteOn(1, rnd_tmp, vel2, channel2);
  delay(200);
  noteOff(1, rnd_tmp, 0, channel2);
  delay(10);
}

void testMidiIn() {
  midi_read();
}





//task header
TaskHandle_t LOOP2;
void loop2(void *parameter);

////  SETUP  //////////////////////////////////////////////////////////////////////
void setup()
{
  // Start the I2C Bus as Master
  Wire.begin();
  Wire.setClock(I2C_FREQ); 

  initializeOledDisplay();

  initializeUI();

  initializeMIDI();

  //rotary encoders @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  r0.newRotaryEncoder(39, 34); // sw0 36 //in test 36,39
  r1.newRotaryEncoder(32, 33); // sw1 35
  r2.newRotaryEncoder(26, 27); //sw2 25
  r3.newRotaryEncoder(12, 13); //sw3 14

  //MPR121 touch
  cap1.begin(MPR1);
  cap2.begin(MPR2);

  //i2s
  initializeI2S();

  //buttons
  initializeButtons();

  //Serial monitor
  Serial.begin(115200); //IF SERIAL OPEN, MIDI0 DOESN'T WORK, MIDI2

  //TASKS
  //(task function, name, stack size, parameter, priority, task handle, core)
  xTaskCreatePinnedToCore(loop2, "LOOP2", 50000, NULL, 3, &LOOP2, 0);
  midiQueue = xQueueCreate(queueSize, PKT_LENGTH);
  sequencerQueue = xQueueCreate(queueSize, PKT_LENGTH);
  bigQueue = xQueueCreate(4 * queueSize, PKT_LENGTH); //4 *queue size
  if ((midiQueue == NULL) || (sequencerQueue == NULL) || (bigQueue == NULL))
  {
    Serial.println("ERROR CREATING THE QUEUES, not enough heap memory");
  }
  delay(50); //needed to startup task1

  initializeQueues();

  

  noSleep();

  UI.println("START");

  //Serial.println(getCpuFrequencyMhz());
  //setCpuFrequencyMhz(80); //Set CPU clock to 80MHz for example
  //Serial.println(getCpuFrequencyMhz());
}

void loop()
{ //loop on second core (1)

  //MAIN FUNCTIONS
  //communicate();
  //midi_play();
  //sequence();
  //sensors();
  //manage_UI();
  //updateSynth();

  //testBrightness();
  testTouch();
  //testSpeed();
  //testRotEnc0(); 
  screenSaver(); //working, check integration
  //testMidiOut(); // working , both channel
  //testMidiIn();
  //testSD(); //working ma bisogna premere la sd, speriamo in un nuovo modulo
  
  testOledDisplay();


  delay(2);
}

void loop2(void *parameter)
{
  while (1)
  { // usa poco (tanti delay) il core0 altrimenti wifi e bluetooth non ce la fanno
    testBrightness();
    delay(50);
  }
}
