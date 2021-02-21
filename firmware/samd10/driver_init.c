/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

struct timer_descriptor TIMER_0;

struct dac_sync_descriptor DAC_0;

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TC1);
	_gclk_enable_channel(TC1_GCLK_ID, CONF_GCLK_TC1_SRC);

	timer_init(&TIMER_0, TC1, _tc_get_timer());
}

void DAC_0_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA02, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA02, PINMUX_PA02B_DAC_VOUT);
}

void DAC_0_CLOCK_init(void)
{

	_pm_enable_bus_clock(PM_BUS_APBC, DAC);
	_gclk_enable_channel(DAC_GCLK_ID, CONF_GCLK_DAC_SRC);
}

void DAC_0_init(void)
{
	DAC_0_CLOCK_init();
	dac_sync_init(&DAC_0, DAC);
	DAC_0_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA09

	gpio_set_pin_level(LED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(LED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED, GPIO_PIN_FUNCTION_OFF);

	TIMER_0_init();
	DAC_0_init();
}
