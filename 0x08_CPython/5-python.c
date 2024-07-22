#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

void print_python_int(PyObject *p)
{
    /* Check if p is a Python integer */
    if (!p || !PyLong_Check(p))
        printf("Invalid Int Object\n");

    /* Cast p to PyVarObject pointer */
    PyVarObject *num = (PyVarObject *) p;

    /* Initialize variables */
    int neg = 0;
    unsigned long long total = 0;

    /* Determine if the number is negative */
    if (num->ob_digit[num->ob_size - 1] >> (PyLong_SHIFT - 1))
        neg = 1;

    /* Print the value with appropriate sign */
    if (neg)
        printf("-");

    /* Calculate the total value of the Python integer */
    for (ssize_t i = num->ob_size - 1; i >= 0; i--)
        total = (total << PyLong_SHIFT) + num->ob_digit[i];

    /* Print the calculated value */
    printf("%llu\n", total);
}
