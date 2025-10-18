#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


/Allocate memory for 'count' emails/
char **allocateEmails(int count) {
    char *emails = (char *)malloc(count * sizeof(char *));
    if (emails == NULL) return NULL;

    for (int i = 0; i < count; i++) {
        emails[i] = (char *)malloc(MAX_EMAIL_LENGTH * sizeof(char));
        if (emails[i] == NULL) {
            // Free any previously allocated memory if failure occurs
            for (int j = 0; j < i; j++) {
                free(emails[j]);
            }
            free(emails);
            return NULL;
        }
    }

    return emails;
}

/* Reads 'count' email addresses from user */
void readEmails(char **emails, int count) {
    for (int i = 0; i < count; i++) {
        printf("Enter email %d: ", i + 1);
        fgets(emails[i], MAX_EMAIL_LENGTH, stdin);
        
        // Remove newline character if present
        size_t len = strlen(emails[i]);
        if (emails[i][len - 1] == '\n') {
            emails[i][len - 1] = '\0';
        }
    }
}

/* Displays 'count' email addresses*/
void displayEmails(char **emails, int count) {
    for (int i = 0; i < count; i++) {
        printf("Student %d: %s\n", i + 1, emails[i]);
    }
}

/Shrinks the email array from oldCount to newCount/
char **shrinkEmails(char **emails, int oldCount, int newCount) {
    if (newCount > oldCount) return emails; // No need to shrink

    /Free the emails that are no longer needed/
    for (int i = newCount; i < oldCount; i++) {
        free(emails[i]);
    }

    char *resized = (char *)realloc(emails, newCount * sizeof(char *));
    if (resized == NULL) {
        /If realloc fails, original pointer is still valid/
        return NULL;
    }

    return resized;
}

/Frees all memory used by the email array/
void freeEmails(char **emails, int count) {
    for (int i = 0; i < count; i++) {
        free(emails[i]);
    }
    free(emails);
}