#include <Arduino.h>
#include <Wire.h>
#include "i2cHelper.h"
#include "WaveshareIMU.h"
#include "timer.h"

int rate = 115200;
float PYR[3];
float degSec[3];
float gForce[3];
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
  IMU_GetDegSec(degSec);
  IMU_GetGForce(gForce);
  Serial.print("PYR: ");
  Serial.print(PYR[0]);
  Serial.print(" | ");
  Serial.print(PYR[1]);
  Serial.print(" | ");
  Serial.println(PYR[2]);
  Serial.print("Gyro deg sec: ");
  Serial.print(degSec[0]);
  Serial.print(" | ");
  Serial.print(degSec[1]);
  Serial.print(" | ");
  Serial.println(degSec[2]);
  Serial.print("Accel gForce: ");
  Serial.print(gForce[0]);
  Serial.print(" | ");
  Serial.print(gForce[1]);
  Serial.print(" | ");
  Serial.println(gForce[2]);
  Serial.print("Magn XYZ: ");
  Serial.print(MPU9250.magnRaw[0]);
  Serial.print(" | ");
  Serial.print(MPU9250.magnRaw[1]);
  Serial.print(" | ");
  Serial.println(MPU9250.magnRaw[2]);
  Serial.print("Compass Deg: ");
  Serial.println(getCompassDegrees());
  printCnt = 0;
}
