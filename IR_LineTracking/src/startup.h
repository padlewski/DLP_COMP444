#ifndef STARTUP_H
#define STARTUP_H

#include <Arduino.h>
#include "line_follow.h"
#include "action_move.h"

static const byte STARTUP_STATE_ID = B00000000;
static const String STARTUP_STATE_NAME = "STARTUP";

// actions
void SUA_UpdateIr(void);
void SUA_UpdateMag(void);
void SUA_CheckState(void);

struct StartupState {
    // setup some memory for movement
    byte mSize = 2;
    byte directions[2] = {0, 0};
    byte speeds[2][M_N] = {{0,0,0,0},{0,0,0,0}};
    unsigned int times[2] = {0,0};
    byte mode;
    TimedActionMs *current;
} SU_State;

struct TimedActionMs SU_ActionUpdate = buildActionMs("update sensors", 25, &SUA_UpdateIr, true);
struct TimedActionMs SU_ActionCheckState = buildActionMs("check state", 50, &SUA_CheckState, true);

// setup and start ir calibration
void SU_SetIrCal(void) {
    // create a lazy alias
    static struct ActionMoveSequenceState * const _ = &actionMoveSequenceState;
    _->size = SU_State.mSize;
    _->directions =  SU_State.directions;
    _->speeds = SU_State.speeds;
    _->times = SU_State.times;
}

// setup and start Magnatrometer calibration
void SU_SetMagCal(void) {
    
}

// we have finished the setup 
void SU_SetComplete(void) {

}

void SUA_CheckState(void) {
    switch(SU_State.mode) {
        case 0:
            SU_SetIrCal();
            break;
        case 1:
            SU_SetMagCal();
            break;
        default:
            SU_SetComplete();
            break;
    }
}

void SUA_UpdateIr(void) {
    
}

// from https://appelsiini.net/2018/calibrate-magnetometer/
void SUA_UpdateMag(void) {
  static int min[3] = {32767, 32767, 32767};
  static int max[3] = {-32768, -32768, -32768};
  static float avg[3] = {0,0,0};
  static float avg_delta = 0;
  MPU9250_ReadCalibrateMag();
  for(int i = 0 ; i < 3 ; ++i) {
    if(MPU9250.magnRaw[i] < min[i]) min[i] = MPU9250.magnRaw[i];
    if(MPU9250.magnRaw[i] > max[i]) max[i] = MPU9250.magnRaw[i];
    avg[i] = ((float)(max[i] - min[i]) / 2.0f);
  }
  MPU9250.magn_OffsetErr.X = (int)((min[0] + max[0]) / 2);
  MPU9250.magn_OffsetErr.X = (int)((min[1] + max[1]) / 2);
  MPU9250.magn_OffsetErr.X = (int)((min[2] + max[2]) / 2);
  avg_delta = ((float)(avg[0] + avg[1] + avg[2]) / 3.0f);
  MPU9250.magn_Corr[0] = avg_delta / avg[0];
  MPU9250.magn_Corr[1] = avg_delta / avg[1];
  MPU9250.magn_Corr[2] = avg_delta / avg[2];
}

#endif // STARTUP_H
