#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

const int SPEED_PINS[4] = {3, 11, 5, 6};
const int DIR_PINS[4]   = {4, 12, 8, 7};

const byte MOTOR_SPEED[4] = {0, 0, 0, 0};

const byte FORWARD[4]   = {LOW, HIGH, LOW, HIGH};
const byte BACK[4]      = {HIGH, LOW, HIGH, LOW};
const byte RIGHT[4]     = {LOW, LOW, HIGH, HIGH};
const byte LEFT[4]      = {HIGH, HIGH, LOW, LOW};
const byte CLOCKWISE[4] = {LOW, LOW, LOW, LOW};
const byte COUNTER[4]   = {HIGH, HIGH, HIGH, HIGH};

byte Mi;

void setupMotors(void) {
    for (Mi = 0 ; Mi < 4 ; ++Mi){
        pinMode(SPEED_PINS[Mi], OUTPUT);
        pinMode(DIR_PINS[Mi], OUTPUT);
    }
}

void move(const byte direction[4], const byte speed){
    for (Mi = 0 ; Mi < 4 ; ++Mi) {
        digitalWrite(DIR_PINS[Mi], direction[Mi]);
        analogWrite(SPEED_PINS[Mi], speed);
    }
}


#endif