#include <Python.h>
#include <stdio.h>

void print_python_int(PyObject *p) {
    if (!p || !PyLong_Check(p)) {
        printf("Invalid Int Object\n");
        return;
    }

    // Cast p to PyLongObject pointer
    PyLongObject *num = (PyLongObject *) p;

    // Check if the number is negative
    int neg = (num->ob_size < 0);

    // Prepare variables for calculation
    unsigned long int total = 0;
    const unsigned long int base = (1UL << PyLong_SHIFT);

    // Determine the start and end of the digit array to iterate
    ssize_t start = 0;
    ssize_t end = num->ob_size - 1;

    // Adjust start and end indices based on negative sign
    if (neg) {
        start = 1;
    }

    // Iterate through the digits of the number
    for (ssize_t i = start; i <= end; ++i) {
        // Check for overflow
        if (total > (ULONG_MAX - ((unsigned long int)num->ob_digit[i] * base)) / base) {
            printf("C unsigned long int overflow\n");
            return;
        }
        total = total * base + (unsigned long int)num->ob_digit[i];
    }

    // Print the value with appropriate sign
    if (neg) {
        printf("-");
    }
    printf("%lu\n", total);
}
