#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main()
{
    char **emails = allocateEmails(INITIAL_STUDENTS);
    if (emails == NULL) {
        fprintf(stderr, "Memory allocation failed for email list.\n");
        return 1;
    }

    printf("Enter email addresses for %d students:\n", INITIAL_STUDENTS);
    readEmails(emails, INITIAL_STUDENTS);

    printf("\nStored Email Addresses:\n");
    displayEmails(emails, INITIAL_STUDENTS);

    /* Shrink the list to 6 students*/
    emails = shrinkEmails(emails, INITIAL_STUDENTS, REDUCED_STUDENTS);
    if (emails == NULL) {
        fprintf(stderr, "Memory reallocation failed.\n");
        return 1;
    }

    printf("\nUpdated Email Addresses (after reduction to %d students):\n", REDUCED_STUDENTS);
    displayEmails(emails, REDUCED_STUDENTS);

    freeEmails(emails, REDUCED_STUDENTS);

    return 0;
}