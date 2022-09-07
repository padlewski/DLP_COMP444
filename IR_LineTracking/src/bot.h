#ifndef BOT_H
#define BOT_H

#include "Arduino.h"
#include "timer.h"
#include "ir_line.h"
#include "mecanum.h"
#include "WaveshareIMU.h"

static int LF_SPEED[4] = {60, 60, 60, 60};
static int BK_SPEED[4] = {0, 0, 60, 60};
// static int FR_SPEED[4] = {60, 60, 0, 0};
// static int RI_SPEED[4] = {0, 60, 0, 60};
// static int LE_SPEED[4] = {60, 0, 60, 0};

struct BotState {
    byte headingsNESW[4] = {0, 64, 128, 192};
    byte trgHeading;
    int  offsetHeading;
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