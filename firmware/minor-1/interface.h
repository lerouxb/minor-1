#ifndef INTERFACE_H
#define INTERFACE_H

#include <arduino.h>
#include "pins.h"
#include "engine.h"

// about 25000 ticks per second
#define DEBOUNCE_TICKS 1250
#define REPEAT_TICKS1 6000 // first repeat is slower to prevent accidents
#define REPEAT_TICKS2 2000

#define TWEAK_TICKS 25000

//typedef enum { MODE_OCTAVE, MODE_SHAPE1, MODE_SHAPE2, MODE_DETUNE, MODE_SYNC } mode_t;
typedef enum { MENU_OCTAVE, MENU_OSCILLATOR1, MENU_OSCILLATOR2/*, MENU_FILTER, MENU_ENVELOPE, MENU_PHOTODIODE, MENU_ARPEGGIO*/ } menu_t;
typedef enum { ITEM_OCTAVE, ITEM_SHAPE, ITEM_PULSE_RANGE, ITEM_PULSE_SPEED, ITEM_DETUNE, ITEM_SYNC /*,ITEM_MIX*/ } menuitem_t;

typedef struct {
    menuitem_t items[7]; // 7 is the max possible number
    uint8_t length;
} menuitems_t;

typedef struct {
    uint8_t leftRight;
    uint8_t upDown;
} navdeltas_t;

/*
Modes:

1. octave
2. oscillator 1 (shape, pulse range, pulse sweep, mix)
3. oscillator 2 (shape, pulse range, pulse sweep, detune, sync, mix)
4. filter (cutoff, resonance)
5. envelope (attack, decay)
6. photodiode (pulse sweep 1, pulse sweep 2, filter cutoff, filter resonance)
7. arpeggio

*/

void updateInterface();
void initDisplay();

#endif

