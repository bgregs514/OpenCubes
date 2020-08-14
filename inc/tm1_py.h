#ifndef TM1_PY_H
#define TM1_PY_H

#include <Python.h>
//#include "python3.8/Python.h"
#include <gtk/gtk.h> // needed for the gpointer reference; need to rethink dependencies and defs

#define PY_SCRIPT "py_test"
#define PY_FUNC   "get_names"

struct cube_names {
        char **name;
        int size;
};

PyObject *init_py();
void close_py();
struct cube_names *load_py(PyObject *pModule, char *);

#endif
