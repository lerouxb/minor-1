#include <driver_init.h>
#include <compiler.h>
#include "waveforms.h"

extern uint32_t tick;
extern int32_t jump;

uint32_t phase = 0; // the number that wraps around every at the same frequency as the note being played
uint32_t tick = 0; // a global tick number that increments every time the dac sample ISR fires

uint8_t sample = 127;

extern waveform_t waveform;

ISR(TCA0_OVF_vect) {
    ++tick;

    // output the sample we calculated last time as the first thing we do so the timing is consistent no matter which branches we descend later in the code
    DAC_0_set_output(sample);

    if (jump == -1) {
        sample = 127; // middle value
        DISABLE_set_level(false); // off before next sample
    }
    else {
        phase = phase + jump;

        switch (waveform) {
            case Pulse:
                sample = pulse_sample();
                break;

            case Saw:
                sample = saw_sample();
                break;

            case Triangle:
                sample = triangle_sample();
                break;
        }

        DISABLE_set_level(true); // on before next sample
    }

    // for now no post-processing

    /* The interrupt flag has to be cleared manually */
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
