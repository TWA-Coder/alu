#include "attendance_system.h"

Block* create_genesis_block() {
    Block* genesis = (Block*)malloc(sizeof(Block));
    genesis->index = 0;
    genesis->timestamp = time(NULL);
    strcpy(genesis->student_id, "SYSTEM");
    strcpy(genesis->full_name, "Genesis Block");
    strcpy(genesis->course_code, "N/A");
    strcpy(genesis->status, "N/A");
    
    // 64 zeros for previous hash
    memset(genesis->previous_hash, '0', 64);
    genesis->previous_hash[64] = '\0';
    
    genesis->sig_len = 0; // Genesis block isn't typically signed by a user
    
    compute_hash(genesis, genesis->hash);
    genesis->next = NULL;
    
    return genesis;
}

Block* mark_attendance(Block* last_block, const char* student_id, const char* status) {
    Student* st = find_student(student_id);
    if (!st) {
        printf("ERROR: Student ID '%s' not found in registry.\n", student_id);
        return NULL;
    }
    
    Block* new_block = (Block*)malloc(sizeof(Block));
    new_block->index = last_block->index + 1;
    new_block->timestamp = time(NULL);
    
    strcpy(new_block->student_id, st->student_id);
    strcpy(new_block->full_name, st->full_name);
    strcpy(new_block->course_code, st->course_code);
    strcpy(new_block->status, status);
    strcpy(new_block->previous_hash, last_block->hash);
    
    // Sign the block
    if (!sign_block(new_block)) {
        printf("ERROR: Failed to sign the block.\n");
        free(new_block);
        return NULL;
    }
    
    // Compute hash
    compute_hash(new_block, new_block->hash);
    new_block->next = NULL;
    
    last_block->next = new_block;
    printf("SUCCESS: Attendance marked for %s (%s) - Block #%d created.\n", st->full_name, st->student_id, new_block->index);
    return new_block;
}

int validate_chain(Block* genesis) {
    Block* current = genesis;
    Block* previous = NULL;
    
    printf("\n--- Validating Blockchain ---\n");
    
    while (current != NULL) {
        // Validate Hash
        char calculated_hash[65];
        compute_hash(current, calculated_hash);
        if (strcmp(current->hash, calculated_hash) != 0) {
            printf("VALIDATION FAILED: Block #%d hash is invalid!\n", current->index);
            printf("Expected: %s\nCalculated: %s\n", current->hash, calculated_hash);
            return 0;
        }
        
        // Validate Link
        if (previous != NULL) {
            if (strcmp(current->previous_hash, previous->hash) != 0) {
                printf("VALIDATION FAILED: Block #%d previous_hash does not match Block #%d hash!\n", current->index, previous->index);
                return 0;
            }
        }
        
        // Validate Signature
        if (current->index > 0) {
            if (!verify_signature(current)) {
                printf("VALIDATION FAILED: Block #%d digital signature is invalid!\n", current->index);
                return 0;
            }
        }
        
        previous = current;
        current = current->next;
    }
    
    printf("VALIDATION SUCCESS: Blockchain is fully valid and intact.\n");
    return 1;
}

void view_records(Block* genesis) {
    Block* current = genesis;
    printf("\n--- Attendance Records (Blockchain) ---\n");
    while (current != NULL) {
        char time_str[26];
        ctime_r(&current->timestamp, time_str);
        time_str[24] = '\0'; // Remove newline
        
        printf("Block #%d | Time: %s | ID: %s | Name: %s | Course: %s | Status: %s\n",
               current->index, time_str, current->student_id, current->full_name, current->course_code, current->status);
        printf("  Prev Hash: %s...\n", current->previous_hash);
        printf("  Hash:      %s...\n", current->hash);
        printf("  Signature Validity: %s\n\n", (current->index == 0) ? "N/A (Genesis)" : (verify_signature(current) ? "Valid" : "INVALID"));
        
        current = current->next;
    }
}

void tamper_block(Block* genesis, int block_index, const char* new_status) {
    Block* current = genesis;
    while (current != NULL && current->index != block_index) {
        current = current->next;
    }
    
    if (current == NULL) {
        printf("ERROR: Block #%d not found.\n", block_index);
        return;
    }
    
    if (current->index == 0) {
        printf("ERROR: Cannot tamper with Genesis block for this demo.\n");
        return;
    }
    
    printf("\n--- Tampering with Block #%d ---\n", block_index);
    printf("Old Status: %s\n", current->status);
    strcpy(current->status, new_status);
    printf("New Status: %s\n", current->status);
    printf("NOTE: Hashes and signatures were NOT recalculated to demonstrate tampering detection.\n");
}

void free_chain(Block* genesis) {
    Block* current = genesis;
    while (current != NULL) {
        Block* temp = current;
        current = current->next;
        free(temp);
    }
}
