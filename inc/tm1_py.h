#include <Python.h>
//#include "python3.8/Python.h"
#include <gtk/gtk.h> // needed for the gpointer reference; need to rethink dependencies and defs

struct py_data {
        char *script;
        char *func;
};

struct cube_names {
        char **name;
        int size;
};

struct cube_names *load_py(struct py_data *, char *);
