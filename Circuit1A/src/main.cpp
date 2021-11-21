#include <Arduino.h>

int potPosition;
const unsigned int tickThrottle = 10;
unsigned int tickStatsLapse = 0;
const unsigned int tickStatsReset = 30000;
unsigned long previousTimeMs = 0;
unsigned long previousTimeUs = 0;
unsigned int timeMsDelta = 0;
unsigned int minMsDelta = 8888888;
unsigned int maxMsDelta = 0;
unsigned int timeUsDelta = 0;
bool ledOn = false;
const unsigned int ledIntervalMs = 2000;
unsigned int ledLapseMs = 0;
const unsigned int writeIntervalMs = 1000;
unsigned int writeLapseMs = 0;

void calcTickStats() {
  if(minMsDelta > timeMsDelta) minMsDelta = timeMsDelta;
  if(maxMsDelta < timeMsDelta) maxMsDelta = timeMsDelta;
  tickStatsLapse += timeMsDelta;
  if(tickStatsLapse < tickStatsReset) return;
  tickStatsLapse -= tickStatsReset;
  minMsDelta = timeMsDelta;
  maxMsDelta = timeMsDelta;
}

void tick() {
  unsigned long timeMs = millis();
  // unsigned long timeUs = micros();
  // TODO: Handle overflow
  timeMsDelta = (unsigned int)(timeMs - previousTimeMs);
  // timeUsDelta = (unsigned int)(timeUs - previousTimeUs);
  previousTimeMs = timeMs;
  // previousTimeUs = timeUs;
  calcTickStats();
  //delay(tickThrottle);
}

void updateWrite() {
  writeLapseMs += timeMsDelta;
  if (writeLapseMs < writeIntervalMs) return; // not time to update yet
  writeLapseMs -= writeIntervalMs;
  Serial.print("previousTimeMs: ");
  Serial.println(previousTimeMs);
  // Serial.print("previousTimeUs: ");
  // Serial.println(previousTimeUs);
  Serial.print("minMsDelta: ");
  Serial.print(minMsDelta);
  Serial.print("  | maxMsDelta: ");
  Serial.println(maxMsDelta);
  Serial.print("  | last timeMsDelta: ");
  Serial.println(timeMsDelta);
  Serial.println(ledOn ? "LED State: HIGH" : "LED State: LOW");
}

void updateLed() {
  ledLapseMs += timeMsDelta;
  if (ledLapseMs < ledIntervalMs) return; // not time to update yet
  ledLapseMs -= ledIntervalMs;            // reset lapse
  if(ledOn) { 
    digitalWrite(13, LOW);
    ledOn = false;
  } else {
    digitalWrite(13, HIGH);
    ledOn = true;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  tick();
  updateLed();
  updateWrite();
}
