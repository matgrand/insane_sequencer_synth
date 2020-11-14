#include "ui.h"

myUI UI;


unsigned long lastOperationTime = 0; // for screensaver

void initializeUI()
{
    //tft screen
    pinMode(0, OUTPUT);
    digitalWrite(0, HIGH);
    ledcSetup(0, 5000, 8); //channel, pwm frequency, resolution (256 levels with 8 bit)
    ledcAttachPin(0, 0);   //GPIO 0, channel 0
    UI.setBrightness(100);
    // tft.init();
    // tft.setRotation(3); //1 = dritto, 3 = rovescio
    // tft.fillScreen(TFT_BLACK);

    UI.hasChanged = true;
    UI.currentSection = JUST_PLAY;
    UI.settings_window = false;
    UI.debug_window = true;
}

void myUI::println(char *_string)
{
    noSleep();
    Serial.println(_string);
    //tft.println(_string);
    currLine++;
    if (currLine > 15)
    {
        currLine = 0;
    }
    //tft.println("                        ");
    //tft.setCursor(0, currLine * 8);
}

void myUI::print(char *_string)
{
    noSleep();
    Serial.print(_string);
    //tft.print(_string);
}

void myUI::println(int _num)
{
    noSleep();
    Serial.println(_num);
    //tft.print(_num);
    if (_num < 1000000000)
        //tft.print(" ");
    if (_num < 100000000)
        //tft.print(" ");
    if (_num < 10000000)
        //tft.print(" ");
    if (_num < 1000000)
        //tft.print(" ");
    if (_num < 100000)
        //tft.print(" ");
    if (_num < 10000)
        //tft.print(" ");
    if (_num < 1000)
        //tft.print(" ");
    if (_num < 100)
        //tft.print(" ");
    if (_num < 10)
        //tft.print(" ");
    //tft.println();
    currLine++;
    if (currLine > 15)
    {
        currLine = 0;
        //tft.setCursor(0, 0);
    }
    //tft.println("                        ");
    //tft.setCursor(0, currLine * 8);
}

void myUI::print(int _num)
{
    noSleep();
    Serial.print(_num);
    //tft.print(_num);
}

void myUI::println(byte _num)
{
    println((int)_num);
}

void myUI::print(byte _num)
{
    print((int)_num);
}

void myUI::println()
{
    noSleep();
    Serial.println();
    //tft.println();
    currLine++;
    if (currLine > 15)
    {
        currLine = 0;
    }
    //tft.println("                        ");
    //tft.setCursor(0, currLine * 8);
}

void myUI::setBrightness(byte _brightness)
{
    brightness = 255 - _brightness;
    ledcWrite(0, brightness);
}

//OLED SMALL SCREEN WITH UI BASED ON LIBRARY SSD1306Wire.h AND OLEDDisplayUi.h
SSD1306Wire display(0x3c, 21, 22, GEOMETRY_128_64, I2C_ONE, I2C_FREQ); //set I2C frequency to 400kHz

OLEDDisplayUi ui(&display);

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState *state)
{
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(128, 0, String(millis()));
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // draw an xbm image.
    // Please note that everything that should be transitioned
    // needs to be drawn relative to x and y

    display->drawXbm(x + 34, y + 15, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
    // Besides the default fonts there will be a program to convert TrueType fonts into this format
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0 + x, 13 + y, "Arial 10");

    display->setFont(ArialMT_Plain_16);
    display->drawString(0 + x, 22 + y, "Arial 16");

    display->setFont(ArialMT_Plain_24);
    display->drawString(0 + x, 33 + y, "Arial 24");
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // Text alignment demo
    display->setFont(ArialMT_Plain_10);

    // The coordinates define the left starting point of the text
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0 + x, 13 + y, "Left aligned (0,10)");

    // The coordinates define the center of the text
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64 + x, 24 + y, "Center aligned (64,22)");

    // The coordinates define the right end of the text
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128 + x, 35 + y, "Right aligned (128,33)");
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // Demo for drawStringMaxWidth:
    // with the third parameter you can define the width after which words will be wrapped.
    // Currently only spaces and "-" are allowed for wrapping
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawStringMaxWidth(0 + x, 13 + y, 128, "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore.");
}

