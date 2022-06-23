#ifndef MPU9250_H
#define MPU9250_H

/**
 * @brief Implementation of MPU9250
 * 
 * This is an adaptation of the Waveshare code sample
 */

#include <Arduino.h>
#include "i2cHelper.h"

typedef struct {
	byte Index;
	int AvgBuffer[8];
} MPU9250_AvgTypeDef;

struct {
    int magn[3] = {0,0,0};
    int magnRaw[3] = {0,0,0};
    int magnMin[3] = {8190, 8190, 8190};
    int magnMax[3] = {-8190, -8190, -8190};
    int magnAvg[3] = {0,0,0};
    float magnScale[3] = {1.0f, 1.0f, 1.0f};
    // int accel[3] = {0,0,0};
    // int accelRaw[3] = {0,0,0};
    // int gyro[3] = {0,0,0};
    // int gyroRaw[3] = {0,0,0};
    // int gyroOffset[3] = {0,0,0};
} MPU9250;

// Register addresses
static const byte   SMPLRT_DIV		= 0x19;	// Sample Rate Divider. Typical values:0x07(125Hz) 1KHz internal sample rate
static const byte   CONFIG			= 0x1A;	// Low Pass Filter.Typical values:0x06(5Hz)
static const byte   GYRO_CONFIG		= 0x1B;	// Gyro Full Scale Select. Typical values:0x10(1000dps)
static const byte   ACCEL_CONFIG	= 0x1C;	// Accel Full Scale Select. Typical values:0x08(4g)

// Config pin [Bit 7 = ACTL | Bit 6 = OPEN | Bit 5 = LATCH _INT_EN | Bit 4 = NT_ANYRD_2CLEAR | Bit 3 = ACTL_FSYNC | Bit 2 = FSYNC_INT_MODE_EN | Bit 1 = BYPASS_EN | Bit 0 = Not]
static const byte   INT_PIN_CFG     = 0x37;

static const byte	ACCEL_XOUT_H	= 0x3B;
static const byte	ACCEL_XOUT_L	= 0x3C;
static const byte	ACCEL_YOUT_H	= 0x3D;
static const byte	ACCEL_YOUT_L	= 0x3E;
static const byte	ACCEL_ZOUT_H	= 0x3F;
static const byte	ACCEL_ZOUT_L	= 0x40;

static const byte	TEMP_OUT_H		= 0x41;
static const byte	TEMP_OUT_L		= 0x42;

static const byte	GYRO_XOUT_H		= 0x43;
static const byte	GYRO_XOUT_L		= 0x44;	
static const byte	GYRO_YOUT_H		= 0x45;
static const byte	GYRO_YOUT_L		= 0x46;
static const byte	GYRO_ZOUT_H		= 0x47;
static const byte	GYRO_ZOUT_L		= 0x48;

static const byte MAG_CTRL          = 0x0A; // Magnatrometor control register
static const byte MAG_DAT_STAT_1    = 0x02; // Data status
static const byte MAG_XOUT_L		= 0x03;
static const byte MAG_XOUT_H		= 0x04;
static const byte MAG_YOUT_L		= 0x05;
static const byte MAG_YOUT_H		= 0x06;
static const byte MAG_ZOUT_L		= 0x07;
static const byte MAG_ZOUT_H		= 0x08;
static const byte MAG_DAT_STAT_2    = 0x09; // Data status 2 overflow

static const byte PWR_MGMT_1		= 0x6B;	// Power Management. Typical values:0x00(run mode)
static const byte WHO_AM_I		    = 0x75;	// identity of the device

static const byte GYRO_ADDRESS      = B1101000;	// Gyro and Accel device address
static const byte MAG_ADDRESS       = B0001100; // compass device address
static const byte ACCEL_ADDRESS     = B1101000; // accelerometer device address

static const byte ADDRESS_AD0_LOW   = 0xD0; // address pin low (GND), default for InvenSense evaluation board
static const byte ADDRESS_AD0_HIGH  = 0xD1; // address pin high (VCC)
static const byte WHO_AM_I_VAL      = 0x73; // identity of MPU9250 is 0x71. identity of MPU9255 is 0x73.

// Forward declarations
bool MPU9250_Check(void);
void MPU9250_InitGyrOffset(void);
int MPU9250_CalAvgValue(byte*, int*, int);
void MPU9250_READ_ACCEL(void);
void MPU9250_READ_GYRO(void);
void MPU9250_READ_MAG(void);

