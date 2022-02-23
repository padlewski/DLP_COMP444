#include <Arduino.h>
#include "controller.h"

void setup() {
  #ifdef _PRINT_
  Serial.begin(9600);
  #endif
  pinMode(switchPin, INPUT_PULLUP);
}

void loop() {
  update();
  calc();
  #ifndef _PRINT_
  move();
  #endif
  #ifdef _PRINT_
  Serial.print("Front: "); Serial.print(sr4Front.cm); Serial.println(" cm");
  Serial.print("Back: "); Serial.print(sr4Back.cm); Serial.println(" cm");
  Serial.print("Following: "); Serial.println(following);
  Serial.print("Error: "); Serial.println(error);
  Serial.print("Speed L R: "); Serial.print(motors.left.speed); Serial.print(" | "); Serial.println(motors.right.speed);
  Serial.println("------------------------");
  delay(3000);
  #endif
}

