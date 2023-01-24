
#include <falpy/falpy_blockchain.h>

//////////////////
/// Setup Type ///
//////////////////

PyMethodDef FalpyBlockchain_methods[] = {
	{"add", (PyCFunction)FalpyBlockchain_add, METH_O,
		 "Add Block To Chain"
	},
	{"print", (PyCFunction)FalpyBlockchain_print, METH_NOARGS,
		 "Prints every block added to the Chain"
	},
	{"get_last", (PyCFunction)FalpyBlockchain_get_last, METH_NOARGS,
		 "Add Block To Chain"
	},
	{NULL} //Sentinel
};

PyTypeObject FalpyBlockchainType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "falpy.Blockchain",
	.tp_doc = PyDoc_STR("falpy Blockchain"),
	.tp_basicsize = sizeof(FalpyBlockchainObject),
	.tp_itemsize = 0, //May want to make variable sized (almost certainly)
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_new = PyType_GenericNew,
	.tp_init = (initproc)FalpyBlockchain_init,
	.tp_dealloc = (destructor)FalpyBlockchain_dealloc,
	.tp_methods = FalpyBlockchain_methods,
};

////////////////////////////
/// Function Definitions ///
////////////////////////////

FALPY_LOCAL_SYMBOL int
FalpyBlockchain_init(FalpyBlockchainObject *self, PyObject *args, PyObject *kwds)
{
	FalpyBlockBuilderObject *block_builder = NULL;
	int hash_size;

	if (!PyArg_ParseTuple(args, "O!i", &FalpyBlockBuilderType, &block_builder, &hash_size))
	{
		return -1;
	}

	if (self->blockchain != NULL) //TODO: Proper new but for now I dont have time
		delete_federated_blockchain(self->blockchain);

	//Not possible (I think due to ParseTuple)
	if (block_builder == NULL) return -1;

	self->blockchain = new_federated_blockchain(block_builder->builder, hash_size);

	return 0;
}



FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockchain_print(FalpyBlockchainObject *self, PyObject *Py_UNUSED(ignored))
{
	print_blockchain(self->blockchain);
	Py_RETURN_NONE;
}

//int add_block_to_chain(federated_blockchain_t *chain, federated_block_t *block);
FALPY_LOCAL_SYMBOL PyObject* //METH_O
FalpyBlockchain_add(FalpyBlockchainObject *self, FalpyBlockObject *block)
{
	add_block_to_chain(self->blockchain, block->block);

	Py_RETURN_NONE;
}


//METH_FASTCALL
//(PyObject *self, PyObject *const *args, Py_ssize_t nargs);


//TODO: Couldve made it like a python lis (as in it carries a list of blocks) but that would
// negate the blockchain benefits, however thenre may be a hybrid approach
//int get_last_added_block(federated_blockchain_t *chain, const federated_block_t *block);
FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockchain_get_last(FalpyBlockchainObject *self, PyObject *Py_UNUSED(ignored))
{
	//Been doing this manually below, TODO: see if this is beytter
	//FalpyBlockObject *block = (FalpyBlockObject *)PyType_GenericNew(&FalpyBlockType, NULL, NULL);

	PyTypeObject *type = &FalpyBlockType;
	FalpyBlockObject *block = (FalpyBlockObject *)type->tp_alloc(type, 0); //auto incref

	get_last_added_block(self->blockchain, &block->block);

	return block;
}

FALPY_LOCAL_SYMBOL void
FalpyBlockchain_dealloc(FalpyBlockchainObject *self)
{
	//Must I check null before delete?
	if (self->blockchain != NULL)
	{
		delete_federated_blockchain(self->blockchain);
	}
	Py_TYPE(self)->tp_free((PyObject *)self);
}