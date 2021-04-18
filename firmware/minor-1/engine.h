#ifndef ENGINE_H
#define ENGINE_H

#include <arduino.h>
#include "notes.h"

#define MIN_PULSE 429496729
#define MAX_PULSE 3865470567

typedef enum { WAVE_PULSE, WAVE_SAW, WAVE_TRIANGLE } waveform_t;

typedef struct {
    waveform_t waveform;
    int32_t jump;
    uint32_t phase;

    // only for pulse waveforms
    // TODO: min and max or range
    uint32_t pulse;
    int32_t pulseAmount;

    // detune range: 7 semitones higher or lower than the center position
    int8_t detune; // 10 == one semitone. So -70 to 70.
} oscillator_t;

void clearNote();
void setNote(uint8_t octave, int8_t key);
uint8_t nextSample();

waveform_t getOscillatorWaveform(uint8_t oscillatorNumber);
void setOscillatorWaveform(uint8_t oscillatorNumber, waveform_t newWaveform);
int8_t getOscillatorDetune(uint8_t oscillatorNumber);
void setOscillatorDetune(uint8_t oscillatorNumber, int8_t newDetune);
bool getOscillatorSync();
void setOscillatorSync(bool newSync);

#endif

