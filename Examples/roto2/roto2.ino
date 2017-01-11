// Copyright (c) 2015 Peter Teichman

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "drawbar_osc.h"
#include "vibrato.h"
#include "voice_allocator.h"

// GUItool: begin automatically generated code
DrawbarOsc               osc1;
DrawbarOsc               osc2;
DrawbarOsc               osc3;
DrawbarOsc               osc4;
DrawbarOsc               osc5;
DrawbarOsc               osc6;
DrawbarOsc               osc7;
DrawbarOsc               osc8;
AudioEffectEnvelope      envelope1;
AudioEffectEnvelope      envelope2;
AudioEffectEnvelope      envelope3;
AudioEffectEnvelope      envelope4;
AudioEffectEnvelope      envelope5;
AudioEffectEnvelope      envelope6;
AudioEffectEnvelope      envelope7;
AudioEffectEnvelope      envelope8;
AudioSynthWaveformSine   perc;
AudioEffectEnvelope      percEnvelope;
Vibrato                  vibrato;
AudioMixer4              mixer1; // Mixes voices 1-4
AudioMixer4              mixer2; // Mixes voices 5-8
AudioMixer4              mixer3; // Mixes voices with percussion
AudioFilterBiquad        antialias;
AudioOutputI2S           i2s1;
AudioConnection          patchCord17(osc1, envelope1);
AudioConnection          patchCord18(osc2, envelope2);
AudioConnection          patchCord19(osc3, envelope3);
AudioConnection          patchCord20(osc4, envelope4);
AudioConnection          patchCord21(osc5, envelope5);
AudioConnection          patchCord22(osc6, envelope6);
AudioConnection          patchCord23(osc7, envelope7);
AudioConnection          patchCord24(osc8, envelope8);
AudioConnection          patchCord25(perc, percEnvelope);
AudioConnection          patchCord26(envelope1, 0, mixer1, 0);
AudioConnection          patchCord27(envelope2, 0, mixer1, 1);
AudioConnection          patchCord28(envelope3, 0, mixer1, 2);
AudioConnection          patchCord29(envelope4, 0, mixer1, 3);
AudioConnection          patchCord30(envelope5, 0, mixer2, 0);
AudioConnection          patchCord31(envelope6, 0, mixer2, 1);
AudioConnection          patchCord32(envelope7, 0, mixer2, 2);
AudioConnection          patchCord33(envelope8, 0, mixer2, 3);
AudioConnection          patchCord34(mixer1, 0, mixer3, 0);
AudioConnection          patchCord35(mixer2, 0, mixer3, 1);
AudioConnection          patchCord36(percEnvelope, 0, mixer3, 2);
AudioConnection          patchCord37(mixer3, 0, vibrato, 0);
AudioConnection          patchCord38(vibrato, 0, antialias, 0);
AudioConnection          patchCord39(antialias, 0, i2s1, 0);
AudioConnection          patchCord40(antialias, 0, i2s1, 1);
AudioControlSGTL5000     audioShield;
// GUItool: end automatically generated code

DrawbarWavetable wav;
VoiceAllocator voices;

void setup() {
    Serial.begin(115200);

    AudioMemory(20);
    audioShield.enable();
    audioShield.volume(0.7);

    voices.Init();
    initVoice(0);
    initVoice(1);
    initVoice(2);
    initVoice(3);
    initVoice(4);
    initVoice(5);
    initVoice(6);
    initVoice(7);

    wav.Init();
    wav.AllDrawbars(0x888800001L);

    mixer1.gain(0, 0.25);
    mixer1.gain(1, 0.25);
    mixer1.gain(2, 0.25);
    mixer1.gain(3, 0.25);
    mixer2.gain(0, 0.25);
    mixer2.gain(1, 0.25);
    mixer2.gain(2, 0.25);
    mixer2.gain(3, 0.25);

    percEnvelope.attack(20.0);
    percEnvelope.sustain(0.0);
    percEnvelope.decay(80.0);

    vibrato.SetMode(C2);

    mixer3.gain(0, 1.0);
    mixer3.gain(1, 1.0);
    mixer3.gain(2, 0.0625);

    antialias.setLowpass(0, 6000, 0.707);

    usbMIDI.setHandleNoteOff(OnNoteOff);
    usbMIDI.setHandleNoteOn(OnNoteOn);
    usbMIDI.setHandleVelocityChange(OnVelocityChange);
    usbMIDI.setHandleControlChange(OnControlChange);
    usbMIDI.setHandleProgramChange(OnProgramChange);
    usbMIDI.setHandleAfterTouch(OnAfterTouch);
    usbMIDI.setHandlePitchChange(OnPitchChange);
}

