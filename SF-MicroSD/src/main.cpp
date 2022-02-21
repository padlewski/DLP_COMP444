#include <Arduino.h>
#include "disk.h"
#include "ir_10-150cm.h"
#include "ir_pololu_prox.h"

IR ir = init(A0, 5);
byte run = 1;
const char eol[2] = "\n";
char f_runs[] = "runs.txt";
char f_test[] = "test.txt";
char f_log[] = "data.log";
byte idx = 0;
static const byte MAX = B11111111; // 255
byte buff[MAX];
ProximitySensor prox = init(DD6, DD5, true);

void updateBuffer(void);
void setRuns(void);
void dumpBuffer(void);

void setup() {
  Serial.begin(57600);
  while (!Serial);  // Wait for serial port to connect
  // The SD library takes care of setting SS_PIN as an output.
  pinMode(cardDetect, INPUT);
  initializeCard();
  setRuns();
  testWrite(f_test);
}

void loop() {
  read(&ir);
  if(ir.cm < 20) setOff(&prox);
  else setOn(&prox);
  testRead(f_test);
  Serial.print("Run #: "); Serial.println(run);
  Serial.print("IR: ");Serial.print(ir.volts);Serial.print("v | ");Serial.print(ir.cm);Serial.println(" cm");
  update(&prox);
  Serial.println(prox.blocked ? "Blocked" : "Open");
  updateBuffer();
  delay(2500);
}

void updateBuffer(void) {
  static const size_t writeSize = 1 + sizeof(ir.volts) + sizeof(ir.cm) + sizeof(eol);
  if((idx + writeSize) >= MAX) dumpBuffer();
  buff[idx++] = run;
  size_t size = sizeof(ir.volts);
  memcpy(buff, (uint8_t*) &ir.volts, size);
  idx += size;
  size = sizeof(ir.cm);
  memcpy(buff, (uint8_t*) &ir.cm, size);
  idx += size;
  memcpy(buff, eol, 2);
  Serial.print("Buffer size: ");
  Serial.print(idx);
  Serial.print(" | sizeof: ");
  Serial.println(sizeof(buff));
}

void setRuns(void){
  byte buf[] = {0};
  if(SD.exists(f_runs)){
    uint16_t bytesRead;
    readFromFile(f_runs, buf, 1, 0, &bytesRead);
    if(bytesRead > 0){
      run += buf[0];
    }
  }
  buf[0] = run;
  overwriteToFile(f_runs, buf, 1);
}

void dumpBuffer(void) {
  Serial.println("Writeing to File");
  writeToFile(f_log, buff, idx);
  idx = 0;
  Serial.print("File in bytes: ");
  Serial.println(getSize(f_log));
}
