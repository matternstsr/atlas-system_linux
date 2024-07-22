#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p)
{
    Py_ssize_t length;
    char *ascii;
    
    str = (PyASCIIObject *)p;
    
    if (!PyUnicode_Check(p))
    {
        wprintf(L"[.] string object info\n");
        wprintf(L"  [ERROR] Invalid String Object\n");
    }

    length = PyUnicode_GET_LENGTH(p);

    if (str->state.ascii)
    {
        ascii = PyUnicode_AsUTF8(p);
        wprintf(L"[.] string object info\n");
        wprintf(L"  type: compact ascii\n");
        wprintf(L"  length: %ld\n", length);
        wprintf(L"  value: %s\n", ascii);
    }
    else
    {
        unicode_str = PyUnicode_AsUnicode(p);
        wprintf(L"[.] string object info\n");
        wprintf(L"  type: compact unicode object\n");
        wprintf(L"  length: %ld\n", length);
        wprintf(L"  value: %ls\n", PyUnicode_AS_UNICODE(p));
        for (Py_ssize_t i = 0; i < length; ++i)
            wprintf(L"%c", unicode_str[i]);
        wprintf(L"\n");
    }
}
