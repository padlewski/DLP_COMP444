#ifndef RGB_LED_HPP
#define RGB_LED_HPP

#include <Arduino.h>
#include "writer.hpp"

#define  C_OFF     {0, 0, 0}        // BLACK
#define  C_WHITE   {255,255,255}
#define  C_RED     {255,0,0}
#define  C_ORANGE  {255, 128, 0}
#define  C_YELLOW  {255, 255, 0}
#define  C_GREEN   {0, 255, 0}
#define  C_CYAN    {0, 255, 255}
#define  C_BLUE    {0, 0, 255}
#define  C_MAGENTA {255, 0, 255}

// Based on https://www.rapidtables.com/web/color/RGB_Color.html
// pull only colours that have a 255 in R G or B
#define C_TOMATO     {255, 99, 71}
#define C_CORAL      {255, 127, 80}
#define C_SALMON     {255, 160, 122}
#define C_AQUA       {127, 255, 212}
#define C_ROYAL_BLUE {65, 105, 255}
#define C_PINK       {255, 20, 147}
#define C_BROWN      {255, 170, 100} // Based off a modified sandy brown
#define C_MINT       {245, 255, 250}

const byte COLOURS[17][3] = 
    {C_OFF, C_WHITE, C_MINT, C_YELLOW, C_ORANGE, C_TOMATO, C_SALMON, C_CORAL, C_RED, C_PINK,
     C_MAGENTA, C_AQUA, C_ROYAL_BLUE, C_BLUE, C_CYAN, C_BROWN, C_GREEN};

struct RgbLedState {
    const int pinRed;
    const int pinGreen;
    const int pinBlue;
    byte dimFactor;
    bool ledOn;
    int colour;
    byte state;
};

class RgbLedStrategy {
    byte status;

public:
    virtual void load(RgbLedState&) = 0;
    virtual void update(const unsigned int time, RgbLedState&) = 0;
    void setColour(RgbLedState &state, int colourIndex, byte dimFactor = 1) {
        analogWrite(state.pinRed, COLOURS[colourIndex][0] / dimFactor);
        analogWrite(state.pinGreen, COLOURS[colourIndex][1] / dimFactor);
        analogWrite(state.pinBlue, COLOURS[colourIndex][2] / dimFactor);
    }

    void setColour(RgbLedState &state, const byte colour[3], byte dimFactor = 1) {
        analogWrite(state.pinRed, colour[0] / dimFactor);
        analogWrite(state.pinGreen, colour[1] / dimFactor);
        analogWrite(state.pinBlue, colour[2] / dimFactor);
    }

    bool equal(byte first[3], byte second[3]) {
        return first[0] == second[0] && first[1] == second[1] && first[2] == second[2];
    }
};

class TriggerStrategy: public RgbLedStrategy {
    const int *trigger;
    int threshold;
public:
    TriggerStrategy(const int *trigger, int threshold):
    trigger(trigger), threshold(threshold) {}

    ~TriggerStrategy() = default;

    void load(RgbLedState &state) override {
        #ifdef _PRINT_
        Serial.println("Loading TriggerStrategy");
        #endif
        state.state = 0;
        setColour(state, 0);
    }

    void update(unsigned int time, RgbLedState &state) override {
        if(*trigger > threshold) {
            state.ledOn = false;
            state.colour = 0;
            setColour(state, 0);
        } else state.state = 1;
    }
};

class VariableTriggerStrategy: public RgbLedStrategy {
    const int *trigger;
    const int *pot;

public:
    VariableTriggerStrategy(const int *trigger, const int *pot):
    trigger(trigger), pot(pot) {}

    ~VariableTriggerStrategy() = default;

    void load(RgbLedState &state) override {
        #ifdef _PRINT_
        Serial.println("Loading TriggerStrategy");
        #endif
        state.state = 0;
        setColour(state, 0);
    }

    void update(unsigned int time, RgbLedState &state) override {
        if(*trigger > *pot) {
            state.ledOn = false;
            setColour(state, 0);
            return;
        }
        if(state.ledOn) return;
        state.ledOn = true;
        setColour(state, state.colour);
    }
};

class ColourStrategy: public RgbLedStrategy {
    const int *trigger;
    int colourIndex;

    int getColourIndex() {
        int idx = (*trigger / 64) + 1;
        return idx > 16 ? 16 : idx < 1 ? 1 : idx;
    }

public: 
    ColourStrategy(const int *trigger):
    trigger(trigger), colourIndex(0) {}

    ~ColourStrategy() = default;

    void load(RgbLedState &state) override {
        #ifdef _PRINT_
        Serial.println("Loading ColourStrategy");
        #endif
        colourIndex = getColourIndex();
    }

