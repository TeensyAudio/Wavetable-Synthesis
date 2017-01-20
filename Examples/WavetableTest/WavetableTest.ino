#include "piano.h" // http://www.freesound.org/people/kiddpark/sounds/201159/
#include <Bounce.h>
#include <AudioWavetableSynth.h>
#include <AudioWavetableAllocator.h>

///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioWavetableAllocator  allocator;
AudioMixer4              mixer1;         //xy=371.3332977294922,158.3333282470703
AudioOutputI2S           i2s1;           //xy=525.3333282470703,184.3333282470703
AudioConnection          patchCord1(*allocator.getVoice(0), 0, mixer1, 0);
AudioConnection          patchCord2(*allocator.getVoice(1), 0, mixer1, 1);
AudioConnection          patchCord3(*allocator.getVoice(2), 0, mixer1, 2);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=353.3333282470703,260.33331298828125
// GUItool: end automatically generated code

// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 ms debounce time
Bounce button2 = Bounce(2, 15);

const int NUM_VOICES = 3;
uint8_t voices[NUM_VOICES];

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
  for (int i=0; i<NUM_VOICES; i++) {
    voices[i]=0;
  }
}

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();

  if (button0.fallingEdge()) {
    voices[0] = allocator.play(AudioWaveform_Loop, 2, 0.5);
  }
  if (button1.fallingEdge()) {
    voices[1] = allocator.play(AudioWaveform_Loop, 2, 0.5);
  }
  if (button2.fallingEdge()) {
    voices[2] = allocator.play(AudioWaveform_Loop, 2, 0.5);
  }
  if (button0.risingEdge()) {
    allocator.stop(voices[0]);
  }
  if (button1.risingEdge()) {
    allocator.stop(voices[1]);
  }
  if (button2.risingEdge()) {
    allocator.stop(voices[2]);
  }
  
}

