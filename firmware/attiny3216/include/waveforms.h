#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <driver_init.h>

#define MIN_PULSE 429496729
#define MAX_PULSE 3865470567

typedef enum WaveShapes {Pulse, Saw, Triangle} waveform_t;

uint8_t pulse_sample();
uint8_t saw_sample();
uint8_t triangle_sample();

#endif

