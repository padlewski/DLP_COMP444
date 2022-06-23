#ifndef MERCANUM_H
#define MERCANUM_H

#include <Arduino.h>

#define M_LOOP for (Mi = 0 ; Mi < M_N ; ++Mi)

// enum dir {
byte Forward = 0;
byte Back = 1;
byte Right = 2;
byte Left = 3;
byte Clockwise = 4;
byte Counter = 5;
byte FrontTurnRight = 6;
byte FrontTurnLeft = 7;
byte BackTurnRight = 8;
byte BackTurnLeft = 9;
// };

const int SPEED_PINS[4] = {3, 11, 5, 6};
const int DIR_PINS[4]   = {4, 12, 8, 7};

const byte MOTOR_SPEED[4] = {0, 0, 0, 0};

const byte FORWARD[4]   = {LOW, HIGH, LOW, HIGH};
const byte BACK[4]      = {HIGH, LOW, HIGH, LOW};
const byte RIGHT[4]     = {LOW, LOW, HIGH, HIGH};
const byte F_RIGHT[4]   = {LOW, LOW, LOW, HIGH};
const byte B_RIGHT[4]   = {LOW, HIGH, LOW, LOW};   // Turn right using back 
const byte LEFT[4]      = {HIGH, HIGH, LOW, LOW};
const byte F_LEFT[4]    = {HIGH, HIGH, LOW, HIGH};
const byte B_LEFT[4]    = {LOW, HIGH, HIGH, HIGH}; // Turn left using back
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
    case 6:
        return F_RIGHT;
        break;
    case 7:
        return F_LEFT;
        break;
    case 8:
        return B_RIGHT;
        break;
    case 9:
        return B_LEFT;
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

void M_move(const byte direction[M_N], const int speed){
    M_LOOP {
        digitalWrite(DIR_PINS[Mi], direction[Mi]);
        analogWrite(SPEED_PINS[Mi], speed);
    }
}

void M_move(const byte direction[M_N], const int speed[M_N]){
    M_LOOP {
        digitalWrite(DIR_PINS[Mi], direction[Mi]);
        analogWrite(SPEED_PINS[Mi], speed[Mi]);
    }
}

void M_stop() {
    M_LOOP analogWrite(SPEED_PINS[Mi], 0);
}

#endif // MERCANUM_H