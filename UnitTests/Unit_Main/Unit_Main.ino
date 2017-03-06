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

#define NUM_VOICES 64

#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//---------------------------------------------------------------------------------------
#include <AudioSynthWavetable.h>
#include "VoiceOohs_samples.h"
//#include "GuitarHarmonics_samples.h"
//#include "SolarWind_samples.h"
#include "SteelGuitar_samples.h"
//#include "PerfeSine_samples.h"
//---------------------------------------------------------------------------------------
AudioOutputI2S            i2s1;
AudioSynthWavetable       wavetable[NUM_VOICES];
AudioMixer4               mixer[21];
AudioAnalyzePeak          test;
//---------------------------------------------------------------------------------------
AudioConnection PatchCord[] = {
    {wavetable[0], 0, mixer[0], 0},
    {wavetable[1], 0, mixer[0], 1},
    {wavetable[2], 0, mixer[0], 2},
    {wavetable[3], 0, mixer[0], 3},
    {wavetable[4], 0, mixer[1], 0},
    {wavetable[5], 0, mixer[1], 1},
    {wavetable[6], 0, mixer[1], 2},
    {wavetable[7], 0, mixer[1], 3},
    {wavetable[8], 0, mixer[2], 0},
    {wavetable[9], 0, mixer[2], 1},
    {wavetable[10], 0, mixer[2], 2},
    {wavetable[11], 0, mixer[2], 3},
    {wavetable[12], 0, mixer[3], 0},
    {wavetable[13], 0, mixer[3], 1},
    {wavetable[14], 0, mixer[3], 2},
    {wavetable[15], 0, mixer[3], 3},
    {wavetable[16], 0, mixer[4], 0},
    {wavetable[17], 0, mixer[4], 1},
    {wavetable[18], 0, mixer[4], 2},
    {wavetable[19], 0, mixer[4], 3},
    {wavetable[20], 0, mixer[5], 0},
    {wavetable[21], 0, mixer[5], 1},
    {wavetable[22], 0, mixer[5], 2},
    {wavetable[23], 0, mixer[5], 3},
    {wavetable[24], 0, mixer[6], 0},
    {wavetable[25], 0, mixer[6], 1},
    {wavetable[26], 0, mixer[6], 2},
    {wavetable[27], 0, mixer[6], 3},
    {wavetable[28], 0, mixer[7], 0},
    {wavetable[29], 0, mixer[7], 1},
    {wavetable[30], 0, mixer[7], 2},
    {wavetable[31], 0, mixer[7], 3},
    {wavetable[32], 0, mixer[8], 0},
    {wavetable[33], 0, mixer[8], 1},
    {wavetable[34], 0, mixer[8], 2},
    {wavetable[35], 0, mixer[8], 3},
    {wavetable[36], 0, mixer[9], 0},
    {wavetable[37], 0, mixer[9], 1},
    {wavetable[38], 0, mixer[9], 2},
    {wavetable[39], 0, mixer[9], 3},
    {wavetable[40], 0, mixer[10], 0},
    {wavetable[41], 0, mixer[10], 1},
    {wavetable[42], 0, mixer[10], 2},
    {wavetable[43], 0, mixer[10], 3},
    {wavetable[44], 0, mixer[11], 0},
    {wavetable[45], 0, mixer[11], 1},
    {wavetable[46], 0, mixer[11], 2},
    {wavetable[47], 0, mixer[11], 3},
    {wavetable[48], 0, mixer[12], 0},
    {wavetable[49], 0, mixer[12], 1},
    {wavetable[50], 0, mixer[12], 2},
    {wavetable[51], 0, mixer[12], 3},
    {wavetable[52], 0, mixer[13], 0},
    {wavetable[53], 0, mixer[13], 1},
    {wavetable[54], 0, mixer[13], 2},
    {wavetable[55], 0, mixer[13], 3},
    {wavetable[56], 0, mixer[14], 0},
    {wavetable[57], 0, mixer[14], 1},
    {wavetable[58], 0, mixer[14], 2},
    {wavetable[59], 0, mixer[14], 3},
    {wavetable[60], 0, mixer[15], 0},
    {wavetable[61], 0, mixer[15], 1},
    {wavetable[62], 0, mixer[15], 2},
    {wavetable[63], 0, mixer[15], 3},
    /********************************/
    {mixer[0], 0, mixer[16], 0},
    {mixer[1], 0, mixer[16], 1},
    {mixer[2], 0, mixer[16], 2},
    {mixer[3], 0, mixer[16], 3},
    {mixer[4], 0, mixer[17], 0},
    {mixer[5], 0, mixer[17], 1},
    {mixer[6], 0, mixer[17], 2},
    {mixer[7], 0, mixer[17], 3},
    {mixer[8], 0, mixer[18], 0},
    {mixer[9], 0, mixer[18], 1},
    {mixer[10], 0, mixer[18], 2},
    {mixer[11], 0, mixer[18], 3},
    {mixer[12], 0, mixer[19], 0},
    {mixer[13], 0, mixer[19], 1},
    {mixer[14], 0, mixer[19], 2},
    {mixer[15], 0, mixer[19], 3},
    /********************************/
    {mixer[16], 0, mixer[20], 0},
    {mixer[17], 0, mixer[20], 1},
    {mixer[18], 0, mixer[20], 2},
    {mixer[19], 0, mixer[20], 3},
    /********************************/
    {mixer[20], 0, i2s1, 0},
    {mixer[20], 0, i2s1, 1},
    {mixer[20], 0, test, 0},
  };
