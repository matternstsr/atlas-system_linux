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
    int neg = (num->ob_size < 0);
    unsigned long int total = 0;
    const unsigned long int base = (1UL << PyLong_SHIFT);

    /* Iterate through the digits of the number */
    ssize_t i;
    for (i = 0; i < ABS(num->ob_size); ++i) {
        /* Check for overflow */
        if (total > (ULONG_MAX - ((unsigned long int)num->ob_digit[i] * base)) / base) {
            printf("C unsigned long int overflow\n");
            return;
        }
        total = total * base + (unsigned long int)num->ob_digit[i];
    }

    /* Print the value with appropriate sign */
    if (neg) {
        printf("-");
    }
    printf("%lu\n", total);
}
