#ifndef TINY4KOLED_ASL_H
#define TINY4KOLED_ASL_H

#include <i2c_master.h>
#include "Tiny4kOLED_common.h"

static void _asl_init(void) {
	//TinyI2C.init(); // TODO

#ifndef TINY4KOLED_QUICK_BEGIN
	while (!_asl_beginTransmission()) {
		delay(10);
	}
	_asl_endTransmission();
#endif
}

static bool _asl_beginTransmission(void) {
	//return TinyI2C.start(SSD1306, 0); // TODO
        return true; // TODO
}

static bool _asl_write(uint8_t byte) {
	//return TinyI2C.write(byte); // TODO
        return true; // TODO
}

static uint8_t _asl_endTransmission(void) {
	//TinyI2C.stop(); // TODO
	return 0;
}

SSD1306Device oled(&_asl_init, &_asl_beginTransmission, &_asl_write, &_asl_endTransmission);

#endif
