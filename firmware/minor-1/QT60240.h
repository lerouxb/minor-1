#ifndef QT60240_H
#define QT60240_H

#define TOUCH_ADDRESS 117

uint8_t recalibrateTouch();
uint8_t readKeys(uint8_t * buf);
void printKeys(uint8_t * buf);
uint8_t readTouchData(uint8_t offset, uint8_t count, uint8_t * buf);
void printTouchData(uint8_t count, uint8_t * buf);
uint8_t readHexCharsFromSerial(int count, uint8_t * buf);
uint8_t writeSetupsData(uint8_t * buf);

#endif
