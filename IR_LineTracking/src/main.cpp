#include <Arduino.h>
#include "ir_line.h"
#include "timer.h"
//#include "line_follow.h"
#include "mecanum.h"
#include "WaveshareIMU.h"
#include "startup.h"

//static const int I2C_RATE = 115200;

struct BotState {
  byte current;
  byte previous;
} botState;

uint8_t irInputs[IR_NUM_SENSORS] = {A0,A1,A2,A3,A4};
IrLineSensor lineSensor = IR_init(irInputs);
// byte previousDir;
// byte currentDir;
// byte previousSpeed;
// byte currentSpeed;

void doUpdate();
void doPrint();
// void doMove();
// void doUpdateIr() {
//   IR_update(&lineSensor);
// }

// struct TimedActionMs update = TMR_buildActionMs("update", 23, &doUpdateIr, true);
struct TimedActionMs print = TMR_buildActionMs("print", 3000, &doPrint, true);
struct TimedActionMs update = TMR_buildActionMs("update", 20, &doUpdate, true);

// struct TimedActionMs move = TMR_buildActionMs("move", 2000, &doMove, true);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  Wire.begin(); // setup the I2C connection
  IMU_Init();
  delay(1000);
  TMR_tick();
  IR_coldStart(&lineSensor);
  M_setupMotors();
  update.next = &print;
  print.next = SU_init(&lineSensor);
  timerState.actions = &update;
  Serial.println("Setup Complete");
}


void loop() {
  TMR_tick();
//   M_move(M_getDirection(Left), STARTUP_SPEEDS);
//   delay(1000);
//   M_move(M_getDirection(Right), STARTUP_SPEEDS);
//   delay(1000);
}

void doUpdate() {
  if(SU_State.mode == STARTUP_STATE_COMPLETE) print.next = NULL;
}

// void doMove() {
//   previousDir = currentDir;
//   if(currentDir == 2) currentDir = 3;
//   else currentDir = 2;
//   M_move(M_getDirection(currentDir), currentSpeed);
// }


void doPrint() {
  Serial.println(lineSensor.status, BIN);
  Serial.print("Raw: ");
  for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.raw[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Min: ");
  for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.min[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Max: ");
  for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.max[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Thesh: ");
  for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.threshold[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  Serial.print("Norm: ");
  for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
    Serial.print(lineSensor.normalized[i]);
    Serial.print(" | ");
  }
  Serial.println(" ");
  if(lineSensor.hiMode) Serial.println("Hi Mode");
  else Serial.println("LowMode");
  Serial.println("Deg.");Serial.println(getCompassDegrees());
}