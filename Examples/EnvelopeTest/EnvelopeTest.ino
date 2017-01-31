#include "piano.h" // http://www.freesound.org/people/kiddpark/sounds/201159/
#include <Bounce.h>
#include <AudioAllocWavetable.h>
#include <effect_envelope_test.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include </Users/ConnorReilly/Desktop/Arduino.app/Contents/Java/hardware/teensy/avr/libraries/SD/SD.h>
#include <SerialFlash.h>

AudioSynthWavetable wavetable[3];
AudioEffectEnvelopeTest env[3];
AudioMixer4 mixer;
AudioOutputI2S i2s1;
AudioConnection patchCord[] = {
  {wavetable[0], 0, env[0], 0},
  {wavetable[1], 0, env[1], 0},
  {wavetable[2], 0, env[2], 0},
  {env[0], 0, mixer, 0},
  {env[1], 0, mixer, 1},
  {env[2], 0, mixer, 2},
  {mixer, 0, i2s1, 0},
  {mixer, 0, i2s1, 1}
};
AudioControlSGTL5000 sgtl5000_1;

// Bounce objects to read pushbuttons 
Bounce button[] = { {0, 15}, {1, 15}, {2, 15} };

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 3; ++i)
    pinMode(i, INPUT_PULLUP);

  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  for (int i = 0; i < 3; ++i) {
    mixer.gain(i, 0.4);
    wavetable[i].setSample(AudioWaveform_Loop);
    env[i].delay(1000);
    env[i].attack(11000);
    env[i].hold(11000);
    env[i].decay(11000);
    env[i].sustain(.667);
    env[i].release(11000);
  }
}

void loop() {
  // Update all the button objects

  for (int i = 0; i < 3; ++i) {
    button[i].update();
    byte note = 0;
    switch (i) {
    case 0: note = 72; break;
    case 1: note = 76; break;
    case 2: note = 79; break;
    }
    if (button[i].fallingEdge()) {
      wavetable[i].playNote(note);
      env[i].noteOn();
    }
    if (button[i].risingEdge()) {
      env[i].noteOff();
    }
    if (env[i].isIdle()) {
      wavetable[i].stop();
    }
  }
}

