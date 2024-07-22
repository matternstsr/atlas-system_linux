#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

void print_python_int(PyObject *p)
{
    /* Check if p is a Python integer */
    if (!p || !PyLong_Check(p))
        printf("Invalid Int Object\n");

    /* Cast p to PyLongObject pointer */
    PyLongObject *num = (PyLongObject *) p;

    /* Extract the size of the Python integer */
    ssize_t size = num->ob_size;

    /* Check if the Python integer is negative */
    int neg = 0;
    if (size < 0)
    {
        neg = 1;
        size = -size;
    }

    /* Check for overflow (size > 3 is considered overflow) */
    if (size > 3)
        printf("C unsigned long int overflow\n");

    /* Calculate the value of the Python integer manually */
    unsigned long long total = 0;
    for (ssize_t i = size - 1; i >= 0; i--)
        total = (total << PyLong_SHIFT) + num->ob_digit[i];

    /* Print the value with appropriate sign */
    if (neg)
        printf("-");
    printf("%llu\n", total);
}
