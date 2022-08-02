#ifndef __FALPY_TF_MODEL_MATRIX_H__
#define __FALPY_TF_MODEL_MATRIX_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL FALPY_ARRAY_API
#include <numpy/arrayobject.h>

#include <falpy/_falpyconfig.h>

typedef struct FALPY_LOCAL_SYMBOL falpy_matrix
{
  PyArrayObject npy_array;

} falpy_matrix_t;


FALPY_LOCAL_SYMBOL PyObject*
falpy_model_matrix(PyObject *self, PyObject *args);

//FALPY_LOCAL_SYMBOL PyObject*
//falpy_


#endif