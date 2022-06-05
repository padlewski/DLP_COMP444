#include <Arduino.h>
#include "ir_line.h"
#include "timer.h"
//#include "line_follow.h"
#include "mecanum.h"
#include "WaveshareIMU.h"
#include "startup.h"

struct BotState {
  byte current;
  byte previous;
} botState;



uint8_t irInputs[NUM_SENSORS] = {A0,A1,A2,A3,A4};
IrLineSensor lineSensor = initIrLineSensor(irInputs);
byte previousDir;
byte currentDir;
byte previousSpeed;
byte currentSpeed;

void doUpdateState();
void doPrint();
void doMove();
void doUpdateIr() {
  updateIrLineSensor(&lineSensor);
}
void doMagCalibration();

struct TimedActionMs update = buildActionMs("update", 23, &doUpdateIr, true);
struct TimedActionMs print = buildActionMs("print", 3000, &doPrint, true);
struct TimedActionMs move = buildActionMs("move", 2000, &doMove, true);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  TMR_tick();
  IR_coldStart(&lineSensor);
  M_setupMotors();
  update.next = &print;
  print.next = &move;
  timerState.actions = &update;
  previousDir = currentDir = 0;
  previousSpeed = currentSpeed = 0;
  M_move(M_getDirection(currentDir), currentSpeed); // stop
  currentSpeed = 150;
}

void loop() {
  TMR_tick();
}

void doUpdateState() {

}

void doMove() {
  previousDir = currentDir;
  if(currentDir == 2) currentDir = 3;
  else currentDir = 2;
  M_move(M_getDirection(currentDir), currentSpeed);
}


void doPrint() {
  Serial.println(lineSensor.status, BIN);
  Serial.print("Raw: ");
  for (uint8_t i = 0 ; i < NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.raw[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Min: ");
  for (uint8_t i = 0 ; i < NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.min[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Max: ");
  for (uint8_t i = 0 ; i < NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.max[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Thesh: ");
  for (uint8_t i = 0 ; i < NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.threshold[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Norm: ");
  for (uint8_t i = 0 ; i < NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.normalized[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  if(lineSensor.hiMode) Serial.println("Hi Mode");
  else Serial.println("LowMode");
}