#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H

#include "bot.h"
#include "startup.h"

// Forward declarations
void LFA_updateState(void);
void LF_ExecTurnAround(void);
void LF_ExecTurnLeftF(void);
void LF_ExecIntersection(void);
void LF_Calibrate(void);
void LF_setFollow(void);
void LF_selectDirection(void);
void LF_find(void);
void LF_updateCardinal(void);
void LF_fixSkew(void);

struct TimedActionMs doMonitorState = TMR_buildActionMs("Follow State", 40, &LFA_updateState, true);

void LFA_updateState(void) {
    if(IR_STATUS_MASK & lineSensor.status) 
        botState.lastKnownIr = lineSensor.status;
    if(botState.currentState & BOT_PAUSED) return; // we are paused do nothing
    if((~botState.currentState & BOT_CALIBRATED) == BOT_CALIBRATED) LF_Calibrate();
    if(botState.currentState == BOT_CALIBRATED) LF_setFollow();
}

void LF_Calibrate(void) {
    if(not doMonitorState.next) {
        doMonitorState.next = SU_init();
        return;
    }
    // check current progress
    if(SU_State.mode != STARTUP_STATE_COMPLETE) return;
    // we are complete
    doMonitorState.next = NULL;
    updateBotState(BOT_CALIBRATED);
}

void LF_setFollow(void) {
    actionMoveUntilState.direction = Forward;
    // memcpy(actionMoveUntilState.speed, LF_SPEED, SIZE_SPEEDS);
    actionMoveUntilState.speeds = &LF_SPEED;
    doMoveUntil.active = true;
    updateBotState(BOT_FOLLOW);
    LF_find();
}

bool checkIsNotCorrecting(void) {
    return checkIsIrOffline() || checkIsIrCentered() || checkIsIntersection(); 
}


bool checkIsIrNotFollowing(void) {
    return checkIsIrOffline() || checkIsIrNotCentered() || checkIsIntersection();
}

bool checkLeftTurnComplete(void) {
    return lineSensor.status & B1;
}

bool checkRightTurnComplete(void) {
    return lineSensor.status & B10000;
}

void LF_ExecTurnAround(void) {
    #ifdef _PRINT_
    Serial.println("ExecTurnAround");
    #endif
    // Choose a random pivot
    actionMoveUntilState.direction = random(2) ? Clockwise : Counter;
    actionMoveUntilState.speeds = &LF_SPEED;
    actionMoveUntilState.check = &checkIsIrCentered;
    actionMoveUntilState.until = &untilCenteredRotate;
    actionMoveUntilState.run = &LF_find;
}

void LF_ExecTurn(byte dir) {
    #ifdef _PRINT_
    Serial.println("ExecTurn");
    #endif
    Serial.println(dir, BIN);
    actionMoveUntilState.direction = dir ? Clockwise : Counter;
    actionMoveUntilState.speeds = &LF_SPEED;
    actionMoveUntilState.check = dir ? &checkRightTurnComplete : &checkLeftTurnComplete;
    actionMoveUntilState.until = &untilNoop;
    actionMoveUntilState.run = &LF_find;
}

void LF_ExecFollow(void) {
    #ifdef _PRINT_
    Serial.println("ExecFollow");
    #endif
    actionMoveUntilState.direction = Forward;
    actionMoveUntilState.speeds = &LF_SPEED;
    actionMoveUntilState.check = &checkIsIrNotFollowing;
    actionMoveUntilState.until = &LF_updateCardinal;
    actionMoveUntilState.run = &LF_find;
}

void LF_ExecCorrection(void) {
    #ifdef _PRINT_
    Serial.println("ExecCorrection");
    #endif
    actionMoveUntilState.direction = IR_leftOrRight(&SU_State.lastKnownLinePosition) < IR_IS_CENTERED ? FrontLeft : FrontRight;
    actionMoveUntilState.speeds = &LF_SPEED;
    actionMoveUntilState.check = &checkIsNotCorrecting;
    actionMoveUntilState.until = &untilCenteredFrontTurn;
    actionMoveUntilState.run = &LF_find;
}

