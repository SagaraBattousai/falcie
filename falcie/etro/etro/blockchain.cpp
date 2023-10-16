
#include <iostream>
#include <utility>
#include <etro/block.h>
#include <etro/blockchain.h>

blockchain_t* new_blockchain()
{
	return new blockchain_t();
}

void print_blockchain(blockchain_t *chain)
{
	for (auto it = chain->begin(); it != chain->end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << std::string(68, '+') << std::endl;
}


int add_block_to_chain(blockchain_t *chain, block_t *block)
{
	chain->Add(std::move(*block)); //Moves data or is it still allocated? TODO:!!

	//TODO: But leave commented for now to compile and avoid seg fault
	//delete block;
	//block = NULL; //NULL for C

	return 0;

}

int get_last_added_block(blockchain_t *chain, const block_t **block)
{
	//*block = &(chain->GetLast());
	const block_t& last = chain->GetLast();
	*block = new block_t(last); //^^ forces copy ctor and not move 

	return 0;
}


void delete_blockchain(blockchain_t *chain)
{
	delete chain;
}




