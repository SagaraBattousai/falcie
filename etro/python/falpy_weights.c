
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL FALPY_ARRAY_API
#include <numpy/arrayobject.h>


#include <stdio.h>

#include <falpy/falpy_weights.h>


//////////////////
/// Setup Type ///
//////////////////

PyMemberDef FalpyWeights_members[] = {
	{"array", T_OBJECT_EX, offsetof(FalpyWeightsObject, npy_array),
	READONLY, "underlying data as a numpy array"},
	{NULL} //Sentinel
};

PyMethodDef FalpyWeights_methods[] = {
	//{"add7", (PyCFunction)Custom_name, METH_NOARGS,
	//	 "Return the name, combining the first and last name"
	//},
	{NULL} //Sentinel
};

PyGetSetDef FalpyWeights_getsetters[] = {
		{"g_array", (getter)FalpyWeights_getarray, NULL,
		 "Get the matrix data as an array", NULL},
		{NULL}  /* Sentinel */
};


PyTypeObject FalpyWeightsType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		.tp_name = "falpy.Weights",
		.tp_doc = PyDoc_STR("falpy Weights"),
		.tp_basicsize = sizeof(FalpyWeightsObject),
		.tp_itemsize = 0, //May want to make variable sized
		.tp_flags = Py_TPFLAGS_DEFAULT,
		.tp_new = PyType_GenericNew, //FalpyWeights_new, //Probably unneeded but ...
		.tp_init = (initproc)FalpyWeights_init,
		.tp_dealloc = (destructor)FalpyWeights_dealloc,
		.tp_members = FalpyWeights_members,
		.tp_methods = FalpyWeights_methods,
		.tp_str = FalpyWeights_str,
		.tp_getset = FalpyWeights_getsetters,
};


////////////////////////////
/// Function Definitions ///
////////////////////////////

//FALPY_LOCAL_SYMBOL PyObject*
//FalpyWeights_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
//	FalpyWeightsObject *self;
//	self = (FalpyWeightsObject *)type->tp_alloc(type, 0);
//	self->npy_array = NULL;
//	self->matrix = NULL;
//
//	return (PyObject *)self;
//}

//Object has:
//PyArrayObject *npy_array;
//matrix_t *matrix;


//TF wrapper will call this and make a list of weights objects
//Therefore this creates 1 matrix etc 
//probably should have custom new then now I think of it?
FALPY_LOCAL_SYMBOL int
FalpyWeights_init(FalpyWeightsObject *self, PyObject *args, PyObject *unused_for_now)//kwds)
{
	//TODO: Shape must be passed too!!!!!

	PyObject *arg1 = NULL; //Borrowed
	PyObject *arr1 = NULL;

	//matrix_t *matrix;

	if (!PyArg_ParseTuple(args, "O", &arg1))
	{
		return -1;
	}

	//Wait a minute, will NPY_ARRAY_INOUT_ARRAY allow
	//me to keep modifying the underlying obj?

	arr1 = PyArray_FROM_OTF(
		arg1,
		NPY_FLOAT32,
		NPY_ARRAY_IN_ARRAY //OUT_ARRAY
	);

	if (arr1 == NULL)
	{
		return -1;
	}

	float *data;
	int n_dims = PyArray_NDIM(arr1);
	npy_intp *dims = PyArray_DIMS(arr1);
	npy_intp d[1];

	arr1 = PyArray_Ravel(arr1, NPY_CORDER);

	if (PyArray_AsCArray(
		&arr1,
		&data,
		d,
		1,
		PyArray_DescrFromType(NPY_FLOAT32) //But ref is stolen so do I need inc?
	) < 0) //Or just not 0 ?
	{
		Py_DECREF(arr1); //Or X incase Ravel F's up?
		return -1;
	}

	if (self->matrix != NULL)
	{
		//Do I need to do something to the array since its underlying data is going or ....
		delete_matrix(self->matrix);
	}

	self->matrix = new_matrix(data, n_dims, dims); //Copies data

	PyArray_Free(arr1, data);
	Py_DECREF(arr1);

	return FalpyWeights_setup_array(self, n_dims, dims);
}

FALPY_LOCAL_SYMBOL PyObject*
FalpyWeights_str(FalpyWeightsObject *self)
{
	size_t size;
	const char *str = matrix_as_string(self->matrix, &size);
	PyObject *pyStr = PyUnicode_FromStringAndSize(str, size);
	free(str);
	return pyStr;
}

FALPY_LOCAL_SYMBOL PyObject*
FalpyWeights_getarray(FalpyWeightsObject *self, void *closure)
{
	Py_INCREF(self->npy_array);
	return self->npy_array;
}

//TODO: Clearly somethin wrong here (like when 
//[0.00117184, -0.00108006, -9.17783e-05] in C becomes
//array([4.e-45, 0.e+00, 6.e-45], dtype=float32)

FALPY_LOCAL_SYMBOL int
FalpyWeights_setup_array(FalpyWeightsObject *self, int n_dims, npy_intp *dims)
{
	//Does base member need to be set? The object it beclongs to already contains this data.
	//Also which object needs its ref incremented (I assume the one who wns the data)
	PyObject *weight_array = 
		PyArray_SimpleNewFromData(n_dims, dims, NPY_FLOAT32, get_matrix_data(self->matrix));

	PyObject *tmp = self->npy_array;
	self->npy_array = weight_array;
	Py_XDECREF(tmp);

	return 0;
}

FALPY_LOCAL_SYMBOL void
FalpyWeights_dealloc(FalpyWeightsObject *self)
{
	Py_XDECREF(self->npy_array);
	if (self->matrix != NULL)
	{
		delete_matrix(self->matrix);
	}

	Py_TYPE(self)->tp_free((PyObject*)self);
}