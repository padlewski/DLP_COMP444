#include <Arduino.h>

#ifdef _EXPERIMENT_1_  // Add more colours

#include "writer.hpp"
#include "timer.hpp"
#include "analog.hpp"
#include "rgb_led.hpp"

Timer timer;
Analog photoresistor(A0, 200, "Photoresistor");
Analog potentiometer(A1, 200, "Potentiometer");
#ifdef _PRINT_
Printer* printers[2] =  { new AnalogPrinter(photoresistor), new AnalogPrinter(potentiometer)};
Writer writer(2500, printers, 2);
#endif
RgbLedStrategy* strategies[2] = { new TriggerStrategy(photoresistor.getValue(), 700),
                                  new ColourStrategy(potentiometer.getValue()) };
        // {pin,pin,pin,dim,on,colour,state} dim divisor = 2 or half pwm value                    
RgbLed led({9,10,11,2,false,0,0}, strategies, 2);

void setup() {
  #ifdef _PRINT_
  Serial.begin(9600);
  #endif
}

void loop() {
  timer.tick();
  photoresistor.update(timer.getDeltaMs());
  potentiometer.update(timer.getDeltaMs());
  led.update(timer.getDeltaMs());
  #ifdef _PRINT_
  writer.update(timer.getDeltaMs());
  #endif
}

#elif _EXPERIMENT_2_  // Flash colours

#include "timer.hpp"
#include "rgb_led.hpp"

Timer timer;
RgbLedStrategy* strategies[1] = { new FlashStrategy(1000, 300) };
        // {pin,pin,pin,dim,on,colour,state} dim divisor = 1 full strength                   
RgbLed led({9,10,11,1,false,0,0}, strategies, 1);

void setup() {
  #ifdef _PRINT_
  Serial.begin(9600);
  #endif
}

void loop() {
  timer.tick();
  led.update(timer.getDeltaMs());
}

#elif _EXPERIMENT_3_ // Adjust threashold

#include "writer.hpp"
#include "timer.hpp"
#include "analog.hpp"
#include "rgb_led.hpp"

Timer timer;
Analog photoresistor(A0, 200, "Photoresistor");
Analog potentiometer(A1, 200, "Potentiometer");
#ifdef _PRINT_
Printer* printers[2] =  { new AnalogPrinter(photoresistor), new AnalogPrinter(potentiometer)};
Writer writer(2500, printers, 2);
#endif
RgbLedStrategy* strategies[1] = { new VariableTriggerStrategy(photoresistor.getValue(), potentiometer.getValue())};
        // {pin,pin,pin,dim,on,colour,state} dim divisor = 2 or half pwm value | colour 8 = red                  
RgbLed led({9,10,11,1,false,8,0}, strategies, 1);

void setup() {
  #ifdef _PRINT_
  Serial.begin(9600);
  #endif
}

void loop() {
  timer.tick();
  photoresistor.update(timer.getDeltaMs());
  potentiometer.update(timer.getDeltaMs());
  led.update(timer.getDeltaMs());
  #ifdef _PRINT_
  writer.update(timer.getDeltaMs());
  #endif
}

#elif _EXPERIMENT_4_ // smooth transition

#include "timer.hpp"
#include "rgb_led.hpp"

Timer timer;
RgbLedStrategy* strategies[1] = { new PulseStrategy(100, 50) };
        // {pin,pin,pin,dim,on,colour,state} dim divisor = 1 full strength                   
RgbLed led({9,10,11,1,false,0,0}, strategies, 1);

void setup() {
  #ifdef _PRINT_
  Serial.begin(9600);
  #endif
}

void loop() {
  timer.tick();
  led.update(timer.getDeltaMs());
}

#else
// forward declarations
void red ();
void orange ();
void yellow ();
void green ();
void cyan ();
void blue ();
void magenta ();
void turnOff ();

int photoresistor = A0;          //variable for storing the photoresistor value
int potentiometer = A1;          //this variable will hold a value based on the position of the knob
int threshold = 700;            //if the photoresistor reading is lower than this value the light will turn on

//LEDs are connected to these pins
int RedPin = 9;
int GreenPin = 10;
int BluePin = 11;

void setup() {
  Serial.begin(9600);           //start a serial connection with the computer
  //set the LED pins to output
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
}

void loop() {
  photoresistor = analogRead(A0);         //read the value of the photoresistor
  potentiometer = analogRead(A1);

  Serial.print("Photoresistor value:");
  Serial.print(photoresistor);          //print the photoresistor value to the serial monitor
  Serial.print("  Potentiometer value:");
  Serial.println(potentiometer);          //print the potentiometer value to the serial monitor

  if (photoresistor < threshold) {        //if it's dark (the photoresistor value is below the threshold) turn the LED on
    //These nested if statements check for a variety of ranges and
    //call different functions based on the current potentiometer value.
    //Those functions are found at the bottom of the sketch.
    if (potentiometer > 0 && potentiometer <= 150)
      red();
    if (potentiometer > 150 && potentiometer <= 300)
      orange();
    if (potentiometer > 300 && potentiometer <= 450)
      yellow();
    if (potentiometer > 450 && potentiometer <= 600)
      green();
    if (potentiometer > 600 && potentiometer <= 750)
      cyan();
    if (potentiometer > 750 && potentiometer <= 900)
      blue();
    if (potentiometer > 900)
      magenta();
  }
  else {                                //if it isn't dark turn the LED off
    turnOff();                            //call the turn off function
  }
  delay(100);                             //short delay so that the printout is easier to read
}

void red () {
  //set the LED pins to values that make red
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
}
void orange () {
  //set the LED pins to values that make orange
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 50);
  analogWrite(BluePin, 0);
}
void yellow () {
  //set the LED pins to values that make yellow
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 0);
}
void green () {
  //set the LED pins to values that make green
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 0);
}
void cyan () {
  //set the LED pins to values that make cyan
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 100);
}
void blue () {
  //set the LED pins to values that make blue
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 100);
}
void magenta () {
  //set the LED pins to values that make magenta
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 100);
}
void turnOff () {
  //set all three LED pins to 0 or OFF
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
}
#endif
