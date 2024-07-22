#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);

/** Print basic information about a Python list object.
 *  Arguments:
 *    p: PyObject pointer representing the Python list object.
 */

void print_python_list(PyObject *p)
{
    Py_ssize_t size, allocated, i;
    PyObject *elem;
    PyListObject *list = NULL;
    const char *type = NULL;

    /* Check if p is a valid PyListObject */
    if (!PyList_Check(p)) {
        fprintf(stderr, "[ERROR] Invalid List Object\n");
        return;
    }

    /* Retrieve size (number of elements) & allocated memory of the list */
    size = PyList_Size(p);
    allocated = ((PyListObject *)p)->allocated;
    list = (PyListObject *)p;

    /* Print Python list information */
    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", allocated);

    /* Iterate through each element of the list and print its type */
    for (i = 0; i < size; i++)
    {
        elem = list->ob_item[i];
        type = elem->ob_type->tp_name;
        printf("Element %zd: %s\n", i, type);
        if (elem->ob_type == &PyBytes_Type)
			print_python_bytes(elem);
        else if (elem->ob_type == &PyFloat_Type)
			print_python_float(elem);
    }
}

/** Print basic information about a Python bytes object.
 *  Arguments:
 *    p: PyObject pointer representing the Python bytes object.
 */
void print_python_bytes(PyObject *p)
{
    Py_ssize_t size, i;
    const char *str;

    printf("[.] bytes object info\n");
    /* Check if p is a valid PyBytesObject */
    if (!PyBytes_Check(p)) {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }

    /* Retrieve the size (length) of the bytes object */
    size = PyBytes_Size(p);

    /* Print bytes object information */
    printf("  size: %zd\n", size);

    /* Print the string representation of the bytes object */
    str = PyBytes_AsString(p);
    printf("  trying string: %s\n", str);  /* Use PyBytes_AsString directly */
    size =  size < 9 ? size + 1 : 10;
    /* Print the first 10 bytes of the bytes object in hexadecimal format */
    printf("  first %zd bytes: ", size); 
    /* Adjusted to print up to 10 bytes */
    for (i = 0; i < size; i++)
    {
        printf("%02x", (unsigned char)str[i]);
        if (i + 1 < size)  /* Changed to iterate up to size */
            printf(" ");
    }
    printf("\n");
}

/** Print basic information about a Python float object.
 *  Arguments:
 *    p: PyObject pointer representing the Python float object.
 */
void print_python_float(PyObject *p)
{
    double value;

    /* Check if p is a valid PyFloatObject */
    if (!PyFloat_Check(p)) {
        printf("[.] float object info\n");
        printf("  [ERROR] Invalid Float Object\n");
        return;
    }

    /* Retrieve the float value */
    value = PyFloat_AsDouble(p);

    /* Print float object information */
    printf("  [.] float object info\n");
    printf("  value: %f\n", value);
}
