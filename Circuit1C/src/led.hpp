#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>
#include "writer.hpp"

struct LedState {
    const int pin;
    bool ledOn;
    byte state;
};

class LedStrategy {
    byte status;

public:
    virtual void load(LedState&) = 0;
    virtual void update(const unsigned int time, LedState&) = 0;
};

class TriggerStrategy: public LedStrategy {
    const int *trigger;
    int threshold;
public:
    TriggerStrategy(const int *trigger, int threshold):
    trigger(trigger), threshold(threshold) {}

    ~TriggerStrategy() = default;

    void load(LedState &state) override {
        #ifdef _PRINT_
        Serial.println("Loading TriggerStrategy");
        #endif
        state.state = 0;
        state.ledOn = false;
        digitalWrite(state.pin, LOW);
    }

    void update(unsigned int time, LedState &state) override {
        if(*trigger > threshold) return;
        state.state = 1;
    }
};

class FlashStrategy: public LedStrategy {
    unsigned int onInterval;
    unsigned int offInterval;
    unsigned int interval;
    unsigned int timeLapsed;
    byte cycles;
    byte count;
public:
    FlashStrategy(unsigned int onInterval, unsigned int offInterval, byte cycles) :
    onInterval(onInterval), offInterval(offInterval), cycles(cycles) {};

    ~FlashStrategy() = default;

    void load(LedState &state) override {
        #ifdef _PRINT_
        Serial.println("Loading FlashStrategy");
        #endif
        state.state = 1;
        count = 0;
        interval = 0;
        timeLapsed = 0;
        state.ledOn = true;
        digitalWrite(state.pin, HIGH);
        interval = onInterval;
    }

    void update(const unsigned int time, LedState &state) override {
        timeLapsed += time; 
        if(timeLapsed < interval) return; // nothing to do
        timeLapsed = 0;
        if(state.ledOn) {
            digitalWrite(state.pin, LOW);
            ++count;
        } else {
            digitalWrite(state.pin, HIGH);
        }
        state.ledOn = !state.ledOn;
        if(count >= cycles) state.state = 0;
    }
};


class Led {
    LedState ledState;
    LedStrategy** strategies;
    unsigned int count;
    unsigned int currentStrategy;

public:
    Led(LedState ledState, LedStrategy **strategies, unsigned int count):
    ledState(ledState), strategies(strategies), count(count) {
        pinMode(ledState.pin, OUTPUT);
        strategies[currentStrategy]->load(ledState);
    }

    ~Led() {
        delete* strategies;
    }

    void update(const unsigned int time){
        strategies[currentStrategy]->update(time, ledState);
        if(ledState.state == currentStrategy) return;
        // change strategy
        currentStrategy = ledState.state;
        strategies[currentStrategy]->load(ledState);
    }
};

#endif  // LED_HPP
