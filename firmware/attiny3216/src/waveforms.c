#include "waveforms.h"

extern int32_t jump;
extern uint32_t phase;

uint32_t pulse = 0;
int32_t pulse_amount = 85899; /* should take 2 seconds to travel a 32 bit int one direction*/

uint8_t pulse_sample() {

    // forwards/backwards
    if (pulse_amount > 0) {
        if (pulse + pulse_amount > MAX_PULSE) {
            pulse_amount = -pulse_amount;
        }
    }
    else  {
        if (pulse + pulse_amount < MIN_PULSE) {
            pulse_amount = -pulse_amount;
        }
    }

    // TODO: this will drift because the last one won't make it to the end before bouncing back
    pulse += pulse_amount;

    // funny how we're using a DAC and right now we're only using it to make a square wave :)
    if (phase < pulse) {
        //DAC_0_set_output(255);
        return 255;
    }
    else {
        //DAC_0_set_output(0);
        return 0;
    }
}

uint8_t saw_sample() {

    // top 8 bits of the 32 bit number
    return phase >> 24;
}

uint8_t triangle_sample() {

    int8_t temp = phase >> 24;
    int8_t mask = temp >> 7;

    return (uint8_t) (temp ^ mask) << 1;
}