void drawFrame5(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = {drawFrame1, drawFrame2, drawFrame3, drawFrame4, drawFrame5};

// how many frames are there?
int frameCount = 5;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = {msOverlay};
int overlaysCount = 1;

void initializeOledDisplay()
{
    // The ESP is capable of rendering 60fps in 80Mhz mode
    // but that won't give you much time for anything else
    // run it in 160Mhz mode or just set it to 30 fps
    ui.setTargetFPS(FPS);

    // Customize the active and inactive symbol
    ui.setActiveSymbol(activeSymbol);
    ui.setInactiveSymbol(inactiveSymbol);

    // You can change this to
    // TOP, LEFT, BOTTOM, RIGHT
    ui.setIndicatorPosition(TOP);

    // Defines where the first frame is located in the bar.
    ui.setIndicatorDirection(LEFT_RIGHT);

    // You can change the transition that is used
    // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
    ui.setFrameAnimation(SLIDE_LEFT);

    // Add frames
    ui.setFrames(frames, frameCount);

    // Add overlays
    ui.setOverlays(overlays, overlaysCount);

    // Initialising the UI will init the display too.
    ui.init();

    display.flipScreenVertically();
}

void testOledDisplay()
{
    if ((millis() - lastOperationTime) < secondsBeforeAimationStarts * 1000) {
        int remainingTimeBudget = ui.update();
        //Serial.println(remainingTimeBudget);
    }
}

//GENERAL
void update_UI() // based on modification on the UI struct made by the other blocks, update screen and operations
{
}

void noSleep()
{
    lastOperationTime = millis();
}



//SCREEN SAVER GAME OF LIFE oled 128x64
//settings
volatile int animationDelay = 20;              //ms
volatile int secondsBeforeAimationStarts = 15; //secondi di inattivita' prima che vada in screen saver

// unsigned long lastOperationTime = 0; // for screensaver
bool firstTimeGof = true;
uint8_t grid[GRIDX][GRIDY];
uint8_t newgrid[GRIDX][GRIDY];

void checkEnd(unsigned long _start, unsigned long _finish)
{
    static int prevTimeGOF = 0;
    static int endCounter = 0;
    int currTimeGOF = _finish - _start;
    if ((currTimeGOF - prevTimeGOF) < 4 || (currTimeGOF - prevTimeGOF) > 4)
        endCounter++;
    else
        endCounter = endCounter - 64;
    if (endCounter < 0)
        endCounter = 0;
    prevTimeGOF = currTimeGOF;
    if (endCounter > 1200)
    {
        firstTimeGof = true;
        endCounter = 0;
        display.clear();
    }
}

int getNumberOfNeighbors(int x, int y)
{
    return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}

void drawGrid()
{
    for (int16_t x = 1; x < GRIDX - 1; x++)
    {
        for (int16_t y = 1; y < GRIDY - 1; y++)
        {
            if ((grid[x][y]) != (newgrid[x][y]))
            {
                if (newgrid[x][y] == 1) {
                    display.setColor(WHITE);
                    display.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY);
                }
                else {

                    display.setColor(BLACK);
                    display.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY);
                }
            }
        }
    }
    display.display();
}

void initGrid()
{
    for (int16_t x = 0; x < GRIDX; x++)
    {
        for (int16_t y = 0; y < GRIDY; y++)
        {
            newgrid[x][y] = 0;
            if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1)
            {
                grid[x][y] = 0;
            }
            else
            {
                if (random(3) == 1)
                    grid[x][y] = 1;
                else
                    grid[x][y] = 0;
            }
        }
    }
}

void computeCA()
{
    for (int16_t x = 1; x < GRIDX; x++)
    {
        for (int16_t y = 1; y < GRIDY; y++)
        {
            int neighbors = getNumberOfNeighbors(x, y);
            if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3))
            {
                newgrid[x][y] = 1;
            }
            else if (grid[x][y] == 1)
                newgrid[x][y] = 0;
            if (grid[x][y] == 0 && (neighbors == 3))
            {
                newgrid[x][y] = 1;
            }
            else if (grid[x][y] == 0)
                newgrid[x][y] = 0;
        }
    }
}

void gameOfLife()
{
    unsigned long startGOF = micros();
    computeCA();
    drawGrid();
    delay(animationDelay);
    for (int16_t x = 1; x < GRIDX - 1; x++)
    {
        for (int16_t y = 1; y < GRIDY - 1; y++)
        {
            grid[x][y] = newgrid[x][y];
        }
    }
    checkEnd(startGOF, micros());
}

void screenSaver()
{
    if ((millis() - lastOperationTime) > secondsBeforeAimationStarts * 1000)
    {
        if (firstTimeGof)
        {
            display.clear();
            initGrid();
            drawGrid();
        }
        firstTimeGof = false;
        gameOfLife();
    }
    else
    {
        if (!firstTimeGof)
        {
            display.clear();
        }
        firstTimeGof = true;
    }
}




