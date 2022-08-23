
#include <stdio.h>

#include <etro/matrix.h>
#include <etro/federatedblock.h>
#include <etro/federatedblockchain.h>
#include <etro/networkupdate.h>

int main(void)
{
	builder_params_t params = { .version = 0x01, .target = 0x21FFFFFF, .hash_algo = SHA256 };
	federated_block_builder_t *bb = new_federeated_block_builder(&params);

	federated_blockchain_t *bc = new_federated_blockchain(bb, 32);

	federated_block_t *b = build_federated_block(bb);

	network_update_t *nu1 = new_network_update();
	//TODO:try nd=1 as well or force in falpy t always be 2D min
	float w11[] = { 1,2,3,4,5,6,7,8,9 };
	intptr_t w11_d[] = { 1,9 };
	add_matrix_to_update(nu1, new_matrix(w11, 2, w11_d));
	float w12[] = { 10,11,12 };
	intptr_t w12_d[] = { 1,3 };
	add_matrix_to_update(nu1, new_matrix(w12, 2, w12_d));

	network_update_t *nu2 = new_network_update();
	float w21[] = { 2,4,6,8,10,12,14,16,18 };
	intptr_t w21_d[] = { 1,9 };
	add_matrix_to_update(nu2, new_matrix(w21, 2, w21_d));
	set_network_update_examples_seen(nu1, 4);

	float w22[] = { 24,22,20 };
	intptr_t w22_d[] = { 1,3 };
	add_matrix_to_update(nu2, new_matrix(w22, 2, w22_d));
	set_network_update_examples_seen(nu2, 2);

	add_local_update(b, nu1);
	add_local_update(b, nu2);

	add_block_to_chain(bc, b);

	federated_block_t *last = NULL;
	get_last_added_block(bc, &last);

	network_update_t *gu;
	get_global_update(last, &gu);
	printf("Examples seen: %lli\n", get_network_update_examples_seen(gu));


	matrix_t **matrices;
	int64_t count;
	//get_network_update_weights(gu, &matrices, &count);
	get_network_update_weights(gu, &matrices, &count);

	printf("Number of weights = %lli\n", count);

	float *data = get_matrix_data(matrices[0]);

	for (int i = 0; i < 9; i++)
	{
		printf("%f, ", data[i]);
	}

	printf("\n");

	float *data2 = get_matrix_data(matrices[1]);

	for (int i = 0; i < 3; i++)
	{
		printf("%f, ", data2[i]);
	}

	printf("\n");

	print_blockchain(bc);


	return 0;
}