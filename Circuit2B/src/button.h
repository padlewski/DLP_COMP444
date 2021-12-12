#ifndef BUTTON_H
#define BUTTON_H


struct ButtonAction {
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

#endif // BUTTON_H