#include <Arduino.h>

#ifdef _EXPERIMENT_1_

#include "button.h"
#include "buzzer.h"

Buzzer buzzer = {10};

void play_c() {
  play(&buzzer, 'c');
}

void play_g() {
  play(&buzzer, 'g');
}

void play_C() {
  play(&buzzer, 'C');
}

ButtonAction c = {2, INPUT_PULLUP, false, &play_c};
ButtonAction g = {3, INPUT_PULLUP, false, &play_g};
ButtonAction C = {4, INPUT_PULLUP, false, &play_C};

int i;
bool playing;
const int buttonCount = 3;
ButtonAction *buttons[buttonCount] = {&c, &g, &C};

void setup() {
  for(i = 0 ; i < buttonCount ; ++i )
    pinMode(buttons[i]->pin, buttons[i]->mode);
}

void loop() {
  playing = false;
  for(i = 0 ; i < buttonCount ; ++i){
    if(digitalRead(buttons[i]->pin) == LOW){
      buttons[i]->action();
      playing = true;
      break;
    }
  }
  if(not playing) noTone(buzzer.pin);
}

#elif _EXPERIMENT_2_

#include "button.h"
#include "buzzer.h"

Buzzer buzzer = {10};

void setPressed(ButtonAction *button) {
  if(digitalRead(button->pin) == LOW) button->pressed = true;
  else button->pressed = false;
}

ButtonAction b1 = {2, INPUT_PULLUP, false};
ButtonAction b2 = {3, INPUT_PULLUP, false};
ButtonAction b3 = {4, INPUT_PULLUP, false};

int i;
const int buttonCount = 3;
ButtonAction *buttons[buttonCount] = {&b1, &b2, &b3};

void setup() {
  for(i = 0 ; i < buttonCount ; ++i )
    pinMode(buttons[i]->pin, buttons[i]->mode); 
}

void loop() {
  for(i = 0 ; i < buttonCount ; ++i) setPressed(buttons[i]);
  if(b1.pressed && b2.pressed && b3.pressed) play(&buzzer, 'C');
  else if(b2.pressed && b3.pressed) play(&buzzer, 'b');
  else if(b1.pressed && b3.pressed) play(&buzzer, 'a');
  else if(b1.pressed && b2.pressed) play(&buzzer, 'g');
  else if(b3.pressed) play(&buzzer, 'f');
  else if(b2.pressed) play(&buzzer, 'e');
  else if(b1.pressed) play(&buzzer, 'd');
  else noTone(buzzer.pin);
}

#elif _EXPERIMENT_3_

#include "button.h"
#include "buzzer.h"

Buzzer buzzer = {10};
byte flags;

void setPressed(ButtonAction *button) {
  if(digitalRead(button->pin) == LOW) { 
    button->pressed = true;
    flags |= button->flag;
  } else button->pressed = false;
}

ButtonAction b1 = {2, INPUT_PULLUP, false, 0b00000001, &setPressed};
ButtonAction b2 = {3, INPUT_PULLUP, false, 0b00000010, &setPressed};
ButtonAction b3 = {4, INPUT_PULLUP, false, 0b00000100, &setPressed};

int i;
const int buttonCount = 3;
ButtonAction *buttons[buttonCount] = {&b1, &b2, &b3};

void setup() {
  for(i = 0 ; i < buttonCount ; ++i )
    pinMode(buttons[i]->pin, buttons[i]->mode); 
}

void loop() {
  flags = 0; // clear the flags
  for(i = 0 ; i < buttonCount ; ++i) buttons[i]->action(buttons[i]);
  switch (flags) {
  case 0b00000001:
    play(&buzzer, 'd');
    break;
  case 0b00000010:
    play(&buzzer, 'e');
    break;
  case 0b00000011:
    play(&buzzer, 'g');
    break;
  case 0b00000100:
    play(&buzzer, 'f');
    break;
  case 0b00000101:
    play(&buzzer, 'a');
    break;
  case 0b0000110:
    play(&buzzer, 'b');
    break;
  case 0b0000111:
    play(&buzzer, 'C');
    break;
  default:
    noTone(buzzer.pin);
    break;
  }
}

#else

//set the pins for the button and buzzer
int firstKeyPin = 2;
int secondKeyPin = 3;
int thirdKeyPin = 4;

int buzzerPin = 10;


void setup() {
  //set the button pins as inputs
  pinMode(firstKeyPin, INPUT_PULLUP);
  pinMode(secondKeyPin, INPUT_PULLUP);
  pinMode(thirdKeyPin, INPUT_PULLUP);

  //set the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);
}

void loop() {

  if (digitalRead(firstKeyPin) == LOW) {      //if the first key is pressed
    tone(buzzerPin, 262);                     //play the frequency for c
  }
  else if (digitalRead(secondKeyPin) == LOW) { //if the second key is pressed
    tone(buzzerPin, 330);                     //play the frequency for e
  }
  else if (digitalRead(thirdKeyPin) == LOW) { //if the third key is pressed
    tone(buzzerPin, 392);                     //play the frequency for g
  }
  else {
    noTone(buzzerPin);                        //if no key is pressed turn the buzzer off
  }
}

#endif

/*
  note  frequency
  c     262 Hz
  d     294 Hz
  e     330 Hz
  f     349 Hz
  g     392 Hz
  a     440 Hz
  b     494 Hz
  C     523 Hz
*/