//---------------------------------------------------------------------------------------
AudioControlSGTL5000      sgtl5000_1;

enum testEnum { LATENCY, PROC, ENV, DONE };

elapsedMillis timer_env = 0;
elapsedMillis timer_latency = 0;
elapsedMillis timer = 0;
int count;
int note;
int passed_env = 0;
int passed_latency = 0;
int passed_proc = 0;
int longest_latency = 0;
int shortest_latency = sizeof(int);
bool flag_latency = false;
bool flag_init = false;
envelopeStateEnum current_section = STATE_IDLE;

const sample_data* SAMPLES_LATENCY = VoiceOohs;
const sample_data* SAMPLES_PROC = VoiceOohs;
const sample_data* SAMPLES_ENV = SteelGuitar;

const int TICK = 500;           // Timer period (ms)
const int UPPER_BOUND = 127;    // Highest tested note
const int LOWER_BOUND = 1;      // Lowest tested note
const int BAR_LENGTH = 20;
const int NUM_TESTS_LATENCY = UPPER_BOUND - LOWER_BOUND + 1;
const int THRESHOLD_LATENCY = 10;
const int START_NOTE_PROC = 20;
const float MAX_PROC_USAGE = 95.0;
const int NUM_TESTS_ENV = 4;
const int TOLERANCE_ENV = 2;

