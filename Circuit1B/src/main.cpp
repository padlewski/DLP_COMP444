/*
* Original code based on SparkFun Circuit 1B 
* https://github.com/sparkfun/SIK-Guide-Code/blob/master/SIK_Circuit_1B-Potentiometer/SIK_Circuit_1B-Potentiometer.ino
*/
#include <Arduino.h>
#include "potentiometer.hpp"
#include "led.hpp"
#include "timer.hpp"
#include "writer.hpp"

// Experiment 1 Useing timer and other classes
#ifdef _EXPERIMENT_1

Timer timer;
Potentiometer pot(A0);
Led led(13, pot.getValue());

#ifdef _PRINT
TimerPrinter *timerPrinter = new TimerPrinter(timer);
PotPrinter *potPrinter = new PotPrinter(pot);
Printer *printers[2] = { timerPrinter, potPrinter };
Writer writer(2000, printers, 2);
#endif // _PRINT

void setup() {
  Serial.begin(9600);
}

void loop() {
  timer.tick();
  pot.read();
  led.update(timer.getDeltaMs());
  #ifdef _PRINT
  writer.update(timer.getDeltaMs());
  #endif // _PRINT
}

// Experiment 2 - Dimming with potentiometer
#elif _EXPERIMENT_2

Timer timer;
Potentiometer pot(A0);
DimmableLed led(13, pot.getValue());

#ifdef _PRINT
TimerPrinter *timerPrinter = new TimerPrinter(timer);
PotPrinter *potPrinter = new PotPrinter(pot);
DimmableLedPrinter * ledPrinter = new DimmableLedPrinter(led);
Printer *printers[3] = { timerPrinter, potPrinter, ledPrinter };
Writer writer(2000, printers, 3);
#endif // _PRINT

void setup() {
  Serial.begin(9600);
}

void loop() {
  timer.tick();
  pot.read();
  led.update(timer.getDeltaUs());
#ifdef _PRINT
  writer.update(timer.getDeltaMs());
#endif // _PRINT
}

#elif _EXPERIMENT_3

Timer timer;
Potentiometer pot(A0);
Led flashLed(13, pot.getValue());
DimmableLed dimmLed(11, pot.getValue());
DimmableLedFlipped dimmLedFlip(12, pot.getValue());
LedFlipped flashLedFlip(10, pot.getValue());


#ifdef _PRINT
TimerPrinter *timerPrinter = new TimerPrinter(timer);
PotPrinter *potPrinter = new PotPrinter(pot);
DimmableLedPrinter * ledPrinter = new DimmableLedPrinter(dimmLed);
Printer *printers[3] = { timerPrinter, potPrinter, ledPrinter };
Writer writer(2000, printers, 3);
#endif // _PRINT

void setup() {
  Serial.begin(9600);
}

void loop() {
  timer.tick();
  pot.read();
  flashLed.update(timer.getDeltaMs());
  dimmLed.update(timer.getDeltaUs());
  dimmLedFlip.update(timer.getDeltaUs());
  flashLedFlip.update(timer.getDeltaMs());

#ifdef _PRINT
  writer.update(timer.getDeltaMs());
#endif // _PRINT
}

// See platform.ini 'build_flags' to set experiment
#else 
int potPosition;       //this variable will hold a value based on the position of the potentiometer

void setup()
{
  Serial.begin(9600);       //start a serial connection with the computer
  pinMode(13, OUTPUT);      //set pin 13 as an output that can be set to HIGH or LOW
}

void loop()
{
  //read the position of the pot
  potPosition = analogRead(A0);    //set potPosition to a number between 0 and 1023 based on how far the knob is turned
  Serial.println(potPosition);     //print the value of potPosition in the serial monitor on the computer
  //change the LED blink speed based on the pot value
  digitalWrite(13, HIGH);           // Turn on the LED
  delay(potPosition);              // delay for as many milliseconds as potPosition (0-1023)
  digitalWrite(13, LOW);            // Turn off the LED
  delay(potPosition);              // delay for as many milliseconds as potPosition (0-1023)
}
#endif // C1B_DEFAULT