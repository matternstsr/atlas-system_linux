#include <Python.h>
#include <stdio.h>

/**
 * print_python_int - Function to print Python integers.
 * @p: Pointer to the Python object representing an integer.
 */
void print_python_int(PyObject *p) {
    /* Check if p is NULL or not a Python long integer */
    if (!p || !PyLong_Check(p)) {
        printf("Invalid Int Object\n");
        return;
    }

    /* Check if the number is zero */
    if (PyLong_AsLong(p) == 0) {
        printf("0\n");
        return;
    }

    /* Temporary variable to store the sign */
    int sign = 1;

    /* Temporary variable to hold the absolute value of the number */
    PyObject *abs_value = p;

    /* Determine if the number is negative */
    if (PyLong_AsLong(p) < 0) {
        sign = -1;
        abs_value = PyNumber_Negative(p);
    }

    /* Convert the absolute value to a string */
    PyObject *str_value = PyObject_Str(abs_value);
    const char *str = PyUnicode_AsUTF8(str_value);

    /* Print the value with appropriate sign */
    if (sign == -1) {
        printf("-");
    }
    printf("%s\n", str);

    /* Free the allocated objects */
    Py_DECREF(abs_value);
    Py_DECREF(str_value);
}
