#include "interface.h"

uint8_t recalibrateTouch() {
    uint8_t error;

    Wire.beginTransmission(TOUCH_ADDRESS);
    Wire.write(125); // recalibrate all keys address
    Wire.write(0x55); // recalibrate all keys
    error = Wire.endTransmission(false);

    if (error) {
        Serial.print("oopsie (0x55): ");
        Serial.println(error);
        return error;
    }

    return 0;
}

uint8_t readKeys(uint8_t * buf) {

    uint8_t error;

    Wire.beginTransmission(TOUCH_ADDRESS);
    Wire.write(1);
    error = Wire.endTransmission(false);

    if (error) {
      Serial.print("oopsie (keys): ");
      Serial.println(error);
      return error;
    }

    for (uint8_t i=0; i<2; ++i) {
      Wire.requestFrom(TOUCH_ADDRESS, 1);
      buf[i] = (uint8_t) Wire.read();
    }

    return 0;
}

void printKeys(uint8_t * buf) {

    Serial.print("KEYS");

    for (uint8_t i=0; i<2; ++i) {
      Serial.print(' ');
      Serial.print(buf[i]);
    }

    Serial.println("");
}


uint8_t readTouchData(uint8_t offset, uint8_t count, uint8_t * buf) {
    uint8_t error;

    for (int i=0; i<count; ++i) {
      buf[i] = 0;
    }

    Wire.beginTransmission(TOUCH_ADDRESS);
    Wire.write(offset);
    error = Wire.endTransmission(false);

    if (error) {
        Serial.print("oopsie (read): ");
        Serial.println(error);
        return error;
    }

    for (uint8_t i=0; i<count; ++i) {
      Wire.requestFrom(TOUCH_ADDRESS, 1);
      buf[i] = Wire.read();
    }

    return 0; // or error if we read too few?
}

void printTouchData(uint8_t count, uint8_t * buf) {

  for (int i=0; i<count; ++i) {
    Serial.print(' ');

    if (buf[i] < 16) {
      Serial.print('0');
    }
    Serial.print(buf[i], HEX);
  }
}

uint8_t readHexCharsFromSerial(int count, uint8_t * buf) {

  unsigned long start = millis();
  int numProcessed = 0;
  char charBuf[3];
  uint8_t numChars = 0;

  while (numProcessed < count) {
    if (millis() > start + SERIAL_READ_TIMEOUT) {
        return ERROR_SERIAL_PARSE_TIMEOUT; // timeout error;
    }

    if (!Serial.available()) {
      continue; // try again next time
    }

    charBuf[numChars] = Serial.read();
    ++numChars;

    if (numChars != 3) {
      continue; // first or second char: not done reading a byte yet.
    }

    // every third character we parse the two-char hex number and start over again

    // check that the third char is a space for all chars except the last one where it should be a \r
    if (numProcessed == count - 1) {
      if (charBuf[2] != '\r') {
        return ERROR_SERIAL_PARSE_RETURN; // parse error
      }
    }
    else {
      if (charBuf[2] != ' ') {
        return ERROR_SERIAL_PARSE_SPACE; // parse error
      }
    }

    buf[numProcessed] = (uint8_t) strtol(charBuf, 0, 16);

    numChars = 0;
    ++numProcessed;
  }

  return 0; // success!
}

uint8_t writeSetupsData(uint8_t * buf) {

  uint8_t error;

  Wire.beginTransmission(TOUCH_ADDRESS);
  Wire.write(130); // setups write unlock
  Wire.write(0x55); // unlock write
  error = Wire.endTransmission(false);

  if (error) {
    Serial.print("oopsie (write unlock): ");
    Serial.println(error);
    return error;
  }

  for (int i = 0; i < 123; ++i) {
    Wire.beginTransmission(TOUCH_ADDRESS);
    Wire.write(131 + i);
    Wire.write(buf[i]);
    error = Wire.endTransmission(false);

    if (error) {
      Serial.print("oopsie (write ");
      Serial.print(i);
      Serial.print("): ");
      Serial.println(error);
      return error;
    }
  }

  return 0;
}
