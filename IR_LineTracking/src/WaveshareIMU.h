#ifndef WAVESHAREIMU_H
#define WAVESHAREIMU_H

#include <Arduino.h>
#include "MPU9250.h"

static const byte IMU_PRES_TYPE_BM180 = 1;
static const byte IMU_PRES_TYPE_BM280 = 2;

static const byte NORTH = 0;
static const byte EAST = 1;
static const byte SOUTH = 2;
static const byte WEST = 3;

float IMU_toGauss(const int*);

void IMU_Init(void) {	
	MPU9250_Init();
}

//from https://digilent.com/blog/how-to-convert-magnetometer-data-into-compass-heading/
float IMU_getCompassDegrees(){
    float x = IMU_toGauss(&MPU9250.magn[0]);
    float y = IMU_toGauss(&MPU9250.magn[1]);
    if(y == 0) {
        if(x < 0.0f) return 180.0f;
        else return 0.0f;
    }
    float result = atan2(y,x) * (180.0 / PI);
    if(result >= 360.0f) return result - 360.0f;
    if(result < 0.0f) return result + 360.0f;
    return result;
}

byte IMU_getCompassAsByte() {
    return map(IMU_getCompassDegrees(), 0, 360, 0, 255);
}

byte IMU_getHeading(){
    static byte h = IMU_getCompassAsByte(); 
    if(h >= 32 || h < 96) return EAST;
    if(h >= 96 || h < 160) return SOUTH;
    if(h >= 160 || h < 223) return WEST;
    return NORTH;
}

// calculate the angle between the heading and target in degrees
// if negative heading is left of target
// based on https://stackoverflow.com/questions/5024375/getting-the-difference-between-two-headings
int *IMU_calcOffsetDeg(int target, int heading) {
    static int a = ((heading - target + 540) % 360) - 180;
    return &a;
}

// calculate the angle between the heading and target in byte 
// if negative heading is left of target, we return an int because
// we need the positive or negative value
// Adapted from OffsetDeg (posed comment on stackoverflow stating solution was successful)
int *IMU_calcOffsetByte(byte target, byte heading) {
    static int a = (((int)heading - (int)target + 384) % 256) - 128;
    return &a;
}

// Range should be 250, 500, 1000, 2000
// float IMU_toDegSec(const int *val, const int range){
//     static float RNG = float(range);
//     static float MIN = RNG * -1.0f;
//     static float INTV = RNG / 0x7FFF;
//     float result = float(*val) * INTV;
//     if(result > RNG) return RNG;
//     if(result < MIN) return MIN;
//     return result;
// }

// void IMU_GetDegSec(float xyz[3]) {
//     xyz[0] = IMU_toDegSec(&MPU9250.gyro[0], 1000);
//     xyz[1] = IMU_toDegSec(&MPU9250.gyro[1], 1000);
//     xyz[2] = IMU_toDegSec(&MPU9250.gyro[2], 1000);
// }

// Range should be 2, 4, 8, 16
// float IMU_toGforce(const int *val, const int range){
//     static float RNG = float(range);
//     static float MIN = RNG * -1.0f;
//     static float INTV = RNG / 0x7FFF;
//     float result = float(*val) * INTV;
//     if(result > RNG) return RNG;
//     if(result < MIN) return MIN;
//     return result;
// }

// void IMU_GetGForce(float xyz[3]) {
//     xyz[0] = IMU_toGforce(&MPU9250.accel[0], 4);
//     xyz[1] = IMU_toGforce(&MPU9250.accel[1], 4);
//     xyz[2] = IMU_toGforce(&MPU9250.accel[2], 4);
// }

// map from page 50 of RM-MPU-9255.pdf
// Guass is magnetic flux density
float IMU_toGauss(const int *val) {
    static float MIN = -4912.0f;
    static float MAX = 4912.0f;
    static float interval = 0.6f; // 14 bit mode
    float result = float(*val) * interval;
    if(result > MAX) return MAX;
    if(result < MIN) return MIN;
    return result;
}

#endif