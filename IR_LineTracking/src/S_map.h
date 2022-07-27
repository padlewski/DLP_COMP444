#ifndef S_MAP_H
#define S_MAP_H

#include <Arduino.h>

static const byte PG_SIZE = 0x20; // 32 bytes page size

static int nodeCount        = 0;
static int nodeCurrentIdx   = 0;
static byte nodeCurrentVal  = 0;
static byte nodeOldVal      = 0;

static byte pgCurrent           = 1;
static byte pgCount             = 1;
static byte pgPrevious[PG_SIZE] = {0};
static byte pgCurr[PG_SIZE]     = {0};
static byte pgNext[PG_SIZE]     = {0};

static byte status = 1;

static byte mapSize = 0;

static byte registerCurrent = 0;

void turnPage(void);
void trunBack(void);
void addNode(void);
void updateNode(void);

void processBuffer(const byte[PG_SIZE] buff, const byte size) {
    registerCurrent = buff[0]
    
};

void addNode(byte node) {
    // TODO: Error handling for empty node
    //       and support multiple pages
    nodeCurrentIdx = nodeCount++;
    pgCurr[nodeCurrentIdx] = node;
}

void updateNode(byte node, byte idx) {
    // TODO: Error handling for empty node and overflow
    if(idx >= nodeCount) return 0;
    nodeOldVal = pgCurr[idx];
    pgCurr[idx] = nodeCurrenVal = node;
}

void turnPage() {
    memcpy(pgPrevious, pgCurr, PG_SIZE);
    memcpy(pgCurr, pgNext, PG_SIZE);
}

void turnBack() {
    memcpy(pgNext, pgCurr, PG_SIZE);
    memcpy(pgCurr, pgPrevious, PG_SIZE);
}

#endif // S_MAP_H
