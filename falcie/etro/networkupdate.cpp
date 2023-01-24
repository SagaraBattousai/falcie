
#include <etro/networkupdate.h>
#include <cstring>

network_update_t* new_network_update()
{
	return new lindzei::NetworkUpdate();
}

void set_network_update_examples_seen(network_update_t *update, std::int64_t examples_seen)
{
	update->examples_seen = examples_seen;
}

std::int64_t get_network_update_examples_seen(network_update_t *update)
{
	return update->examples_seen;
}

int add_matrix_to_update(network_update_t *update, matrix_t *matrix)
{
	//Hopfully copy ctor
	update->delta_weights.emplace_back(*matrix);

	return 0;
}

int get_network_update_weights(const network_update_t *update, matrix_t ***matrices, std::int64_t *count)
{
	*count = update->delta_weights.size();
	*matrices = new (matrix_t*[*count]);
	for (std::int64_t i = 0; i < *count; i++) //Would const auto ref be better?
	{
		//Calls Copy Ctor
		(*matrices)[i] = new matrix_t(update->delta_weights[i]);
	}

	return 0;
}


void delete_network_update(network_update_t *update)
{
	//TODO: Do I need to delete all matrices in delta_weights? are they ptrs??
	//for(auto& m : update->delta_weights)
	delete update;
}