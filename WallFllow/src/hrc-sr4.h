#ifndef HRC_SR4_H
#define HRC_SR4_H

#include <Arduino.h>

float SR4_TO_INCHES = 148.0;
float SR4_TO_CM = 58.0;

struct HrcSr4 {
    uint8_t pinTrigger;
    uint8_t pinEcho;
    float   echoTime;
    float   in;
    float   cm;
};

HrcSr4 initSr4(uint8_t trigger, uint8_t echo) {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    return (HrcSr4) { trigger, echo, 0.0f, 0.0f, 0.0f };
}

void updateSr4(HrcSr4 *sensor) {
    float avg = 0.0f;
    for (int i = 0 ; i < 10 ; ++i){
        digitalWrite(sensor->pinTrigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(sensor->pinTrigger, LOW);
        avg += pulseIn(sensor->pinEcho, HIGH);
    }
    sensor->echoTime = avg / 10;
    sensor->in = sensor->echoTime / SR4_TO_INCHES;
    sensor->cm = sensor->echoTime / SR4_TO_CM;
}

#endif // HRC_SR4_H