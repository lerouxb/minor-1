#include <string.h>
#include <atmel_start.h>
#include <util/delay.h>
#include <util/delay.h>
#include "notes.h"

volatile adc_result_t ADC_0_measurement;

int32_t note = 44946936; // Middle C

int main(void) {

    atmel_start_init();

    DAC_0_enable();

    sei();

    while (1) {
        ADC_0_measurement = 0;
        // figure out which note was pressed (if any)
        for (int i=0; i<5; ++i) {
            ADC_0_measurement += ADC_0_get_conversion(4);
        }
        ADC_0_measurement = ADC_0_measurement / 5;

        unsigned int midpoint = 755;
        int8_t key = -1;

        for (int8_t i=0; i<13; ++i) {
            if (ADC_0_measurement > (midpoint - 10) && ADC_0_measurement < (midpoint + 10)) {
                key = i;
                break;
            }
            midpoint += 20;
        }

        if (key == -1) {
            // Prefer to keep the previous note if it looks like something was pressed because the most likely explanation is that two keys are held down and it is _just_ barely putting us out of range of either of them.
            if (ADC_0_measurement < 745) {
                note = -1;
            }
        }
        else {
            note = notes[39 - 24 + key];
        }

        printf("%d -> %d\r\n", ADC_0_measurement, key);
        //_delay_ms(100);
    }
}
