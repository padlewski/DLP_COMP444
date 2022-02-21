#ifndef ir_pololu_prox_h
#define ir_pololu_prox_h

// Pololu IR proximity sensore (38khz)
// https://www.pololu.com/product/2578/blog

#include <Arduino.h>

struct ProximitySensor {
    bool isSwichable;
    uint8_t pinOut;
    uint8_t pinTrg;
    bool blocked;
    bool active;
};

void setOn(ProximitySensor*);
void setOff(ProximitySensor*);

ProximitySensor init(uint8_t pinOut){
    pinMode(pinOut, INPUT);
    return (ProximitySensor) {false, pinOut, 0, 0, 1};
}

ProximitySensor init(uint8_t pinOut, uint8_t pinTrigger, bool startOn){
    pinMode(pinOut, INPUT);
    pinMode(pinTrigger, OUTPUT);
    ProximitySensor sensor {true, pinOut, pinTrigger, false, false};
    if(startOn) setOn(&sensor);
    else setOff(&sensor);
    return sensor;
}

void toggleState(ProximitySensor *sensor) {
    if(sensor->active) setOff(sensor);
    else setOn(sensor);
}

void setOn(ProximitySensor *sensor) {
    digitalWrite(sensor->pinTrg, HIGH);
    sensor->active = true;
}

void setOff(ProximitySensor *sensor) {
    digitalWrite(sensor->pinTrg, LOW);
    sensor->active = false;
}

void update(ProximitySensor *sensor) {
    sensor->blocked = !digitalRead(sensor->pinOut);
}

#endif // ir_pololu_prox_h