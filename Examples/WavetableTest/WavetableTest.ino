#include "SF2_Decoded_Samples.h" // http://www.freesound.org/people/kiddpark/sounds/201159/
#include <Bounce.h>
#include <AudioSynthWavetable.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWavetable          wavetable1;       //xy=163.3333282470703,120.33332824707031
AudioSynthWavetable          wavetable2;       //xy=165.3333282470703,196.3333282470703
AudioSynthWavetable          wavetable3;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable4;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable5;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable6;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable7;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable8;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable9;       //xy=169.3333282470703,256.3333282470703
AudioSynthWavetable          wavetable10;
AudioMixer4              mixer1;         //xy=371.3332977294922,158.3333282470703
AudioMixer4              mixer2;         //xy=371.3332977294922,158.3333282470703
AudioMixer4              mixer3;
AudioMixer4              mixer4;
AudioOutputI2S           i2s1;           //xy=525.3333282470703,184.3333282470703
AudioConnection          patchCord1(wavetable1, 0, mixer1, 0);
AudioConnection          patchCord2(wavetable2, 0, mixer1, 1);
AudioConnection          patchCord3(wavetable3, 0, mixer1, 2);
AudioConnection          patchCord4(wavetable4, 0, mixer1, 3);
AudioConnection          patchCord5(wavetable5, 0, mixer2, 0);
AudioConnection          patchCord6(wavetable6, 0, mixer2, 1);
AudioConnection          patchCord7(wavetable7, 0, mixer2, 2);
AudioConnection          patchCord8(wavetable8, 0, mixer2, 3);
AudioConnection          patchCord9(wavetable9, 0, mixer3, 0);
AudioConnection          patchCord10(wavetable10, 0, mixer3, 1);
AudioConnection          patchCord11(mixer1, 0, mixer4, 0);
AudioConnection          patchCord12(mixer2, 0, mixer4, 1);
AudioConnection          patchCord13(mixer3, 0, mixer4, 2);
AudioConnection          patchCord14(mixer4, 0, i2s1, 0);
AudioConnection          patchCord15(mixer4, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=353.3333282470703,260.33331298828125
// GUItool: end automatically generated code

int numVoices = 0;

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
  mixer2.gain(0, 0.4);
  mixer2.gain(1, 0.4);
  mixer2.gain(2, 0.4);
  mixer2.gain(3, 0.4);
  mixer3.gain(0, 0.4);
  mixer3.gain(1, 0.4);
  mixer4.gain(0, 0.4);
  mixer4.gain(1, 0.4);
  float x;
  int x1, x2;

  wavetable1.setSamples(samples);  
  wavetable2.setSamples(samples);
  wavetable3.setSamples(samples);
  wavetable4.setSamples(samples);
  wavetable5.setSamples(samples);
  wavetable6.setSamples(samples);
  wavetable7.setSamples(samples);
  wavetable8.setSamples(samples);
  wavetable9.setSamples(samples);
  wavetable10.setSamples(samples);
  
  wavetable1.amplitude(1);
  wavetable2.amplitude(1);
  wavetable3.amplitude(1);
  wavetable4.amplitude(1);
  wavetable5.amplitude(1);
  wavetable6.amplitude(1);
  wavetable7.amplitude(1);
  wavetable8.amplitude(1);
  wavetable9.amplitude(1);
  wavetable10.amplitude(1);

  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  if (numVoices > 9) return;
  switch (numVoices) {
    case 0:
      wavetable1.playNote(note);
      break;
    case 1:
      wavetable2.playNote(note);
      break;
    case 2:
      wavetable3.playNote(note);
      break;
    case 3:
      wavetable4.playNote(note);
      break;
    case 4:
      wavetable5.playNote(note);
      break;
    case 5:
      wavetable6.playNote(note);
      break;
    case 6:
      wavetable7.playNote(note);
      break;
    case 7:
      wavetable8.playNote(note);
      break;
    case 8:
      wavetable9.playNote(note);
      break;
     case 9:
      wavetable10.playNote(note);
      break;
  }
  numVoices++;
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  if (numVoices <= 0) return;
  switch (numVoices) {
    case 1:
      wavetable1.stop();
      break;
    case 2:
      wavetable2.stop();
      break;
    case 3:
      wavetable3.stop();
      break;
    case 4:
      wavetable4.stop();
      break;
    case 5:
      wavetable5.stop();
      break;
    case 6:
      wavetable6.stop();
      break;
    case 7:
      wavetable7.stop();
      break;
    case 8:
      wavetable8.stop();
      break;
    case 9:
      wavetable9.stop();
      break;
     case 10:
      wavetable10.stop();
      break;
  }
  numVoices--;
}

void loop() {
  usbMIDI.read();
}

