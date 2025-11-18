#include "data_structures.h"
#include "signal_control.h"
#include "vehicle_detection.h"
#include "logging.h"
#include "menu_system.h"

Lane laneA;
Lane laneB;

void setup() {
    // Initialize lanes
    laneA.laneID = 1;
    laneA.vehicleCount = 0;
    laneA.queueSize = 0;
    laneA.queue = NULL;

    laneB.laneID = 2;
    laneB.vehicleCount = 0;
    laneB.queueSize = 0;
    laneB.queue = NULL;

    initializeTrafficLights();
    initializeLogging();
}

void loop() {
    detectVehicles(&laneA, &laneB);
    updateTrafficLights(&laneA, &laneB);
    logData(&laneA, &laneB);
    handleMenu();
    delay(100); // Adjust as needed
}