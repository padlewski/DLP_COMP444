/***************************************************
* The main file for the I2C slave arduino responsible
* for writing the map and possibly other data to the
* sd card.
***************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "i2c_nano.h"
#include "S_map.h"

void handleRequest(void);
void handleReceive(void);

void setup() {
    Wire.begin(SLAVE_ADDR);         // set address for device
    Wire.onRequest(handleRequest);  // register request handler
    Wire.onReceive(handleReceive);  // register receive handler
}

void loop() {
    delay(50);
}

// Data is being requested
void handleRequest() {
    switch(register) {
        case NANO_MODE_SET:
        case NANO_LAST_IDX:
            Wire.write(nodeCurrentIdx);
            break;
        default:
            Wire.write(B11111111); 
    }
}

// Data is being received
void handleReceive() {
    static buffer[PG_SIZE] = {0};
    static byte i = 0;
    while(Wire.available()) {
        buffer[i++] = Wire.read();
        //TODO: Shouldn't get over 32 bytes, but could handle just in case
    }
    processBuffer(buffer, i);
    for(; i > 0 ;) buffer[--i] = 0;  // clear the buffer
}
