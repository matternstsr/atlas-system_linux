#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p)
{
    Py_ssize_t length;
    Py_UNICODE *unicode_str;
    char *ascii;

    if (!PyUnicode_Check(p))
    {
        wprintf(stderr, L"[.] string object info\n");
        wprintf(stderr, L"  [ERROR] Invalid String Object\n");
    }

    length = PyUnicode_GET_LENGTH(p);

    if (PyUnicode_AS_UNICODE(p))
    {
        ascii = PyUnicode_AsUTF8(p);
        wprintf(stdout, L"[.] string object info\n");
        wprintf(stdout, L"  type: compact ascii\n");
        wprintf(stdout, L"  length: %ld\n", length);
        wprintf(stdout, L"  value: %s\n", ascii);
    }
    else
    {
        unicode_str = PyUnicode_AsUnicode(p);
        wprintf(stdout, L"[.] string object info\n");
        wprintf(stdout, L"  type: compact unicode object\n");
        wprintf(stdout, L"  length: %ld\n", length);
        wprintf(stdout, L"  value: L");
        for (Py_ssize_t i = 0; i < length; ++i)
            wprintf(stdout, L"%c", unicode_str[i]);
        wprintf(stdout, L"\n");
    }
}
