#include "tm1_py.h"
#include "Python.h"

struct cube_names *load_py(PyObject *pModule, char *args)
{
        PyObject *pFunc = NULL, *pValue = NULL, *pArgs = NULL;
        struct cube_names *names = malloc(sizeof(struct cube_names));

        if (pModule != NULL) {
                pFunc = PyObject_GetAttrString(pModule, PY_FUNC);

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

/* TODO: take another look at decref python vars; may have a memory leak */
err_free:
	printf("-> tm1_py.c: err_free\n");
	free(names);
	names = NULL;
std_free:
        return names;
}

PyObject *init_py()
{
        PyObject *pName = NULL, *pModule = NULL;

	Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
        
        pName = PyUnicode_DecodeFSDefault(PY_SCRIPT);
        if (pName == NULL) {
                printf("NULL pName\n");
        }

	pModule = PyImport_GetModule(pName);
	if (!pModule)
		pModule = PyImport_Import(pName);
        if (pModule == NULL) {
                printf("NULL pModule\n");
        }

	Py_XDECREF(pName);
	return pModule;
}

void close_py()
{
	Py_FinalizeEx();
}
