#include "interface.h"

#include <Tiny4kOLED.h>

// minor-1.ino
extern volatile uint32_t tick;
extern volatile uint8_t sample;

// engine.cpp
extern bool isPlaying; // TODO: function

// global so we can display it easily
uint16_t rawKey;
int8_t currentKey = -1;
uint8_t currentOctave = 2;
menu_t currentMenu = MENU_OCTAVE;
menuitem_t currentMenuItem = ITEM_OCTAVE;
menuitems_t currentMenuItems = { items: { ITEM_OCTAVE }, length: 1};

char menuItemValue[17]; // 16 chars plus 1

// up/down buttons state
uint32_t lastTick = 0;
// TODO: rename these things?
uint16_t bounceTimeout = 0;
uint16_t repeatTimeout = 0;
bool repeating = false;

// display state
uint32_t frame = 0;
bool displayIsOn = false;
uint16_t tweakTimeout = 0;

int8_t getPressedKey();
navdeltas_t getNavigationDeltas();
void updateCurrentMenuItems();
void updateMenuItem(int8_t delta);
void updateMenuItemValue(int8_t delta);
void setCurrentMenu(menu_t newMenu);
char * getMenuName(menu_t menu);
char * getMenuItemName(menuitem_t menuItem);
char * getMenuItemValue(menu_t menu, menuitem_t menuItem);
void initDisplay();
void clearDisplay();
void updateDisplay();

// EXPORTS

void updateInterface() {

    // LEFT/RIGHT

    navdeltas_t deltas = getNavigationDeltas();

    if (deltas.leftRight != 0) {
        updateMenuItemValue(deltas.leftRight);
    }

    if (deltas.upDown != 0) {
        updateMenuItem(deltas.upDown);
    }

    // KEYBOARD

    //int8_t key = getPressedKey();
    int8_t key = 0; // TODO
    rawKey = 1000; // TODO

    if (digitalReadFast(BUTTON_SELECT_PIN) == false) { // TODO: optimise
        if (currentKey != -1) {
            currentKey = -1;
            clearNote();
        }

        // the mode button is held down so if a key is pressed then the user is selecting a mode
        tweakTimeout = TWEAK_TICKS; // turn on the screen
        switch (key) {
            case 0: {
                setCurrentMenu(MENU_OCTAVE);
                break;
            }
            case 1: {
                setCurrentMenu(MENU_OSCILLATOR1);
                break;
            }
            case 2: {
                setCurrentMenu(MENU_OSCILLATOR2);
                break;
            }
            default: {
                break;
            }
        }
    }
    else {

        // mode button not held down, so if a key was pressed, then the user is playing notes

        // Prefer to keep the previous note if it looks like something was pressed because the most likely explanation is that two currentKeys are held down and it is _just_ barely putting us out of range of either of them.
        if (key == -1 && rawKey > 740) {
            key = currentKey;
        }

        currentKey = key;

        if (currentKey == -1) {
            clearNote();
        }
        else {
            // TODO: change mode if that button is held down rather than playing a note
            setNote(currentOctave, currentKey);
        }
    }

    // DISPLAY

    //updateDisplay();
}


// INTERNALS

// INPUT

int8_t getPressedKey() {

    // TODO: make these çonstants defines somewhere

    rawKey = analogRead(KEYS_PIN);
    rawKey = (rawKey + analogRead(KEYS_PIN)) / 2;

    uint16_t startPoint = 747;
    int8_t key = -1;

    // /shrug
    if (rawKey > 1000) {
        return 12;
    }

    for (int8_t i=0; i<13; ++i) {
        uint8_t next = 20; // 20.25 is more ideal

        // this is never going to work in production, but I'm trying to salvage this prototype so I can at least work on the synth engine..
        if (i == 4) {
            next = 12;
        }
        else if (i == 5) {
            next = 28;
        }

        if (rawKey >= startPoint && rawKey < startPoint + next) {
            key = i;
            break;
        }

        startPoint += next;
    }

    return key;

}

