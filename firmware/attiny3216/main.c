#include <string.h>
#include <atmel_start.h>
#include <util/delay.h>
#include <util/delay.h>
#include "notes.h"

volatile adc_result_t ADC_0_measurement;

int16_t note = 686; // Middle C

int main(void)
{
	atmel_start_init();

        DAC_0_enable();

	while (1) {
            // figure out which note was pressed (if any)
	    ADC_0_measurement = ADC_0_get_conversion(4);

            unsigned int midpoint = 780;
            int8_t key = -1;

            for (int i=0; i<13; ++i) {
                if (ADC_0_measurement > midpoint - 5 && ADC_0_measurement < midpoint + 5) {
                    key = i;
                    break;
                }
                midpoint += 20;
            }

            if (key == -1) {
                note = -1;
            }
            else {
                note = notes[38 - 24 + key]; // this should be 39 but then it is too high. weird.
            }

	    //printf("%d -> %d\r\n", ADC_0_measurement, key);
            /*
	    printf("%d -> %d\r\n", ADC_0_measurement, key);
            _delay_ms(100);
            */
	}
}
