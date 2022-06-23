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


void doUpdate(void);
void doPrint(void);

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
  IR_coldStart(&lineSensor);
  M_setupMotors();
  doUpdateIr.active = true;
  doUpdateMpu.active = true;
  TMR_tick();
  doUpdateIr.next = &doUpdateMpu;
  doUpdateMpu.next = &update;
  update.next = &doMoveSequence;
  doMoveSequence.next = &doMoveUntil;
  doMoveUntil.next = &print;
  print.next = SU_init(&lineSensor);
  timerState.actions = &doUpdateIr;
  Serial.println("Setup Complete");
}

void loop() {
  TMR_tick();
}

void doUpdate() {
  if(SU_State.mode == STARTUP_STATE_COMPLETE) {
    print.next = NULL;
  }
}

void doPrint() {
  Serial.print("Line Sensor Status: ");
  Serial.println(lineSensor.status, BIN);
  Serial.println("Deg.");Serial.println(IMU_getCompassDegrees());
}