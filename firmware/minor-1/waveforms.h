#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <arduino.h>

#define MIN_PULSE 429496729
#define MAX_PULSE 3865470567

typedef enum WaveShapes {Pulse, Saw, Triangle} waveform_t;

uint8_t nextSample();

#endif

