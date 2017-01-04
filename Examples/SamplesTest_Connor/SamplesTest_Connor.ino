// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-3: Playing Samples

// WAV files converted to code by wav2sketch
#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/
#include <Bounce.h>
#include <wavetable.h>


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;       //xy=163.3333282470703,120.33332824707031
AudioPlayMemory          playMem2;       //xy=165.3333282470703,196.3333282470703
AudioPlayMemory          playMem3;       //xy=169.3333282470703,256.3333282470703
AudioMixer4              mixer1;         //xy=371.3332977294922,158.3333282470703
AudioOutputI2S           i2s1;           //xy=525.3333282470703,184.3333282470703
AudioConnection          patchCord1(playMem1, 0, mixer1, 0);
AudioConnection          patchCord2(playMem2, 0, mixer1, 1);
AudioConnection          patchCord3(playMem3, 0, mixer1, 2);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=353.3333282470703,260.33331298828125
// GUItool: end automatically generated code

// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 ms debounce time
Bounce button2 = Bounce(2, 15);

const int AudioSample_Size = (int)(sizeof(AudioSampleCashregister)/4);
unsigned int AudioSampleCashregister_Interpolate[AudioSample_Size];
unsigned int AudioSampleCashregister_Interpolate1[AudioSample_Size];

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  mixer1.gain(0, 0.4);
  mixer1.gain(1, 0.4);
  mixer1.gain(2, 0.4);
  mixer1.gain(3, 0.4);
  unsigned int s0;
  unsigned int s1;
  float x;
  int x1, x2;

 for (int i = 0; i < (int)((AudioSample_Size-1)/2); i++) {
    s1 = AudioSampleCashregister[i*2];
    AudioSampleCashregister_Interpolate1[i] = s1;
 } 

 for (int i = 0; i < AudioSample_Size-1; i++) {
    x = i*.9;
    if (floor(x) == x) {
      s1 = AudioSampleCashregister[(int)x];
    }
    else {
      x1 = floor(x);
      x2 = ceil(x);
      s0 = AudioSampleCashregister[x1];
      s1 = AudioSampleCashregister[x2];
      s1 = (max(s0,s1)-min(s0,s1)) + min(s0,s1);
    }
    AudioSampleCashregister_Interpolate[i] = s1;
 } 
}

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();

  if (button0.fallingEdge()) {
    playMem1.play(AudioSampleCashregister); 
  }
  if (button1.fallingEdge()) {
    playMem2.play(AudioSampleCashregister_Interpolate);
  }
  if (button2.fallingEdge()) {
    playMem3.play(AudioSampleCashregister_Interpolate1);
  }

/*
  int knob = analogRead(A3);
  if (button0.fallingEdge()) {
    if (knob < 512) {
      playMem1.play(AudioSampleSnare);
    } else {
      playMem1.play(AudioSampleKick);
    }
  }
  if (button1.fallingEdge()) {
    if (knob < 512) {
      playMem2.play(AudioSampleTomtom);
    } else {
      playMem4.play(AudioSampleGong);
    }
  }
  if (button2.fallingEdge()) {
    if (knob < 512) {
      playMem3.play(AudioSampleHihat);
    } else {
      playMem3.play(AudioSampleCashregister);
    }
  }
*/

}
