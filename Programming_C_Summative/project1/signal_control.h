// signal_control.h
#ifndef SIGNAL_CONTROL_H
#define SIGNAL_CONTROL_H

#include "data_structures.h"

void updateTrafficLights(Lane *laneA, Lane *laneB);
void initializeTrafficLights();
void setSignal(int intersection, const char* color); // 'R', 'Y', 'G'

#endif // SIGNAL_CONTROL_H

// signal_control.cpp
#include "signal_control.h"

void initializeTrafficLights() {
    // Pin setup for LEDs
    pinMode(A2, OUTPUT); // Red A
    pinMode(A3, OUTPUT); // Yellow A
    pinMode(A4, OUTPUT); // Green A
    pinMode(B5, OUTPUT); // Red B
    pinMode(B6, OUTPUT); // Yellow B
    pinMode(B7, OUTPUT); // Green B
}

void setSignal(int intersection, const char* color) {
    // Logic to set signals based on intersection and color
    if (intersection == 1) {
        digitalWrite(A2, color == "R" ? HIGH : LOW);
        digitalWrite(A3, color == "Y" ? HIGH : LOW);
        digitalWrite(A4, color == "G" ? HIGH : LOW);
    } else {
        digitalWrite(B5, color == "R" ? HIGH : LOW);
        digitalWrite(B6, color == "Y" ? HIGH : LOW);
        digitalWrite(B7, color == "G" ? HIGH : LOW);
    }
}

void updateTrafficLights(Lane *laneA, Lane *laneB) {
    // Logic to dynamically update traffic lights based on vehicle presence
    // Implement timing logic and signal state changes
}