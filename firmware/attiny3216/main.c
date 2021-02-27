#include <atmel_start.h>
#include <util/delay.h>
#include <stdio.h>
#include <atomic.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) {
	    printf("hello world\r\n");
            _delay_ms(1000);
	}
}

