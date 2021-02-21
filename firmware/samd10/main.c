#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	uint16_t i = 0;

	dac_sync_enable_channel(&DAC_0, 0);

	for (;;) {
		dac_sync_write(&DAC_0, 0, &i, 1);
		i = (i + 1) % 1024;
	}
}
