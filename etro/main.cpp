
#include <etro/chain.h>
#include <etro/blockchain.h>
#include <iostream>
#include <cstdint>
#include <cstring>

extern "C" {
#include <etro/crypto.h>
#include <openssl/evp.h>
#include <etro/blockheader.h>
#include <etro/math.h>
#include <etro/cactuar.h>
}

int main()
{
	Chain<int, 2> c = Chain<int, 2>();

	c.Add(1);
	c.Add(2);
	c.Add(9);
	c.Add(16);
	c.Add(125);
	c.Add(216);

	for (auto it = c.begin(); it != c.end(); ++it)
	{
		std::cout << *it << std::endl;
	}

	return 0;

}