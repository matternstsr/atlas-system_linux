#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

void print_python_list(PyObject *p)
{
    Py_ssize_t size, alloc;
    Py_ssize_t i;
    PyObject *item;

    /* Check if p is a Python list */
    if (!PyList_Check(p))
    {
        fprintf(stderr, "Invalid list object\n");
    }

    /* Get size and allocated size of the list */
    size = PyList_Size(p);
    alloc = ((PyListObject *)p)->allocated;

    /* Print basic list information */
    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %ld\n", size);
    printf("[*] Allocated = %ld\n", alloc);

    /* Iterate over each element in the list */
    for (i = 0; i < size; i++)
    {
        /* Get the last "i" item from the list */
        item = PyList_GetItem(p, i);
        /* Print type name of the current element */
        printf("Element %ld: %s\n", i, Py_TYPE(item)->tp_name);
    }
}
