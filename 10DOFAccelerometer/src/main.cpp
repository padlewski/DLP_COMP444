#include <Arduino.h>
#include <Wire.h>
#include "i2cHelper.h"
#include "WaveshareIMU.h"
#include "timer.h"

int rate = 115200;
float PYR[3];
unsigned int sensorCnt;
unsigned int printCnt;

void updateSensors(void);
void printData(void);

void setup() {
  Serial.begin(9600); 
  Wire.begin(rate); // setup the I2C connection
  IMU_Init();
  delay(1000);
}

void loop() {
  tick();
  updateSensors();
  printData();
}

void updateSensors(void){
  sensorCnt += timerState.deltaMs;
  if(sensorCnt < 200) return;
  MPU9250_READ_ACCEL();
  MPU9250_READ_GYRO();
  MPU9250_READ_MAG();
  sensorCnt = 0;
}

void printData(void) {
  printCnt += timerState.deltaMs;
  if(printCnt < 5000) return;
  Serial.println("==============================");
  IMU_GetYawPitchRoll(PYR);
  Serial.print("PYR: ");
  Serial.print(PYR[0]);
  Serial.print(" | ");
  Serial.print(PYR[1]);
  Serial.print(" | ");
  Serial.println(PYR[2]);
  Serial.print("Gyro XYZ: ");
  Serial.print(MPU9250.gyro[0]);
  Serial.print(" | ");
  Serial.print(MPU9250.gyro[1]);
  Serial.print(" | ");
  Serial.println(MPU9250.gyro[2]);
  Serial.print("Accel X: ");
  Serial.print(MPU9250.accel[0]);
  Serial.print(" | ");
  Serial.print(MPU9250.accel[1]);
  Serial.print(" | ");
  Serial.println(MPU9250.accel[2]);
  Serial.print("Magn XYZ: ");
  Serial.print(MPU9250.magn[0]);
  Serial.print(" | ");
  Serial.print(MPU9250.magn[1]);
  Serial.print(" | ");
  Serial.println(MPU9250.magn[2]);
  printCnt = 0;
}
