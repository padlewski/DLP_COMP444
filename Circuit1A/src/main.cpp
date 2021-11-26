#include <Arduino.h>
#include "Timer.hpp"
#include "Led.hpp"
#include "TimerPrinter.cpp"

//Led led(13, 10); // LED starts to flicker Onboard ok
//Led led(13, 12); // Onboard LED starts to flicker
Led led(13, 10, 10000);
Timer clock;
TimerPrinter printer(clock);
Writer writer(2000, printer);

void setup() {
  Serial.begin(9600);
}

void loop() {
  clock.tick();
  led.update(clock.getDeltaUs());
  writer.update(clock.getDeltaMs());
}
