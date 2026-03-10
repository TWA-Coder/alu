#define PY_SSIZE_T_CLEAN
#include <Python.h>

// The C implementation of our numeric computation
static PyObject* fast_math_sum_squares(PyObject *self, PyObject *args) {
    long long n;

    // Parse the Python integer argument into a C long long
    if (!PyArg_ParseTuple(args, "L", &n)) {
        return NULL;
    }

    double total = 0.0;
    // Fast numeric loop without Python object overhead
    for (long long i = 1; i <= n; i++) {
        total += (double)i * i;
    }

    // Convert the C double back into a Python float object
    return PyFloat_FromDouble(total);
}

// Method definition array
static PyMethodDef FastMathMethods[] = {
    {"sum_squares", fast_math_sum_squares, METH_VARARGS, "Calculate the sum of squares up to n."},
    {NULL, NULL, 0, NULL} // Sentinel indicating the end of the array
};

// Module definition structure
static struct PyModuleDef fastmathmodule = {
    PyModuleDef_HEAD_INIT,
    "fast_math",                           // Module name
    "A C extension for fast math operations.", // Module description
    -1,                                    // Size of per-interpreter state
    FastMathMethods
};

// Module initialization function called by Python when importing
PyMODINIT_FUNC PyInit_fast_math(void) {
    return PyModule_Create(&fastmathmodule);
}
