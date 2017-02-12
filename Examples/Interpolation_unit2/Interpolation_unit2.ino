#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//---------------------------------------------------------------------------------------
#include <AudioSynthWavetable.h>
#include "SF2_Decoded_Samples.h"
//---------------------------------------------------------------------------------------
AudioAnalyzeNoteFrequency notefreq;
AudioOutputI2S            i2s1;
AudioSynthWavetable       wavetable;
AudioMixer4               mixer;
//---------------------------------------------------------------------------------------
AudioConnection patchCord0(wavetable, 0, mixer, 0);
AudioConnection patchCord1(mixer, 0, notefreq, 0);
AudioConnection patchCord2(mixer, 0, i2s1, 0);
AudioConnection patchCord3(mixer, 0, i2s1, 1);
//---------------------------------------------------------------------------------------
AudioControlSGTL5000     sgtl5000_1;

elapsedMillis timer = 0;
int count = 60;
int note = 0;
double error = 0;
float freq = 0;
double freqSum = 0;
float prob = 0;
int analysis_count = 0;
int delay_count = 0;
int passed = 0;
int analysis_count_total = 0;
bool flag_analysis = false;
bool flag_stop = false;

void setup() {
  AudioMemory(30);
  /*
   *  Initialize the yin algorithm's absolute
   *  threshold, this is good number.
   */
  notefreq.begin(.15);
  wavetable.setSamples(samples, sizeof(samples)/sizeof(sample_data));
  while (timer < 2000); // Spin for serial monitor
}

void loop() {
  if (!flag_stop) {
    if (timer >= 500) {
      if (delay_count >= 2) {
        analysis_count_total++;
        freq = freqSum/analysis_count;
        error = freq/wavetable.noteToFreq(note);
        if (error < 1) error = 1 - error;
        else error -= 1;
        Serial.printf("error=%3.5f\n", error);
        if (error < 0.002) {
          Serial.println("Passed!\n");
          passed++;
        } else Serial.println("Failed...\n");
        analysis_count = 0;
        freqSum = 0;
        delay_count = 0;
      }
      
      if (count == 60 && flag_analysis) {
        Serial.printf("%d/%d tests passed.\n\n", passed, analysis_count_total);
        flag_stop = true;
        return;
      }
      
      note = count--;
      wavetable.playNote(note); // A3 = 57
      Serial.printf("note=%d\n", note);
      for (int i=0; i<100; i++)
        Serial.print("-");
      Serial.println();
      timer = 0;
    }
    
    if (notefreq.available()) {
      if (delay_count++ >= 2) {
        analysis_count++;
        freq = notefreq.read();
        prob = notefreq.probability();
        freqSum += freq;
        Serial.printf("Expected: %3.2f | Actual: %3.2f | Probability: %.2f\n", wavetable.noteToFreq(note), freq, prob);
        flag_analysis = true;
      }
    }
    
    if (count == 20) count = 100;
    
  } else wavetable.stop();
}