navdeltas_t getNavigationDeltas() {

    navdeltas_t deltas = { leftRight: 0,  upDown: 0 };

    // tick wraps about once every 2 days :)
    if (lastTick > tick) {
        lastTick = 0;
    }

    while (lastTick < tick) {
        ++lastTick;

        if (tweakTimeout > 0) {
            --tweakTimeout;
        }

        // don't bother checking if the buttons are released if we're not bouncing or repeating
        if (bounceTimeout > 0 || repeatTimeout > 0) {
            // always count down bounce regardless of whether a button is held down or not
            if (bounceTimeout > 0) {
                --bounceTimeout;
            }

            // TODO: optimise. these are the only things on port C
            // TODO: the assumption here is that the button is still the same one. Which might be assuming too much
            if (digitalReadFast(BUTTON_LEFT_PIN) == true && digitalReadFast(BUTTON_RIGHT_PIN) == true && digitalReadFast(BUTTON_UP_PIN) == true && digitalReadFast(BUTTON_DOWN_PIN) == true) {
                // immediately clear repeat if the button is lifted
                repeatTimeout = 0;
                repeating = false;
            }
            else {
                // count down repeat while a button is held down
                if (repeatTimeout > 0) {
                    --repeatTimeout;
                }
            }
        }
    }

    // if we're still bouncing or repeating, ignore all presses
    if (bounceTimeout != 0 || repeatTimeout != 0) {
        return deltas;
    }

    if (digitalReadFast(BUTTON_LEFT_PIN) == false) {
        deltas.leftRight = -1;
    }
    else if (digitalReadFast(BUTTON_RIGHT_PIN) == false) {
        deltas.leftRight = 1;
    }
    else if (digitalReadFast(BUTTON_UP_PIN) == false) {
        deltas.upDown = -1;
    }
    else if (digitalReadFast(BUTTON_DOWN_PIN) == false) {
        deltas.upDown = 1;
    }
    else {
        // nothing was pressed
        return deltas;
    }

    // This only applies if a button was pressed

    bounceTimeout = DEBOUNCE_TICKS;
    if (repeating) {
        repeatTimeout = REPEAT_TICKS2;
    }
    else {
        repeating = true;
        repeatTimeout = REPEAT_TICKS1;
    }

    return deltas;
}


void updateCurrentMenuItems() {

    switch (currentMenu) {

        case MENU_OCTAVE: {
            currentMenuItems.items[0] = ITEM_OCTAVE;
            currentMenuItems.length = 1;
            break;
        }

        case MENU_OSCILLATOR1: {
            waveform_t currentWaveForm = getOscillatorWaveform(0);

            switch (currentWaveForm) {
                case WAVE_PULSE: {
                    currentMenuItems.items[0] = ITEM_SHAPE;
                    currentMenuItems.items[1] = ITEM_PULSE_RANGE;
                    currentMenuItems.items[2] = ITEM_PULSE_SPEED;
                    currentMenuItems.length = 3;
                    break;
                }

                default: {
                    currentMenuItems.items[0] = ITEM_SHAPE;
                    currentMenuItems.length = 1;
                    break;
                }
            }
            break;
        }

        case MENU_OSCILLATOR2: {
            waveform_t currentWaveForm = getOscillatorWaveform(1);

            switch (currentWaveForm) {
                case WAVE_PULSE: {
                    currentMenuItems.items[0] = ITEM_SHAPE;
                    currentMenuItems.items[1] = ITEM_PULSE_RANGE;
                    currentMenuItems.items[2] = ITEM_PULSE_SPEED;
                    currentMenuItems.items[3] = ITEM_DETUNE;
                    currentMenuItems.items[4] = ITEM_SYNC;
                    currentMenuItems.length = 5;
                    break;
                }

                default: {
                    currentMenuItems.items[0] = ITEM_SHAPE;
                    currentMenuItems.items[1] = ITEM_DETUNE;
                    currentMenuItems.items[2] = ITEM_SYNC;
                    currentMenuItems.length = 3;
                    break;
                }
            }
            break;
        }

        default: {
            break;
        }
    }
}

void setCurrentMenu(menu_t newMenu) {

    if (newMenu == currentMenu) {
        return;
    }

    switch (newMenu) {
        case MENU_OCTAVE:
        case MENU_OSCILLATOR1:
        case MENU_OSCILLATOR2: {
            currentMenu = newMenu;
            updateCurrentMenuItems();
            currentMenuItem = currentMenuItems.items[0];
            break;
        }

        default:
            break;
    }
}

void updateMenuItem(int8_t delta) {

    tweakTimeout = TWEAK_TICKS;

    uint8_t currentIndex = 0;
    for (uint8_t i = 0; i < currentMenuItems.length; ++i) {
        if (currentMenuItems.items[i] == currentMenuItem) {
            currentIndex = i;
            break;
        }
    }

    uint8_t newIndex = currentIndex + delta;
    if (newIndex >= 0 && newIndex < currentMenuItems.length) {
        currentMenuItem = currentMenuItems.items[newIndex];
    }
}

void updateMenuItemValue(int8_t delta) {

    tweakTimeout = TWEAK_TICKS;

    switch (currentMenuItem) {

        case ITEM_OCTAVE: {
            uint8_t newOctave = currentOctave + delta;
            if (newOctave >= 0 && newOctave <= 7) {
                currentOctave = newOctave;
            }
            break;
        }

        case ITEM_SHAPE: {
            uint8_t oscillatorNumber = currentMenu == MENU_OSCILLATOR1 ? 0 : 1;
            waveform_t currentWaveform = getOscillatorWaveform(oscillatorNumber);
            waveform_t newWaveform = currentWaveform + delta;
            if (newWaveform >= WAVE_PULSE && newWaveform <= WAVE_TRIANGLE) {
                setOscillatorWaveform(oscillatorNumber, newWaveform);
                updateCurrentMenuItems(); // Pulse waves have pulse options, others do not.
            }
            break;
        }

        // TODO: ITEM_PULSE_RANGE
        // TODO: ITEM_PULSE_SPEED

        case ITEM_DETUNE: {
            // only the second oscillator gets detuned
            int8_t currentDetune = getOscillatorDetune(1);
            int8_t newDetune = currentDetune + delta;
            if (newDetune >= -70 && newDetune <= 70) {
                setOscillatorDetune(1, newDetune);
            }
            break;
        }


        case ITEM_SYNC: {
            // this lives in the second oscillator's menu
            bool currentSync = getOscillatorSync();
            setOscillatorSync(!currentSync);
            break;
        }

        default: {
            break;
        }
    }
}

