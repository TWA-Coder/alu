#include <stdio.h>
#include <string.h>
#include "main.h"
/**
 * main - Entry point of the program
 * Return: Always 0 (Success)
*/
int main()
{
    char names[SIZE][NAME_LENGTH] = 
    {
        "John Doe",
        "Jane Smith",
        "Alice Johnson",
        "Bob Brown",
        "Charlie Davis",
        "Eve Wilson",
        "Frank Miller",
        "Grace Lee",
        "Hank Taylor",
        "Ivy Anderson",
        "Jack Thomas",
        "Kathy White",
        "Larry Harris",
        "Mona Clark",
        "Nina Lewis",
        "Oscar Walker",
        "Paul Hall",
        "Quincy Allen",
        "Rachel Young",
        "Steve King"
    };
    char order[10];
    int (*comparePtr)(const char *, const char *) = NULL;

    printf("Enter sorting order (asc/desc): ");
    scanf("%s", order);

    if (strcmp(order, "asc") == 0) {
        comparePtr = ascending;
    } else if (strcmp(order, "desc") == 0) {
        comparePtr = descending;
    } else {
        printf("Invalid sorting order. Please enter 'asc' or 'desc'.\n");
        return 1;
    }

    bubble_sort(names, comparePtr);

    printf("\nSorted list (%s):\n", order);
    for (int i = 0; i < SIZE; i++) {
        printf("%s\n", names[i]);
    }

    return 0;
}

/**
 * ascending - Compare two strings for ascending order
 * @a: First string
 * @b: Second string
 * Return: Negative if a < b, zero if a == b, positive if a > b
*/
int ascending(const char *a, const char *b)
{
    return strcmp(a, b) > 0;
}

/**
 * descending - Compare two strings for descending order
 * @a: First string
 * @b: Second string
 * Return: Negative if a > b, zero if a == b, positive if a < b
*/
int descending(const char *a, const char *b)
{
    return strcmp(a, b) < 0;
}

/**
 * bubbleSort - Sort an array of names using bubble sort algorithm
 * @names: 2D array of names to be sorted
 * @compare: Function pointer for comparison
 * 
 * Return: void
*/
void bubble_sort(char names[SIZE][NAME_LENGTH], int(*compare)(const char *, const char *))
{
    char temp[NAME_LENGTH];
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (compare(names[j], names[j + 1])) {
                strcpy(temp, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], temp);
            }
        }
    }
}
