#ifndef __FALPY_FEDERATED_NETWORK_UPDATE_H__
#define __FALPY_FEDERATED_NETWORK_UPDATE_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <falpy/_falpyconfig.h>

#include <etro/networkupdate.h>


typedef struct FALPY_LOCAL_SYMBOL FalpyNetworkUpdateObject
{
  PyObject_HEAD
    network_update_t *update;

} FalpyNetworkUpdateObject;

FALPY_LOCAL_SYMBOL extern PyTypeObject FalpyNetworkUpdateType;

FALPY_LOCAL_SYMBOL int
FalpyNetworkUpdate_init(FalpyNetworkUpdateObject *self, PyObject *args, PyObject *kwds);

FALPY_LOCAL_SYMBOL PyObject* //METH_VARARGS <- makes it easier to assert type //METH_O //or fast?
FalpyNetworkUpdate_set_examples_seen(FalpyNetworkUpdateObject *self, PyObject *args);

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyNetworkUpdate_get_examples_seen(FalpyNetworkUpdateObject *self, PyObject *Py_UNUSED(ignored));

FALPY_LOCAL_SYMBOL PyObject* //METH_VARARGS <- makes it easier to assert type //METH_O
FalpyNetworkUpdate_add_weight(FalpyNetworkUpdateObject *self, PyObject *args);

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyNetworkUpdate_get_update_weights(FalpyNetworkUpdateObject *self, PyObject *Py_UNUSED(ignored));

FALPY_LOCAL_SYMBOL void
FalpyNetworkUpdate_dealloc(FalpyNetworkUpdateObject *self);


#endif