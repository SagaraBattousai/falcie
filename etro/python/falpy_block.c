
#include <stdint.h>

//#include <falpy/falpy_weights.h>
#include <falpy/falpy_block.h>

//////////////////
/// SETUP TYPE ///
//////////////////

PyMethodDef FalpyBlockBuilder_methods[] = {
	{"build", (PyCFunction)FalpyBlockBuilder_build, METH_NOARGS,
		 "Build Block using builder's settings."
	},
	//Should make only work once in lindzei
	{"genisis", (PyCFunction)FalpyBlockBuilder_genisis, METH_NOARGS,
		 "Build Genisis Block using builder's settings."
	},
	{NULL} //Sentinel
};

PyTypeObject FalpyBlockBuilderType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "falpy.BlockBuilder",
	.tp_doc = PyDoc_STR("falpy BlockBuilder"),
	.tp_basicsize = sizeof(FalpyBlockBuilderObject),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_new = PyType_GenericNew,
	.tp_init = (initproc)FalpyBlockBuilder_init,
	.tp_dealloc = (destructor)FalpyBlockBuilder_dealloc,
	.tp_methods = FalpyBlockBuilder_methods,
};


static PyMethodDef FalpyBlock_methods[] = {
	{"add_local_update", (PyCFunction)FalpyBlock_add_local_update, METH_O,
		 "Add a local update to a block."
	},
	{"get_global_update", (PyCFunction)FalpyBlock_get_global_update, METH_NOARGS,
		 "Return the global update associated with this block."
	},
	{NULL} //Sentinel
};

PyTypeObject FalpyBlockType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		.tp_name = "falpy.Block",
		.tp_doc = PyDoc_STR("falpy Block"),
		.tp_basicsize = sizeof(FalpyBlockObject),
		.tp_itemsize = 0,
		.tp_flags = Py_TPFLAGS_DEFAULT,
		.tp_new = PyType_GenericNew,
	//.tp_init = (initproc)FalpyBlockBuilder_init,
	.tp_dealloc = (destructor)FalpyBlock_dealloc,
	.tp_str = FalpyBlock_str,
	//.tp_members = FalpyWeights_members,
	.tp_methods = FalpyBlock_methods,
};


////////////////////////////
/// Function Definitions ///
////////////////////////////

FALPY_LOCAL_SYMBOL PyObject* //METH_O
FalpyBlock_add_local_update(FalpyBlockObject *self, FalpyNetworkUpdateObject *update)
{
	add_local_update(self->block, update->update); //TODO: Null checks? (tho func should check them)
	Py_RETURN_NONE;
}

FALPY_LOCAL_SYMBOL PyObject*
FalpyBlock_str(FalpyBlockObject *self)
{
	size_t size;
	const char *str = block_as_string(self->block, &size);
	PyObject *pyStr = PyUnicode_FromStringAndSize(str, size);
	free(str);
	return pyStr;
}

FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlock_get_global_update(FalpyBlockObject *self, PyObject *Py_UNUSED(ignored))
{
	FalpyNetworkUpdateObject *update =
		(FalpyNetworkUpdateObject *)PyType_GenericNew(&FalpyNetworkUpdateType, NULL, NULL);

	if (self->block == NULL)
	{
		//TODO: Set an exception
		printf("AHHHH");
	}
	get_global_update(self->block, &update->update); //TODO: Null checks? (tho func should check them)

	return update;
}

FALPY_LOCAL_SYMBOL void
FalpyBlock_dealloc(FalpyBlockObject *self)
{
	delete_block(self->block);

	Py_TYPE(self)->tp_free((PyObject *)self);

}


//For now, force SHA256 as the hasing algorithm.
FALPY_LOCAL_SYMBOL int
FalpyBlockBuilder_init(FalpyBlockBuilderObject *self, PyObject *args, PyObject *kwds)
{

	uint32_t version;
	uint32_t target;

	if (!PyArg_ParseTuple(args, "II", &version, &target))
	{
		return -1;
	}

	if (self->builder != NULL)
	{
		delete_builder(self->builder);
	}

	builder_params_t params = { .version = version, .target = target, .hash_algo = SHA256 };

	//TODO: check new succeeded.
	self->builder = new_federeated_block_builder(&params);

	return 0;

}

//inline federated_block_t* build_federated_block(federated_block_builder_t *builder);
FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockBuilder_build(FalpyBlockBuilderObject *self, PyObject *Py_UNUSED(ignored))
{
	PyTypeObject *type = &FalpyBlockType;
	FalpyBlockObject *block = (FalpyBlockObject *)type->tp_alloc(type, 0); //auto incref
	block->block = build_federated_block(self->builder);
	return block;

}

//inline federated_block_t* build_genisis_block(federated_block_builder_t *builder);
FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyBlockBuilder_genisis(FalpyBlockBuilderObject *self, PyObject *Py_UNUSED(ignored))
{
	PyTypeObject *type = &FalpyBlockType;
	FalpyBlockObject *genisis = (FalpyBlockObject *)type->tp_alloc(type, 0); //auto incref
	genisis->block = build_genisis_block(self->builder);
	return genisis;
}

FALPY_LOCAL_SYMBOL void
FalpyBlockBuilder_dealloc(FalpyBlockBuilderObject *self)
{
	if (self->builder != NULL)
	{
		delete_builder(self->builder);
	}
	Py_TYPE(self)->tp_free((PyObject *)self);

}