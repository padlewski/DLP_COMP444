#include <Arduino.h>
#include "motors.h"

const int PIN_SWITCH = 2;

void setup() {
  Serial.begin(9600);
  setupMotors();
  pinMode(PIN_SWITCH, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(PIN_SWITCH) == HIGH){ 
    //Switch is off stop
    move(FORWARD, 0);
    Serial.println("OFF");
    delay(1000);
    return;
  }
  Serial.println("ON");
  move(FORWARD, 100);
  delay(1000);
  move(BACK, 100);
  delay(2000);
  move(FORWARD, 100);
  delay(1000);
  move(RIGHT, 100);
  delay(1000);
  move(LEFT, 100);
  delay(2000);
  move(RIGHT, 100);
  delay(1000);
  move(CLOCKWISE, 100);
  delay(3000);
  move(COUNTER, 100);
  delay(3000);
  move(FORWARD, 0);
  delay(10000);
}