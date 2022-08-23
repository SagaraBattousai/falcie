#ifndef __FALPY_FEDERATED_BLOCK_H__
#define __FALPY_FEDERATED_BLOCK_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <falpy/_falpyconfig.h>

#include <etro/federatedblock.h>
#include <falpy/falpy_networkupdate.h>


typedef struct FALPY_LOCAL_SYMBOL FalpyBlockObject
{
  PyObject_HEAD
    federated_block_t *block;

} FalpyBlockObject;

FALPY_LOCAL_SYMBOL extern PyTypeObject FalpyBlockType;

typedef struct FALPY_LOCAL_SYMBOL FalpyBlockBuilderObject
{
  PyObject_HEAD
    federated_block_builder_t *builder;

} FalpyBlockBuilderObject;

FALPY_LOCAL_SYMBOL extern PyTypeObject FalpyBlockBuilderType;


FALPY_LOCAL_SYMBOL PyObject* //METH_O
FalpyBlock_add_local_update(FalpyBlockObject *self, FalpyNetworkUpdateObject *update);

FALPY_LOCAL_SYMBOL PyObject*
FalpyBlock_str(FalpyBlockObject *self);

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlock_get_global_update(FalpyBlockObject *self, PyObject *Py_UNUSED(ignored));

FALPY_LOCAL_SYMBOL void
FalpyBlock_dealloc(FalpyBlockObject *self);


//For now, force SHA256 as the hasing algorithm.
FALPY_LOCAL_SYMBOL int
FalpyBlockBuilder_init(FalpyBlockBuilderObject *self, PyObject *args, PyObject *kwds);

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockBuilder_build(FalpyBlockBuilderObject *self, PyObject *Py_UNUSED(ignored));

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockBuilder_genisis(FalpyBlockBuilderObject *self, PyObject *Py_UNUSED(ignored));

FALPY_LOCAL_SYMBOL void
FalpyBlockBuilder_dealloc(FalpyBlockBuilderObject *self);


#endif