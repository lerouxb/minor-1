#include "interface.h"

void readStatus();
void readSetups();
void writeSetups();
int8_t checkKeyStatus(uint8_t * buf);
int8_t checkOctaveStatus (uint8_t * buf);

// minor-1.ino
extern volatile uint32_t tick;
extern volatile uint8_t sample;
extern volatile bool touchChanged;

int8_t currentKey = -1;
uint8_t currentOctave = 3;

int8_t previousOctaveStatus = 0;

uint8_t statusData[2];
uint8_t keyData[120];
uint8_t setupsData[123];
CmdParser cmdParser;
// use own buffer from serial input
CmdBuffer<32> myBuffer;



// EXPORTS
void setupInterface() {

    myBuffer.setEndChar('\r');
    myBuffer.setEcho(true);
}

void updateInterface() {

    // TODO: somehow go to sleep if we're not playing

    //Serial.print("elapsed:  ");
    //RSerial.println(micros());

    if (touchChanged)  {
        // This should be safe because the change pin won't go high again until we read the keys which only happens afterwards. And even if that happens immediately after reading the boolean will be set and we'll only clear it next time.
        touchChanged = false;

        if (!readKeys(statusData)) {

            printKeys(statusData);

            //  look  at  the  two  bytes  in  statusData  and  determine  which  keys  are  pressed
            //  TODO:  can  we  go  polyphonic?
            currentKey = checkKeyStatus(statusData);

            int8_t octaveDelta = checkOctaveStatus(statusData);
            int8_t newOctave = currentOctave + octaveDelta;
            if (newOctave >= MIN_OCTAVE && newOctave <= MAX_OCTAVE) {
                currentOctave = newOctave;
            }

            if (currentKey == -1) {
                clearNote();
            }
            else {
                setNote(currentOctave, currentKey);
            }

        }
    }

    while (Serial.available()) {

        if (myBuffer.readFromSerial(&Serial, 1000)) {
            if (cmdParser.parseCmd(&myBuffer) == CMDPARSER_ERROR) {
                Serial.println("parser error");
            }
            else {
                Serial.println(".");

                if (cmdParser.equalCommand("RSTATUS") && cmdParser.getParamCount() == 1) {
                    readStatus();
                }

                if (cmdParser.equalCommand("RSETUPS") && cmdParser.getParamCount() == 1) {
                    readSetups();
                }

                if (cmdParser.equalCommand("WSETUPS") && cmdParser.getParamCount() == 1) {
                    writeSetups();
                }

            }
        }
        else {
            //  timeout  occurred  after  starting  to  read  a  command  but  before  it  could  read  a  full  command
            Serial.println("timeout");
        }
    }
}

void readStatus() {

    // 4 to 123: key data (120 bytes)
    readTouchData(4, 120, keyData);
    Serial.print("STATUS");
    printTouchData(120, keyData);
    Serial.println("");
}

void readSetups() {

    // 131 to 253: setups (123 bytes)
    readTouchData(131, 123, setupsData);
    Serial.print("SETUPS");
    printTouchData(123, setupsData);
    Serial.println("");
}

void writeSetups() {
    // read 123 bytes from serial as two-character hex codes separated by spaces, then a \r
    uint8_t error = readHexCharsFromSerial(123, setupsData);

    if (error) {
        Serial.print("error reading hex chars: ");
        Serial.print(error);
        Serial.println("");
        return;
    }

    // then write these over i2c
    error = writeSetupsData(setupsData);
    if (!error) {
        Serial.println("OK");
    }
}

int8_t checkKeyStatus(uint8_t * buf) {

    // highest note wins ignoring the three buttons

    if (buf[1] & 64) {
        return 12; // c2
    }
    if (buf[0] & 32) {
        return 11; // b
    }
    if (buf[1] & 32) {
        return 10; // a#
    }
    if (buf[0] & 16) {
        return 9; // a
    }
    if (buf[1] & 16) {
        return 8; // g#
    }
    if (buf[0] & 8) {
        return 7; // g
    }
    if (buf[1] & 8) {
        return 6; // f#
    }
    if (buf[0] & 4) {
        return 5; // f
    }
    if (buf[1] & 4) {
        return 4; // e
    }
    if (buf[0] & 2) {
        return 3; // d#
    }
    if (buf[1] & 2) {
        return 2; // d
    }
    if (buf[0] & 1) {
        return 1; // c#
    }
    if (buf[1] & 1) {
        return 0; // c
    }

    //if (buf[0] & 64) {
    //    // k1
    //}
    //if (buf[1] & 128) {
    //   // k2
    //}
    //if (buf[0] & 128) {
    //    // k3
    //}

    return -1;
}

int8_t checkOctaveStatus (uint8_t * buf) {

    // debounce strategy: it has to have changed. Otherwise pressing notes while the octave buttons are pressed will retrigger it

    if (buf[0] & 64) {
        if (previousOctaveStatus != 1) {
            previousOctaveStatus = 1;
            return 1; // k1
        }
    }
    else if (buf[0] & 128) {

        if (previousOctaveStatus != -1) {
            previousOctaveStatus = -1;
            return -1; // k3
        }
    }

    previousOctaveStatus = 0;
    return 0;
}
