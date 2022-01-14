#ifndef WAVESHAREIMU_H
#define WAVESHAREIMU_H

#include <Arduino.h>
#include "MPU9250.h"

//static const float M_pi = 3.1415926535f;
static const byte IMU_PRES_TYPE_BM180 = 1;
static const byte IMU_PRES_TYPE_BM280 = 2;

static const float Kp = 4.50f;   // proportional gain governs rate of convergence to accelerometer/magnetometer
static const float Ki = 1.0f;    // integral gain governs rate of convergence of gyroscope biases

volatile float q0, q1, q2, q3;
float invSqrt(float);
void IMU_AHRSupdate(float, float, float, float, float, float, float, float, float);
void IMU_GetQuater(void);
void IMU_GetYawPitchRoll(float*);
float toGauss(const int*);

void IMU_Init(void) {	
    //byte u8Ret; 
	MPU9250_Init();
    //u8Ret = BMP280_Init();
    // if(u8Ret != BMP280_RET_OK)
    // {
    //     u8PressureType = IMU_PRES_TYPE_BM180;
    //     printf("\r\nPressure type is BM180\r\n");
    //     BMP180_Init();
    // }
    // else
    // {
    //     u8PressureType = IMU_PRES_TYPE_BM280;
    //     printf("\r\nPressure type is BM280\r\n");
    // }
	
  	q0 = 1.0f;  
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
}

void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
{
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
	float ex, ey, ez, halfT = 0.024f;

	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;   
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;          

	norm = invSqrt(ax * ax + ay * ay + az * az);       
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	norm = invSqrt(mx * mx + my * my + mz * mz);          
	mx = mx * norm;
	my = my * norm;
	mz = mz * norm;

	// compute reference direction of flux
	hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
	hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
	hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);         
	bx = sqrt((hx * hx) + (hy * hy));
	bz = hz;     

	// estimated direction of gravity and flux (v and w)
	vx = 2 * (q1q3 - q0q2);
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
	wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
	wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);  

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;	
		ezInt = ezInt + ez * Ki * halfT;

		gx = gx + Kp * ex + exInt;
		gy = gy + Kp * ey + eyInt;
		gz = gz + Kp * ez + ezInt;
	}

	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;  

	norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
}

void IMU_GetQuater(void) {
	float MotionVal[9];			
    MPU9250_READ_ACCEL();
    MPU9250_READ_GYRO();
    MPU9250_READ_MAG();
    MotionVal[0] = MPU9250.gyro[0]/32.8;
    MotionVal[1] = MPU9250.gyro[1]/32.8;
    MotionVal[2] = MPU9250.gyro[2]/32.8;
    MotionVal[3] = MPU9250.accel[0];
    MotionVal[4] = MPU9250.accel[1];
    MotionVal[5] = MPU9250.accel[2];
    MotionVal[6] = MPU9250.magn[0];
    MotionVal[7] = MPU9250.magn[1];
    MotionVal[8] = MPU9250.magn[2];    
 	IMU_AHRSupdate((float)MotionVal[0] * 0.0175, (float)MotionVal[1] * 0.0175, (float)MotionVal[2] * 0.0175,
   	(float)MotionVal[3], (float)MotionVal[4], (float)MotionVal[5], (float)MotionVal[6], (float)MotionVal[7], MotionVal[8]);
}

void IMU_GetYawPitchRoll(float *angles) {
    IMU_GetQuater(); 
	angles[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
	angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   
}

//from https://digilent.com/blog/how-to-convert-magnetometer-data-into-compass-heading/
float getCompassDegrees(){
    float gauss_x = toGauss(&MPU9250.magn[0]);
    float gauss_y = toGauss(&MPU9250.magn[1]);
    float gauss_z = toGauss(&MPU9250.magn[2]);
    if(gauss_x == 0) {
        if(gauss_y < gauss_x) return 90.0f;
        else return 0.0f;
    }
    float result = atan(gauss_y/gauss_z)*(180.0f/3.1415926535f);
    if(result > 360.0f) return result - 360.0f;
    if(result < 0.0f) return result + 360.0f;
    return result;
}

// map from page 50 of RM-MPU-9255.pdf
// Guass is magnetic flux density
float toGauss(const int *val) {
    static float MIN = -4912.0f;
    static float MAX = 4912.0f;
    static float interval = 0.15f;
    float result = *val * interval;
    if(result > MAX) return MAX;
    if(result < MIN) return MIN;
    return result;
}

float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;                //get bits for floating value
	i = 0x5f3759df - (i >> 1);          //gives initial guss you
	y = *(float*)&i;                    //convert bits back to float
	y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
	return y;
}

#endif