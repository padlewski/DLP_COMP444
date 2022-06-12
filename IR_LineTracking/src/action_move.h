#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include <Arduino.h>
#include "timer.h"
#include "mecanum.h"

void ActionMoveSequenceDo(void);
void ActionMoveUntilDo(void);

struct ActionMoveSequenceState{
    byte size;
    byte index;
    byte* directions;
    byte (*speeds)[4];   // two dimensional array 
    unsigned int* times; // time to run
    unsigned int count;  // how many times the action as been triggered
    TimedActionMs action = TMR_buildActionMs("ACTION_MOVE_SEQUENCE", 25, &ActionMoveSequenceDo, false);
} actionMoveSequenceState;

void ActionMoveSequenceDo(void) {
    // create a lazy alias
    static struct ActionMoveSequenceState * const _ = &actionMoveSequenceState;
    M_move(M_getDirection(_->directions[_->index]), _->speeds[_->index]);
    ++_->index;
    if(_->index >= _->size) _->index = 0;
    ++_->count;
}

struct ActionMoveUntilState{
    byte direction;
    byte speed[4] = {0,0,0,0};
    unsigned int count;
    bool(*check)(); // The condition which end the action
    void(*run)();   // the callback to run when check returns true
    TimedActionMs action = TMR_buildActionMs("ACTION_MOVE_UNTIL", 25, &ActionMoveUntilDo, false);
} actionMoveUntilState;

void ActionMoveUntilDo() {
    ++actionMoveUntilState.count;
    if(actionMoveUntilState.check()) actionMoveUntilState.run();
}

#endif // ACTION_MOVE_H