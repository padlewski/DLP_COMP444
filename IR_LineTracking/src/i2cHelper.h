#ifndef I2CHELPER_H
#define I2CHELPER_H

#include <Arduino.h>
#include <Wire.h>

byte writeByte(const byte *device, const byte *address, const byte val) {
    Wire.beginTransmission(*device);  // connect to the device
    Wire.write(*address); // map the register
    Wire.write(val);
    return Wire.endTransmission();
}

byte readRegister(const byte *device, const byte *address) {
    Wire.beginTransmission(*device);  // connect to the device
    Wire.write(*address); // map the register
    Wire.endTransmission();
    Wire.requestFrom(int(*device), 1);
    return Wire.read();
}

void fillBuffer(const byte *device, const byte *address, byte *buffer, const byte *size) {
    Wire.beginTransmission(*device);  // connect to the accelerometer
    Wire.write(*address); // map the register
    Wire.endTransmission();
    Wire.requestFrom(*device, *size);
    for(int i = 0 ; i < *size; ++i) buffer[i] = Wire.read();
}

void read2BytesHigh(const byte *device, const byte *address, int *result) {
    static byte buf[2];
    Wire.beginTransmission(*device);  // connect to the device
    Wire.write(*address); // map the register
    Wire.endTransmission();
    Wire.requestFrom(int(*device), 2);
    buf[0] = Wire.read();
    buf[1] = Wire.read();
    *result = (buf[0]<<8)|buf[1];
}

// first byte is high
void fillHigh(const byte *source, int *destination, int size) {
    for(int i = 0, n = 0 ; i  < size ; ++i, n = 2 * i){
        destination[i] = (source[n]) << 8 | source[n + 1];
    }
}

// first byte is low
void fillLow(const byte *source, int *destination, int size) {
    for(int i = 0, n = 0 ; i  < size ; ++i, n = 2 * i){
        destination[i] = (source[n + 1]) << 8 | source[n];
    }
}

#endif