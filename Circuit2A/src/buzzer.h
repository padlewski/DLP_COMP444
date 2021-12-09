#ifndef BUZZER_H
#define BUZZER_H

struct Buzzer{
    int pin;
};

struct Note{
    char note;
    int frequency;
};

struct Chord{
    char note;
    int count;
};

struct Song{
    int length;
    Chord* music;
    int tempo;
    int step;
    int playCount;
};

const int numNotes = 15;
const Note notes[numNotes] = { {' ',0},
                {'c',131},{'d',247},{'e',165},{'f',175},{'g',196},{'a',220},{'b',247},
                {'C',262},{'D',294},{'E',330},{'F',349},{'G',392},{'A',440},{'B',494} };

Note find(char note) {
    for(int i = 1 ; i < numNotes ; ++i)
        if(notes[i].note == note) return notes[i];
    return notes[0]; // note not found return a rest
}

void play(Buzzer buzzer, Song *song) {
    auto chord = song->music[song->step];
    auto note = find(chord.note);
    auto len = chord.count * song->tempo;
    tone(buzzer.pin, note.frequency, len);
    ++song->step;
    if(song->step >= song->length) { 
        song->step = 0;
        --song->playCount;
    }
    delay(len);
    delay(60);
}

#endif