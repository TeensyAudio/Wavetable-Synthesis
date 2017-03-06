# Examples

This section is currently under construction

This is an example:

```
#include "AudioStream.h"

class AudioEffectTemplate : public AudioStream
{
public:
        AudioEffectTemplate() : AudioStream(1, inputQueueArray);
        virtual void update(void);
private:
        audio_block_t *inputQueueArray[1];
};
```

