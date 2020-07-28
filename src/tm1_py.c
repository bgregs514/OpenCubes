#include "tm1_py.h"
#include "Python.h"

struct cube_names *load_py(struct py_data *py_init, char *args)
{
        PyObject *pName = NULL, *pModule = NULL, *pFunc = NULL;
        PyObject *pValue = NULL, *pArgs = NULL;
        struct py_data *data = py_init;
        struct cube_names *names = malloc(sizeof(struct cube_names));

        Py_Initialize();
        PyRun_SimpleString("import sys");
        // this needs to be updated, but for now it saves a lot of hardcoding
        PyRun_SimpleString("sys.path.append(\".\")");
        
        pName = PyUnicode_DecodeFSDefault(data->script);
        if (pName == NULL) {
                printf("NULL pName\n");
		goto err_free;
        }

        pModule = PyImport_Import(pName);
        if (pModule == NULL) {
                printf("NULL pModule\n");
		goto err_free;
        }

        if (pModule != NULL) {
                pFunc = PyObject_GetAttrString(pModule, data->func);

                pArgs = PyTuple_New(1);
                PyObject *pParam = PyUnicode_FromString(args);
                PyTuple_SetItem(pArgs, 0, pParam);

		/* TODO: set timeout for long runner/connection issue;
		   this will wait for a long time if it's not handled */
                pValue = PyObject_CallObject(pFunc, pArgs);
		Py_XDECREF(pParam);
                if (pValue != NULL && PyList_Check(pValue) == 1) {
                        PyObject *item = NULL;
                        char const *key = NULL;
                        Py_ssize_t n = PyList_Size(pValue);

                        item = PyList_GetItem(pValue, 0);
                        if (item == NULL) {
                                printf("PY: Return object empty.\n");
                        } else {
                                names->name = (char**)malloc(n * sizeof(char *));
                                names->size = n;
                                for (int i = 0; i < n; i++) {
                                        item = PyList_GetItem(pValue, i);
                                        key = PyUnicode_AsUTF8(item);
                                        names->name[i] = (char *)malloc(100 * sizeof(char));
                                        strcpy(names->name[i], key);
                                }
                        }
                } else {
			Py_XDECREF(pParam);
			printf("***Rest API object returned NULL***\n");
			printf("\t:Try checking the IP address in the python script\n");
			goto err_free;
		}
        }
	goto std_free;

err_free:
	printf("-> tm1_py.c: err_free\n");
	free(names);
	names = NULL;
	Py_INCREF(pArgs); /* pArgs never gets incremented, so the decref will blow up in py_finalize;
			     a bit of a hack, but it works for now and keeps the code generic */
std_free:
        Py_XDECREF(pName);
        Py_XDECREF(pValue);
        Py_XDECREF(pModule);
        Py_XDECREF(pFunc);
        Py_XDECREF(pArgs);
        Py_Finalize();
	printf("-> tm1_py.c: Cleaned up python variables\n");
        return names;
}
