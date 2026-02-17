#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Linux implementation of _kbhit
int _kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// Linux implementation of _getch
int _getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

#define MAX_EVENTS 20

// Event Structure (Doubly Linked List Node)
typedef struct Event {
    int id;
    time_t timestamp;
    double power;     // kWh
    double voltage;   // V
    double frequency; // Hz
    char alert[30];   // e.g., "Overvoltage", "Normal"
    struct Event *next;
    struct Event *prev;
} Event;

// Global State
Event *head = NULL;
Event *tail = NULL;
Event *current_cursor = NULL; // For navigation
int event_count = 0;
int next_event_id = 1;
int live_display = 0; // 0 = Paused Display, 1 = Show Live Events

// Function Prototypes
void init_system();
void add_event();
void remove_oldest();
void clear_events();
void display_event(Event *e);
void process_input(char cmd);
void generate_random_event_data(double *p, double *v, double *f, char *a);

// Initialization
void init_system() {
    head = NULL;
    tail = NULL;
    current_cursor = NULL;
    event_count = 0;
    next_event_id = 1;
    live_display = 0; // Start with display paused
    srand((unsigned int)time(NULL));
    printf("System Initialized.\n");
    printf("Events are being collected in the background.\n");
    printf("Commands: r (live display), h (pause display), n (next), p (prev), c (clear), x (exit)\n");
}

// Remove the oldest event (Head of the list)
void remove_oldest() {
    if (head == NULL) return;

    Event *temp = head;
    
    // If the cursor is on the node being removed, move it to the next one
    if (current_cursor == temp) {
        current_cursor = temp->next;
    }

    if (head == tail) {
        head = NULL;
        tail = NULL;
    } else {
        head = head->next;
        if (head) head->prev = NULL;
    }

    free(temp);
    event_count--;
}

// Generate random dummy data
void generate_random_event_data(double *p, double *v, double *f, char *a) {
    *p = (rand() % 500) / 10.0; // 0.0 to 49.9 kWh
    *v = 220.0 + ((rand() % 200) - 100) / 10.0; // 210.0 to 230.0 V
    *f = 50.0 + ((rand() % 10) - 5) / 10.0; // 49.5 to 50.5 Hz
    
    if (*v > 228.0) strcpy(a, "HIGH VOLTAGE");
    else if (*v < 212.0) strcpy(a, "LOW VOLTAGE");
    else strcpy(a, "NORMAL");
}

// Display single event
void display_event(Event *e) {
    if (!e) {
        printf("No Event Selected.\n");
        return;
    }
    char buff[20];
    struct tm * timeinfo = localtime (&e->timestamp);
    strftime(buff, sizeof(buff), "%H:%M:%S", timeinfo);
    
    printf("[ID:%d] %s | P:%.2fkWh V:%.1fV F:%.1fHz | %s\n",
           e->id, buff, e->power, e->voltage, e->frequency, e->alert);
}

// Add a new event to the end (Tail)
void add_event() {
    // 1. Enforce Max Log Size
    if (event_count >= MAX_EVENTS) {
        remove_oldest();
    }

    // 2. Allocate New Event
    Event *new_event = (Event *)malloc(sizeof(Event));
    if (!new_event) {
        printf("Memory Allocation Failed!\n");
        return;
    }

    // 3. Populate Data
    new_event->id = next_event_id++;
    new_event->timestamp = time(NULL);
    generate_random_event_data(&new_event->power, &new_event->voltage, &new_event->frequency, new_event->alert);
    new_event->next = NULL;
    new_event->prev = NULL;

    // 4. Link into List
    if (tail == NULL) {
        head = new_event;
        tail = new_event;
        current_cursor = new_event; // Start cursor at first event if list was empty
    } else {
        tail->next = new_event;
        new_event->prev = tail;
        tail = new_event;
    }
    
    event_count++;

    // If live display is active, print it
    if (live_display) {
        printf("LIVE -> ");
        display_event(new_event);
    }
}

// Clear all events
void clear_events() {
    Event *curr = head;
    while (curr) {
        Event *next = curr->next;
        free(curr);
        curr = next;
    }
    head = NULL;
    tail = NULL;
    current_cursor = NULL;
    event_count = 0;
    printf("All events cleared.\n");
}

// Main Navigation Logic
void process_input(char cmd) {
    switch(cmd) {
        case 'n': // Next (Newer)
            if (current_cursor && current_cursor->next) {
                current_cursor = current_cursor->next;
                printf("NAV [Next] -> ");
                display_event(current_cursor);
            } else {
                printf("NAV: At Newest Event (ID: %d)\n", current_cursor ? current_cursor->id : 0);
            }
            break;
        case 'p': // Prev (Older)
            if (current_cursor && current_cursor->prev) {
                current_cursor = current_cursor->prev;
                printf("NAV [Prev] -> ");
                display_event(current_cursor);
            } else {
                printf("NAV: At Oldest Event (ID: %d)\n", current_cursor ? current_cursor->id : 0);
            }
            break;
        case 'r': // Run (Show Live)
            if (!live_display) {
                live_display = 1;
                printf("--- LIVE DISPLAY STARTED ---\n");
            }
            break;
        case 'h': // Halt/Pause (Hide Live)
            if (live_display) {
                live_display = 0;
                printf("--- LIVE DISPLAY PAUSED (Events still collecting) ---\n");
            }
            break;
        case 'c': // Clear
            clear_events();
            // next_event_id = 1; // Requirement doesn't explicitly Reset ID, but usually good practice? Or not.
            // Let's reset for fresh start feel.
            next_event_id = 1; 
            break;
        case 'x': // Exit
            printf("Terminating system...\n");
            clear_events();
            exit(0);
            break;
    }
}

int main() {
    init_system();
    
    time_t last_t = time(NULL);
    
    while (1) {
        // 1. Periodic Event Generation (Simulated Meter Input)
        // Check time to simulate event arrival every 2 seconds
        time_t now = time(NULL);
        if (now - last_t >= 2) {
            add_event(); // Always add event (Collection is continuous)
            last_t = now;
        }

        // 2. Handle User Input
        if (_kbhit()) {
            char cmd = _getch();
            process_input(cmd);
        }
        
    }
    return 0;
}
