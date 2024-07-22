#include <stdio.h>
#include <Python.h>

void print_python_int(PyObject *p) {
    /* Check if p is a Python integer */
    if (!p || !PyLong_Check(p)) {
        printf("Invalid Int Object\n");
        return;
    }

    /* Extract the value of the Python integer */
    unsigned long x = PyLong_AsUnsignedLong(p);
    if (x == (unsigned long)-1 && PyErr_Occurred()) {
        /* PyErr_Occurred() returns true if a prob PyLong_AsUnsignedLong */
        PyErr_Clear(); /* Clear the error indicator */
        printf("C unsigned long int overflow\n");
    } else {
        /* Print the value of x */
        printf("%lu\n", x);
    }
}

