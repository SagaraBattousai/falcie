
//DEBUG
#include <stdio.h>
#include <etro/matrix.h>

#include <falpy/falpy_networkupdate.h>
#include <falpy/falpy_weights.h>

//////////////////
/// Setup Type ///
//////////////////

PyMethodDef FalpyNetworkUpdate_methods[] = {
	{"set_examples_seen", (PyCFunction)FalpyNetworkUpdate_set_examples_seen, METH_VARARGS,
		 "Sets the number of examples that this update has seen."
	},
	{"get_examples_seen", (PyCFunction)FalpyNetworkUpdate_get_examples_seen, METH_NOARGS,
		 "Gets the number of examples that this update has seen."
	},
	{"add_weight", (PyCFunction)FalpyNetworkUpdate_add_weight, METH_VARARGS,
		 "Adds a weight to the update."
	},
	{"get_weights", (PyCFunction)FalpyNetworkUpdate_get_update_weights, METH_NOARGS,
		 "Returns a list of weights from the update."
	},
	{NULL} //Sentinel
};

PyTypeObject FalpyNetworkUpdateType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		.tp_name = "falpy.NetworkUpdate",
		.tp_doc = PyDoc_STR("falpy NetworkUpdate"),
		.tp_basicsize = sizeof(FalpyNetworkUpdateObject),
		.tp_itemsize = 0,
		.tp_flags = Py_TPFLAGS_DEFAULT,
		.tp_new = PyType_GenericNew,
		.tp_init = (initproc)FalpyNetworkUpdate_init,
		.tp_dealloc = (destructor)FalpyNetworkUpdate_dealloc,
	//.tp_members = FalpyWeights_members,
	.tp_methods = FalpyNetworkUpdate_methods,
};

////////////////////////////
/// Function Definitions ///
////////////////////////////

FALPY_LOCAL_SYMBOL int
FalpyNetworkUpdate_init(FalpyNetworkUpdateObject *self, PyObject *args, PyObject *kwds)
{
	if (self->update != NULL)
	{
		delete_network_update(self->update);
	}

	self->update = new_network_update();

	return 0;
}

//void set_network_update_examples_seen(network_update_t *update, int64_t examples_seen);
FALPY_LOCAL_SYMBOL PyObject* //METH_VARARGS <- makes it easier to assert type //METH_O
FalpyNetworkUpdate_set_examples_seen(FalpyNetworkUpdateObject *self, PyObject *args)
{
	//TODO:: Decide if i should return the old examples_seen value
	int64_t examples_seen;

	if (!PyArg_ParseTuple(args, "L", &examples_seen))
	{
		return (PyObject*) -1;
	}

	if (self->update == NULL)
	{
		//TODO: Set error
		return (PyObject *) -1;
	}

	set_network_update_examples_seen(self->update, examples_seen);

	Py_RETURN_NONE;

}


FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyNetworkUpdate_get_examples_seen(FalpyNetworkUpdateObject *self, PyObject *Py_UNUSED(ignored))
{
	return PyLong_FromLongLong(get_network_update_examples_seen(self->update)); //TODO: NULL CHECK
}

//int add_matrix_to_update(network_update_t *update, matrix_t *matrix);
FALPY_LOCAL_SYMBOL PyObject* //METH_VARARGS <- makes it easier to assert type //METH_O
FalpyNetworkUpdate_add_weight(FalpyNetworkUpdateObject *self, PyObject *args)
{
	FalpyWeightsObject *weights;
	if (!PyArg_ParseTuple(args, "O!", &FalpyWeightsType, &weights))
	{
		return NULL;
	}

	if (self->update == NULL || weights->matrix == NULL)
	{
		//TODO: Set error
		return (PyObject*) - 1; //Msvc doesn't mind but Clang insists on cast
	}

	add_matrix_to_update(self->update, weights->matrix);

	Py_RETURN_NONE;
}


//int get_network_update_weights(network_update_t *update, matrix_t ***matrices, int64_t *count);
FALPY_LOCAL_SYMBOL PyObject* //METH_NOARGS
FalpyNetworkUpdate_get_update_weights(FalpyNetworkUpdateObject *self, PyObject *Py_UNUSED(ignored))
{
	matrix_t **matricies;
	int64_t count;

	if (get_network_update_weights(self->update, &matricies, &count) != 0)
	{
		//TODO: Set error
		return NULL;
	}

	PyObject *list = PyList_New((Py_ssize_t)count);
	for (int64_t i = 0; i < count; i++)
	{
		PyTypeObject *type = &FalpyWeightsType;
		FalpyWeightsObject *weight = (FalpyWeightsObject *)type->tp_alloc(type, 0);
		//^^ auto incref

		//Fairly certain VV is true MUST REDESIGN
		weight->matrix = matricies[i];

		///////////////////////////////////////
		//DEBUG
		//size_t x;
		//printf("&&\n$$\nThis is from networkupdate.c\n%s\n$$\n", matrix_as_string//(weight->matrix, &x));

		//size_t ndims = get_matrix_dimension_count(matricies[i]);
		//const intptr_t *dims = get_matrix_dimensions(matricies[i]);
		//printf("%lli, (%lli", ndims, dims[0]);
		//if (ndims > 1)
		//{
		//	printf(", %lli", dims[1]);
		//}			
		//printf(")\n&&\n");
		
		///////////////////////////////////////
		
		//TODO: Check with if
		FalpyWeights_setup_array(weight,
			get_matrix_dimension_count(matricies[i]),
			get_matrix_dimensions(matricies[i])
		);

		//if (PyList_SetItem(list, i, weight) != 0) //Don't want to discard I think (could carry)
		//{
		//	return NULL;
		//}

		PyList_SET_ITEM(list, i, weight); //Don't want to discard I think (could carry)

	}

	return list;
}




FALPY_LOCAL_SYMBOL void
FalpyNetworkUpdate_dealloc(FalpyNetworkUpdateObject *self)
{
	delete_network_update(self->update);
	Py_TYPE(self)->tp_free((PyObject *)self);

}