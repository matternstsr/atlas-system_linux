#include <Python.h>
#include <stdio.h>

void print_python_int(PyObject *p)
{
    /* Check if p is a Python integer */
    if (!p || !PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }

    /* Cast p to PyLongObject pointer */
    PyLongObject *num = (PyLongObject *) p;

    /* Initialize variables */
    int neg = 0;
    unsigned long int total = 0;
    unsigned long int base = 1;

    /* Determine if the number is negative */
    if (num->ob_digit[num->ob_size - 1] >> (PyLong_SHIFT - 1))
        neg = 1;

    /* Calculate the total value of the Python integer */
    for (ssize_t i = 0; i < num->ob_size; i++)
    {
        total += num->ob_digit[i] * base;
        base <<= PyLong_SHIFT;
    }

    /* Print the value with appropriate sign */
    if (neg)
        printf("-");

    /* Print the calculated value */
    printf("%lu\n", total);
}
