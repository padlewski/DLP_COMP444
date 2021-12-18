#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "buzzer.h"
#include "button.h"
#include "timer.h"

TimerState timerState = {0,0,0,0};

struct GameSettings {
    Buzzer buzzer = {10};       // the buzzer
    const int inputs = 4;       // number of inputs

    // buttons inputs
    const Button buttons[4] = { {2, INPUT_PULLUP, false},
                                {4, INPUT_PULLUP, false},
                                {6, INPUT_PULLUP, false},
                                {8, INPUT_PULLUP, false} };
    // led inputs
    const int leds[4] = {3, 5, 7 ,9};
    // button sounds
    const char buttonTones[4] = {'e', 'g', 'b', 'D'};
    // win song
    const Tone winSong[4] = {{'C', 300}, {'D', 750}, {'D', 750}, {'G', 2000}};      
    // lose song
    const Tone loseSong[2] = {{'a', 750}, {'g', 750}};
    #ifdef _EXPERIMENT_1_     
    int rounds = 6;                // number of rounds
    #else
    int rounds = 15;
    #endif
    unsigned long timelimit = 2500; // wait time for input    
    unsigned long reminder = 7500; // reminder flash when waiting
} settings;

struct GameState {
    byte mode;                  // game mode
    int ctr;                    // shared counter
    unsigned long time;         // elapsed time
    int p1round;                // player 1 round
    int p2round;                // player 2 round
    int *round;                 // current round         
    int p1sequence[16];         // player 1 memory sequence
    int p2sequence[16];         // player 1 memory sequence
    int *sequence;              // pointer to current player's sequence
    int player;                // current player
    // current state handler
    void (*handler)(GameState*, GameSettings*);
};

void handleOutput(GameState *state, GameSettings *settings);
void handleInput(GameState *state, GameSettings *settings);
void handleWin(GameState *state, GameSettings *settings);
void handleLoss(GameState *state, GameSettings *settings);
void handleWait(GameState *state, GameSettings *settings);
void handleChangePlayer(GameState *state, GameSettings *settings);
void initGame(GameState *state);

void displaySequence(int idx, long len, GameSettings *settings) {
    digitalWrite(settings->leds[idx], HIGH);
    play(&settings->buzzer, settings->buttonTones[idx]);
    delay(len);
    noTone(settings->buzzer.pin);
    digitalWrite(settings->leds[idx], LOW);
}

int getPressed(GameSettings *settings) {
    for(int i = 0 ; i < settings->inputs ; ++i){
        if(isPressed(&settings->buttons[i])) return i;
    }
    return 4;
}

void handleIntro(GameState *state, GameSettings *settings) {
    initGame(state);
    for(int i = 0 ; i < settings->inputs ; ++i) {
        displaySequence(i, 500, settings);
    }
    state->handler = &handleOutput;
    delay(250);
    tick(&timerState);
}

void handleOutput(GameState *state, GameSettings *settings) {
    ++*state->round;
    if(*state->round > settings->rounds){
        state->handler = &handleWin;
        return;
    }
    delay(500);
    for(state->ctr = 0 ; state->ctr < *state->round ; ++state->ctr){
        displaySequence(state->sequence[state->ctr], 300, settings);
        delay(300);
    }
    state->ctr = 0; //reset the counter
    state->handler = &handleInput;
    tick(&timerState);
}

void handleInput(GameState *state, GameSettings *settings) {
    if(state->ctr >= *state->round) {
        if(state->mode == 4) state->handler = &handleChangePlayer;
        else state->handler = &handleOutput;
        return;
    }
    tick(&timerState);
    state->time += timerState.deltaMs;
    if (state->time > settings->timelimit) {
        state->handler = &handleLoss;
        return;
    }
    int pressed = getPressed(settings);
    if(pressed == 4) return;
    displaySequence(pressed, 300, settings);
    if(pressed != state->sequence[state->ctr]) {
        state->handler = &handleLoss;
        return;
    }
    if(state->ctr < *state->round){
        state->time = 0;
        tick(&timerState);
        ++state->ctr;
        return;
    }
}

void handleWin(GameState *state, GameSettings *settings) {
    for(int i = 0 ; i < settings->inputs ; ++i) 
        digitalWrite(settings->leds[i], HIGH);
    for(int i = 0 ; i < 4 ; ++i) {
        delay(play(&settings->buzzer, &settings->winSong[i]));
        delay(25);
    }
    for(int i = 0 ; i < settings->inputs ; ++i) 
        digitalWrite(settings->leds[i], LOW);
    state->handler = &handleWait;
    state->time = 0;
    tick(&timerState);
}

void handleLoss(GameState *state, GameSettings *settings) {
    for(int i = 0 ; i < settings->inputs ; ++i) 
        digitalWrite(settings->leds[i], HIGH);
    for(int i = 0 ; i < 2 ; ++i) {
        delay(play(&settings->buzzer, &settings->loseSong[i]));
        delay(25);
    }
    for(int i = 0 ; i < settings->inputs ; ++i) 
        digitalWrite(settings->leds[i], LOW);
    state->handler = &handleWait;
    state->time = 0;
    tick(&timerState);
}

void handleWait(GameState *state, GameSettings *settings) {
    tick(&timerState);
    state->time += timerState.deltaMs;
    if(getPressed(settings) < settings->inputs) {
        state->handler = &handleIntro;
        state->time = 0;
        return;
    }
    if(state->time > settings->reminder) {
        for(int i = 0 ; i < settings->inputs ; ++i) {
            for(int n = 0 ; n < settings->inputs ; ++n) 
                digitalWrite(settings->leds[n], HIGH);
            delay(250);
            for(int n = 0 ; n < settings->inputs ; ++n) 
                digitalWrite(settings->leds[n], LOW);
            delay(250);
        }
        state->time = 0;
    }
}

void handleChangePlayer(GameState *state, GameSettings *settings){
    #ifdef _PRINT_
    Serial.print("Switch Player: ");
    Serial.print(state->player);
    #endif
    if(state->player) {
        state->sequence = state->p1sequence;
        state->round = &state->p1round;
        state->player = 0;
    } else {
        state->sequence = state->p2sequence;
        state->round = &state->p2round;
        state->player = 1;
    }
    #ifdef _PRINT_
    Serial.print(" to ");
    Serial.println(state->player);
    #endif
    state->handler = &handleOutput;
}

void initGame(GameState *state){
    state->player = 0;
    state->p1round = 0;
    state->p2round = 0;
    state->ctr = 0;
    for(int i = 0 ; i < 16 ; ++i) {
        state->p1sequence[i] = random(4);
        state->p2sequence[i] = random(4);
    }
    state->sequence = state->p1sequence;
    state->round = &state->p1round;
}

#endif // GAME_H