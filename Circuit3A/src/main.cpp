#include <Arduino.h>
#include <Servo.h>          //include the servo library

int potPosition;           //this variable will store the position of the potentiometer
int servoPosition;         //the servo will move to this position
int light;

Servo myservo;              //create a servo object


void setup() {
  myservo.attach(9);        //tell the servo object that its servo is plugged into pin 9
#ifdef _PRINT_
  Serial.begin(9600);
#endif
}

void loop() {
  potPosition = analogRead(A0);                     //use analog read to measure the position of the potentiometer (0-1023)
#ifdef _EXPERIMENT_1_
  servoPosition = map(potPosition, 0, 1023, 160, 20); // reverse the pot 
#elif _EXPERIMENT_2_
  servoPosition = map(potPosition, 0, 1023, 60, 120); // small range
#elif _EXPERIMENT_3_
  light = analogRead(A1);
  servoPosition = map(light, 0, 1023, 20, 160);
#ifdef _PRINT_
  Serial.println(light);
#endif
#else
  servoPosition = map(potPosition, 0, 1023, 20, 160); //convert the potentiometer number to a servo position from 20-160
                                                      //Note: its best to avoid driving the little SIK servos all the
                                                      //way to 0 or 180 degrees it can cause the motor to jitter, which is bad for the servo.
#endif
  myservo.write(servoPosition);                      //move the servo to the 10 degree position
}