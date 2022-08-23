#ifndef __FALPY_WEIGHTS_H__
#define __FALPY_WEIGHTS_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#include <numpy/ndarraytypes.h>

#include <falpy/_falpyconfig.h>

#include <etro/matrix.h>

//Local?
typedef struct FALPY_LOCAL_SYMBOL FalpyWeightsObject
{
  PyObject_HEAD
    PyArrayObject *npy_array;
  matrix_t *matrix;

} FalpyWeightsObject;

FALPY_LOCAL_SYMBOL extern PyTypeObject FalpyWeightsType;

FALPY_LOCAL_SYMBOL PyObject*
FalpyWeights_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

FALPY_LOCAL_SYMBOL int
FalpyWeights_init(FalpyWeightsObject *self, PyObject *args, PyObject *kwds);

FALPY_LOCAL_SYMBOL PyObject*
FalpyWeights_str(FalpyWeightsObject *self);

FALPY_LOCAL_SYMBOL int
FalpyWeights_setup_array(FalpyWeightsObject *self, int n_dims, npy_intp *dims);


FALPY_LOCAL_SYMBOL void
FalpyWeights_dealloc(FalpyWeightsObject *self);



FALPY_LOCAL_SYMBOL extern PyTypeObject FalpyWeightsType; //Probably should be in falpy.h

#endif