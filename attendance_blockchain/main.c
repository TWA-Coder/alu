#include "attendance_system.h"

int main() {
    printf("Initializing Blockchain Attendance System...\n");
    
    // 1. Generate Keypair for the session
    generate_keypair();
    if (!keypair) {
        printf("CRITICAL ERROR: Failed to generate ECDSA keypair.\n");
        return 1;
    }
    
    // 2. Load Registry
    if (!load_registry("students.txt")) {
        return 1;
    }
    printf("Loaded %d students into registry.\n", num_students);
    
    // 3. Initialize Blockchain with Genesis Block
    Block* genesis = create_genesis_block();
    Block* tail = genesis;
    printf("Genesis Block created.\n\n");
    
    // 4. CLI Loop
    int choice;
    char id_input[30];
    char status_input[20];
    int block_num;
    
    while (1) {
        printf("==============================\n");
        printf("  Blockchain Attendance System\n");
        printf("==============================\n");
        printf("1. Mark Attendance\n");
        printf("2. View Records\n");
        printf("3. Validate Blockchain\n");
        printf("4. Tamper with Block (Demo)\n");
        printf("5. Exit\n");
        printf("Select an option: ");
        
        if (scanf("%d", &choice) != 1) {
            // clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Invalid input.\n\n");
            continue;
        }
        
        switch(choice) {
            case 1:
                printf("Enter Student ID: ");
                scanf("%20s", id_input);
                printf("Enter Status (PRESENT, ABSENT, LATE): ");
                scanf("%10s", status_input);
                
                Block* new_block = mark_attendance(tail, id_input, status_input);
                if (new_block) {
                    tail = new_block;
                }
                break;
                
            case 2:
                view_records(genesis);
                break;
                
            case 3:
                validate_chain(genesis);
                break;
                
            case 4:
                printf("Enter Block Index to tamper: ");
                scanf("%d", &block_num);
                printf("Enter new forged status: ");
                scanf("%10s", status_input);
                tamper_block(genesis, block_num, status_input);
                break;
                
            case 5:
                printf("Exiting... Cleaning up memory.\n");
                free_chain(genesis);
                free_keypair();
                return 0;
                
            default:
                printf("Invalid option. Try again.\n");
        }
        printf("\n");
    }
    
    return 0;
}
