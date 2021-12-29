#include <Arduino.h>

const int trigPin = 11;           //connects to the trigger pin on the distance sensor
const int echoPin = 12;           //connects to the echo pin on the distance sensor

const int redPin = 3;             //pin to control the red LED inside the RGB LED
const int greenPin = 5;           //pin to control the green LED inside the RGB LED
const int bluePin = 6;            //pin to control the blue LED inside the RGB LED

float distance = 0;               //stores the distance measured by the distance sensor
float INCHES = 148.0;
float CM = 58.0;

float getDistance();

void setup()
{
  #ifdef _PRINT_
  Serial.begin (9600);        //set up a serial connection with the computer
  #endif

  pinMode(trigPin, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  //set the RGB LED pins to output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

#if _EXPERIMENT_1_

void loop() {
  distance = getDistance();   //variable to store the distance measured by the sensor

#ifdef _PRINT_
  Serial.print(distance);     //print the distance that was measured
  Serial.println(" in");      //print units after the distance
#endif

  if (distance <= 12) {
    //make the RGB LED red
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else if (12 < distance && distance < 36) { 
    //make the RGB LED yellow
    analogWrite(redPin, 255);
    analogWrite(greenPin, 50);
    analogWrite(bluePin, 0);
  } else { 
    //make the RGB LED green
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
  delay(100);      //delay 50ms between each reading
}

#elif _EXPERIMENT_2_

void loop() {
  distance = getDistance(); 
#ifdef _PRINT_
  Serial.print(distance); 
  Serial.println(" cm"); 
#endif
  if (distance <= 10) {
    //make the RGB LED red
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else if (10 < distance && distance < 45) { 
    //make the RGB LED yellow
    analogWrite(redPin, 255);
    analogWrite(greenPin, 50);
    analogWrite(bluePin, 0);
  } else { 
    //make the RGB LED green
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
  delay(100);      //delay 50ms between each reading
}

#elif _EXPERIMENT_3_

void loop() {
  distance = getDistance(); 
#ifdef _PRINT_
  Serial.print(distance); 
  Serial.println(" cm"); 
#endif
  if (distance < 10) {
    //make the RGB LED red
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else if (distance < 30) {
    //make the RGB LED orange
    analogWrite(redPin, 255);
    analogWrite(greenPin, 128);
    analogWrite(bluePin, 0);
  } else if (distance < 50) { 
    //make the RGB LED yellow
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  } else if (distance < 100) {
    //make the RGB LED blue
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
  } else { 
    //make the RGB LED green
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
  delay(100);      //delay 50ms between each reading
}

#else

void loop() {
  distance = getDistance();   //variable to store the distance measured by the sensor

  Serial.print(distance);     //print the distance that was measured
  Serial.println(" in");      //print units after the distance

  if (distance <= 10) {                       //if the object is close

    //make the RGB LED red
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

  } else if (10 < distance && distance < 20) { //if the object is a medium distance

    //make the RGB LED yellow
    analogWrite(redPin, 255);
    analogWrite(greenPin, 50);
    analogWrite(bluePin, 0);

  } else {                                    //if the object is far away

    //make the RGB LED green
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }

  delay(50);      //delay 50ms between each reading
}

//------------------FUNCTIONS-------------------------------
#endif

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance() {
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor
#if defined _EXPERIMENT_2_  || defined _EXPERIMENT_3_
  calculatedDistance = echoTime / CM;
#else
  calculatedDistance = echoTime / INCHES;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
#endif
  return calculatedDistance;              //send back the distance that was calculated
}