#ifndef __FALPY_FEDERATED_BLOCKCHAIN_H__
#define __FALPY_FEDERATED_BLOCKCHAIN_H__

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <falpy/_falpyconfig.h>

#include <etro/blockchain.h>
#include <falpy/falpy_block.h>


typedef struct FALPY_LOCAL_SYMBOL FalpyBlockchainObject
{
  PyObject_HEAD
    blockchain_t *blockchain;

} FalpyBlockchainObject;

FALPY_LOCAL_SYMBOL extern PyTypeObject FalpyBlockchainType;

FALPY_LOCAL_SYMBOL int
FalpyBlockchain_init(FalpyBlockchainObject *self, PyObject *args, PyObject *kwds);

FALPY_LOCAL_SYMBOL PyObject* //METH_O
FalpyBlockchain_add(FalpyBlockchainObject *self, FalpyBlockObject *block);

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockchain_print(FalpyBlockchainObject *self, PyObject *Py_UNUSED(ignored));

//METH_FASTCALL
//(PyObject *self, PyObject *const *args, Py_ssize_t nargs);

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockchain_get_last(FalpyBlockchainObject *self, PyObject *Py_UNUSED(ignored));

FALPY_LOCAL_SYMBOL void
FalpyBlockchain_dealloc(FalpyBlockchainObject *self);


#endif