#ifndef MAIN_H
#define MAIN_H

void tax_on_salary(float *gross, float *tax);
void medication_insurance(float *gross, float *medInsurance);
void maternity_leave(float *gross, float *maternity);
void social_security_fund(float *gross, float *social);
void net_salary_calculator(float *gross, float *net);
// Sorting names function prototypes
#define SIZE 20
#define NAME_LENGTH 100

int ascending(const char *a, const char *b);
int descending(const char *a, const char *b);
void bubble_sort(char names[SIZE][NAME_LENGTH], int(*compare)(const char *, const char *));

//memory management function prototypes
#define MAX_EMAIL_LEN 100
void displayEmails(char **emails, int count);
void removeDuplicateEmails(char **emails, int *count);
void shrinkArray(char ***emails, int newSize);
void freeEmails(char **emails, int count);


#endif // MAIN_H