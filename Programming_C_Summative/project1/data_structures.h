// data_structures.h
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef struct {
    int laneID;
    int vehicleCount;
    unsigned long greenTime;
    unsigned long yellowTime;
    unsigned long redTime;
    int *queue; // dynamic vehicle queue
    int queueSize;
} Lane;

#endif // DATA_STRUCTURES_H