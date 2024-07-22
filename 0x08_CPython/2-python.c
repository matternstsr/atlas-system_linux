#include <Python.h>
#include <stdio.h>

/** Print basic information about a Python list object.
 *  Arguments:
 *    p: PyObject pointer representing the Python list object.
 */

void print_python_bytes(PyObject *p)
{
    Py_ssize_t size, i;
    const char *str;

    /* Check if p is a valid PyBytesObject */
    if (!PyBytes_Check(p)) {
        fprintf(stderr, "[ERROR] Invalid Bytes Object\n");
        return;
    }

    /* Retrieve the size (length) of the bytes object */
    size = PyBytes_Size(p);

    /* Print bytes object information */
    printf("[.] bytes object info\n");
    printf("  size: %zd\n", size);

    /* Print the string representation of the bytes object */
    str = PyBytes_AsString(p);
    printf("  trying string: %s\n", str);  /* Use PyBytes_AsString directly */

    /* Print the first 10 bytes of the bytes object in hexadecimal format */
    printf("  first %zd bytes: ", size < 10 ? size : 10);  /*Adjusted to print up to 10 bytes */
    for (i = 0; i < (size < 10 ? size : 10); i++)
    {
        printf("%02x", (unsigned char)str[i]);
        if (i < size - 1) /* Changed to iterate up to size */
            printf(" ");
    }
    printf(" 00\n");  /* Add the extra '00' byte at the end */
}


/** Print basic information about a Python bytes object.
 *  Arguments:
 *    p: PyObject pointer representing the Python bytes object.
 */
void print_python_bytes(PyObject *p)
{
    Py_ssize_t size, i;
    const char *str;

    /* Check if p is a valid PyBytesObject */
    if (!PyBytes_Check(p)) {
        fprintf(stderr, "[ERROR] Invalid Bytes Object\n");
        return;
    }

    /* Retrieve the size (length) of the bytes object */
    size = PyBytes_Size(p);

    /* Print bytes object information */
    printf("[.] bytes object info\n");
    printf("  size: %zd\n", size);

    /* Try to print the string representation of the bytes object */
    PyObject *repr = PyObject_Repr(p);
    const char *repr_str = PyUnicode_AsUTF8(repr);
    printf("  trying string: %s\n", repr_str);
    Py_XDECREF(repr);  /* Release the reference to the repr object */

    /* Print the first 10 bytes of the bytes object in hexadecimal format */
    printf("  first %zd bytes: ", size < 9 ? size + 1:10);
    str = PyBytes_AsString(p);
    for (i = 0; i < (size < 10 ? size : 10); i++)
    {
        printf("%02x", (unsigned char)str[i]);
        if (i < (size < 10 ? size : 10) - 1)
            printf(" ");
    }
    printf("\n");
}