void MPU9250_Init(void) {
    writeByte(&GYRO_ADDRESS, &PWR_MGMT_1, 0x00); // internal 20MHz oscillator
	writeByte(&GYRO_ADDRESS, &SMPLRT_DIV, 0x07); // 1kHz / 1 + this value (7)
	writeByte(&GYRO_ADDRESS, &CONFIG, 0x06);
    // Set the gyro to resolution to +-1000 dps (controlled by bits 3 and 4)
	writeByte(&GYRO_ADDRESS, &GYRO_CONFIG, B00010000);
    // Set the accel config to a resolution of 4g (conrolled by bits 3 and 4)
	writeByte(&GYRO_ADDRESS, &ACCEL_CONFIG, B00001000);
	
	delay(35); // small delay for initialization
	
	// MPU9250_InitGyrOffset();
}

// void MPU9250_READ_ACCEL(void) { 
// 	static int OutBuffer[3] = {0,0,0};
// 	static MPU9250_AvgTypeDef MPU9250_Filter[3];
//     read2BytesHigh(&ACCEL_ADDRESS, &ACCEL_XOUT_H, &MPU9250.accelRaw[0]);
//     read2BytesHigh(&ACCEL_ADDRESS, &ACCEL_YOUT_H, &MPU9250.accelRaw[1]);
//     read2BytesHigh(&ACCEL_ADDRESS, &ACCEL_ZOUT_H, &MPU9250.accelRaw[2]);
//     for(byte i = 0; i < 3; i ++) {
//         OutBuffer[i] = MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, MPU9250.accelRaw[i]);
//     }
//     MPU9250.accel[0] = OutBuffer[0];
//     MPU9250.accel[1] = OutBuffer[1];
//     MPU9250.accel[2] = OutBuffer[2]; 
// }

// void MPU9250_READ_GYRO(void)    { 
//     static int OutBuffer[3] = {0};
//     static MPU9250_AvgTypeDef MPU9250_G_Filter[3];
//     read2BytesHigh(&GYRO_ADDRESS, &GYRO_XOUT_H, &MPU9250.gyroRaw[0]);
//     read2BytesHigh(&GYRO_ADDRESS, &GYRO_YOUT_H, &MPU9250.gyroRaw[1]);
//     read2BytesHigh(&GYRO_ADDRESS, &GYRO_ZOUT_H, &MPU9250.gyroRaw[2]);
//     for(byte i = 0; i < 3; i ++)	{
//         OutBuffer[i] = MPU9250_CalAvgValue(&MPU9250_G_Filter[i].Index, MPU9250_G_Filter[i].AvgBuffer, MPU9250.gyroRaw[i]);
//     }
//     MPU9250.gyro[0] = OutBuffer[0] - MPU9250.gyroOffset[0];
//     MPU9250.gyro[1] = OutBuffer[1] - MPU9250.gyroOffset[1];
//     MPU9250.gyro[2] = OutBuffer[2] - MPU9250.gyroOffset[2];
// }

void MPU9250_READ_MAG(void) { 
	static int OutBuffer[3] = {0};
    static byte status = 0;
    static const byte readBufferSize = 6;
    static byte readBuffer[readBufferSize] = {0,0,0,0,0,0};
	static MPU9250_AvgTypeDef MPU9250_Filter[3];
    static float avgDelta = 0.0f;
    unsigned long timeout = 10;

    writeByte(&GYRO_ADDRESS, &INT_PIN_CFG, 0x02); //turn on Bypass Mode 
    delay(timeout);
    writeByte(&MAG_ADDRESS, &MAG_CTRL, 0x01);	// single pole mode 14 bit mode
    do {
        delay(timeout += 5);
        status = readRegister(&MAG_ADDRESS,&MAG_DAT_STAT_1);
        if (timeout > 30) return; // timeout while reading magnetometer 
    } while (not status);
    if(status & B00000010) goto JMP_MAG_READ_ERROR; // overrun skip read
    fillBuffer(&MAG_ADDRESS, &MAG_XOUT_L, readBuffer, &readBufferSize);
    if(readRegister(&MAG_ADDRESS, &MAG_DAT_STAT_2)) {
        JMP_MAG_READ_ERROR:
        return; // Overflow or overrun
    }
    // first byte is low byte on compass
    fillLow(&readBuffer[0], &MPU9250.magnRaw[0], 1);
    fillLow(&readBuffer[2], &MPU9250.magnRaw[1], 1);
    fillLow(&readBuffer[4], &MPU9250.magnRaw[2], 1);
    MPU9250.magnRaw[2] = -MPU9250.magnRaw[2];
    for(int i = 0 ; i < 3 ; ++i) {
        if(MPU9250.magnRaw[i] > -8190 && MPU9250.magnRaw[i] < MPU9250.magnMin[i]) 
            MPU9250.magnMin[i] = MPU9250.magnRaw[i];
        if(MPU9250.magnRaw[i] < 8190 && MPU9250.magnRaw[i] > MPU9250.magnMax[i]) 
            MPU9250.magnMax[i] = MPU9250.magnRaw[i];
        MPU9250.magnAvg[i] = (MPU9250.magnMax[i] + MPU9250.magnMin[i]) / 2;
    }
    avgDelta = ((float)(MPU9250.magnAvg[0] + MPU9250.magnAvg[1] + MPU9250.magnAvg[2]) / 3.0f);
    MPU9250.magnScale[0] = avgDelta / MPU9250.magnAvg[0];
    MPU9250.magnScale[1] = avgDelta / MPU9250.magnAvg[1];
    MPU9250.magnScale[2] = avgDelta / MPU9250.magnAvg[2];
    for(byte i = 0; i < 3; i ++) {
        OutBuffer[i] = MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, MPU9250.magnRaw[i]);
        MPU9250.magn[i] = (OutBuffer[i] - MPU9250.magnAvg[i] ) * MPU9250.magnScale[i];
    }
}

