#include <string.h>
#include <atmel_start.h>
#include <util/delay.h>
#include "notes.h"
#include "waveforms.h"

// about 25000 ticks per second
#define DEBOUNCE_TICKS 1250
#define REPEAT_TICKS1 6000 // first repeat is slower to prevent accidents
#define REPEAT_TICKS2 2000

volatile adc_result_t ADC_0_measurement;
extern volatile uint32_t tick;

// sound stuff
int32_t jump = 44946936; // Middle C
uint8_t octave = 0;
waveform_t waveform = Pulse;

// button stuff
uint32_t last_tick = 0;
uint16_t bounce = 0;
uint16_t repeat = 0;
bool repeating = false;

void my_function ();


int8_t check_up_down() {

    int8_t delta = 0;

    // tick wraps about once every 2 days :)
    if (last_tick > tick) {
        last_tick = 0;
    }

    while (last_tick < tick) {
        ++last_tick;

        if (bounce > 0) {
            if (BTN1_get_level() == true && BTN2_get_level() == true) {
                --bounce;
                if (bounce == 0) {
                    repeating = false;
                }
            }
            else {
                bounce = DEBOUNCE_TICKS; // reset to the start so that bouncing switches don't whittle away at the count
            }
        }

        if (repeat > 0) {
            --repeat;
        }
    }

    if (bounce == 0 || repeat == 0) {
        if (BTN1_get_level() == false) {
            delta = -1;
        }
        else if (BTN2_get_level() == false) {
            delta = 1;
        }

        if (delta != 0) {
            bounce = DEBOUNCE_TICKS;
            if (repeating) {
                repeat = REPEAT_TICKS2;
            }
            else {
                repeating = true;
                repeat = REPEAT_TICKS1;
            }
        }
    }

    return delta;
}

int8_t get_key(adc_result_t value) {

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

int main(void) {

    atmel_start_init();

    DISABLE_set_level(false); // start off
    DAC_0_enable();

    sei();


    while (1) {
        //LED_toggle_level(); // produces tons of noise that affects the DAC

        int8_t delta = check_up_down();

        if (delta != 0) {
            if (octave + delta >= 0 && octave + delta <= 7) {
                octave += delta;
                printf("%d\r\n", octave);
                my_function();
            }
        }

        ADC_0_measurement = ADC_0_get_conversion(4);

        int8_t key = get_key(ADC_0_measurement);

        if (key == -1) {
            // Prefer to keep the previous note if it looks like something was pressed because the most likely explanation is that two keys are held down and it is _just_ barely putting us out of range of either of them.
            if (ADC_0_measurement < 745) {
                jump = -1;
            }
        }
        else {
            //jump = notes[39 - 24 + key];
            jump = notes[3 + octave * 12 + key];
        }
    }
}
