#include <atmel_start.h>

static struct timer_task sample_task;

uint16_t i = 0;

void write_sample_to_dac(void) {
    dac_sync_write(&DAC_0, 0, &i, 1);
    i = (i + 14) % 1024;
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();


	dac_sync_enable_channel(&DAC_0, 0);

	//for (;;) {
		//dac_sync_write(&DAC_0, 0, &i, 1);

                /*
                DAC_CRITICAL_SECTION_ENTER();
                ((Dac *)(DAC_0.device.hw))->DATA.reg = i;
                //hri_dac_wait_for_sync(DAC_0.device.hw);
                DAC_CRITICAL_SECTION_LEAVE();
                */

		//i = (i + 1) % 1024;
	//}

	sample_task.interval = 1;
	sample_task.cb = write_sample_to_dac;
	sample_task.mode = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &sample_task);
	timer_start(&TIMER_0);

        while (1) {
        }
}
