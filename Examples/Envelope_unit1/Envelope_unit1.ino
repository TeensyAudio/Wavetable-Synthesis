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

#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//---------------------------------------------------------------------------------------
#include <AudioSynthWavetable.h>
#include "SteelGuitar_samples.h"
#include "AudioSampleCashregister.h"
//---------------------------------------------------------------------------------------
AudioOutputI2S            i2s1;
AudioSynthWavetable       wavetable;
AudioMixer4               mixer;
//---------------------------------------------------------------------------------------
AudioConnection patchCord0(wavetable, 0, mixer, 0);
AudioConnection patchCord1(mixer, 0, i2s1, 0);
AudioConnection patchCord2(mixer, 0, i2s1, 1);
//---------------------------------------------------------------------------------------
AudioControlSGTL5000      sgtl5000_1;

elapsedMillis timer_env = 0;
int count = 0;
int passed = 0;
int note = 0;
envelopeStateEnum current_section = STATE_IDLE;
bool flag_stop = false;

const sample_data* SAMPLES = SteelGuitar;
const int TICK = 500;           // Timer period (ms)
const int TOLERANCE = 2;
const int NUM_TESTS = 4;
const int UPPER_BOUND = 127;
const int START_NOTE = 1;      // Note below lowest tested note

const int ENV_EXPECTED[] = {
  -1,
  SAMPLES[0].DELAY_ENV,
  SAMPLES[0].ATTACK_ENV,
  SAMPLES[0].HOLD_ENV,
  SAMPLES[0].DECAY_ENV,
  -1,
  SAMPLES[0].RELEASE_ENV
};

const String STATE_TO_STR[] = {
  "IDLE",
  "DELAY",
  "ATTACK",
  "HOLD",
  "DECAY",
  "SUSTAIN",
  "RELEASE"
};

void setup() {
  AudioMemory(40);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  wavetable.setSamples(SAMPLES, sizeof(SAMPLES)/sizeof(sample_data));
  while (timer_env < 2000); // Spin for serial monitor
  count = START_NOTE-1;
}

void loop() {
  if (!flag_stop) {
    if (current_section == STATE_IDLE) {
      if (count++ == NUM_TESTS) {
        flag_stop = true;
        Serial.printf("\n%d/%d tests passed\n\n", passed, 5*NUM_TESTS);
        return;
      }

      while (timer_env < 1000);
      note = count + (count - 1) * UPPER_BOUND / NUM_TESTS;
      timer_env = 0;
      wavetable.playNote(note); // A3 = 57
      current_section = wavetable.getEnvState();
      Serial.print("\nState=");
      Serial.print(STATE_TO_STR[(int)current_section]);
      Serial.println();
    }
    
    if (current_section != wavetable.getEnvState()) {
      int duration = (int)timer_env;
      timer_env = 0;
      Serial.printf("Duration: %dms\n", duration);
      Serial.printf("Expected: %dms\n", ENV_EXPECTED[(int)current_section]);
      int error = duration - ENV_EXPECTED[(int)current_section];
      error = error < 1 ? -error : error;
      Serial.printf("Error = %dms\n", error);
      if (error <= TOLERANCE) {
        Serial.println("Passed!\n");
        passed++;
      } else Serial.println("Failed...\n");
      current_section = wavetable.getEnvState();
      Serial.print("State=");
      Serial.print(STATE_TO_STR[(int)current_section]);
      Serial.println();
      if (current_section == STATE_SUSTAIN) {
        while (timer_env < 1000);
        timer_env = 0;
        wavetable.stop();
        current_section = wavetable.getEnvState();
        Serial.print("\nState=");
        Serial.print(STATE_TO_STR[(int)current_section]);
        Serial.println();
      }
    }
  }
}

