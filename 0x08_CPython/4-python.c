#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p)
{
    Py_ssize_t length;
    Py_UNICODE *unicode_str;
    char *ascii;

    if (!PyUnicode_Check(p))
    {
        fprintf(stderr, "[.] string object info\n");
        fprintf(stderr, "  [ERROR] Invalid String Object\n");
    }

    length = PyUnicode_GET_LENGTH(p);

    if (PyUnicode_AS_UNICODE(p))
    {
        ascii = PyUnicode_AsUTF8(p);
        fprintf(stdout, "[.] string object info\n");
        fprintf(stdout, "  type: compact ascii\n");
        fprintf(stdout, "  length: %ld\n", length);
        fprintf(stdout, "  value: %s\n", ascii);
    }
    else
    {
        unicode_str = PyUnicode_AsUnicode(p);
        fprintf(stdout, "[.] string object info\n");
        fprintf(stdout, "  type: compact unicode object\n");
        fprintf(stdout, "  length: %ld\n", length);
        fprintf(stdout, "  value: ");
        for (Py_ssize_t i = 0; i < length; ++i)
            fprintf(stdout, "%c", unicode_str[i]);
        fprintf(stdout, "\n");
    }
}
