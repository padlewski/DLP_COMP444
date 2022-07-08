#ifndef ACTIONS_H
#define ACTIONS_H

#include "bot.h"

void ActionMoveSequenceDo(void);
void ActionMoveUntilDo(void);
void ActionUpdateIr(void);
void ActionUpdateMpu(void);
void ActionCheckSwitch(void);

struct TimedActionMs doUpdateIr = TMR_buildActionMs("Do IR", 30, &ActionUpdateIr, false);
struct TimedActionMs doUpdateMpu = TMR_buildActionMs("Do Mpu", 100, &ActionUpdateMpu, false);
struct TimedActionMs doCheckSwitch = TMR_buildActionMs("Do Switch", 50, &ActionCheckSwitch, false);
struct TimedActionMs doMoveSequence = TMR_buildActionMs("Do MOVE_S", 25, &ActionMoveSequenceDo, false);
struct TimedActionMs doMoveUntil = TMR_buildActionMs("Do MOVE_U", 200, &ActionMoveUntilDo, false);

struct ActionMoveSequenceState{
    byte size;
    byte index;
    byte* directions;
    int (*speeds)[4];   // two dimensional array 
    unsigned int* times; // time to run
    unsigned int count;  // how many times the action as been triggered
    TimedActionMs *action = &doMoveSequence;
} actionMoveSequenceState;

void ActionMoveSequenceDo(void) {
    // create a lazy alias
    static struct ActionMoveSequenceState * const _ = &actionMoveSequenceState;
    M_move(_->directions[_->index], _->speeds[_->index]);
    doMoveSequence.interval = _->times[_->index];
    ++_->index;
    if(_->index >= _->size) _->index = 0;
    ++_->count;
}

struct ActionMoveUntilState{
    byte direction;
    int speed[4] = {0,0,0,0};
    unsigned int count;
    bool(*check)(); // The condition which end the action
    void(*run)();    // the callback to run when check returns true
    void(*until)(); // the callback to run until positive
    TimedActionMs *action = &doMoveUntil;
} actionMoveUntilState;

void ActionMoveUntilDo() {
    ++actionMoveUntilState.count;
    M_move(actionMoveUntilState.direction, actionMoveUntilState.speed);
    if(actionMoveUntilState.check()) actionMoveUntilState.run();
    else actionMoveUntilState.until();
}

struct ActionCheckSwtichState {
    TimedActionMs *circuit;
} actionCheckSwitchState;

void ActionCheckSwitch(void) {
    if(not digitalRead(switchPin)) { // LOW = active
        if(botState.currentState & BOT_PAUSED) {
            // returning from paused state resume movement and actions
            doCheckSwitch.next = actionCheckSwitchState.circuit; 
            M_move(M_DIR_PREV, M_SPEED_PREV);
            updateBotState(BOT_PAUSED);
        }
        return;
    }
    if(botState.currentState & BOT_PAUSED) return; // already paused
    // pause
    actionCheckSwitchState.circuit = doCheckSwitch.next;
    doCheckSwitch.next = NULL;
    M_stop();
    updateBotState(BOT_PAUSED);
}

void ActionUpdateIr(void) {
    IR_update();
}

void ActionUpdateMpu(void) {
    MPU9250_READ_MAG();
}

bool checkIsIrCentered(void) {
    return IR_isCentered(&lineSensor.status);
}

bool checkIsIrOffline(void) {
    return IR_isOffLine(&lineSensor.status);
}

bool checkIsIrCenteredOrOffline(void) {
    return checkIsIrCentered() || checkIsIrOffline();
}

bool checkIsIrNotCentered(void) {
    return !IR_isCentered(&lineSensor.status);
}

bool checkIsIrFollowing(void) {
    byte n = IR_leftOrRight(&lineSensor.status);
    return n > 15 || n < 35;
}

bool checkIsIntersection(void) {
    return IR_isIntersection(&lineSensor.status);
}
 
void untilNoop(void) { return; }

void untilCenteredRotate(void) {
    if(IR_isOffLine(&lineSensor.status)) return; // off the line
    actionMoveUntilState.direction = IR_leftOrRight(&lineSensor.status) < IR_IS_CENTERED ? Counter : Clockwise;
}

void untilCenteredStrafe(void) {
    if(IR_isOffLine(&lineSensor.status)) return; // off the line
    actionMoveUntilState.direction = IR_leftOrRight(&lineSensor.status) < IR_IS_CENTERED ? Left : Right;
}

void untilCenteredFrontTurn(void) {
    if(IR_isOffLine(&lineSensor.status)) return; // off the line
    actionMoveUntilState.direction = IR_leftOrRight(&lineSensor.status) < IR_IS_CENTERED ? FrontLeft : FrontRight;
}

void untilCenteredBackTurn(void) {
    if(IR_isOffLine(&lineSensor.status)) return; // off the line
    actionMoveUntilState.direction = IR_leftOrRight(&lineSensor.status) < IR_IS_CENTERED ? BackLeft : BackRight;
}

#endif // ACTIONS_H