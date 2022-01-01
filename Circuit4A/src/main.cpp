#include <Arduino.h>

#include <LiquidCrystal.h>          //the liquid crystal library contains commands for printing to the display

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);   // tell the RedBoard what pins are connected to the display

#ifdef _EXPERIMENT_1_

#include "timer.h"

TimerState state;
char buff[16];

void setup() {
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Time since reset");
  lcd.setCursor(0, 1);
  snprintf(buff, 16, "%14lds", getSeconds());
  String msg(buff);
  lcd.print(msg);
  delay(1000);
}

#elif _EXPERIMENT_2_


#include "timer.h"

TimerState state;
char buff[16];

void setup() {
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Time since reset");
  lcd.setCursor(0, 1);
  lcd.print(formatHHMMSS(millis()));
  delay(1000);
}

#elif _EXPERIMENT_3_

#include "button.h"
#include "timer.h"
#include "magic.h"

Button button = {4, INPUT_PULLUP};

TimerState timerState;
char buff[16];

void setup() {
  randomSeed(analogRead(A0));
  pinMode(button.pin, button.mode);
  lcd.begin(16,2);
  lcd.clear();
  state.state = 0;
  state.tDelta = 0;
}

void loop() {
  tick(&timerState);
  if(isPressed(&button) && state.state != 1) {
    play();
  }
  update(timerState.deltaMs, &lcd);
}

#else

void setup() {

  lcd.begin(16, 2);                 //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                      //clear the display
}

void loop() {

  lcd.setCursor(0, 0);              //set the cursor to the 0,0 position (top left corner)
  lcd.print("Hello, world!");       //print hello, world! starting at that position

  lcd.setCursor(0, 1);              //move the cursor to the first space of the bottom row
  lcd.print(millis() / 1000);       //print the number of seconds that have passed since the last reset
}

#endif