const int ENV_EXPECTED[] = {
  -1,
  SAMPLES_ENV[0].DELAY_ENV,
  SAMPLES_ENV[0].ATTACK_ENV,
  SAMPLES_ENV[0].HOLD_ENV,
  SAMPLES_ENV[0].DECAY_ENV,
  -1,
  SAMPLES_ENV[0].RELEASE_ENV
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

testEnum testType = (testEnum)0;
int testTypeVal = testType;

void setup() {
  AudioMemory(100);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  while (timer < 2000);   // Spin for serial monitor
}

void loop() {
  switch (testType) {
  
  case LATENCY:
  
    if (!flag_init) {
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.print("BEGIN TEST: LATENCY");
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.println("\n");
      count = UPPER_BOUND;
      passed_latency = 0;
      note = 0;
      longest_latency = 0;
      timer_latency = 0;
      shortest_latency = sizeof(int);
      flag_latency = false;
      wavetable[0].setSamples(SAMPLES_LATENCY, sizeof(SAMPLES_LATENCY)/sizeof(sample_data));
      flag_init = true;
    }
    
    if (timer >= TICK) {
      if (count < LOWER_BOUND) {
        wavetable[0].stop();
        Serial.printf("Longest latency: %dms\n", longest_latency);
        Serial.printf("Shortest latency: %dms\n", shortest_latency);
        Serial.printf("%d/%d tests passed.\n\n", passed_latency, NUM_TESTS_LATENCY);
        testTypeVal++;
        testType = (testEnum)testTypeVal;
        flag_init = false;
        return;
      }
      
      note = count--;
      timer_latency = 0;
      wavetable[0].playNote(note); // A3 = 57
      Serial.printf("note = %d\n", note);
      timer = 0;
      flag_latency = false;
    }
    
    if (test.available() && !flag_latency) {
      int latency = (int)timer_latency;
      Serial.printf("Latency: %dms\n", latency);
      if (latency <= THRESHOLD_LATENCY) {
        Serial.println("Passed!\n");
        passed_latency++;
      } else Serial.println("Failed...\n");
      if (latency > longest_latency) longest_latency = latency;
      if (latency < shortest_latency) shortest_latency = latency;
      flag_latency = true;
    }
    
    break;

  /**********************************************************************************************/

  case ENV:
  
    if (!flag_init) {
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.print("BEGIN TEST: ENVELOPE");
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.println("\n");
      count = 0;
      passed_env = 0;
      wavetable[0].setSamples(SAMPLES_ENV, sizeof(SAMPLES_ENV)/sizeof(sample_data));
      flag_init = true;
    }
    
    if (current_section == STATE_IDLE) {
      if (count++ == NUM_TESTS_ENV) {
        Serial.printf("%d/%d tests passed\n\n", passed_env, 5*NUM_TESTS_ENV);
        testTypeVal++;
        testType = (testEnum)testTypeVal;
        flag_init = false;
        return;
      }
      
      timer_env = 0;
      while (timer_env < 1000);
      note = count > 1 ? (count - 1) * UPPER_BOUND / (NUM_TESTS_ENV-1) : 1;
      Serial.printf("---------NOTE = %d---------\n\n", note);
      timer_env = 0;
      wavetable[0].playNote(note); // A3 = 57
      current_section = wavetable[0].getEnvState();
      Serial.print("State = ");
      Serial.print(STATE_TO_STR[(int)current_section]);
      Serial.println();
    }
    
    if (current_section != wavetable[0].getEnvState()) {
      int duration = (int)timer_env;
      timer_env = 0;
      Serial.printf("Duration: %dms\n", duration);
      Serial.printf("Expected: %dms\n", ENV_EXPECTED[(int)current_section]);
      int error = duration - ENV_EXPECTED[(int)current_section];
      error = error < 1 ? -error : error;
      Serial.printf("Error = %dms\n", error);
      if (error <= TOLERANCE_ENV) {
        Serial.println("Passed!\n");
        passed_env++;
      } else Serial.println("Failed...\n");
      
      current_section = wavetable[0].getEnvState();
      Serial.print("State = ");
      Serial.print(STATE_TO_STR[(int)current_section]);
      Serial.println();
      
      if (current_section == STATE_SUSTAIN) {
        while (timer_env < 1000);
        timer_env = 0;
        wavetable[0].stop();
        current_section = wavetable[0].getEnvState();
        Serial.print("\nState = ");
        Serial.print(STATE_TO_STR[(int)current_section]);
        Serial.println();
      }
    }
    
    break;

  /**********************************************************************************************/
    
  case PROC:
  
    if (!flag_init) {
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.print("BEGIN TEST: PROC USAGE");
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.println("\n");
      count = 0;
      note = START_NOTE_PROC;
      for (int i=0; i<NUM_VOICES; i++)
        wavetable[i].setSamples(SAMPLES_PROC, sizeof(SAMPLES_PROC)/sizeof(sample_data));
      flag_init = true;
    }
    
    if (timer >= TICK) {
      float proc_usage = AudioProcessorUsage();
      if (count > 0)
        Serial.printf("Processor usage: %3.5f\n\n", proc_usage);
      if (count == NUM_VOICES || proc_usage > MAX_PROC_USAGE) {
        passed_proc = count;
        Serial.printf("%d/%d concurrent voices\n", passed_proc, NUM_VOICES);
        if (count == NUM_VOICES)
          Serial.println("Passed!\n");
        else
          Serial.println("Failed...\n");
        for (int i=0; i<NUM_VOICES; i++)
          wavetable[i].stop();
        testTypeVal++;
        testType = (testEnum)testTypeVal;
        flag_init = false;
        return;
      }
      wavetable[count++].playNote(note++);
      Serial.printf("voices: %d\n", count);
      timer = 0;
    }
    
    break;

  /**********************************************************************************************/
    
  default:
    if (!flag_init) {
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.print("SUMMARY");
      for (int i=0; i<BAR_LENGTH; i++)
        Serial.print("-");
      Serial.println("\n");
      Serial.println("LATENCY:");
      Serial.printf("Longest latency: %dms\n", longest_latency);
      Serial.printf("Shortest latency: %dms\n", shortest_latency);
      Serial.printf("%d/%d tests passed.\n\n", passed_latency, NUM_TESTS_LATENCY);
      Serial.println("PROC USAGE:");
      Serial.printf("%d/%d tests passed\n\n", passed_proc, NUM_VOICES);
      Serial.println("ENVELOPE:");
      Serial.printf("%d/%d tests passed", passed_env, 5*NUM_TESTS_ENV);
      flag_init = true;
    }
  }
}

