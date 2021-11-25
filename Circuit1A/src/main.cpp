#include <Arduino.h>
#include "timer.hpp"
#include "led.hpp"
#include "timer_printer.cpp"

int potPosition;
led _led(13, 50, 40000);
timer clock;
timer_printer printer(clock);
writer _writer(2000, printer);

void setup() {
  Serial.begin(9600);
}

void loop() {
  clock.tick();
  _led.update(clock.getDeltaUs());
  _writer.update(clock.getDeltaMs());
}
