#include <Tiny4kOLED.h>
#include "notes.h"
#include "waveforms.h"

#ifndef MILLIS_USE_TIMERD0
#error "This sketch is written for use with TCD0 as the millis timing source"
#endif

// about 25000 ticks per second
#define DEBOUNCE_TICKS 1250
#define REPEAT_TICKS1 6000 // first repeat is slower to prevent accidents
#define REPEAT_TICKS2 2000

#define LED_PIN (PIN_PA3)
#define KEYS_PIN (PIN_PA4)
#define ENABLE_PIN (PIN_PA5)
#define DAC_PIN (PIN_PA6)
#define BUTTON_DOWN_PIN (PIN_PC0)
#define BUTTON_UP_PIN (PIN_PC1)
#define BUTTON_SELECT_PIN (PIN_PC2)


volatile uint32_t tick = 0;
volatile uint8_t sample = 127;

// in waveforms.cpp
extern volatile int32_t jump;
extern volatile uint32_t phase;
extern waveform_t waveform;

// keyboard state
uint16_t keyADC;
int8_t key;
uint8_t octave = 2;

// up/down buttons state
uint32_t lastTick = 0;
uint16_t bounce = 0;
uint16_t repeat = 0;
bool repeating = false;

uint32_t frame = 0;

void setup() {

    DACReference(INTERNAL0V55);

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
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);

    // TODO: make sure all pins are dealt with from a power consumption perspective

    oled.begin();
    oled.setFont(FONT8X16);
    clearDisplay();
    oled.on();
}


void loop() {

    ++frame;

    // UP/DOWN

    int8_t delta = checkUpDown();

    if (delta != 0) {
        if (octave + delta >= 0 && octave + delta <= 7) {
            octave += delta;
        }
    }

    // KEYBOARD

    keyADC = analogRead(KEYS_PIN);
    key = getKey(keyADC);

    if (key == -1) {
        // Prefer to keep the previous note if it looks like something was pressed because the most likely explanation is that two keys are held down and it is _just_ barely putting us out of range of either of them.
        if (keyADC < 740) {
            jump = -1;
        }
    }
    else {
        jump = notes[3 + (octave * 12) + key];
    }

    // DISPLAY

    updateDisplay();
}


void clearDisplay() {

    // Setup the first half of memory.
    oled.clear();

    // Switch the half of RAM that we are writing to, to be the half that is non currently displayed.
    oled.switchRenderFrame();

    // Setup the second half of memory.
    oled.clear();

    // Switch back to being ready to render on the first frame while displaying the second frame.
    oled.switchFrame();
}


int8_t getKey(uint16_t value) {

    // TODO: subtract 10, use as start, check > that and < that plus 20. Should be faster.
    unsigned int midpoint = 755;
    int8_t key = -1;

    for (int8_t i=0; i<13; ++i) {
        if (value > (midpoint - 10) && value < (midpoint + 10)) {
            key = i;
            break;
        }
        midpoint += 20;
    }

    return key;
}


void updateDisplay() {

    uint8_t step = frame % 4;

    if (step == 0) {

        // Clear whatever random data has been left in memory.
        oled.clear();
    }
    else {

        // Position the text cursor
        // In order to keep the library size small, text can only be positioned
        // with the top of the font aligned with one of the four 8 bit high RAM pages.
        // The Y value therefore can only have the value 0, 1, 2, or 3.
        // usage: oled.setCursor(X IN PIXELS, Y IN ROWS OF 8 PIXELS STARTING WITH 0);

        if (keyADC < 740) {
            // save the screen if nothing is pressed
            return;
        }

        if (step == 1) {
            oled.setCursor(0, 0); // x then y
            oled.print(frame);
        }

        else if (step == 2) {
            oled.setCursor(0, 2);
            oled.print(octave);
            oled.print(' ');
            oled.print(key); // TODO: or key name and octave in one?

            // TODO: octave top-right
            // TODO: waveform bottom-right
        }

        else {
            // Swap which half of RAM is being written to, and which half is being displayed.
            // This is equivalent to calling both switchRenderFrame and switchDisplayFrame.
            // To see the benefit of double buffering, try this code again with this line commented out.
            oled.switchFrame();
        }
    }
}


int8_t checkUpDown() {

    // tick wraps about once every 2 days :)
    if (lastTick > tick) {
        lastTick = 0;
    }

    while (lastTick < tick) {
        ++lastTick;

        // don't bother checking if the buttons are released if we're not bouncing or repeating
        if (bounce > 0 || repeat > 0) {
            // always count down bounce regardless of whether a button is held down or not
            if (bounce > 0) {
                --bounce;
            }

            if (digitalReadFast(BUTTON_DOWN_PIN) == true && digitalReadFast(BUTTON_UP_PIN) == true) {
                // immediately clear repeat if the button is lifted
                repeat = 0;
                repeating = false;
            }
            else {
                // count down repeat while a button is held down
                if (repeat > 0) {
                    --repeat;
                }
            }
        }
    }

    // if we're still bouncing or repeating, ignore all presses
    if (bounce != 0 || repeat != 0) {
        return 0;
    }

    int8_t delta = 0;

    if (digitalReadFast(BUTTON_DOWN_PIN) == false) {
        delta = -1;
    }
    else if (digitalReadFast(BUTTON_UP_PIN) == false) {
        delta = 1;
    }
    else {
        // neither was pressed
        return 0;
    }

    // This only applies if a button was pressed

    bounce = DEBOUNCE_TICKS;
    if (repeating) {
        repeat = REPEAT_TICKS2;
    }
    else {
        repeating = true;
        repeat = REPEAT_TICKS1;
    }

    return delta;
}

ISR(TCA0_OVF_vect) {

    //digitalWriteFast(LED_PIN, true);

    ++tick;

    if (jump == -1) {
        digitalWriteFast(ENABLE_PIN, false); // off before next sample
    }
    else {
        digitalWriteFast(ENABLE_PIN, true); // on before next sample
    }

    // write the previously calculated sample first so the timing is reliable no matter how long it takes to calculate the next sample
    analogWrite(DAC_PIN, sample);

    // calculate the sample for next time
    sample = nextSample();

    //digitalWriteFast(LED_PIN, false);

    // The interrupt flag has to be cleared manually
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
