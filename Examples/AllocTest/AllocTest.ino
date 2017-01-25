#include "piano.h" // http://www.freesound.org/people/kiddpark/sounds/201159/
#include <Bounce.h>
#include <AudioAllocWavetable.h>
#include <AudioSynthWavetable.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define SAME_NOTE_TEST

const unsigned int NUM_VOICES = 3;

AudioSynthWavetable wavetable[NUM_VOICES];
AudioAllocWavetable alloc(wavetable, NUM_VOICES, AudioWaveform_Loop);
AudioMixer4 mixer;
AudioOutputI2S i2s1;
AudioConnection patchCord[] = {
	{wavetable[0], 0, mixer, 0},
	{wavetable[1], 0, mixer, 1},
	{wavetable[2], 0, mixer, 2},
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
		//wavetable[i].setSample(AudioWaveform_Loop);
	}
}

void loop() {
	// Update all the button objects

	for (int i = 0; i < 3; ++i) {
		button[i].update();
		byte note = 0;
		switch (i) {
#ifdef SAME_NOTE_TEST
  		case 0: note = 72; break;
      case 1: note = 72; break;
      case 2: note = 72; break;
#else
      case 0: note = 72; break;
      case 1: note = 76; break;
      case 2: note = 79; break;
#endif
		}
		if (button[i].fallingEdge())
      alloc.playNote(note);
		if (button[i].risingEdge())
      alloc.stopNote(note);
	}
}

