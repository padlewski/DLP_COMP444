#ifndef OCANADA_H
#define OCANADA_H

#include "buzzer.h"

// score from https://en.wikipedia.org/wiki/O_Canada
const Chord chords[89]= {       
        {' ',8}, // rest for intro
        {'a',8},{'C',6},{'C',2},{'f',12},{'g',4},{'a',4},{'b',4},{'C',4},{'D',4},{'g',12},
        {' ',4},{'a',8},{'b',6},{'b',2},{'C',12},{'D',4},{'E',4},{'E',4},{'D',4},{'D',4},
        {'C',12},{'g',3},{'a',1},{'b',6},{'a',2},{'g',4},{'a',3},{'b',1},{'C',6},{'b',2},
        {'a',4},{'b',3},{'C',1},{'D',4},{'C',4},{'b',4},{'a',4},{'g',12},{'g',3},{'a',1},
        {'b',6},{'a',2},{'g',4},{'a',3},{'b',1},{'C',6},{'b',2},{'a',4},{'a',4},{'g',4},
        {'C',4},{'C',2},{'b',2},{'a',2},{'b',2},{'C',12},{' ',4},{'a',8},{'C',6},{'C',2},
        {'a',12},{' ',4},{'b',8},{'D',6},{'D',2},{'g',12},{' ',4},{'C',12},{'C',6},{'C',2},
        {'D',4},{'b',4},{'a',4},{'g',4},{'f',8},{'g',8},{'a',12},{' ',4},{'C',8},{'F',6},
        {'F',2},{'D',4},{'b',4},{'a',4},{'g',4},{'C',8},{'E',8},{'F',16}
    };

Song o_canada = {
    .length = 89,
    .music = chords,
    .tempo = 100,
    .step = 0,
    .playCount = 0
};

#endif