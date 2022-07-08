#ifndef BOT_H
#define BOT_H

#include "Arduino.h"
#include "timer.h"
#include "ir_line.h"
#include "mecanum.h"
#include "WaveshareIMU.h"

struct BotState {
    byte lastKnownIr;
    byte currentState;
    byte previousState;
} botState;

static const byte BOT_PAUSED = B10000000;
static const byte BOT_CALIBRATED = B00000001;
static const byte BOT_FOLLOW = B00000010;

static const byte switchPin = 9;

void updateBotState(byte mask) {
    botState.previousState = botState.currentState;
    botState.currentState ^= mask;
}

#endif // BOT_H