bool MPU9250_Check(void) {
   	return WHO_AM_I_VAL == readRegister(&GYRO_ADDRESS, &WHO_AM_I);
}

int MPU9250_CalAvgValue(byte *pIndex, int *pAvgBuffer, int InVal) {	
	pAvgBuffer[*pIndex] = InVal;  // Add the current value to the buffer
    ++(*pIndex);
  	*pIndex &= 0x07;                // increment the index
  	long pOutVal = 0;
	for(byte i = 0; i < 8; ++i) {
    	pOutVal += pAvgBuffer[i];
  	}
  	return int(pOutVal / 8);
}

/**
  * @brief  Initializes gyroscopes gyroOffset
  * @param  None
  * @retval None
  */
// void MPU9250_InitGyrOffset(void) {
// 	byte i;
// 	int	TempGx = 0, TempGy = 0, TempGz = 0;
//  	for(i = 0; i < 32; i ++) {
// 		MPU9250_READ_GYRO();
// 		TempGx += MPU9250.gyro[0];
// 		TempGy += MPU9250.gyro[1];
// 		TempGz += MPU9250.gyro[2];
// 		delay(100);
// 	}
// 	MPU9250.gyroOffset[0] = TempGx >> 5;
// 	MPU9250.gyroOffset[1] = TempGy >> 5;
// 	MPU9250.gyroOffset[2] = TempGz >> 5;
// }

// from https://appelsiini.net/2018/calibrate-magnetometer/
// void MPU9250_ReadCalibrateMag(void) {
//     static const byte readBufferSize = 8;
//     static byte readBuffer[readBufferSize] = {0,0,0,0,0,0,0,0};
//     static float avgDelta = 0.0f;
//     writeByte(&GYRO_ADDRESS, &INT_PIN_CFG, 0x02); //turn on Bypass Mode 
//     delay(10);
//     writeByte(&MAG_ADDRESS, &MAG_CTRL, 0x01);	
//     delay(10);
//     fillBuffer(&MAG_ADDRESS, &MAG_DAT_STAT_1, readBuffer, &readBufferSize);
    // if(not readRegister(&MAG_ADDRESS,&MAG_DAT_STAT_1)) return; // Data not ready, skip
    // if(readRegister(&MAG_ADDRESS, &MAG_DAT_STAT_2)) return; // Overflow , skip
    // read2BytesHigh(&MAG_ADDRESS, &MAG_XOUT_H, &MPU9250.magnRaw[0]);
    // read2BytesHigh(&MAG_ADDRESS, &MAG_YOUT_H, &MPU9250.magnRaw[1]);
    // read2BytesHigh(&MAG_ADDRESS, &MAG_ZOUT_H, &MPU9250.magnRaw[2]);
//     if(not readBuffer[0]) return; // Data not ready, skip
//     if(readBuffer[7]) return; // Overflow , skip
//     fillHigh(&readBuffer[1], &MPU9250.magnRaw[0], 1);
//     fillHigh(&readBuffer[3], &MPU9250.magnRaw[1], 1);
//     fillHigh(&readBuffer[5], &MPU9250.magnRaw[2], 1);
//     MPU9250.magnRaw[2] = -MPU9250.magnRaw[2];
//     for(int i = 0 ; i < 3 ; ++i) {
//         if(MPU9250.magnRaw[i] < MPU9250.magnMin[i]) MPU9250.magnMin[i] = MPU9250.magnRaw[i];
//         if(MPU9250.magnRaw[i] > MPU9250.magnMax[i]) MPU9250.magnMax[i] = MPU9250.magnRaw[i];
//         MPU9250.magnAvg[i] = (MPU9250.magnMax[i] - MPU9250.magnMin[i]) / 2;
//     }
//     avgDelta = ((float)(MPU9250.magnAvg[0] + MPU9250.magnAvg[1] + MPU9250.magnAvg[2]) / 3.0f);
//     MPU9250.magnScale[0] = avgDelta / MPU9250.magnAvg[0];
//     MPU9250.magnScale[1] = avgDelta / MPU9250.magnAvg[1];
//     MPU9250.magnScale[2] = avgDelta / MPU9250.magnAvg[2];
// }

#endif