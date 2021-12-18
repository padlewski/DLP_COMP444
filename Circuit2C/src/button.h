#ifndef BUTTON_H
#define BUTTON_H


struct Button {
    int pin;
    int mode;
    bool pressed;
    #ifdef _EXPERIMENT_3_
    byte flag;
    void (*action)(ButtonAction*);
    #elif _EXPERIMENT_1_
    void (*action)();
    #endif
};

bool isPressed(const Button *button) {
    return digitalRead(button->pin) == LOW;
}

#endif // BUTTON_H