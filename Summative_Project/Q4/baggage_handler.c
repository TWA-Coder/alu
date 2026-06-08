#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CAPACITY 5
#define TOTAL_LUGGAGE 20

// --- SHARED MEMORY / STATE ---
int conveyor_belt[MAX_CAPACITY];
int belt_count = 0;              // Current number of items on the belt
int belt_head = 0;               // Index where Consumer removes items (out)
int belt_tail = 0;               // Index where Producer inserts items (in)

int total_loaded = 0;            // Total luggage placed onto the conveyor belt
int total_dispatched = 0;        // Total luggage transferred into the aircraft

int is_system_running = 1;       // Simulation termination flag

// --- SYNCHRONIZATION PRIMITIVES ---
// Mutex to protect all shared variables listed above against race conditions.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition Variables
// 'cond_space_available' signals the Loader when the belt is no longer full
pthread_cond_t cond_space_available = PTHREAD_COND_INITIALIZER;
// 'cond_item_available' signals the Aircraft when the belt is no longer empty
pthread_cond_t cond_item_available = PTHREAD_COND_INITIALIZER;

/*
 * CONVEYOR BELT LOADER (Producer Thread)
 * --------------------------------------
 * Responsible for continuously loading luggage onto the conveyor belt. 
 * Must wait if belt_count reaches MAX_CAPACITY.
 */
void* conveyor_loader_func(void* arg) {
    for (int i = 1; i <= TOTAL_LUGGAGE; i++) {
        // Simulating the time it takes to physically gather luggage (2 seconds)
        sleep(2);

        // Enter Critical Section
        pthread_mutex_lock(&mutex);

        // Checking Full Belt Condition: 
        // We use a 'while' loop rather than 'if' to protect against spurious wakeups.
        while (belt_count == MAX_CAPACITY) {
            printf("[Loader] Belt is FULL (5/5). Waiting for space...\n");
            // Unlocks mutex, waits for space signal, then securely re-locks on wakeup
            pthread_cond_wait(&cond_space_available, &mutex);
        }

        // Insert Item into Belt Ring Buffer
        conveyor_belt[belt_tail] = i;        // Store luggage ID
        belt_tail = (belt_tail + 1) % MAX_CAPACITY;
        belt_count++;
        total_loaded++;

        printf("[Loader] Loaded Luggage ID: %d onto belt. (Belt Size: %d/%d)\n", 
               i, belt_count, MAX_CAPACITY);

        // Signal Aircraft Loader that the belt is no longer empty
        pthread_cond_signal(&cond_item_available);

        // Exit Critical Section
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/*
 * AIRCRAFT LOADER (Consumer Thread)
 * ---------------------------------
 * Responsible for removing luggage from the belt and placing it inside the aircraft.
 * Must wait if belt_count is purely 0.
 */
void* aircraft_loader_func(void* arg) {
    while (1) {
        // Enter Critical Section
        pthread_mutex_lock(&mutex);

        // Check if simulation ended gracefully
        if (total_dispatched == TOTAL_LUGGAGE) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Checking Empty Belt Condition:
        while (belt_count == 0) {
            // Unlocks mutex, waits for item signal, then securely re-locks on wakeup
            pthread_cond_wait(&cond_item_available, &mutex);
        }

        // Remove Item from Belt Ring Buffer
        int luggage_id = conveyor_belt[belt_head];
        belt_head = (belt_head + 1) % MAX_CAPACITY;
        belt_count--;

        printf("[Aircraft] Removed Luggage ID: %d from belt. (Belt Size: %d/%d)\n", 
               luggage_id, belt_count, MAX_CAPACITY);

        // Signal Conveyor Loader that space has officially opened up
        pthread_cond_signal(&cond_space_available);

        // Exit Critical Section to prevent bottlenecking while loading plane!
        pthread_mutex_unlock(&mutex);

        // Simulating the time it takes to place item properly into aircraft hold (4 seconds)
        // ** Important: We run `sleep` OUTSIDE the mutex lock to prevent starving the producer.
        sleep(4);

        // Re-lock briefly purely to update statistical reporting counters securely
        pthread_mutex_lock(&mutex);
        total_dispatched++;
        printf("[Aircraft] Successfully secured Luggage ID: %d into Aircraft hold.\n", luggage_id);
        
        if (total_dispatched == TOTAL_LUGGAGE) {
            is_system_running = 0; // Terminate condition for monitor
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/*
 * MONITORING THREAD
 * -----------------
 * Checks in every 5 seconds to deliver statistical health tracking safely.
 */
void* monitoring_func(void* arg) {
    while (1) {
        sleep(5); // Periodically check every 5 seconds
        
        // Safely lock immediately before copying Shared Primitive States
        pthread_mutex_lock(&mutex);
        
        if (is_system_running == 0) {
            pthread_mutex_unlock(&mutex);
            break; 
        }

        printf("\n--- [MONITOR REPORT] ---\n");
        printf("Total Luggage Loaded to belt so far:    %d\n", total_loaded);
        printf("Total Luggage Dispatched to aircraft:   %d\n", total_dispatched);
        printf("Current Conveyor Belt Size Utilization: %d/%d\n", belt_count, MAX_CAPACITY);
        printf("------------------------\n\n");

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    printf("=== Starting Airport Baggage Handling Simulation! ===\n\n");

    pthread_t loader_thread, aircraft_thread, monitor_thread;

    // Booting up the respective threads mapped to their routines
    pthread_create(&loader_thread, NULL, conveyor_loader_func, NULL);
    pthread_create(&aircraft_thread, NULL, aircraft_loader_func, NULL);
    pthread_create(&monitor_thread, NULL, monitoring_func, NULL);

    // Waiting for final routines to cleanly finish terminating internally
    pthread_join(loader_thread, NULL);
    pthread_join(aircraft_thread, NULL);
    pthread_join(monitor_thread, NULL);

    // Destroy synchronizing constraints as cleanup 
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_space_available);
    pthread_cond_destroy(&cond_item_available);

    printf("\n=== Simulation Complete: All %d luggage items successfully loaded into aircraft! ===\n", TOTAL_LUGGAGE);
    return 0;
}
