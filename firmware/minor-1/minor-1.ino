#include "pins.h"
#include "engine.h"
#include "interface.h"

// TODO: check every menu option

#ifndef MILLIS_USE_TIMERD0
#error "This sketch is written for use with TCD0 as the millis timing source"
#endif

// engine.cpp
extern boolean isPlaying; // TODO: make it a function?

// globals used by interface and engine
volatile uint32_t tick = 0;
volatile uint8_t sample = 127;

void setup() {

    DACReference(INTERNAL2V5);

    // setup a timer overflow interrupt to fire at 25khz
    // system clock is 20Mhz. Prescaler is 16. Period is 49 (so it will be 49+1 = 50)
    TCA0.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp   /* Compare 0 Interrupt: disabled */
                          | 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
                          | 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
                          | 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */

    TCA0.SINGLE.PER = 49; /* 49 + 1 == 50 */

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc /* System Clock / 16 */
                        | 1 << TCA_SINGLE_ENABLE_bp /* Module Enable: enabled */;

    // TODO: set up the sleep stuff

    // presumably we don't have to configure DAC_PIN, KEYS_PIN, serial or i2c pins

    pinMode(LED_PIN, OUTPUT);
    pinMode(DAC_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);

    // TODO: make sure all pins are dealt with from a power consumption perspective

    //initDisplay();

    Serial.begin(115200);
}


void loop() {

    updateInterface();
}


ISR(TCA0_OVF_vect) {

    digitalWriteFast(LED_PIN, true);

    ++tick;

    // TODO: we need to sleep while not playing and wake as soon as a key is pressed
    if (isPlaying) {
        // TODO: optimise
        digitalWriteFast(ENABLE_PIN, true); // off before next sample
    }
    else {
        // TODO: optimise
        digitalWriteFast(ENABLE_PIN, false); // on before next sample
    }

    // write the previously calculated sample first so the timing is reliable no matter how long it takes to calculate the next sample
    analogWrite(DAC_PIN, sample);

    // calculate the sample for next time
    sample = nextSample();

    digitalWriteFast(LED_PIN, false); // TODO: optimise

    // The interrupt flag has to be cleared manually
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
