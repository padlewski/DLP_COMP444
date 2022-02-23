#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "hrc-sr4.h"
#include "ir_pololu_prox.h"
#include "motors.h"

HrcSr4 sr4Back = initSr4(DD6, DD5);
HrcSr4 sr4Front = initSr4(DD4, DD3);
ProximitySensor prox = initProx(DD2);
int8_t switchPin = DD7;
Motors motors = initMotors(13, 12, 11, 8, 9, 10);

const int RSPEED = 70;
const int LSPEED = 90;
const float THREASHOLD = 35.0f; // cm to wall
const float TOLERANCE = 5.0f;   // cm of tolerance
int16_t following; // - away, 0 parallel , + toward
float distance;
float error; 

void update(void) {
  updateProx(&prox);
  updateSr4(&sr4Back);
  updateSr4(&sr4Front);
}

void stop() {
    motors.right.speed = 0;
    motors.left.speed = 0;
}

void forward() {
    motors.right.speed = RSPEED;
    motors.left.speed = LSPEED;
}

void back() {
    motors.right.speed = -1 * RSPEED;
    motors.left.speed = -1 * LSPEED;
}

void moveRight(int amt) {
    #ifdef _PRINT_
    Serial.print("Move Right: "); Serial.println(amt);
    #endif
    motors.right.speed = RSPEED;
    motors.left.speed = LSPEED + amt;
}

void moveLeft(int amt) {
    #ifdef _PRINT_
    Serial.print("Move Left: "); Serial.println(amt);
    #endif
    motors.right.speed = RSPEED + amt;
    motors.left.speed = LSPEED;
}

int16_t setDirection(void) {
    float offset = sr4Back.cm - sr4Front.cm;
    if(abs(offset) < 4) return 0; // close to parallel
    // If back is closer (smaller) will be negative or moveing away
    return int(offset);
}

void doLeveling(void) {
    if(following == 0) return forward(); // nothing to do keep going
    int16_t adj = constrain(error, -25, 25);
    if(adj < 0) moveRight(abs(adj));
    moveLeft(abs(adj)); 
}

void doAway(void) {
    if(following > 0) return moveLeft(25); // time to change direction
    // are we moving away enough
    int16_t adj = constrain(following, -25, 25);
    moveLeft(25 - abs(adj)); // dont turn too far away
}

void doTowards(void) {
    if(following < 0) return moveRight(25); // time to change direction
    // are we moving away enough
    int16_t adj = constrain(following, -25, 25);
    moveRight(25 - abs(adj)); // dont turn too far away
}

void doAdjustment(void) {
    if(abs(error) < TOLERANCE) return doLeveling();
    if(error < 0) return doAway();
    doTowards();
}

void calc(void) {
    if(prox.blocked || digitalRead(switchPin) == LOW) {
        stop();
        return;
    }
    following = setDirection();
    distance = min(sr4Back.cm, sr4Front.cm);
    error = distance - THREASHOLD;
    //forward();
    doAdjustment();
}

void move(void){
    setMotor(&motors.left);
    setMotor(&motors.right);
}

#endif // CONTROLLER_H