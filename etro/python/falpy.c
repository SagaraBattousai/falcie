#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>

#include <falpy/tf_model_matrix.h>


static PyMethodDef FalpyMethods[] = {
	{"model_matrix", falpy_model_matrix, METH_VARARGS, "Placeholder."},

	{NULL, NULL, 0, NULL}

};

static PyModuleDef falpymodule = {
	PyModuleDef_HEAD_INIT,
	"falpy", //Module name
	NULL, // Module Doc
	-1, //State kept in globals but otherwise would be size
	FalpyMethods
};

PyMODINIT_FUNC
PyInit_falpy(void)
{
	return PyModule_Create(&falpymodule);
}