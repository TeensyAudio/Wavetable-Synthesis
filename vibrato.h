enum VibratoMode { Off=0, V1, V2, V3, C1, C2, C3 };

class Vibrato : public AudioStream {
 public:
    Vibrato() : AudioStream(1, inputQueueArray) {
    }

    void Init() {
        
        
        for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++) {
            buf[i] = 0;
        }

    }

    void SetMode(VibratoMode mode) {
        switch (mode) {
        case V1:
            depth = 3;
            mix = 0;
            break;
        case V2:
            depth = 2;
            mix = 0;
            break;
        case V3:
            depth = 1;
            mix = 0;
            break;
        case C1:
            depth = 3;
            mix = 1;
            break;
        case C2:
            depth = 2;
            mix = 1;
            break;
        case C3:
            depth = 1;
            mix = 1;
            break;
        default:
            depth = 8;
        }
    }

    void update(void);

 private:
    audio_block_t *inputQueueArray[1];

    int wp;
    int16_t buf[AUDIO_BLOCK_SAMPLES];

    int depth;
    int mix;
};
