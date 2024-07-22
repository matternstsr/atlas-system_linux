#include <Python.h>
#include <stdio.h>

/**
 * print_python_int - Function to print Python integers.
 * @p: Pointer to the Python object representing an integer.
 */
void print_python_int(PyObject *p)
{
    PyLongObject *num;
    ssize_t size, i;
    int neg = 0, int_shift;
    unsigned long total = 0, current;

    /* Check if p is NULL or not a PyLongObject */
    if (!p || !PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }

    /* Cast p to PyLongObject */
    num = (PyLongObject *)p;
    
    /* Get the size (number of limbs) and sign of the PyLongObject */
    size = ((PyVarObject *)p)->ob_size;
    if (size < 0)
        neg = 1;
    
    /* Adjust size to positive value */
    size = neg ? -size : size;

    /* Check for overflow condition */
    if (size > 3 || (size == 3 && num->ob_digit[2] > 0xf))
    {
        printf("C unsigned long int overflow\n");
        return;
    }
    /* Calculate the integer value from ob_digit array */
    for (i = 0; i < size; i++)
    {
        int_shift = PyLong_SHIFT * i;
        current = ((unsigned long)num->ob_digit[i]) * (1UL << (int_shift));
        total += current;
    }

    /* Print the final value */
    if (neg)
        printf("-");
    printf("%lu\n", total);
}
