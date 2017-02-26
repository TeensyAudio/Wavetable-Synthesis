/*
   C     C#    D     Eb    E     F     F#    G     G#    A     Bb    B
 0 16.35 17.32 18.35 19.45 20.60 21.83 23.12 24.50 25.96 27.50 29.14 30.87
 1 32.70 34.65 36.71 38.89 41.20 43.65 46.25 49.00 51.91 55.00 58.27 61.74
 2 65.41 69.30 73.42 77.78 82.41 87.31 92.50 98.00 103.8 110.0 116.5 123.5
 3 130.8 138.6 146.8 155.6 164.8 174.6 185.0 196.0 207.7 220.0 233.1 246.9
 4 261.6 277.2 293.7 311.1 329.6 349.2 370.0 392.0 415.3 440.0 466.2 493.9
 5 523.3 554.4 587.3 622.3 659.3 698.5 740.0 784.0 830.6 880.0 932.3 987.8
 6 1047  1109  1175  1245  1319  1397  1480  1568  1661  1760  1865  1976
 7 2093  2217  2349  2489  2637  2794  2960  3136  3322  3520  3729  3951
 8 4186  4435  4699  4978  5274  5588  5920  6272  6645  7040  7459  7902
*/
#define NUM_VOICES 32   // Audio concurrency seems to cap at 30

#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//---------------------------------------------------------------------------------------
#include <AudioSynthWavetable.h>
#include "VoiceOohs_samples.h"
//---------------------------------------------------------------------------------------
AudioOutputI2S            i2s1;
AudioSynthWavetable       wavetables[NUM_VOICES];
AudioMixer4               mixer[11];
//---------------------------------------------------------------------------------------
AudioConnection PatchCord[] = {
    {wavetables[0], 0, mixer[0], 0},
    {wavetables[1], 0, mixer[0], 1},
    {wavetables[2], 0, mixer[0], 2},
    {wavetables[3], 0, mixer[0], 3},
    {wavetables[4], 0, mixer[1], 0},
    {wavetables[5], 0, mixer[1], 1},
    {wavetables[6], 0, mixer[1], 2},
    {wavetables[7], 0, mixer[1], 3},
    {wavetables[8], 0, mixer[2], 0},
    {wavetables[9], 0, mixer[2], 1},
    {wavetables[10], 0, mixer[2], 2},
    {wavetables[11], 0, mixer[2], 3},
    {wavetables[12], 0, mixer[3], 0},
    {wavetables[13], 0, mixer[3], 1},
    {wavetables[14], 0, mixer[3], 2},
    {wavetables[15], 0, mixer[3], 3},
    {wavetables[16], 0, mixer[4], 0},
    {wavetables[17], 0, mixer[4], 1},
    {wavetables[18], 0, mixer[4], 2},
    {wavetables[19], 0, mixer[4], 3},
    {wavetables[20], 0, mixer[5], 0},
    {wavetables[21], 0, mixer[5], 1},
    {wavetables[22], 0, mixer[5], 2},
    {wavetables[23], 0, mixer[5], 3},
    {wavetables[24], 0, mixer[6], 0},
    {wavetables[25], 0, mixer[6], 1},
    {wavetables[26], 0, mixer[6], 2},
    {wavetables[27], 0, mixer[6], 3},
    {wavetables[28], 0, mixer[7], 0},
    {wavetables[29], 0, mixer[7], 1},
    {wavetables[30], 0, mixer[7], 2},
    {wavetables[31], 0, mixer[7], 3},
    {mixer[0], 0, mixer[8], 0},
    {mixer[1], 0, mixer[8], 1},
    {mixer[2], 0, mixer[8], 2},
    {mixer[3], 0, mixer[8], 3},
    {mixer[4], 0, mixer[9], 0},
    {mixer[5], 0, mixer[9], 1},
    {mixer[6], 0, mixer[9], 2},
    {mixer[7], 0, mixer[9], 3},
    {mixer[8], 0, mixer[10], 0},
    {mixer[9], 0, mixer[10], 1},
    {mixer[10], 0, i2s1, 0},
    {mixer[10], 0, i2s1, 1},
  };
//---------------------------------------------------------------------------------------
AudioControlSGTL5000      sgtl5000_1;
elapsedMillis timer = 0;
int count = 0;
bool flag_stop = false;
int note = 40;    // Starting note doesn't seem to matter much

const int TICK = 500;     // Timer period (ms)

void setup() {
  AudioMemory(30);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  for (int i = 0; i < NUM_VOICES; i++)
    wavetables[i].setSamples(VoiceOohs, sizeof(VoiceOohs)/sizeof(sample_data));
  while (timer < 2000);   // Spin for serial monitor
}

void loop() {
  if (!flag_stop) {
    if (timer >= TICK) {
      float proc_usage = AudioProcessorUsage();
      if (count > 0)
        Serial.printf("Processor usage: %3.5f\n\n", proc_usage);
      if (count == NUM_VOICES || proc_usage > 90.0) {
        for (int i = 0; i < NUM_VOICES; i++)
          wavetables[i].stop();
        flag_stop = true;
        return;
      }
      wavetables[count++].playNote(note++);
      Serial.printf("voices: %d\n", count);
      timer = 0;
    }
  }
}

