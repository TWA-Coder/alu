#ifndef ATTENDANCE_SYSTEM_H
#define ATTENDANCE_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>

#define MAX_STUDENTS 100

typedef struct {
    char student_id[20];
    char full_name[50];
    char course_code[10];
} Student;

typedef struct Block {
    int index;
    time_t timestamp;
    char student_id[20];
    char full_name[50];
    char course_code[10];
    char status[10];
    char previous_hash[65];
    unsigned char signature[72]; // ECDSA signature (DER encoded usually up to ~72 bytes)
    size_t sig_len; // Needed to track actual signature length
    char hash[65];
    struct Block* next;
} Block;

// Global Registry
extern Student registry[MAX_STUDENTS];
extern int num_students;

// Global Keypair for signing blocks
extern EVP_PKEY *keypair;

// --- registry.c ---
int load_registry(const char* filename);
Student* find_student(const char* student_id);

// --- crypto.c ---
void generate_keypair();
void free_keypair();
void compute_hash(Block* block, char* output_hash);
int sign_block(Block* block);
int verify_signature(Block* block);

// --- blockchain.c ---
Block* create_genesis_block();
Block* mark_attendance(Block* last_block, const char* student_id, const char* status);
int validate_chain(Block* genesis);
void view_records(Block* genesis);
void tamper_block(Block* genesis, int block_index, const char* new_status);
void free_chain(Block* genesis);

#endif
