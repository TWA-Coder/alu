// vehicle_detection.h
#ifndef VEHICLE_DETECTION_H
#define VEHICLE_DETECTION_H

#include "data_structures.h"

void detectVehicles(Lane *laneA, Lane *laneB);
void addVehicle(Lane *lane);

#endif // VEHICLE_DETECTION_H

// vehicle_detection.cpp
#include "vehicle_detection.h"

void detectVehicles(Lane *laneA, Lane *laneB) {
    if (digitalRead(8) == LOW) { // Sensor A1
        addVehicle(laneA);
    }
    if (digitalRead(9) == LOW) { // Sensor A2
        addVehicle(laneA);
    }
    if (digitalRead(10) == LOW) { // Sensor B1
        addVehicle(laneB);
    }
    if (digitalRead(11) == LOW) { // Sensor B2
        addVehicle(laneB);
    }
}

void addVehicle(Lane *lane) {
    lane->queueSize++;
    lane->queue = (int*) realloc(lane->queue, lane->queueSize * sizeof(int));
    lane->queue[lane->queueSize - 1] = millis(); // Timestamp of vehicle detection
}