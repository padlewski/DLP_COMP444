#ifndef BUZZER_H
#define BUZZER_H

#include "Arduino.h"

struct Buzzer{
    int pin;
};

struct Note{
    char note;
    int frequency;
};

const int numNotes = 15;
const Note notes[numNotes] = { {' ',0},
                {'c',262},{'d',294},{'e',330},{'f',349},{'g',392},{'a',440},{'b',494},
                {'C',523},{'D',587},{'E',659},{'F',698},{'G',784},{'A',880},{'B',988} };

Note * find(char note) {
    for(int i = 1 ; i < numNotes ; ++i)
        if(notes[i].note == note) return &(notes[i]);
    return &(notes[0]); // note not found return a rest
}

void play(Buzzer *buzzer, char note) {
    auto n = find(note);
    tone(buzzer->pin, n->frequency);
}

#endif