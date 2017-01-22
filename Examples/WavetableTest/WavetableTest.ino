#include "piano.h" // http://www.freesound.org/people/kiddpark/sounds/201159/
#include <Bounce.h>
#include <AudioAllocWavetable.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioSynthWavetable wavetable[3];
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

const int NUM_VOICES = 3;
uint8_t voices[NUM_VOICES];

void setup() {
	Serial.begin(9600);

	for (int i = 0; i < 3; ++i)
		pinMode(i, INPUT_PULLUP);

	AudioMemory(10);
	sgtl5000_1.enable();
	sgtl5000_1.volume(0.5);

	for (int i = 0; i < 3; ++i)
		mixer.gain(i, 0.4);

	for (int i = 0; i < 3; ++i) {
		wavetable[i].setSample(AudioWaveform_Loop);
	}

	wavetable[0].setFreqAmp(523.25, 0.5);
	wavetable[1].setFreqAmp(659.25, 0.5);
	wavetable[2].setFreqAmp(783.99, 0.5);
}

void loop() {
	// Update all the button objects

	for (int i = 0; i < 3; ++i) {
		button[i].update();
		if (button[i].fallingEdge())
			wavetable[i].play();
		if (button[i].risingEdge())
			wavetable[i].stop();
	}
}

