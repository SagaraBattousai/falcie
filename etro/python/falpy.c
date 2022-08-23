#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#define PY_ARRAY_UNIQUE_SYMBOL FALPY_ARRAY_API
#include <numpy/arrayobject.h>

#include <stdio.h>

//#include <falpy/falpy.h>
#include <falpy/falpy_weights.h>
#include <falpy/falpy_block.h>
#include <falpy/falpy_blockchain.h>
#include <falpy/falpy_networkupdate.h>
#include <falpy/tf_model_matrix.h>

#define CheckTypeIsReady(type) if (PyType_Ready(&type) < 0) return NULL


static PyMethodDef FalpyMethods[] = {
	{"model_matrix", falpy_model_matrix, METH_VARARGS, "Placeholder."},

	{NULL, NULL, 0, NULL}

};

static PyModuleDef falpymodule = {
	PyModuleDef_HEAD_INIT,
	.m_name = "falpy", //Module name
	.m_doc = NULL, // Module Doc
	.m_size = -1, //State kept in globals but otherwise would be size
	.m_methods = FalpyMethods
};

PyMODINIT_FUNC
PyInit_falpy(void)
{

	PyObject *falpy_module;
	//if (PyType_Ready(&FalpyWeightsType) < 0)
		//return NULL;

	CheckTypeIsReady(FalpyWeightsType);
	CheckTypeIsReady(FalpyBlockchainType);
	CheckTypeIsReady(FalpyBlockBuilderType);
	CheckTypeIsReady(FalpyBlockType);
	CheckTypeIsReady(FalpyNetworkUpdateType);


	falpy_module = PyModule_Create(&falpymodule);
	if (falpy_module == NULL)
		return NULL;

	//Too mucky to make a #Define
	Py_INCREF(&FalpyWeightsType);
	if (PyModule_AddObject(falpy_module, "Weights", (PyObject *)&FalpyWeightsType) < 0)
		goto fail;

	Py_INCREF(&FalpyBlockchainType);
	if (PyModule_AddObject(falpy_module, "Blockchain", (PyObject *)&FalpyBlockchainType) < 0)
		goto fail;

	Py_INCREF(&FalpyBlockBuilderType);
	if (PyModule_AddObject(falpy_module, "BlockBuilder", (PyObject *)&FalpyBlockBuilderType) < 0)
		goto fail;

	Py_INCREF(&FalpyBlockType);
	if (PyModule_AddObject(falpy_module, "Block", (PyObject *)&FalpyBlockType) < 0)
		goto fail;

	Py_INCREF(&FalpyNetworkUpdateType);
	if (PyModule_AddObject(falpy_module, "NetworkUpdate", (PyObject *)&FalpyNetworkUpdateType) < 0)
		goto fail;

	import_array();

	return falpy_module;


fail:
	Py_XDECREF(&FalpyWeightsType);
	Py_XDECREF(&FalpyBlockchainType);
	Py_XDECREF(&FalpyBlockBuilderType);
	Py_XDECREF(&FalpyBlockType);
	Py_XDECREF(&FalpyNetworkUpdateType);

	Py_DECREF(falpy_module);
	return NULL;

}