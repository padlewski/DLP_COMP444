#ifndef MAGIC_H
#define MAGIC_H

#include <Arduino.h>
#include <LiquidCrystal.h>

struct Settings{
    const static int choices = 20;
    String play1 = "Your Question?";
    String play2 = "Press for Answer";
    String messages1[choices] = {
        " It is certain.",
        "It is",
        "Without a doubt.",
        "Yes definitely.",
        "You may rely",
        "As I see it,",
        "Most likely.",
        "Outlook good.",
        "Yes.",
        "Signs point to",
        "Reply hazy,",
        "Ask again later.",
        "Better not tell",
        "Cannot predict",
        "Concentrate",
        "Don't count",
        "My reply is no.",
        "My sources say",
        "Outlook",
        "Very doubtful.",
    };
    String messages2[choices] = {
        "",
        "decidedly so.",
        "",
        "",
        "on it.",
        "yes",
        "",
        "",
        "",
        "Yes.",
        "try again.",
        "",
        "you now.",
        "now.",
        "and ask again.",
        "on it.",
        "",
        "no.",
        "not so good.",
        "",
    };
    const static int wait = 3;
    String waiting[wait] = {
        "||||||||||||||||",
        "////////////////",
        "----------------"
    };
    long selectTimeMsMax = 10000;
    long selectTimeMSMin = 3000;
    long displayTime = 15000;
} settings;

struct State {
    long tDelta;
    byte state = 0;
    int msg = 0;
    long waitTime;
    long selectTime;
    int wait;
} state;

void doSelection(long msDelta, LiquidCrystal *display) {
    state.selectTime += msDelta;
    state.tDelta += msDelta;
    if(state.tDelta > 500) {
        // cycle waiting
        display->setCursor(0,0);
        display->print(settings.waiting[state.wait]);
        display->setCursor(0,1);
        display->print(settings.waiting[state.wait]);
        ++state.wait;
        if(state.wait >= settings.wait) state.wait = 0;
        state.tDelta = 0;
    }
    if(state.selectTime < state.waitTime) return;
    state.msg = random(settings.choices);
    state.state = 2;
    state.tDelta = 0;
}

void doDisplayPlay(long msDelta, LiquidCrystal *display) {
    if(state.tDelta == 0){
        display->clear();
        display->setCursor(0,0);
        display->print(settings.play1);
        display->setCursor(0,1);
        display->print(settings.play2);
    }
    state.tDelta += msDelta;
}

void doDisplayMessage(long msDelta, LiquidCrystal *display) {
    if(state.tDelta == 0) {
        display->clear();
        display->setCursor(0,0);
        display->print(settings.messages1[state.msg]);
        display->setCursor(0,1);
        display->print(settings.messages2[state.msg]);
    }
    state.tDelta += msDelta;
    if(state.tDelta > settings.displayTime) {
        state.state = 0;
        state.tDelta = 0;
        display->clear();
        return;
    }
}

void play() {
    state.waitTime = random(settings.selectTimeMSMin, settings.selectTimeMsMax);
    state.tDelta = 0;
    state.selectTime = 0;
    state.wait = 0;
    state.state = 1;
}

void update(long msDelta, LiquidCrystal *display) {
    switch (state.state) {
    case 1: // Selecting
        doSelection(msDelta, display);
        break;
    case 2:
        doDisplayMessage(msDelta, display);
        break;
    default:
        doDisplayPlay(msDelta, display);
        break;
    }
}

#endif