
#include <sstream>
#include <cstdlib>
#include <cstring>

#include <etro/block.h>

block_builder_t* new_block_builder(builder_params_t *params)
{
	block_builder_t *builder = new block_builder_t();

	builder->WithVersion(params->version).WithTarget(params->target);

	return builder;
}

block_t* build_block(block_builder_t *builder)
{
	return new block_t(builder->Build());
}

block_t* build_genisis_block(block_builder_t *builder)
{
	return new block_t(builder->Genisis());
}

const char* block_as_string(block_t *block, size_t *str_size)
{
	std::ostringstream oss;
	oss << *block << std::endl;
	std::string str = oss.str();
	*str_size = str.size();
	//malloc'd so it can be free'd from C
	char* ret = (char *) malloc((*str_size) * sizeof(char));

	if (ret != NULL)
	{
		//or &?
		std::memcpy(ret, str.c_str(), *str_size);
	}

	return ret;
}


//Shoul I change network update to be a unique ptr to the local update ....?
//Global should be shared or ....
int add_local_update(block_t *block, network_update_t *update)
{
	//network_update_t u(*update);
	block->AddLocalUpdate(*update); //Hoping that this will copy, will have to do mem profile

	//delete update, but leave for now to compile TODO:
	return 0; //fix
}

//Won't return since we may add error handling to the C++ true code
int get_global_update(block_t *block, network_update_t **update)
{
	*update = new network_update_t(block->GetGlobalUpdate());

	return 0; //TODO: unhardcode ^^ ptr check
}

void delete_builder(block_builder_t *builder)
{
	delete builder;
}


void delete_block(block_t *block)
{
	delete block;
}
