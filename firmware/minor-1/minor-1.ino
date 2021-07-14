#include <avr/sleep.h>
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

// globals used by interface
volatile bool touchChanged = false;
volatile bool goToSleep = false;


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

    //pinMode(LED_PIN, OUTPUT);
    pinMode(DAC_PIN, OUTPUT);
    pinMode(SHUTDOWN_PIN, OUTPUT);
    pinMode(CHANGE_PIN, INPUT_PULLUP);

    // PIN_PA7
    PORTA.PIN7CTRL |= 0x01; // interrupt on change

    // TODO: make sure all pins are dealt with from a power consumption perspective

    Serial.begin(57600); // at 115200 it wakes too slowly from sleep to be able to read serial communications
    Wire.begin();

    setupInterface();

    Serial.println("OK");

    recalibrateTouch();

    Serial.println("recalibrated");

    // TODO: uart doesn't stay active during power down. it can stay active during standby if we enable that (somewhere?), but maybe we can just set a pin interrupt on RX?
    set_sleep_mode(SLEEP_MODE_STANDBY);
    USART0.CTRLB |= USART_SFDEN_bm; // enable start of frame detection mode
    sleep_enable();
}


void loop() {

    updateInterface();

    // clear interrupts before double-checking to eliminate race conditions
    // see https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
    cli();
    if (goToSleep) {
        // reset goToSleep regardless so that it will be re-set if we don't go to sleep and it still applies
        goToSleep = false;

        // don't go to sleep if there are unprocessed touch events because the touch sensor won't set the change pin high again until we read it. Therefore we have to make very sure we read everything and that it is low so that things don't get stuck
        if (!touchChanged && !isPlaying && digitalRead(CHANGE_PIN) == LOW) {
            sei();
            Serial.flush();
            Serial.println("going to sleep");
            Serial.flush();

            // tripple-check because interrupts are on and we printed things which takes time
            cli();
            if (digitalRead(CHANGE_PIN) == LOW) {
                sei(); // in theory sleep_cpu() straight after sei() is atomic
                sleep_cpu();
            }

            Serial.println("awake!");
        }
    }

    sei(); // in case either if up top was false
}

ISR(TCA0_OVF_vect) {

    //digitalWriteFast(LED_PIN, true);

    ++tick;

    if (isPlaying) {
        digitalWriteFast(SHUTDOWN_PIN, false); // on before next sample
    }
    else {
        goToSleep = true; // this is so the main loop can put the chip to sleep after this interrupt ran
        digitalWriteFast(SHUTDOWN_PIN, true); // off before next sample
    }

    // write the previously calculated sample first so the timing is reliable no matter how long it takes to calculate the next sample
    analogWrite(DAC_PIN, sample);

    // calculate the sample for next time
    sample = nextSample();

    //digitalWriteFast(LED_PIN, false); // TODO: optimise

    // The interrupt flag has to be cleared manually
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR(PORTA_PORT_vect) {
    byte flags = PORTA.INTFLAGS;
    PORTA.INTFLAGS = flags; //clear flags

    if (digitalRead(CHANGE_PIN) == HIGH) {
        touchChanged = true;
    }
}

