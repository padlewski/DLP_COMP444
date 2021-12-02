#include <Arduino.h>
#include "writer.hpp"
#include "timer.hpp"
#include "analog.hpp"
#include "led.hpp"

#ifdef _EXPERIMENT_1_

Timer timer;
Analog photoresistor(A0, 50, "Photoresistor");
LedStrategy* strategies[2] = {
  new TriggerStrategy(photoresistor.getValue(), 500),
  new FlashStrategy(250, 100, 10)
};
Led led({13, 0, 0}, strategies, 2);
Printer* printers[1] = { new AnalogPrinter(photoresistor) };
Writer writer(2500, printers, 1);


void setup() {
  #ifdef _PRINT_
  Serial.begin(9600);
  #endif
  photoresistor.read(); // get inital value
}

void loop() {
  timer.tick();
  photoresistor.update(timer.getDeltaMs());
  led.update(timer.getDeltaMs());
  writer.update(timer.getDeltaMs());
}

#else
int photoresistor = 0;              //this variable will hold a value based on the brightness of the ambient light
int threshold = 300;                //if the photoresistor reading is below this value the the light will turn on

void setup()
{
  Serial.begin(9600);               //start a serial connection with the computer
  pinMode(13, OUTPUT);              //set pin 13 as an output that can be set to HIGH or LOW
}

void loop()
{
  //read the brightness of the ambient light
  photoresistor = analogRead(A0);   //set photoresistor to a number between 0 and 1023 based on how bright the ambient light is
  Serial.println(photoresistor);    //print the value of photoresistor in the serial monitor on the computer

  //if the photoresistor value is below the threshold turn the light on, otherwise turn it off
  if (photoresistor < threshold) {
    digitalWrite(13, HIGH);         // Turn on the LED
  } else {
    digitalWrite(13, LOW);          // Turn off the LED
  }
  delay(100);                       //short delay to make the printout easier to read
}
#endif