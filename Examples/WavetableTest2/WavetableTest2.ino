#include "nylonstrgtr_samples.h"
#include <AudioSynthWavetable.h>
#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define DEBUG_BABY

const char* note_map[] = {
	"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

IntervalTimer midiMapTimer;
IntervalTimer guitarHeroTimer;
IntervalTimer volumeTimer;


const int TOTAL_VOICES = 16;
const int TOTAL_MIXERS = (TOTAL_VOICES-1)/4+2;
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
int notes_played = 0;


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
	{mixer[0], 0, mixer[TOTAL_MIXERS-1], 0},
	{mixer[1], 0, mixer[TOTAL_MIXERS-1], 1},
	{mixer[2], 0, mixer[TOTAL_MIXERS-1], 2},
	{mixer[3], 0, mixer[TOTAL_MIXERS-1], 3},
	{mixer[TOTAL_MIXERS-1], 0, i2s1, 0},
	{mixer[TOTAL_MIXERS-1], 0, i2s1, 1},
};

// Bounce objects to read pushbuttons 
Bounce buttons[] = { {0, 15}, {1, 15}, {2, 15}, };
const int TOTAL_BUTTONS = sizeof(buttons) / sizeof(Bounce);

void printVoices();

void setVolume() {
	sgtl5000_1.volume((analogRead(PIN_A2)-1)/1022.0);
}

void setup() {
	Serial.begin(115200);

	pinMode(0, INPUT_PULLUP);
	pinMode(1, INPUT_PULLUP);
	pinMode(2, INPUT_PULLUP);

	AudioMemory(40);

	sgtl5000_1.enable();
	sgtl5000_1.volume(1.0);
	volumeTimer.begin(setVolume, 100000);

	for (int i = 0; i < TOTAL_VOICES; ++i) {
		mixer[i / 4].gain(i % 4, 1);
		wavetable[i].setSamples(nylonstrgtr, sizeof(nylonstrgtr) / sizeof(sample_data));
		wavetable[i].amplitude(1);
		voices[i].wavetable_id = i;
		voices[i].channel = voices[i].note = 0xFF;
	}

	usbMIDI.setHandleNoteOn(OnNoteOn);
	usbMIDI.setHandleNoteOff(OnNoteOff);

	//performanceTimer.begin(AudioSynthWavetable::print_performance, 2000000);
}

void guitarHeroMode() {
	char line[129];
	line[128] = '\0';
	for (int i = 0; i < 128; ++i) line[i] = '-';
	for (int i = 0; i < used_voices; ++i) line[voices[i].note] = '*';
	Serial.println(line);
}

#ifdef DEBUG_BABY
void printVoices() {
	static int last_notes_played = notes_played;
	if (last_notes_played == notes_played)
		return;
	last_notes_played = notes_played;
	int usage = AudioProcessorUsage();
	Serial.printf("\nCPU:%03i voices:%02i CPU/Voice:%02i evict:%02i", usage, used_voices, usage/used_voices, evict_voice);
	for (int i = 0; i < used_voices; ++i)
		Serial.printf(" %02hhu %-2s", voices[i].channel, note_map[voices[i].note%12]);

}
#endif //DEBUG_BABY

void OnNoteOn(byte channel, byte note, byte velocity) {
	notes_played++;
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
	for (int i = 0; i < TOTAL_BUTTONS; ++i) buttons[i].update();
	if (buttons[0].fallingEdge()) AudioSynthWavetable::print_performance();
	if (buttons[1].risingEdge()) {
		midiMapTimer.end();
		Serial.print('\n');
	}
	if (buttons[1].fallingEdge()) midiMapTimer.begin(printVoices, 5000);
	if (buttons[2].risingEdge()) guitarHeroTimer.end();
	if (buttons[2].fallingEdge()) guitarHeroTimer.begin(guitarHeroMode, 1000000/60);
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

