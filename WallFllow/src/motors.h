#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

struct Motor {
    uint8_t pins[3];
    int16_t speed;
};

struct Motors {
    Motor right;
    Motor left;
};

const uint8_t FORWARD[] = {HIGH, LOW};
const uint8_t BACK[] = {LOW, HIGH};
const uint8_t STOP[] = {LOW, LOW};

Motors initMotors(uint8_t r1, uint8_t r2, uint8_t rS, uint8_t l1, uint8_t l2, uint8_t lS){
    Motor right;
    Motor left;
    right.pins[0] = r1;
    right.pins[1] = r2;
    right.pins[2] = rS;
    left.pins[0] = l1;
    left.pins[1] = l2;
    left.pins[2] = lS;
    for(int i = 0 ; i < 3 ; ++i){
        pinMode(right.pins[i], OUTPUT);
        pinMode(left.pins[i], OUTPUT);
    }
    return (Motors) {right, left};
}

void setDirection(Motor *motor, const uint8_t* direction) {
    digitalWrite(motor->pins[0], direction[0]);
    digitalWrite(motor->pins[1], direction[1]);
}

void setMotor(Motor *motor){
    if(motor->speed > 0) setDirection(motor, FORWARD);
    else if(motor->speed < 0) setDirection(motor, BACK);
    else setDirection(motor, STOP);
    analogWrite(motor->pins[2], abs(motor->speed));
}

#endif // MOTORS_H