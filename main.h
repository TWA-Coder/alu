#ifndef MAIN_H
#define MAIN_H
#define MAX_EMAIL_LENGTH 50
#define INITIAL_STUDENTS 10
#define REDUCED_STUDENTS 6
char **allocateEmails(int count);
void readEmails(char **emails, int count);
void displayEmails(char **emails, int count);
char **shrinkEmails(char **emails, int oldCount, int newCount);
void freeEmails(char **emails, int count);

#endif