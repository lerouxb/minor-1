#ifndef INTERFACE_H
#define INTERFACE_H

#include <arduino.h>
#include <CmdParser.hpp>
#include <Wire.h>

#include "pins.h"
#include "engine.h"
#include "QT60240.h"

#define SERIAL_READ_TIMEOUT 1000

#define ERROR_SERIAL_PARSE_TIMEOUT 1
#define ERROR_SERIAL_PARSE_RETURN 2
#define ERROR_SERIAL_PARSE_SPACE 3

#define MIN_OCTAVE 1 // C2
#define MAX_OCTAVE 6 // C7

void setupInterface();
void updateInterface();

#endif

