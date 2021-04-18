#include "engine.h"

bool isPlaying = false;
bool syncOscillators = true;

/*
int32_t jump;
uint32_t phase = 0;
waveform_t waveform = WAVE_PULSE;

uint32_t pulse = 0;
int32_t pulseAmount = 42949;
*/

oscillator_t oscillators[] = {
    { waveform: WAVE_PULSE, jump: 0, phase: 0, pulse: 0, pulseAmount: 42949, detune: 0 },
    { waveform: WAVE_SAW, jump: 0, phase: 0, pulse: 0, pulseAmount: 42949, detune: 70 }
};



uint8_t pulseSample(oscillator_t *oscillator);
uint8_t sawSample(oscillator_t *oscillator);
uint8_t triangleSample(oscillator_t *oscillator);


// EXPORTS

void clearNote() {

    // for now this is one to one with jump == -1, but once we have things like envelopes that assumption won't hold
    isPlaying = false;

    oscillators[0].jump = -1;
    oscillators[1].jump = -1;
}

void setNote(uint8_t octave, int8_t key) {

    uint8_t noteOffset = 3 + (octave * 12) + key;
    oscillators[0].jump = notes[noteOffset];
    oscillators[1].jump = notes[noteOffset + oscillators[1].detune/10]; // TODO: interpolate for the fractional part

    isPlaying = true;
}

uint8_t nextSample(){

    uint16_t sample = 0;

    if (isPlaying) {
        for (int8_t i = 0; i < 1; ++i) {
            if (oscillators[0].phase < oscillators[0].jump && i == 1 && syncOscillators) {
                // sync the second oscillator to the first one if oscillator sync is enabled and the first oscillator just wrapped
                oscillators[1].phase = oscillators[0].phase;
            }
            else {
                oscillators[i].phase += oscillators[i].jump;
            }

            switch (oscillators[i].waveform) {
                case WAVE_PULSE:
                    sample += pulseSample(&oscillators[i]);
                    break;

                case WAVE_SAW:
                    sample += sawSample(&oscillators[i]);
                    break;

                case WAVE_TRIANGLE:
                    sample += triangleSample(&oscillators[i]);
                    break;
            }
        }
        sample = sample >> 1; // roughly divide by 2
    }
    else {
        //sample = 127; // middle value
        sample = 0;
    }

    // TODO: envelope, filter, resonance

    return (uint8_t) sample;
}

waveform_t getOscillatorWaveform(uint8_t oscillatorNumber) {

    return oscillators[oscillatorNumber].waveform;
}

void setOscillatorWaveform(uint8_t oscillatorNumber, waveform_t newWaveform) {

    oscillators[oscillatorNumber].waveform = newWaveform;
}

int8_t getOscillatorDetune(uint8_t oscillatorNumber) {

    return oscillators[oscillatorNumber].detune;
}

void setOscillatorDetune(uint8_t oscillatorNumber, int8_t newDetune) {

    oscillators[oscillatorNumber].detune = newDetune;
}

bool getOscillatorSync() {

    return syncOscillators;
}

void setOscillatorSync(bool newSync) {

    syncOscillators = newSync;
}


// INTERNALS

uint8_t pulseSample(oscillator_t *oscillator) {

    // TODO: something is buggy in here.

    // forwards/backwards
    uint32_t newPulse = oscillator->pulse + oscillator->pulseAmount; // TODO: this is probably the bug. overflow.
    if (newPulse < MIN_PULSE || newPulse > MAX_PULSE) {
        oscillator->pulseAmount = -oscillator->pulseAmount;
    }

    // TODO: This will drift because the last one won't make it to the end before bouncing back. Does that matter, though?
    oscillator->pulse += oscillator->pulseAmount;

    // funny how we're using a DAC and right now we're only using it to make a square wave :)
    //if (oscillator->phase < oscillator->pulse) {
    if (oscillator->phase < 2147483648) { // TODO
        return 255;
    }
    else {
        return 0;
    }
}

uint8_t sawSample(oscillator_t *oscillator) {

    // top 8 bits of the 32 bit number
    return oscillator->phase >> 24;
}

uint8_t triangleSample(oscillator_t *oscillator) {

    int8_t temp = oscillator->phase >> 24;
    int8_t mask = temp >> 7;

    return (uint8_t) (temp ^ mask) << 1;
}

