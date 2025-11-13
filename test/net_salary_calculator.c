#include <stdio.h>

/**
 * calc_tax - calculates the tax deduction based on salary range
 * @gross: pointer to gross salary
 * @tax: pointer to store calculated tax
 *
 * Return: void
 */
void calc_tax(float *gross, float *tax)
{
	if (*gross <= 100)
	{
		*tax = 0;
	}
	else if (*gross > 100 && *gross <= 300)
	{
		*tax = *gross * 0.2;
	}
	else
	{
		*tax = *gross * 0.28;
	}
}

/**
 * calc_medical_insurance - calculates medical insurance deduction
 * @gross: pointer to gross salary
 * @medical: pointer to store calculated medical insurance
 *
 * Return: void
 */
void calc_medical_insurance(float *gross, float *medical)
{
	*medical = *gross * 0.05;
}

/**
 * calc_maternity_leave - calculates maternity leave deduction
 * @gross: pointer to gross salary
 * @maternity: pointer to store calculated maternity deduction
 *
 * Return: void
 */
void calc_maternity_leave(float *gross, float *maternity)
{
	*maternity = *gross * 0.003;
}

/**
 * calc_social_security - calculates social security fund deduction
 * @gross: pointer to gross salary
 * @social: pointer to store calculated social security deduction
 *
 * Return: void
 */
void calc_social_security(float *gross, float *social)
{
	*social = *gross * 0.03;
}

/**
 * calc_net_salary - aggregates all deductions and calculates net salary
 * @gross: pointer to gross salary
 * @net: pointer to store calculated net salary
 *
 * Return: void
 */
void calc_net_salary(float *gross, float *net)
{
	float tax, medical, maternity, social, total_deductions;

	calc_tax(gross, &tax);
	calc_medical_insurance(gross, &medical);
	calc_maternity_leave(gross, &maternity);
	calc_social_security(gross, &social);

	total_deductions = tax + medical + maternity + social;
	*net = *gross - total_deductions;
}

/**
 * main - entry point of the program
 *
 * Return: 0 on success
 */
int main(void)
{
	float gross, net;

	printf("Enter your gross salary (USD): ");
	scanf("%f", &gross);

	calc_net_salary(&gross, &net);

	printf("\nGross Salary: $%.2f\n", gross);
	printf("Net Salary: $%.2f\n", net);

	return (0);
}
