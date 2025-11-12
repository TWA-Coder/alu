#include <stdio.h>
#include <string.h>

#define SIZE 20
#define MAX_LEN 100

/**
 * ascending - compares two strings for ascending order
 * @a: first string
 * @b: second string
 *
 * Return: 1 if swap needed, 0 otherwise
 */
int ascending(char *a, char *b)
{
	if (strcmp(a, b) > 0)
		return (1);
	return (0);
}

/**
 * descending - compares two strings for descending order
 * @a: first string
 * @b: second string
 *
 * Return: 1 if swap needed, 0 otherwise
 */
int descending(char *a, char *b)
{
	if (strcmp(a, b) < 0)
		return (1);
	return (0);
}

/**
 * bubble_sort - sorts an array of names using bubble sort
 * @names: array of strings to sort
 * @n: number of names
 * @cmp: pointer to comparison function
 *
 * Return: void
 */
void bubble_sort(char names[][MAX_LEN], int n, int (*cmp)(char *, char *))
{
	int i, j;
	char temp[MAX_LEN];

	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if ((*cmp)(names[j], names[j + 1]))
			{
				strcpy(temp, names[j]);
				strcpy(names[j], names[j + 1]);
				strcpy(names[j + 1], temp);
			}
		}
	}
}

/**
 * main - entry point of the program
 *
 * Return: 0 on success
 */
int main(void)
{
	char names[SIZE][MAX_LEN];
	char order[10];
	int i;
	int (*cmp_func)(char *, char *);

	printf("Enter 20 full names (one per line):\n");
	for (i = 0; i < SIZE; i++)
	{
		printf("Name %d: ", i + 1);
		fgets(names[i], MAX_LEN, stdin);
		names[i][strcspn(names[i], "\n")] = '\0';
	}

	printf("\nEnter order (asc for ascending / desc for descending): ");
	scanf("%9s", order);

	if (strcmp(order, "asc") == 0)
		cmp_func = ascending;
	else if (strcmp(order, "desc") == 0)
		cmp_func = descending;
	else
	{
		printf("Invalid order. Defaulting to ascending.\n");
		cmp_func = ascending;
	}

	bubble_sort(names, SIZE, cmp_func);

	printf("\nSorted names (%s):\n", order);
	for (i = 0; i < SIZE; i++)
		printf("%s\n", names[i]);

	return (0);
}
