/* Auto-generated config file peripheral_clk_config.h */
#ifndef PERIPHERAL_CLK_CONFIG_H
#define PERIPHERAL_CLK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

/**
 * \def CONF_CPU_FREQUENCY
 * \brief CPU's Clock frequency
 */
#ifndef CONF_CPU_FREQUENCY
#define CONF_CPU_FREQUENCY 48000000
#endif

// <y> DAC Clock Source
// <id> dac_gclk_selection

// <GCLK_CLKCTRL_GEN_GCLK0_Val"> Generic clock generator 0

// <GCLK_CLKCTRL_GEN_GCLK1_Val"> Generic clock generator 1

// <GCLK_CLKCTRL_GEN_GCLK2_Val"> Generic clock generator 2

// <GCLK_CLKCTRL_GEN_GCLK3_Val"> Generic clock generator 3

// <GCLK_CLKCTRL_GEN_GCLK4_Val"> Generic clock generator 4

// <GCLK_CLKCTRL_GEN_GCLK5_Val"> Generic clock generator 5

// <i> Select the clock source for DAC.
#ifndef CONF_GCLK_DAC_SRC
#define CONF_GCLK_DAC_SRC GCLK_CLKCTRL_GEN_GCLK1_Val
#endif

/**
 * \def CONF_GCLK_DAC_FREQUENCY
 * \brief DAC's Clock frequency
 */
#ifndef CONF_GCLK_DAC_FREQUENCY
#define CONF_GCLK_DAC_FREQUENCY 333333
#endif

// <<< end of configuration section >>>

#endif // PERIPHERAL_CLK_CONFIG_H
