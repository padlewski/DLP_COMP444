#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

struct Button {
    int pin;
    int mode;
};

bool isPressed(const Button *button) {
    return digitalRead(button->pin) == LOW;
}

#endif // BUTTON_H