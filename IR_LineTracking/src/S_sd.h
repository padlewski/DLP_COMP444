#ifndef S_SD_H
#define S_SD_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

static const byte MAP_BUFF_SZ = 32; // Wire has a 32 bit buffer
static const byte MAP_BUFF[MAP_BUF_SZ] = {0};
static       byte MAP_IDX = 0;

const byte chipSelect = 8;
const byte cardDetect = 9;

void MAP_flush() {
    
}

void MAP_saveIntersection(byte intersection){
    
}

bool initialized = false;  // SD.begin() misbehaves if not first call

void writeToFile(char *fileName, byte* buffer, uint16_t size);
void overwriteToFile(char *fileName, byte* buffer, uint16_t size);

void initializeCard(void) {
  Serial.print(F("Initializing SD card..."));
  // Is there even a card?
  if (!digitalRead(cardDetect))  {
    Serial.println(F("No card detected. Waiting for card."));
    while (!digitalRead(cardDetect));
    delay(250); // 'Debounce insertion'
  }
  if (!SD.begin(chipSelect) && !initialized)   {
    Serial.println(F("Initialization failed!"));
    initializeCard(); // Possible infinite retry loop is as valid as anything
  } else {
    initialized = true;
  }
  Serial.println(F("Initialization done."));
}

void testRead(char *fileName){
  File f = SD.open(fileName);
  if(f){
    Serial.println(fileName);
    // read from the file until there's nothing else in it:
    while (f.available()) {
      Serial.write(f.read());
    }
    Serial.println("");
  }
  // close the file:
  f.close();
}

void writeToFile(char *fileName, byte* buffer, uint16_t size){
  File f = SD.open(fileName, FILE_WRITE);
  f.write(buffer, size);
  f.flush();
  f.close();
}

void overwriteToFile(char *fileName, byte* buffer, uint16_t size) {
  if(SD.exists(fileName)){
    SD.remove(fileName);
  }
  writeToFile(fileName, buffer, size);
}

bool readFromFile(char *fileName, byte* buffer, uint16_t size, uint16_t from, uint16_t *bytesRead){
  if(not SD.exists(fileName)) {
    *bytesRead = 0;
    return true;
  }
  *bytesRead = 0;
  File f = SD.open(fileName, FILE_READ);
  if(from) { // starting from in the file
    uint16_t i;
    while(f.available() && i < from){ //move to start
      f.read();
      ++i;
    }
    while(f.available() && *bytesRead < size) {
      buffer[*bytesRead] = f.read();
      ++*bytesRead;
    }
  } else { // start from beginning
    *bytesRead = f.read(buffer, size);
  }
  f.close();
  return *bytesRead < size;
}

uint32_t getSize(char *fileName) {
  File f = SD.open(fileName, FILE_READ);
  return f.size();
  f.close();
}

#endif // S_SD_H
