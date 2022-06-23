#ifndef LINE_FILLOW_H
#define LINE_FILLOW_H

#include "ir_line.h"
#include "timer.h"
#include "mecanum.h"
#include "WaveshareIMU.h"

// Forward declarations
void LFA_updateSensors(void);
void LFA_updateState(void);
void LFA_updateAction(void);

struct TimedActionMs LF_ActionUpdateSensors = TMR_buildActionMs("Update Line Follow", 20, &LFA_updateSensors, true);
struct TimedActionMs LF_ActionUpdateState = TMR_buildActionMs("Follow State", 40, &LFA_updateState, true);
struct TimedActionMs LF_ActionUpdateAction = TMR_buildActionMs("Follow Action", 100, &LFA_updateAction, true);

struct FollowState{
     IrLineSensor *lineSensor;
} followState;

TimedActionMs* LF_setup(IrLineSensor *sensor) {

}

void LFA_updateSensors(void) {
    IR_update(followState.lineSensor);
    MPU9250_READ_MAG();
}

void LFA_updateState(void) {

}

void LFA_updateAction(void) {

}

#endif // LINE_FILLOW_H