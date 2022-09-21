#ifndef MAP_H
#define MAP_H

#include <Arduino.h>
#include "i2cHelper.h"
#include "i2c_nano.h"

static const byte MAP_START = B1000;
static const byte MAP_END   = B1111;
static byte INTERSECTION_BUFF[3] = {0};

byte MAP_set(const byte *intersection, byte *index) {
    INTERSECTION_BUFF[0] = NANO_NODE_SET;
    INTERSECTION_BUFF[1] = *intersection;
    INTERSECTION_BUFF[2] = *index;
    // TODO: We could handle the end transmisson response
    //       but lets work optimistically
    return writeBytes(&NANO_ADDR, INTERSECTION_BUFF, 3);
}

// adds an intersection to the map and returns the index
byte MAP_add(const byte *intersection) {
    MAP_set(intersection, &NANO_SET_TAIL);
    return readRegister(&NANO_ADDR, &NANO_LAST_IDX);
}

// updates an intersetion to the map and returns the old value
byte MAP_update(const byte *intersection, const byte *index) {
    MAP_set(intersection, *index);
    return readRegister(&NANO_ADDR, &NANO_OLD_NODE);
}

// gets the intersection for the specified index
byte MAP_get(const byte *index) {
    writeByte(&NANO_ADDR, &NANO_NODE_GET, *index);
    return readByte(&NANO_ADDR);
}

#endif // MAP_H
