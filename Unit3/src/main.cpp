/**
 * @file main.cpp
 * @brief 
 * @version 0.1
 * @date 2022-01-06
 * @copyright Copyright (c) 2022
 * 
 * Code implementation for COMP444 Unit 3 programming assignment
 * This is to mimic a simple elbow joint using the servo provided in the 
 * SparkFun Inventors Kit (SIK)
 * The user can input a angle of bend into the console and the joint will move
 * to that angle.
 */
#include <Arduino.h>
#include <Servo.h>

const int servoPin = 3; // we should be able to use D3 as a PWM on the RedBoard
Servo elbow;
int flex = 0;

int getAngle(int);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(3000); // I'm a slow typer
  elbow.attach(servoPin);
  Serial.println("Enter an angle for the elbow to flex");
  Serial.println("valid values are form 0 - 170");
  elbow.write(getAngle(flex));
}

void loop() {
  if(Serial.available() > 0){
    flex = Serial.parseInt();
    elbow.write(getAngle(flex));
    Serial.print("Bending to ");
    Serial.println(flex);
    Serial.print("Servo angle: ");
    Serial.println(getAngle(flex));
  }
  delay(300);
}

/**
 * @brief get a mapped angle so as not to jitter the 
 *        servo
 * 
 * @param a the desired angle of flex
 * @return int the servo angle (assume 5 as zeroed) 
 */
int getAngle(int a){
  return map(a, 0, 170, 5, 175);
}