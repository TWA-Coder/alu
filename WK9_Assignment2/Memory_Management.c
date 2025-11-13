#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * main - Entry point of the program
 * Return: Always 0 (Success)
*/

int main()
{
    int i, count = 10;
    char buffer[MAX_EMAIL_LEN];
    char **emails;

    emails = (char **)malloc(count * sizeof(char *));
    if (emails == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter 10 student email addresses:\n");
    for (i = 0; i < count; i++) {
        printf("Email %d: ", i + 1);
        scanf("%s", buffer);
        emails[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        if (emails[i] == NULL) {
            printf("Memory allocation failed for email %d!\n", i+1);
            return 1;
        }
        strcpy(emails[i], buffer);
    }

    void (*funcPtr)(char **, int);
    void (*funcPtrRemove)(char **, int *);

    funcPtr = displayEmails;
    funcPtr(emails, count);

    funcPtrRemove = removeDuplicateEmails;
    funcPtrRemove(emails, &count);

    printf("\nAfter removing duplicates:\n");
    funcPtr(emails, count);

    printf("\nShrinking email array from 10 to 6 students \n", count);
    shrinkArray(&emails, 6);
    count = 6;

    printf("After shrinking the array to 6 students:\n");
    shrinkArray(&emails, 6);
    count = 6;

    printf("After shrinking the array to 6 students:\n");
    funcPtr(emails, count);

    freeEmails(emails, count);
    return 0;
}

/**
 * displayEmails - Display the list of email addresses
 * @emails: Array of email addresses
 * @count: Number of email addresses
 *
 * Return: void
 */

 void displayEmails(char **emails, int count)
 {
    printf("\nList of email addresses:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n",i +1, emails[i]);
    }
 }

 /**
  * removeDuplicateEmails - Remove duplicate email addresses from the list
  * @emails: Array of email addresses
  * @count: Pointer to the number of email addresses
  * 
  * Return: void
 */

 void removeDuplicateEmails(char **emails, int *count)
{
    for(int i = 0; i < *count; i++)
    {
        for(int j = i + 1; j < *count; )
        {
            if(strcmp(emails[i], emails[j]) == 0)
            {
                free(emails[j]);
                for(int k = j; k < *count - 1; k++)
                {
                    emails[k] = emails[k + 1];
                }
                (*count)--;
                j--;
        }
    }
}
}

/**
 * shrinkArray - Shrink the email array to a new size
 * @emails: Pointer to the array of email addresses
 * @newSize: New size of the email array
 * 
 * Return: void
 */
void shrinkArray(char ***emails, int newSize)
{
    *emails = (char **)realloc(*emails, newSize * sizeof(char *));
    if(*emails == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}
/**
 * freeEmails - Free the memory allocated for email addresses
 * @emails: Array of email addresses
 * @count: Number of email addresses
 * 
 * Return: void
*/
void freeEmails(char **emails, int count)
{
    for(int i = 0; i < count; i++)
    {
        free(emails[i]);
    }
    free(emails);
}
