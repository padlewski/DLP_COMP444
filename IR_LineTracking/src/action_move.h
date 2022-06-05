#ifndef ACTION_MOVE_H
#define ACTION_MOVE_H

#include <Arduino.h>
#include "timer.h"
#include "mecanum.h"

void ActionMoveSequenceDo(void);

struct ActionMoveSequenceState{
    byte size;
    byte index;
    byte* directions;
    byte (*speeds)[4]; // two dimensional array 
    unsigned int* times; // time to run
    TimedActionMs action = buildActionMs("ACTION_MOVE", 25, &ActionMoveSequenceDo, false);
} actionMoveSequenceState;

void ActionMoveSequenceDo(void) {
    // create a lazy alias
    static struct ActionMoveSequenceState * const _ = &actionMoveSequenceState;
    M_move(M_getDirection(_->directions[_->index]), _->speeds[_->index]);
    ++_->index;
    if(_->index >= _->size) _->index = 0;
}

#endif // ACTION_MOVE_H