    void update(unsigned int time, RgbLedState &state) override {
        state.state = 0;
        int idx = getColourIndex();
        if(idx == state.colour) return;
        #ifdef _PRINT_
        Serial.print("Colour Index: ");
        Serial.println(idx);
        #endif
        state.colour = idx;
        setColour(state, state.colour, state.dimFactor);
    }
};

class FlashStrategy: public RgbLedStrategy {
    unsigned int onInterval;
    unsigned int offInterval;
    unsigned int interval;
    unsigned int timeLapsed;

public:
    FlashStrategy(unsigned int onInterval, unsigned int offInterval) :
    onInterval(onInterval), offInterval(offInterval) {};

    ~FlashStrategy() = default;

    void load(RgbLedState &state) override {
        #ifdef _PRINT_
        Serial.println("Loading FlashStrategy");
        #endif
        state.state = 1;
        timeLapsed = 0;
        state.ledOn = true;
        setColour(state, state.colour, state.dimFactor);
        interval = onInterval;
    }

    void update(const unsigned int time, RgbLedState &state) override {
        timeLapsed += time; 
        if(timeLapsed < interval) return; // nothing to do
        timeLapsed = 0;
        if(state.ledOn) {
            setColour(state, 0);
        } else {
            ++state.colour;
            if(state.colour > 16) state.colour = 1;
            setColour(state, state.colour, state.dimFactor);
            #ifdef _PRINT_
            Serial.print("Colour: ");
            Serial.println(state.colour);
            #endif
        }
        state.ledOn = !state.ledOn;
    }
};

class PulseStrategy: public RgbLedStrategy {
    byte wheel[7][3] = {C_WHITE, C_MAGENTA, C_BLUE, C_CYAN, C_GREEN, C_YELLOW, C_RED};
    const int count = 7;
    byte colour[3] = {0, 0, 0};
    int colourStep[3] = {0, 0, 0};
    unsigned int interval;
    int resolution;
    unsigned int timeLapse;
    int colourFrom;
    int colourTo;
    int i;

    void setInterval() {
        for (i = 0 ; i < 3 ; ++i){
            colourStep[i] = ((int)wheel[colourTo][i] - (int)wheel[colourFrom][i]) / resolution;
        }
        Serial.print("Interval RGB: { ");
        Serial.print(colourStep[0]);
        Serial.print(", ");
        Serial.print(colourStep[1]);
        Serial.print(", ");
        Serial.print(colourStep[2]);
        Serial.print(" } | Colour From: ");
        Serial.print(colourFrom);
        Serial.print(" | Colour To: ");
        Serial.println(colourTo);
    }

    void tween() {
        for(i = 0 ; i < 3 ; ++i) {
            colour[i] += colourStep[i];
            // deal with overflow or underflow
            if(colour[i] > 255 ) colour[i] = wheel[colourTo][i];
        }
    }

    void printColor() {
        Serial.print("Current RGB: { ");
        Serial.print(colour[0]);
        Serial.print(", ");
        Serial.print(colour[1]);
        Serial.print(", ");
        Serial.print(colour[0]);
        Serial.print(" } | Colour From: ");
        Serial.print(colourFrom);
        Serial.print(" | Colour To: ");
        Serial.println(colourTo);
    }

public:
    PulseStrategy(int interval, int resolution) :
    interval(interval), resolution(resolution), colourFrom(0), colourTo(1) {}

    ~PulseStrategy() = default;

    void load(RgbLedState &state) override {
        for(i = 0 ; i < 3 ; ++i) colour[i] = wheel[colourFrom][i];
        setInterval();
        setColour(state, colour);
    }

    void update(const unsigned int time, RgbLedState &state) override {
        timeLapse += time;
        if(timeLapse < interval) return;
        timeLapse = 0;
        tween();
        setColour(state, colour);
        #ifdef _PRINT_
        printColor();
        #endif
        if(not equal(colour, wheel[colourTo])) return;
        Serial.println("Update Colour");
        colourFrom = colourTo;
        ++colourTo;
        if(colourTo >= count) colourTo = 0;
        setInterval();
    }

};

class RgbLed {
    RgbLedState ledState;
    RgbLedStrategy** strategies;
    unsigned int count;
    unsigned int currentStrategy;

public:
    RgbLed(RgbLedState ledState, RgbLedStrategy **strategies, unsigned int count):
    ledState(ledState), strategies(strategies), count(count) {
        pinMode(ledState.pinRed, OUTPUT);
        pinMode(ledState.pinGreen, OUTPUT);
        pinMode(ledState.pinBlue, OUTPUT);
        strategies[currentStrategy]->load(ledState);
    }

    ~RgbLed() {
        delete* strategies;
    }

    void update(const unsigned int time){
        strategies[currentStrategy]->update(time, ledState);
        if(ledState.state == currentStrategy) return;
        // change strategy
        currentStrategy = ledState.state;
    }
};

#endif  // RGB_LED_HPP