char * getMenuName(menu_t menu) {

    switch (menu) {
        case MENU_OCTAVE: {
            return "Octave";
        }

        case MENU_OSCILLATOR1: {
            return "Oscillator 1";
        }

        case MENU_OSCILLATOR2: {
            return "Oscillator 2";
        }

        default: {
            return "/shrug";
        }
    }
}

char * getMenuItemName(menuitem_t menuItem) {

    switch (menuItem) {
        case ITEM_OCTAVE: {
            return "#";
        }

        case ITEM_SHAPE: {
            return "Shape";
        }

        case ITEM_PULSE_RANGE: {
            return "Pulse Range";
        }

        case ITEM_PULSE_SPEED: {
            return "Pulse Speed";
        }

        case ITEM_DETUNE: {
            return "Detune";
        }

        case ITEM_SYNC: {
            return "Sync";
        }

        default: {
            return "/shrug";
        }
    }
}

char * getMenuItemValue(menu_t menu, menuitem_t menuItem) {

    switch (menuItem) {
        case ITEM_OCTAVE: {
            snprintf(menuItemValue, 17, "%d", currentOctave);
            return menuItemValue;
        }

        case ITEM_SHAPE: {
            uint8_t oscillatorNumber = currentMenu == MENU_OSCILLATOR1 ? 0 : 1;
            waveform_t currentWaveform = getOscillatorWaveform(oscillatorNumber);

            switch (currentWaveform) {
                case WAVE_PULSE: {
                    return "Pulse";
                }

                case WAVE_SAW: {
                    return "Saw";
                }

                case WAVE_TRIANGLE: {
                    return "Triangle";
                }

                default: {
                    return "/shrug";
                }
            }
        }

        case ITEM_PULSE_RANGE: {
            return "xxx"; // TODO
        }

        case ITEM_PULSE_SPEED: {
            return "yyy"; // TODO
        }

        case ITEM_DETUNE: {
            //return "Detune";
            waveform_t currentDetune = getOscillatorDetune(1);
            snprintf(menuItemValue, 17, "%d", currentDetune);
            return menuItemValue;
        }

        case ITEM_SYNC: {
            //return "Sync";
            if (getOscillatorSync()) {
                return "On";
            }
            else {
                return "Off";
            }
        }

        default: {
            return "/shrug";
        }
    }
}

// DISPLAY

void initDisplay() {

    oled.begin();
    oled.setFont(FONT8X16);
    clearDisplay();
    oled.on();

    displayIsOn = true;
}


void clearDisplay() {

    // Setup the first half of memory.
    oled.clear();

    // Switch the half of RAM that we are writing to, to be the half that is non currently displayed.
    oled.switchRenderFrame();

    // Setup the second half of memory.
    oled.clear();

    // Switch back to being ready to render on the first frame while displaying the second frame.
    oled.switchFrame();
}


void updateDisplay() {

    ++frame;

    if (!isPlaying && tweakTimeout == 0) {
        // save the screen's life from burnin if nothing is pressed
        if (displayIsOn) {
            displayIsOn = false;
            oled.off();
        }

        return;
    }

    uint8_t step = frame % 4;

    if (step == 0) {

        // Clear whatever random data has been left in memory.
        oled.clear();
        return;
    }

    // Position the text cursor
    // In order to keep the library size small, text can only be positioned
    // with the top of the font aligned with one of the four 8 bit high RAM pages.
    // The Y value therefore can only have the value 0, 1, 2, or 3.
    // usage: oled.setCursor(X IN PIXELS, Y IN ROWS OF 8 PIXELS STARTING WITH 0);

    // TODO: display different things when playing vs when tweaking

    if (step == 1) {

        oled.setCursor(0, 0);
        if (isPlaying) {
            oled.print(rawKey);
            oled.print(' ');
            oled.print(currentKey);
        }
        else {
            oled.print(getMenuName(currentMenu));
        }
        return;
    }

    if (step == 2) {

        if (isPlaying) {
            // TODO
        }
        else {
            oled.setCursor(0, 2);
            oled.print(getMenuItemName(currentMenuItem));
            char * value = getMenuItemValue(currentMenu, currentMenuItem);
            oled.setCursor(128 - strlen(value)*8, 2);
            oled.print(value);
        }

        if (!displayIsOn) {
            displayIsOn = true;
            oled.on();
        }

        return;
    }

    // Swap which half of RAM is being written to, and which half is being displayed.
    // This is equivalent to calling both switchRenderFrame and switchDisplayFrame.
    // To see the benefit of double buffering, try this code again with this line commented out.
    oled.switchFrame();
}
