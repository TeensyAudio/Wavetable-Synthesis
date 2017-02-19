#include "steelstrgtr_samples.h"
#include <AudioSynthWavetable.h>
#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define DEBUG_BABY

char* note_map[] = {
	"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

IntervalTimer midiMapTimer;

const int TOTAL_VOICES = 16;
const int TOTAL_MIXERS = 5;
struct voice_t {
	int wavetable_id;
	byte channel;
	byte note;
};
voice_t voices[TOTAL_VOICES];

int allocateVoice(byte channel, byte note);
int freeVoice(byte channel, byte note);

int used_voices = 0;
int evict_voice = 0;


AudioControlSGTL5000 sgtl5000_1;
AudioSynthWavetable wavetable[TOTAL_VOICES];
AudioMixer4 mixer[TOTAL_MIXERS];
//AudioAnalyzeNoteFrequency notefreq;
AudioOutputI2S i2s1;
AudioConnection patchCord[] = {
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
	{mixer[0], 0, mixer[4], 0},
	{mixer[1], 0, mixer[4], 1},
	{mixer[2], 0, mixer[4], 2},
	{mixer[3], 0, mixer[4], 3},
	{mixer[4], 0, i2s1, 0},
	{mixer[4], 0, i2s1, 1},
};

// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 ms debounce time
Bounce button2 = Bounce(2, 15);

void printVoices();

void setup() {
	Serial.begin(38400);

	pinMode(0, INPUT_PULLUP);
	pinMode(1, INPUT_PULLUP);
	pinMode(2, INPUT_PULLUP);

	AudioMemory(40);

	sgtl5000_1.enable();
	sgtl5000_1.volume(1);

	for (int i = 0; i < TOTAL_VOICES; ++i) {
		mixer[i / 4].gain(i % 4, 1);
		wavetable[i].setSamples(steelstrgtr, sizeof(steelstrgtr) / sizeof(sample_data));
		wavetable[i].amplitude(1);
		voices[i].wavetable_id = i;
		voices[i].channel = voices[i].note = 0xFF;
	}

	usbMIDI.setHandleNoteOn(OnNoteOn);
	usbMIDI.setHandleNoteOff(OnNoteOff);

	midiMapTimer.begin(printVoices, 50000);
}

#ifdef DEBUG_BABY
void printVoices() {
	static bool first_time = true;
	if (first_time) {
		first_time = false;
		Serial.print(" usd evt::");
		for (int i = 0; i < TOTAL_VOICES; ++i)
			Serial.printf("%02i c   n", i);
		Serial.print('\n');
	}
	Serial.printf("\n %02i %02i", used_voices, evict_voice);
	for (int i = 0; i < used_voices; ++i)
		Serial.printf(" %02hhu %-2s", voices[i].channel, note_map[voices[i].note%12]);
}
#endif //DEBUG_ALLOC

void OnNoteOn(byte channel, byte note, byte velocity) {
#ifdef DEBUG_ALLOC
	//Serial.printf("**** NoteOn: channel==%hhu,note==%hhu ****\n", channel, note);
	printVoices();
#endif //DEBUG_ALLOC
	int voice_id = allocateVoice(channel, note);
	wavetable[voices[voice_id].wavetable_id].playNote(note);
#ifdef DEBUG_ALLOC
	printVoices();
#endif //DEBUG_ALLOC
}

void OnNoteOff(byte channel, byte note, byte velocity) {
#ifdef DEBUG_ALLOC
	//Serial.printf("**** NoteOff: channel==%hhu,note==%hhu ****\n", channel, note);
	printVoices();
#endif //DEBUG_ALLOC

	int voice_id = freeVoice(channel, note);
	if (voice_id == TOTAL_VOICES) return;
	wavetable[voices[voice_id].wavetable_id].stop();

#ifdef DEBUG_ALLOC
	printVoices();
#endif //DEBUG_ALLOC
}

void loop() {
	usbMIDI.read();
}

int allocateVoice(byte channel, byte note) {
	int i;

	if (used_voices < TOTAL_VOICES) {
		//find match
		for (i = 0; i < used_voices && !(voices[i].channel == channel && voices[i].note == note); ++i);
		//if no match
		if (i == used_voices) {
			//find same channel (== same sample)
			while (i < TOTAL_VOICES && voices[i].channel != channel) ++i;
			//if found same channel, swap wavetable objects
			if (i < TOTAL_VOICES) {
				int wavetable_id = voices[i].wavetable_id;
				voices[i].wavetable_id = voices[used_voices].wavetable_id;
				voices[i].channel = voices[used_voices].channel;
				voices[used_voices].wavetable_id = wavetable_id;
			}
			i = used_voices;
			used_voices++;
		}
	} else {
		i = evict_voice;
	}

	voices[i].channel = channel;
	voices[i].note = note;

	evict_voice = i + 1;
	evict_voice = evict_voice >= TOTAL_VOICES ? 0 : evict_voice;

	return i;
}

int freeVoice(byte channel, byte note) {
	int i;

	//find match
	for (i = 0; i < used_voices && !(voices[i].channel == channel && voices[i].note == note); ++i);
	//return TOTAL_VOICES if no match
	if (i == used_voices) return TOTAL_VOICES;

	used_voices--;

	//swap voice data if match (i.e. allocated voices are at beginning of array)
	int wavetable_id = voices[i].wavetable_id;
	voices[i] = voices[used_voices];
	voices[used_voices].channel = channel;
	voices[used_voices].wavetable_id = wavetable_id;

	return used_voices;
}

