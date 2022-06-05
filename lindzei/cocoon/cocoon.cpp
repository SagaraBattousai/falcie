
#include <cstdint>
#include <iostream>


import <vector>;

import pulse;
import lindzei;


int main(void)
{
	lindzei::Federatedblock genisis{};

	genisis.Hash();

	//std::uint32_t tb = 0x20FFFFFF;

	pulse::Target t(0x20FFFFFF);

	for (auto& i: t.Expand() )
		std::cout << std::hex << (int)i;

	std::cout << std::endl;



	/*
	federated_blockchain_t *blockchain = new_federated_blockchain(genisis);

	federatedblock_t b1 = new_federated_block();

	add(blockchain, b1);

	float training_data[4][2] = { { 0.f, 0.f }, {1.f, 0.f }, { 0.f, 1.f }, { 1.f, 1.f } };
	float *training_ptr[4] = { 	training_data[0],  training_data[1],  training_data[2],  training_data[3] };

	float desired_outputs[4] = { 0.f, 1.f, 1.f, 0.f };
	float *desired_ptr[4] = { &desired_outputs[0],  &desired_outputs[1],  &desired_outputs[2],  &desired_outputs[3]};

	float *training_data1[2] = { training_ptr[0], training_ptr[1] };

	float *training_data2[2] = { training_ptr[2], training_ptr[3] };

	float *desired_outputs1[2] = { desired_ptr[0], desired_ptr[1] };
	float *desired_outputs2[2] = { desired_ptr[2], desired_ptr[3] };


	float learning_rate = 0.9;

	int max_epochs = 50000;
	float target_error = 0.00001;

	int64_t no_inputs = 2;
	int64_t no_output = 1;

	int64_t no_layers = 2;
	int64_t nd[2] = { 3, 1 };

	int64_t no_training_data = 2; //each
	int64_t network_dims[3] = { 2, 3, 1 };

	//unsigned seed = 0x45000000;

	neural_network_t *network1 = 
		new_neural_network(no_inputs, no_layers, nd,
			&act, &dact, learning_rate, NULL);

	neural_network_t *network2 = new_neural_network(no_inputs, no_layers, nd,
		&act, &dact, learning_rate, NULL);

	neural_network_t *network_control = new_neural_network(no_inputs, no_layers,
		nd, &act, &dact, learning_rate, NULL);

	float **weights1;
	float **weights2;

	network_update_t *combined_update;

	federatedblock_t b2;

	for (int i = 0; i < 500; i++)
	{
		train_network(desired_outputs1, training_data1, no_training_data, 1, network1);
		train_network(desired_outputs2, training_data2, no_training_data, 1, network2);
		train_network(desired_ptr, training_ptr, 4, 1, network_control);

		get_weight(network1, &weights1);
		get_weight(network2, &weights2);

		network_update_t network1_update = {
			.num_layers = no_layers,
			.network_dims = network_dims,
			.delta_weights = weights1,
			.examples_seen = no_training_data
		};

		network_update_t network2_update = {
			.num_layers = no_layers,
			.network_dims = network_dims,
			.delta_weights = weights2,
			.examples_seen = no_training_data
		};

		b2 = new_federated_block();

		add_local_update(&b2, &network1_update);
		add_local_update(&b2, &network2_update);

		add(blockchain, b2);

		federatedblock_t last = get_last(blockchain);

		get_global_update(&last, &combined_update);

		set_weight(network1, combined_update->delta_weights);
		set_weight(network2, combined_update->delta_weights);

	}

	printf("\nNetwork 1 on all data:\n");
	print_inferencing_results(network1, training_ptr, 4,
		2, desired_ptr, 1);

	printf("\nControl Network on all data:\n");
	print_inferencing_results(network_control, training_ptr, 4,
		2, desired_ptr, 1);

		*/

	return 0;
}