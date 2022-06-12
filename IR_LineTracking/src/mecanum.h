#ifndef MERCANUM_H
#define MERCANUM_H

#include <Arduino.h>

#define M_LOOP for (Mi = 0 ; Mi < M_N ; ++Mi)

enum dir {
    Forward = 0,
    Back = 1,
    Right = 2,
    Left = 3,
    Clockwise = 4,
    Counter = 5
};

const int SPEED_PINS[4] = {3, 11, 5, 6};
const int DIR_PINS[4]   = {4, 12, 8, 7};

const byte MOTOR_SPEED[4] = {0, 0, 0, 0};

const byte FORWARD[4]   = {LOW, HIGH, LOW, HIGH};
const byte BACK[4]      = {HIGH, LOW, HIGH, LOW};
const byte RIGHT[4]     = {LOW, LOW, HIGH, HIGH};
const byte LEFT[4]      = {HIGH, HIGH, LOW, LOW};
const byte CLOCKWISE[4] = {LOW, LOW, LOW, LOW};
const byte COUNTER[4]   = {HIGH, HIGH, HIGH, HIGH};

static const uint8_t M_N = 4;
byte Mi;

const byte* M_getDirection(const byte flag) {
    switch (flag)
    {
    case 0:
        return FORWARD;
        break;
    case 1:
        return BACK;
        break;
    case 2:
        return RIGHT;
        break;
    case 3:
        return LEFT;
        break;
    case 4:
        return CLOCKWISE;
        break;
    case 5:
        return COUNTER;
        break;
    default:
        return FORWARD;
    }
}

void M_setupMotors(void) {
    M_LOOP {
        pinMode(SPEED_PINS[Mi], OUTPUT);
        pinMode(DIR_PINS[Mi], OUTPUT);
    }
}

void M_move(const byte direction[M_N], const byte speed){
    M_LOOP {
        digitalWrite(DIR_PINS[Mi], direction[Mi]);
        analogWrite(SPEED_PINS[Mi], speed);
    }
}

void M_move(const byte direction[M_N], const byte speed[M_N]){
    M_LOOP {
        digitalWrite(DIR_PINS[Mi], direction[Mi]);
        analogWrite(SPEED_PINS[Mi], speed[Mi]);
    }
}

void M_stop() {
    M_LOOP analogWrite(SPEED_PINS[Mi], 0);
}

#endif // MERCANUM_H