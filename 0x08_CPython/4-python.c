#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p)
{
    Py_ssize_t length;
    Py_UNICODE *unicode_str;
    char *ascii;

    if (!PyUnicode_Check(p))
    {
        wprintf(stderr, "[.] string object info\n");
        wprintf(stderr, "  [ERROR] Invalid String Object\n");
    }

    length = PyUnicode_GET_LENGTH(p);

    if (PyUnicode_AS_UNICODE(p))
    {
        ascii = PyUnicode_AsUTF8(p);
        wprintf(stdout, "[.] string object info\n");
        wprintf(stdout, "  type: compact ascii\n");
        wprintf(stdout, "  length: %ld\n", length);
        wprintf(stdout, "  value: %s\n", ascii);
    }
    else
    {
        unicode_str = PyUnicode_AsUnicode(p);
        wprintf(stdout, "[.] string object info\n");
        wprintf(stdout, "  type: compact unicode object\n");
        wprintf(stdout, "  length: %ld\n", length);
        wprintf(stdout, "  value: ");
        for (Py_ssize_t i = 0; i < length; ++i)
            wprintf(stdout, "%c", unicode_str[i]);
        wprintf(stdout, "\n");
    }
}
