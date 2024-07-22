#include <stdio.h>
#include <Python.h>

void print_python_int(PyObject *p)
{
    /* Check if p is a Python integer */
    if (!p || !PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }

    /* Convert Python integer to C long long */
    unsigned long long x = PyLong_AsUnsignedLongLong(p);

    /* Check for overflow or underflow */
    if (x == (unsigned long long)-1 && PyErr_Occurred())
    {
        PyErr_Clear();
        printf("C long long overflow\n");
        return;
    }

    /* Print the value */
    printf("%lld\n", x);
}
