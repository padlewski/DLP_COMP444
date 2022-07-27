#include <Arduino.h>
#include "line_follow.h"

void printDebug(void);

// struct TimedActionMs update = TMR_buildActionMs("update", 23, &doUpdateIr, true);
#ifdef _PRINT_
struct TimedActionMs doPrint = TMR_buildActionMs("print", 3000, &printDebug, true);
#endif

#ifdef _MASTER_
static const byte SLAVE_ADDR = B10011;
void initSlave(void);
#endif

// struct TimedActionMs move = TMR_buildActionMs("move", 2000, &doMove, true);

void setup() {
  randomSeed(analogRead(A5));
  #ifdef _PRINT_
  Serial.begin(9600);
  Serial.println("Starting");
  #endif
  Wire.begin(); // setup the I2C connection
  pinMode(switchPin, INPUT);
  IR_init();
  IMU_Init();
  delay(1000);
  IR_coldStart();
  M_setupMotors();
  doUpdateIr.active = true;
  doUpdateMpu.active = true;
  doCheckSwitch.active = true;
  doUpdateIr.next = &doUpdateMpu;
  #ifdef _PRINT_
  doUpdateMpu.next = &doPrint;
  doPrint.next = &doCheckSwitch;
  #else
  doUpdateMpu.next = &doCheckSwitch;
  #endif
  doCheckSwitch.next = &doMoveSequence;
  doMoveSequence.next = &doMoveUntil;
  doMoveUntil.next = &doMonitorState;
  TMR_tick();
  timerState.actions = &doUpdateIr;
  #ifdef _PRINT_
  Serial.println("Setup Complete");
  #endif
}

void loop() {
  TMR_tick();
}

#ifdef _PRINT_
void printDebug(void) {
  Serial.print("BotState: ");
  Serial.println(botState.currentState, BIN);
  //   Serial.print("Raw: ");
  // for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
  //   Serial.print(lineSensor.raw[i]);
  //   Serial.print(" | ");
  // }
  // Serial.println(" ");
  // Serial.print("Min: ");
  // for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
  //   Serial.print(lineSensor.min[i]);
  //   Serial.print(" | ");
  // }
  // Serial.println(" ");
  // Serial.print("Max: ");
  // for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
  //   Serial.print(lineSensor.max[i]);
  //   Serial.print(" | ");
  // }
  // Serial.println(" ");
  // Serial.print("Thesh: ");
  // for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
  //   Serial.print(lineSensor.threshold[i]);
  //   Serial.print(" | ");
  // }
  // Serial.println(" ");
  // Serial.print("Norm: ");
  // for (uint8_t i = 0 ; i < IR_NUM_SENSORS ; ++i) {
  //   Serial.print(lineSensor.normalized[i]);
  //   Serial.print(" | ");
  // }
  // Serial.println(" ");
  // if(lineSensor.hiMode) Serial.println("Hi Mode");
  // else Serial.println("LowMode");

  // Serial.println("Deg.");Serial.println(IMU_getCompassDegrees());
  Serial.println("ir");
  Serial.println(lineSensor.status, BIN);
  Serial.println(IR_leftOrRight(&lineSensor.status));
  // if(IR_isOffLine(&lineSensor.status)) Serial.println("OffLine");
}
#endif

#ifdef _MASTER_
void initSlave(void){
  
}
#endif
