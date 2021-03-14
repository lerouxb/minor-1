#include "waveforms.h"

volatile int32_t jump;
uint32_t phase = 0;
waveform_t waveform = Pulse;

uint32_t pulse = 0;
//int32_t pulseAmount = 85899; /* should take 2 seconds to travel a 32 bit int one direction*/
int32_t pulseAmount = 42949;

uint8_t pulseSample() {

    // forwards/backwards
    if (pulseAmount > 0) {
        if (pulse + pulseAmount > MAX_PULSE) {
            pulseAmount = -pulseAmount;
        }
    }
    else  {
        if (pulse + pulseAmount < MIN_PULSE) {
            pulseAmount = -pulseAmount;
        }
    }

    // TODO: this will drift because the last one won't make it to the end before bouncing back
    pulse += pulseAmount;

    // funny how we're using a DAC and right now we're only using it to make a square wave :)
    if (phase < pulse) {
        return 255;
    }
    else {
        return 0;
    }
}

uint8_t sawSample() {

    // top 8 bits of the 32 bit number
    return phase >> 24;
}

uint8_t triangleSample() {

    int8_t temp = phase >> 24;
    int8_t mask = temp >> 7;

    return (uint8_t) (temp ^ mask) << 1;
}

uint8_t nextSample() {

    uint8_t sample;

    if (jump == -1) {
        sample = 127; // middle value
    }
    else {
        phase = phase + jump;

        switch (waveform) {
            case Pulse:
                sample = pulseSample();
                break;

            case Saw:
                sample = sawSample();
                break;

            case Triangle:
                sample = triangleSample();
                break;
        }
    }

    // for now no post-processing

    return sample;
}