void loop() {
    usbMIDI.read(); // USB MIDI receive
}

void initVoice(byte voice) {
    switch (voice) {
    case 0:
        osc1.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope1);
        break;
    case 1:
        osc2.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope2);
        break;
    case 2:
        osc3.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope3);
        break;
    case 3:
        osc4.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope4);
        break;
    case 4:
        osc5.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope5);
        break;
    case 5:
        osc6.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope6);
        break;
    case 6:
        osc7.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope7);
        break;
    case 7:
        osc8.Begin(0.0, 1.0, &wav);
        initEnvelope(&envelope8);
        break;
    }
}

void initEnvelope(AudioEffectEnvelope *env) {
    env->attack(2.0);
    env->sustain(1.0);
    env->release(2.0);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
    Serial.print("Note On, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.print(velocity, DEC);
    Serial.println();

    float fund = note2freq(note);

    if (voices.Available() == kNumVoices) {
        perc.frequency(2*fund);
        // Compensate for frequency response: normalize to highest MIDI freq.
        perc.amplitude(log2(4186.0)/log2(fund));
        // percEnvelope.noteOn();
    }
  
    voice_t voice = voices.NoteOn(note);

    switch (voice) {
    case 0:
        osc1.Fundamental(fund);
        envelope1.noteOn();
        break;
    case 1:
        osc2.Fundamental(fund);
        envelope2.noteOn();
        break;
    case 2:
        osc3.Fundamental(fund);
        envelope3.noteOn();
        break;
    case 3:
        osc4.Fundamental(fund);
        envelope4.noteOn();
        break;
    case 4:
        osc5.Fundamental(fund);
        envelope5.noteOn();
        break;
    case 5:
        osc6.Fundamental(fund);
        envelope6.noteOn();
        break;
    case 6:
        osc7.Fundamental(fund);
        envelope7.noteOn();
        break;
    case 7:
        osc8.Fundamental(fund);
        envelope8.noteOn();
        break;
    }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
    Serial.print("Note Off, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.print(velocity, DEC);
    Serial.println();

    Serial.print("CPU=");
    Serial.print(AudioProcessorUsage());
    Serial.print(",");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("    ");
    Serial.print("Memory: ");
    Serial.print(AudioMemoryUsage());
    Serial.print(",");
    Serial.print(AudioMemoryUsageMax());
    Serial.println();

    voice_t voice = voices.NoteOff(note);
    switch (voice) {
    case 0:
        envelope1.noteOff();
        break;
    case 1:
        envelope2.noteOff();
        break;
    case 2:
        envelope3.noteOff();
        break;
    case 3:
        envelope4.noteOff();
        break;
    case 4:
        envelope5.noteOff();
        break;
    case 5:
        envelope6.noteOff();
        break;
    case 6:
        envelope7.noteOff();
        break;
    case 7:
        envelope8.noteOff();
        break;
    }
}

void OnVelocityChange(byte channel, byte note, byte velocity) {
    Serial.print("Velocity Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", note=");
    Serial.print(note, DEC);
    Serial.print(", velocity=");
    Serial.print(velocity, DEC);
    Serial.println();
}

void OnControlChange(byte channel, byte control, byte value) {
    Serial.print("Control Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", control=");
    Serial.print(control, DEC);
    Serial.print(", value=");
    Serial.print(value, DEC);
    Serial.println();

    if (control >= 16 && control < 25) {
        int drawbar = control - 16;
        float val = (float)value / 127.0;
        wav.Drawbar(drawbar, val);
    }
}

void OnProgramChange(byte channel, byte program) {
    Serial.print("Program Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", program=");
    Serial.print(program, DEC);
    Serial.println();
}

void OnAfterTouch(byte channel, byte pressure) {
    Serial.print("After Touch, ch=");
    Serial.print(channel, DEC);
    Serial.print(", pressure=");
    Serial.print(pressure, DEC);
    Serial.println();
}

void OnPitchChange(byte channel, int pitch) {
    Serial.print("Pitch Change, ch=");
    Serial.print(channel, DEC);
    Serial.print(", pitch=");
    Serial.print(pitch, DEC);
    Serial.println();
}

float note2freq(byte note) {
    float tmp = (note-69) / 12.0;
    return 440.0 * pow(2.0, tmp);
}
