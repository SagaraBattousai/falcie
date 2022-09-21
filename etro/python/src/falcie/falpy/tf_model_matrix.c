


#include <falpy/tf_model_matrix.h>

#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL FALPY_ARRAY_API
#include <numpy/arrayobject.h>

#define NULL_CHECK(value) if(value == NULL) return NULL

//So embarrasingly it turns out you can just do model.trainable_weights
//instead of doing it manually for each layer :P
//static 
PyObject*
falpy_model_matrix(PyObject *self, PyObject *args)
{
	PyListObject *trainable_weights;

	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &trainable_weights))
		return NULL;

	Py_ssize_t weight_list_size = PyList_Size(trainable_weights);

	PyListObject *model_matrix = (PyListObject *)PyList_New(weight_list_size);
	NULL_CHECK(model_matrix);

	for (Py_ssize_t i = 0; i < weight_list_size; i++)
	{
		PyObject *elem = PyList_GetItem(trainable_weights, i);
		NULL_CHECK(elem);

		PyObject *numpy_converter = PyObject_GetAttrString(elem, "numpy");
		NULL_CHECK(numpy_converter);

		PyObject *matrix = PyObject_CallNoArgs(numpy_converter);
		NULL_CHECK(matrix);

		//Use macro as adding to new list
		PyList_SET_ITEM(model_matrix, i, matrix);
	}

	return model_matrix;
}