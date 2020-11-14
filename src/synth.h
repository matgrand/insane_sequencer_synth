#ifndef synth_h
#define synth_h
#include "definitions.h"

//I2S
#define I2S_DOUT 25 // i2S Data out oin
#define I2S_BCLK 27 // Bit clock
#define I2S_LRC 26  // Left/Right clock, also known as Frame clock or word select
#define I2S_NUM 0   // i2s port number
#define NUM_BYTES_TO_READ_FROM_FILE 512    // How many bytes to read from wav file at a time

static const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
    .dma_buf_count = 8,                       // 8 buffers
    .dma_buf_len = 64,                        // 64 bytes per buffer, so 8K of buffer space
    .use_apll = 0,
    .tx_desc_auto_clear = true,
    .fixed_mclk = -1};

static const i2s_pin_config_t pin_config =
    {
        .bck_io_num = I2S_BCLK,          // The bit clock connectiom, goes to pin 27 of ESP32
        .ws_io_num = I2S_LRC,            // Word select, also known as word select or left right clock, 26
        .data_out_num = I2S_DOUT,        // Data out from the ESP32, connect to DIN on 38357A //25
        .data_in_num = I2S_PIN_NO_CHANGE // we are not interested in I2S data into the ESP32
};

struct WavHeader_Struct //For wav samples
{
    //   RIFF Section
    char RIFFSectionID[4]; // Letters "RIFF"
    uint32_t Size;         // Size of entire file less 8
    char RiffFormat[4];    // Letters "WAVE"

    //   Format Section
    char FormatSectionID[4]; // letters "fmt"
    uint32_t FormatSize;     // Size of format section less 8
    uint16_t FormatID;       // 1=uncompressed PCM
    uint16_t NumChannels;    // 1=mono,2=stereo
    uint32_t SampleRate;     // 44100, 16000, 8000 etc.
    uint32_t ByteRate;       // =SampleRate * Channels * (BitsPerSample/8)
    uint16_t BlockAlign;     // =Channels * (BitsPerSample/8)
    uint16_t BitsPerSample;  // 8,16,24 or 32

    // Data Section
    char DataSectionID[4]; // The letters "data"
    uint32_t DataSize;     // Size of the data that follows
};

void initializeI2S();

uint16_t generateSamples(byte *samples); //generates samples from the synth engine when required to to it  

//uint16_t ReadFile(byte *samples)  //needs to be adapted to read from 16 different samples

bool fillI2Sbuffer(byte* samples,uint16_t BytesInBuffer);

bool ValidWavData(WavHeader_Struct* Wav); //see if the wav files are correct
void DumpWAVHeader(WavHeader_Struct* Wav); //debugging
void PrintData(const char* Data,uint8_t NumBytes);

//SYNTH ENGINE

// General purpose oscillator
#define OSC_SINE 0
#define OSC_SQUARE 1
#define OSC_TRIANGLE 2
#define OSC_SAW_ANA 3
#define OSC_SAW_DIG 4
#define OSC_NOISE 5



#endif