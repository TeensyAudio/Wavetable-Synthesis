// Copyright (c) 2015 Peter Teichman

#include <Arduino.h>

#include "voice_allocator.h"

voice_t VoiceAllocator::NoteOn(uint8_t tag) {
    voice_t voice = kNoVoice;

    // Check existing voices for tag.
    for (int i=0; i<kNumVoices; i++) {
        if ((voices_[i] & 0x7f) == tag) {
            voice = i;
        }
    }

    // Otherwise, find the first used inactive voice.
    if (voice == kNoVoice) {
        uint32_t lru = kNoVoice;
        for (int i=0; i<kNumVoices; i++) {
            if (voices_[i] & 0x80) {
                continue;
            }

            if (lru_[i] < lru) {
                voice = i;
                lru = lru_[i];
            }
        }
    }

    // Otherwise, find the first used active voice.
    if (voice == kNoVoice) {
        uint32_t lru = 0xFFFFFFFF;
        for (int i=0; i<kNumVoices; i++) {
            if (lru_[i] < lru) {
                voice = i;
                lru = lru_[i];
            }
        }
    }

    if (voice != kNoVoice) {
        voices_[voice] = tag;
        tick_++;
        lru_[voice] = tick_;
    }

    return voice;
}

voice_t VoiceAllocator::NoteOff(uint8_t tag) {
    for (int i=0; i<kNumVoices; i++) {
        if ((voices_[i] & 0x7f) == tag) {
            voices_[i] |= 0x80;
            lru_[i] = tick_++;
            return i;
        }
    }
    return kNoVoice;
}
