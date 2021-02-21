#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	uint16_t i = 0;

	dac_sync_enable_channel(&DAC_0, 0);

	for (;;) {
		//dac_sync_write(&DAC_0, 0, &i, 1);

                DAC_CRITICAL_SECTION_ENTER();
                ((Dac *)(DAC_0.device.hw))->DATA.reg = i;
                //hri_dac_wait_for_sync(DAC_0.device.hw);
                DAC_CRITICAL_SECTION_LEAVE();

		i = (i + 1) % 1024;
	}
}
