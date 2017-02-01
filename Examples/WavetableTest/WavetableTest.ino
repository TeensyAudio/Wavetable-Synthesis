#include "midi_samples.h"
#include <Bounce.h>
#include <AudioSynthWavetable.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const int TOTAL_VOICES = 16;
AudioSynthWavetable wavetable[TOTAL_VOICES];
AudioMixer4 mixer[TOTAL_VOICES / 4 + 1];
AudioOutputI2S i2s1;
AudioConnection patchCord[] = {
	{wavetable[0],  0, mixer[0 / 4], 0},
	{wavetable[1],  0, mixer[1 / 4], 1},
	{wavetable[2],  0, mixer[2 / 4], 2},
	{wavetable[3],  0, mixer[3 / 4], 3},
	{wavetable[4],  0, mixer[4 / 4], 0},
	{wavetable[5],  0, mixer[5 / 4], 1},
	{wavetable[6],  0, mixer[6 / 4], 2},
	{wavetable[7],  0, mixer[7 / 4], 3},
	{wavetable[8],  0, mixer[8 / 4], 0},
	{wavetable[9],  0, mixer[9 / 4], 1},
	{wavetable[10], 0, mixer[10 / 4], 2},
	{wavetable[11], 0, mixer[11 / 4], 3},
	{wavetable[12], 0, mixer[12 / 4], 0},
	{wavetable[13], 0, mixer[13 / 4], 1},
	{wavetable[14], 0, mixer[14 / 4], 2},
	{wavetable[15], 0, mixer[15 / 4], 3},
	{mixer[0], 0, mixer[TOTAL_VOICES / 4], 0},
	{mixer[1], 0, mixer[TOTAL_VOICES / 4], 1},
	{mixer[2], 0, mixer[TOTAL_VOICES / 4], 2},
	{mixer[3], 0, mixer[TOTAL_VOICES / 4], 2},
	{mixer[TOTAL_VOICES / 4], 0, i2s1, 0},
	{mixer[TOTAL_VOICES / 4], 0, i2s1, 1}
};
AudioControlSGTL5000 sgtl5000_1;

uint16_t voices[TOTAL_VOICES];
byte voice_queue[TOTAL_VOICES];
byte channel_map[16];
int voicesAvailable = TOTAL_VOICES;

void setup() {
	Serial.begin(9600);

	pinMode(0, INPUT_PULLUP);
	pinMode(1, INPUT_PULLUP);
	pinMode(2, INPUT_PULLUP);

	AudioMemory(10);

	sgtl5000_1.enable();
	sgtl5000_1.volume(0.5);

	for (int i = 0; i < TOTAL_VOICES; ++i) {
		wavetable[i].setSample(sample);
		wavetable[i].amplitude(0.5);
		mixer[i / 4].gain(i % 4, 0.4);
		voices[i] = 0xFFFF;
		voice_queue[i] = i;
	}

	for (int i = 0; i < 16; ++i) {
		channel_map[i] = 0;
	}

	for (int i = 0; i < TOTAL_VOICES / 4; ++i)
		mixer[TOTAL_VOICES / 4].gain(i, 0.4);


	usbMIDI.setHandleNoteOn(OnNoteOn);
	usbMIDI.setHandleNoteOff(OnNoteOff);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
	int i, j, k;
	j = k = TOTAL_VOICES;
	uint16_t sig = channel << 8 | note;
	for (i = 0; i < TOTAL_VOICES && voices[i] != sig; ++i) {
		j = voices[i] == sig | 0xFF ? i : j;
		k = voices[i] == 0xFFFF ? i : k;
	}

	if (i != TOTAL_VOICES)
		i = i;
	else if (j != TOTAL_VOICES)
		i = j;
	else if (k != TOTAL_VOICES) {
		i = k;
		wavetable[i].setSample(midi_map[channel_map[channel]]);
	} else {
		i = voice_queue[TOTAL_VOICES - 1];
		for (j = TOTAL_VOICES - 1; j > 0; --j)
			voice_queue[j] = voice_queue[j - 1];
		voice_queue[0] = i;
	}
	for (j = 0; i != voice_queue[j]; ++j);
	for (j = voice_queue[j]; j > 0; --j)
		voice_queue[j] = voice_queue[j - 1];

	voices[i] = sig1;
	wavetable[i].playNote(note);
	voicesAvailable--;
}

void OnNoteOff(byte channel, byte note, byte velocity) {
	if (voicesAvailable == TOTAL_VOICES)
		return;
	int i;
	uint16_t sig = channel << 8 | note;
	for (i = 0; i < TOTAL_VOICES && voices[i] != sig; ++i);
	if (i == TOTAL_VOICES)
		return;
	wavetable[i].stop();
	voices[i] = 0xFFFF;
	voicesAvailable++;
}

void loop() {
	usbMIDI.read();
}

