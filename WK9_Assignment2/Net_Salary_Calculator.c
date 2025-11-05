#include <stdio.h>
#include "main.h"

int main()
{
    float grossSalary, netSalary;
     printf("Enter the gross salary: ");
    scanf("%f", &grossSalary);

    net_salary_calculator(&grossSalary, &netSalary);

    printf("The net salary is: %.2f USD\n", netSalary);

    return 0;
}

/**
Tax_on_salary - determine tax based on gross salary
*@param gross - pointer to gross salary
*@param tax - pointer to calculated tax
*
*Return: void
*/
void tax_on_salary(float *gross, float *tax)
{
    if(*gross <= 100)
    {
        *tax = 0;
    }else if(*gross <= 300)
    {
        *tax = 0.02 * (*gross);
    }else
    {
        *tax = 0.28 * (*gross);
    }
}

/**
* medical_insurance - calculate medical insurance
* @param gross - pointer to gross salary
* @param medInsurance - pointer to calculated medical insurance
*
* Return: void
*/
void medication_insurance(float *gross, float *medInsurance)
{
    *medInsurance = 0.05 * (*gross);
}
/**
* maternity_leave - calculate maternity leave contribution
* @param gross - pointer to gross salary
* @param maternity - pointer to calculated maternity leave contribution
*
* Return: void 
*/
void maternity_leave(float *gross, float *maternity)
{
    *maternity = 0.003 * (*gross);
}

/**
* social_security_fund
* @param gross - pointer to gross salary
* @param social - pointer to calculated social security fund contribution
*
* Return: void
*/
void social_security_fund(float *gross, float *social)
{
    *social = 0.03 * (*gross);
}
/**
* net_salary_calculator - calculate net salary after deductions
* @param gross - pointer to gross salary
* @param net - pointer to calculated net salary
*
* Return: void
*/
void net_salary_calculator(float *gross, float *net)
{
    float tax, med, mat, soc;

    tax_on_salary(gross, &tax);
    medication_insurance(gross, &med);
    maternity_leave(gross, &mat);
    social_security_fund(gross, &soc);

    *net = *gross - (tax + med + mat + soc);
}