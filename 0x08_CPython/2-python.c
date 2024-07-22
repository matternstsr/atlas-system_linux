#include <Python.h>
#include <stdio.h>

/** Print basic information about a Python list object.
 *  Arguments:
 *    p: PyObject pointer representing the Python list object.
 */
void print_python_list(PyObject *p)
{
    Py_ssize_t size, allocated, i;
    PyObject *elem;

    /* Check if p is a valid PyListObject */
    if (!PyList_Check(p))
        fprintf(stderr, "Invalid list object\n");

    /* Retrieve size (number of elements) & allocated memory of the list */
    size = PyList_Size(p);
    allocated = ((PyListObject *)p)->allocated;

    /* Print Python list information */
    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %zd\n", size);
    printf("[*] Allocated = %zd\n", allocated);

    /* Iterate through each element of the list and print its type */
    for (i = 0; i < size; i++)
    {
        elem = PyList_GetItem(p, i);
        printf("Element %zd: %s\n", i, Py_TYPE(elem)->tp_name);
    }
}

#include <Python.h>
#include <stdio.h>

/** Print basic information about a Python bytes object.
 *  Arguments:
 *    p: PyObject pointer representing the Python bytes object.
 */
void print_python_bytes(PyObject *p)
{
    Py_ssize_t size, i;
    char *str;
    PyObject *bytes_obj;

    /* Check if p is a valid PyBytesObject */
    if (!PyBytes_Check(p))
        fprintf(stderr, "[ERROR] Invalid Bytes Object\n");

    /* Retrieve the size (length) of the bytes object */
    size = PyBytes_Size(p);
    bytes_obj = PyBytes_FromObject(p);/* save p as bytes object */

    /* Print bytes object information */
    printf("[.] bytes object info\n");
    printf("  size: %zd\n", size);
    printf("  trying string: %s\n", PyUnicode_AsUTF8(bytes_obj));

    /* Print the first 10 bytes of the bytes object */
    printf("  first %zd bytes: ", size + 1);
    str = PyBytes_AsString(p);
    if (size < 10)
    {
        for (i = 0; i < size; i++)
        {
            printf("%02hhx", str[i]);
            if (i < size - 1)
                printf(" ");
        }
    } else {
        for (i = 0; i < 10; i++)
        {
            printf("%02hhx", str[i]);
            if (i < 9)
                printf(" ");
        }
    }
    printf("\n");
    Py_DECREF(bytes_obj);  /* Release the reference to the bytes object */
}
