
//#include <etro/chain.h>
#include <iostream>
#include <cstdint>
#include <cstring>

extern "C" {
#include <etro/crypto.h>
#include <openssl/evp.h>
#include <etro/blockheader.h>
}

void encode_number(void* number, unsigned char* encoded, size_t size)
{
	unsigned char* src = (unsigned char*)number;

	size_t final_byte_index = size - 1;
	for (int i = 0; i < size; i++)
	{
		*(encoded + i) = *(src + (final_byte_index - i));
	}

}

unsigned char* convert_number(void* number, size_t size)
{
	unsigned char* encoded = (unsigned char*)number;


	size_t final_byte_index = size - 1;
	unsigned char tmp;
	for (int i = 0; i < size / 2; i++)
	{
		tmp = *(encoded + (final_byte_index - i));
		*(encoded + (final_byte_index - i)) = *(encoded + i);
		*(encoded + i) = tmp;
	}


	return encoded;
}



int main()
{
	/*
	auto ctx = get_default_sha256_context();
	int x = 37;
	unsigned char y[4] = { (x >> 24) & 255, (x >> 16) & 255, (x >> 8) & 255, x & 255 };
	unsigned char o1[32];
	unsigned char o2[32];
	EVP_DigestUpdate(ctx, &x, 4);
	EVP_DigestFinal_ex(ctx, &(o1[0]), NULL);

	ctx = get_default_sha256_context();
	EVP_DigestUpdate(ctx, &y, 4);
	EVP_DigestFinal_ex(ctx, o2, NULL);

	for (int i = 0; i < 32; i++)
	{
		std::cout << std::hex << (int)o1[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 32; i++)
	{
		std::cout << std::hex << (int)o2[i];
	}
	std::cout << std::endl;
	*/

	size_t size = 8;
	int64_t big = 0x37EFCDAB04030201;
	std::cout << std::hex << big << std::endl;

	void* number = &big;
	unsigned char enc[8];
	encode_number(number, enc, size);

	std::cout << std::hex << big << std::endl;

	std::cout << "0x";

	for (int i = 0; i < 8; i++)
	{
		std::cout << std::hex << (int)enc[i];
	}


	convert_number(number, size);

	std::cout << std::hex << big << std::endl;

	return 0;
}