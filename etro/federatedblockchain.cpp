
#include <iostream>
#include <utility>
#include <etro/federatedblock.h>
#include <etro/federatedblockchain.h>


federated_blockchain_t* new_federated_blockchain(federated_block_builder_t *genisis_builder, const int hash_size)
{
	lindzei::Federatedblock *genisis = build_genisis_block(genisis_builder);
	federated_blockchain_t *chain = new federated_blockchain_t(std::move(*genisis), hash_size);
	delete genisis;
	return chain;
}

void print_blockchain(federated_blockchain_t *chain)
{
	for (auto it = chain->begin(); it != chain->end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << std::string(68, '+') << std::endl;
}


int add_block_to_chain(federated_blockchain_t *chain, federated_block_t *block)
{
	chain->Add(std::move(*block)); //Moves data or is it still allocated? TODO:!!

	//TODO: But leave commented for now to compile and avoid seg fault
	//delete block;
	//block = NULL; //NULL for C

	return 0;

}

int get_last_added_block(federated_blockchain_t *chain, const federated_block_t **block)
{
	//*block = &(chain->GetLast());
	const federated_block_t& last = chain->GetLast();
	*block = new federated_block_t(last); //^^ forces copy ctor and not move 

	return 0;
}


void delete_federated_blockchain(federated_blockchain_t *chain)
{
	delete chain;
}




