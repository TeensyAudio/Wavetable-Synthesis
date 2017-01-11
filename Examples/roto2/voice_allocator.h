// Copyright (c) 2015 Peter Teichman

#ifndef VOICE_ALLOCATOR_H
#define VOICE_ALLOCATOR_H

typedef uint8_t voice_t;

const uint8_t kNumVoices = 8;
const uint8_t kNoVoice = kNumVoices;

class VoiceAllocator
{
 public:
    VoiceAllocator() {
    }

    void Init() {
        tick_ = 0;
        for (int i=0; i<kNumVoices; i++) {
            voices_[i] = kNoVoice;
            lru_[i] = 0;
        }
    }

    // Press down note, return voice to start playing.
    voice_t NoteOn(uint8_t tag);

    // Release note, return the voice it was on.
    voice_t NoteOff(uint8_t tag);

    int Available() {
        int ret = 0;
        for (int i=0; i<kNumVoices; i++) {
            if (voices_[i] >= kNoVoice) {
                ret++;
            }
        }
        return ret;
    }

 private:
    volatile uint32_t tick_;
    uint8_t voices_[kNumVoices];
    uint32_t lru_[kNumVoices];
};

#endif
