#ifndef I2C_NANO_H
#define I2C_NANO_H

#include <Arduino.h>

// Shared constants for i2c interface
static const byte NANO_ADDR = B10011;

// Flags
static const NANO_SET_TAIL = B11111111;

// Register addresses for nano
static const byte NANO_HEAD_NODE    = 0x2;
static const byte NANO_LAST_IDX     = 0x3;
static const byte NANO_LAST_NODE    = 0x4;
static const byte NANO_CURR_IDX     = 0x5;
static const byte NANO_CURR_IDX     = 0x6;
static const byte NANO_OLD_NODE     = 0x7;
static const byte NANO_NODE_GET     = 0xE;
static const byte NANO_NODE_SET     = 0xF;

#endif // I2C_NANO_H