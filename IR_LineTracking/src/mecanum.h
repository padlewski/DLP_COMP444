#ifndef MERCANUM_H
#define MERCANUM_H

#include <Arduino.h>

#define M_LOOP for (Mi = 0 ; Mi < M_N ; ++Mi)

// the first four map to turns and wheels (speed)
byte FrontLeft = 0;
byte FrontRight = 1;
byte BackLeft = 2;
byte BackRight = 3;
byte Forward = 4;
byte Back = 5;
byte Right = 6;
byte Left = 7;
byte Clockwise = 8;
byte Counter = 9;

static const byte SPEED_PINS[4] = {3, 11, 5, 6};
static const byte DIR_PINS[4]   = {4, 12, 8, 7};


static int M_SPEED[4] = {0,0,0,0};
static int M_SPEED_PREV[4] = {0,0,0,0};
static byte M_DIR = Forward;
static byte M_DIR_PREV = Forward;
static const int SPEED_STOP[4] = {0, 0, 0, 0};
static const size_t SIZE_SPEEDS = sizeof(int[4]);

    //  [ FL, FR, BL, BR ]
const bool DIRECTIONS[10][4] = {
    {HIGH, HIGH, LOW, HIGH},    // F_LEFT
    {LOW, LOW, LOW, HIGH},      // F_RIGHT
    {LOW, HIGH, HIGH, HIGH},    // B_LEFT
    {LOW, HIGH, LOW, LOW},      // B_RIGHT
    {LOW, HIGH, LOW, HIGH},     // FORWARD
    {HIGH, LOW, HIGH, LOW},     // BACK
    {LOW, LOW, HIGH, HIGH},     // RIGHT
    {HIGH, HIGH, LOW, LOW},     // LEFT
    {LOW, LOW, LOW, LOW},       // CLOCKWISE
    {HIGH, HIGH, HIGH, HIGH}    // COUNTER
};

static const uint8_t M_N = 4;
byte Mi;

void M_setupMotors(void) {
    M_LOOP {
        pinMode(SPEED_PINS[Mi], OUTPUT);
        pinMode(DIR_PINS[Mi], OUTPUT);
    }
}

void M_move(const byte direction, const int speed[M_N]){
    if(direction == M_DIR && memcmp(speed, M_SPEED, SIZE_SPEEDS) == 0) return; // no change
    M_DIR_PREV = M_DIR;
    M_DIR = direction;
    memcpy(M_SPEED_PREV, M_SPEED, SIZE_SPEEDS);
    memcpy(M_SPEED, speed, SIZE_SPEEDS);
    //const byte* dir = M_getDirection(M_DIR);
    M_LOOP {
        digitalWrite(DIR_PINS[Mi], DIRECTIONS[M_DIR][Mi]);
        analogWrite(SPEED_PINS[Mi], M_SPEED[Mi]);
    }
}

void M_stop() {
    M_move(SPEED_PINS[Mi], SPEED_STOP);
}

#endif // MERCANUM_H