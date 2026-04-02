#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>

/*
 * DOCUMENTATION ALIGNMENT: memory handling strategy
 * We avoid `PySequence_Fast` to ensure ZERO additional heap allocation.
 * Instead, we determine whether the input is a list or tuple. 
 * Then, we natively retrieve items directly via PyList_GET_ITEM or PyTuple_GET_ITEM.
 * These are O(1) time macros mapped exactly onto underlying Python C memory structure.
 */

// Helper to reliably extract length and ensure valid type structure.
static Py_ssize_t get_size_and_validate(PyObject *data) {
    if (PyList_Check(data)) {
        return PyList_GET_SIZE(data);
    } else if (PyTuple_Check(data)) {
        return PyTuple_GET_SIZE(data);
    }
    PyErr_SetString(PyExc_TypeError, "vibration analytics require a list or tuple of floats.");
    return -1;
}

// Helper to pull directly from underlying container avoiding sequence unpacking copies.
static inline double get_item_as_double(PyObject *data, Py_ssize_t idx, int *error) {
    PyObject *item;
    if (PyList_Check(data)) {
        item = PyList_GET_ITEM(data, idx);
    } else {
        item = PyTuple_GET_ITEM(data, idx);
    }
    
    if (!PyFloat_Check(item)) {
        PyErr_SetString(PyExc_TypeError, "All elements inside the sequence must strictly be floats.");
        *error = 1;
        return 0.0;
    }
    
    return PyFloat_AS_DOUBLE(item);
}


/*
 * peak_to_peak(data): mathematical max - min
 * Time Complexity: O(N)
 * We do one single pass.
 */
static PyObject* vibration_peak_to_peak(PyObject *self, PyObject *args) {
    PyObject *data;
    if (!PyArg_ParseTuple(args, "O", &data)) return NULL;

    Py_ssize_t size = get_size_and_validate(data);
    if (size == -1) return NULL;
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "Input sequence cannot be empty.");
        return NULL;
    }

    int err = 0;
    double min_val = get_item_as_double(data, 0, &err);
    if (err) return NULL;
    double max_val = min_val;

    for (Py_ssize_t i = 1; i < size; ++i) {
        double val = get_item_as_double(data, i, &err);
        if (err) return NULL;
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }

    return PyFloat_FromDouble(max_val - min_val);
}

/*
 * rms(data): Root Mean Square
 * Mathematically: sqrt( (x1^2 + x2^2 + ... xn^2) / n )
 * Time Complexity: O(N)
 */
static PyObject* vibration_rms(PyObject *self, PyObject *args) {
    PyObject *data;
    if (!PyArg_ParseTuple(args, "O", &data)) return NULL;

    Py_ssize_t size = get_size_and_validate(data);
    if (size == -1) return NULL;
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "Input sequence cannot be empty.");
        return NULL;
    }

    int err = 0;
    double sum_sq = 0.0;
    
    for (Py_ssize_t i = 0; i < size; ++i) {
        double val = get_item_as_double(data, i, &err);
        if (err) return NULL;
        sum_sq += (val * val);
    }

    double mean_sq = sum_sq / (double)size;
    return PyFloat_FromDouble(sqrt(mean_sq));
}

/*
 * std_dev(data): Sample Standard Deviation
 * 
 * NUMERICAL STABILITY (Welford's Algorithm):
 * To prevent catastrophic cancellation found in the standard format (sum(x^2) - sum(x)^2) / n, 
 * we use Welford's one-pass algorithm.
 * Time Complexity: O(N) single pass.
 */
static PyObject* vibration_std_dev(PyObject *self, PyObject *args) {
    PyObject *data;
    if (!PyArg_ParseTuple(args, "O", &data)) return NULL;

    Py_ssize_t size = get_size_and_validate(data);
    if (size == -1) return NULL;
    if (size < 2) {
        PyErr_SetString(PyExc_ValueError, "Standard deviation requires at least 2 data points.");
        return NULL;
    }

    int err = 0;
    double mean = 0.0;
    double m2 = 0.0; // sum of squares of differences from the current mean

    for (Py_ssize_t i = 0; i < size; ++i) {
        double val = get_item_as_double(data, i, &err);
        if (err) return NULL;
        
        double delta = val - mean;
        mean += delta / (i + 1);
        m2 += delta * (val - mean);
    }

    double variance = m2 / (size - 1); // Sample variance
    return PyFloat_FromDouble(sqrt(variance));
}


/*
 * above_threshold(data, threshold)
 * Loops and strictly compares items > threshold.
 * Time Complexity: O(N).
 */
static PyObject* vibration_above_threshold(PyObject *self, PyObject *args) {
    PyObject *data;
    double threshold;
    
    // "Od" = Object, double.
    if (!PyArg_ParseTuple(args, "Od", &data, &threshold)) return NULL;

    Py_ssize_t size = get_size_and_validate(data);
    if (size == -1) return NULL;

    int err = 0;
    long count = 0;
    
    for (Py_ssize_t i = 0; i < size; ++i) {
        double val = get_item_as_double(data, i, &err);
        if (err) return NULL;
        if (val > threshold) {
            count++;
        }
    }

    return PyLong_FromLong(count);
}

/*
 * summary(data)
 * Time Complexity: O(N).
 * Builds a PyDict embedding 4 stats across a single loop to maximize cache efficiency.
 */
static PyObject* vibration_summary(PyObject *self, PyObject *args) {
    PyObject *data;
    if (!PyArg_ParseTuple(args, "O", &data)) return NULL;

    Py_ssize_t size = get_size_and_validate(data);
    if (size == -1) return NULL;
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "Input sequence cannot be empty.");
        return NULL;
    }

    int err = 0;
    double min_val = get_item_as_double(data, 0, &err);
    if (err) return NULL;
    
    double max_val = min_val;
    double sum_val = min_val;

    for (Py_ssize_t i = 1; i < size; ++i) {
        double val = get_item_as_double(data, i, &err);
        if (err) return NULL;
        
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
        sum_val += val;
    }

    double mean_val = sum_val / (double)size;

    PyObject *dict = PyDict_New();
    if (!dict) return NULL;

    PyDict_SetItemString(dict, "count", PyLong_FromSsize_t(size));
    PyDict_SetItemString(dict, "mean", PyFloat_FromDouble(mean_val));
    PyDict_SetItemString(dict, "min", PyFloat_FromDouble(min_val));
    PyDict_SetItemString(dict, "max", PyFloat_FromDouble(max_val));

    return dict;
}

// ============================================
// Module Method Table Definition
// ============================================
static PyMethodDef VibrationMethods[] = {
    {"peak_to_peak",  vibration_peak_to_peak, METH_VARARGS, "Returns max - min differential."},
    {"rms",           vibration_rms,          METH_VARARGS, "Returns Root Mean Square."},
    {"std_dev",       vibration_std_dev,      METH_VARARGS, "Returns Sample Standard Deviation (Welford Alg)."},
    {"above_threshold",vibration_above_threshold,METH_VARARGS,"Returns count of values > threshold."},
    {"summary",       vibration_summary,      METH_VARARGS, "Returns dict of basic stats."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

// ============================================
// Module Definition Structure
// ============================================
static struct PyModuleDef vibrationmodule = {
    PyModuleDef_HEAD_INIT,
    "vibration",   /* name of module */
    "C extension for rapid industrial vibration array metrics.", /* module documentation */
    -1,            /* size of per-interpreter state of the module */
    VibrationMethods
};

// ============================================
// Module Initialization Function
// ============================================
PyMODINIT_FUNC PyInit_vibration(void) {
    return PyModule_Create(&vibrationmodule);
}
