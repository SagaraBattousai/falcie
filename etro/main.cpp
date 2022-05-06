
#include <etro/chain.h>
#include <etro/blockchain.h>
#include <etro/federated_blockchain.h>
#include <iostream>
#include <cstdint>
#include <cstring>

extern "C" {
#include <etro/crypto.h>
#include <etro/blockheader.h>
#include <etro/math.h>
#include <etro/cactuar.h>
}

int main()
{
	federatedblock_t gen = new_federated_block();
	federated_blockchain_t *c = new_federated_blockchain(gen);

	federatedblock_t first = new_federated_block();

	add(c, first);

	federatedblock_t second = new_federated_block();

	add(c, second);

	federatedblock_t third = new_federated_block();

	add(c, third);

	federatedblock_t forth = new_federated_block();

	add(c, forth);

	for (auto it = c->begin(); it != c->end(); ++it)
	{
		for (int i = 0; i < 32; i++)
		{
			std::cout << std::hex << (int)(*it).header.prev_hash.hash[i];
		}
		std::cout << std::endl;
		sha256hash_t currhash = hash(&(*it).header);
		for (int i = 0; i < 32; i++)
		{
			std::cout << std::hex << (int)currhash.hash[i];
		}
		std::cout << std::endl;
		std::cout << std::endl;

	}

	bool valid = c->Validate();

	if (valid)
	{
		std::cout << "True" << std::endl;
	}
	else
	{
		std::cout << "False" << std::endl;
	}

	return 0;

}