static byte LF_intersection = 0;
void LF_ExecIntersection(void) {
    #ifdef _PRINT_
    Serial.println("ExecIntersection");
    #endif
    LF_intersection = lineSensor.status;
    Serial.println(LF_intersection, BIN);
    actionMoveUntilState.direction = Forward;
    actionMoveUntilState.speeds = &LF_SPEED;
    actionMoveUntilState.check = &checkIsIrCenteredOrOffline; 
    actionMoveUntilState.until = &untilNoop;
    actionMoveUntilState.run = &LF_selectDirection;
}

void LF_find(void) {
    #ifdef _PRINT_
    Serial.println("LF_find");
    #endif
    Serial.println(lineSensor.status, BIN);
    if(IR_isOffLine(&lineSensor.status)) LF_ExecTurnAround();// turn around
    else if(IR_isIntersection(&lineSensor.status)) LF_ExecIntersection();
    else if(checkIsIrCentered()) LF_ExecFollow();
    else LF_ExecCorrection();
}

void LF_selectDirection(void) {
    #ifdef _PRINT_
    Serial.println("Select Direction");
    #endif
    // check far left and far right bits for left and right
    //   FRL - bits for available directions Forward - Right - Left
    // B0111
    byte n = LF_intersection & B1 ? 1 : 0;
    n |= LF_intersection & B10000 ? B10 : B0;
    n |= IR_isOffLine(&lineSensor.status) ? 0 : B100;
    #ifdef _PRINT_
    Serial.println(n, BIN);
    #endif
    switch (n) {
        case 1:
            LF_ExecTurn(0); // left turn
            break;
        case 2:
            LF_ExecTurn(1); // right turn
            break;
        case 3:
            LF_ExecTurn(random(2)); // either left or right
            break;
        case 4:
            LF_ExecFollow(); // this would be weird, only straight ahead
            break;
        case 5:
            // forward and left
            if(random(2)) LF_ExecFollow();
            else LF_ExecTurn(0);
            break;
        case 6:
            // forward and right
            if(random(2)) LF_ExecFollow();
            else LF_ExecTurn(1);
            break;
        case 7:
            {
                long x = random(3);
                if(x < 2) LF_ExecTurn(x);
                else LF_ExecFollow();
            }
            break;
        default:
            LF_ExecTurnAround(); // shouldn't happen
            break;
    }
}

void LF_updateCardinal(void) {
    static byte curr = IMU_getCompassAsByte();
    static byte av = botState.compassHeadingsNESW[IMU_getHeading()];
    // use a weighted Average to maintain realative dir
    av = ((av * 4) + curr ) / 5;
    botState.compassHeadingsNESW[IMU_getHeading()];
}



void LF_fixSkew(void) {
    // rotate to sensor
    if(not IR_isCentered(*lineSensor.status)){
        actionMoveUntilState.direction = IR_leftOrRight(&lineSensor.status) < IR_IS_CENTERED ? Counter : Clockwise;
        actionMoveUntilState.speeds = &LF_SPEED;
        actionMoveUntilState.check = &checkIsIrCentered;
        actionMoveUntilState.until = &untilCenteredRotate;
        actionMoveUntilState.run = &LF_fixSkew;
        return;
    }
    // rotate the back
    actionMoveUntilState.direction = IR_leftOrRight(&lineSensor.status) < IR_IS_CENTERED ? Counter : Clockwise;
        actionMoveUntilState.speeds = &BK_SPEED;
        actionMoveUntilState.check = &checkIsIrCentered;
        actionMoveUntilState.until = &untilCenteredRotate;
        actionMoveUntilState.run = &LF_selectDirection;
    
}

#endif // LINE_FOLLOW_H