#include <Python.h>
#include <stdio.h>

void print_python_string(PyObject *p)
{
    Py_ssize_t length;
    Py_UNICODE *unicode_str;
    PyASCIIObject *str;
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
        wprintf(L"[.] string object info\n");
        wprintf(L"  type: compact ascii\n");
        wprintf(L"  length: %ld\n", length);
	    wprintf(L"  value: %ls\n", PyUnicode_AS_UNICODE(p));
    }
    else
    {
        wprintf(L"[.] string object info\n");
        wprintf(L"  type: compact unicode object\n");
        wprintf(L"  length: %ld\n", length);
	    wprintf(L"  value: %ls\n", PyUnicode_AS_UNICODE(p));
    }
}
