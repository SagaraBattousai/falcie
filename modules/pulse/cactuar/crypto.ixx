module;

//May need to forwad declare and then use these in impl file
extern "C" {
#include <openssl/evp.h>
}

#include <array>


export module cactuar:crypto;

namespace pulse
{
	//Could be HashSize but could use this for hasher factory ...
	//Not named standard way as the hash_type's actual name is all caps.
	export enum class HashType : unsigned char { SHA256 = 32, RIPEMD160 = 20 };


	namespace cactuar
	{
		EVP_MD_CTX* GetHashContext(HashType hash_type);

		//TODO Remove base class as it just causes an extr level of indirection (tho check this)
		class BaseHasher;

		//TODO: Move to impl file
		class BaseHasher
		{
		protected:
			//I need to understand explicit better!
			BaseHasher(EVP_MD_CTX *context, unsigned char *data);

			//Could do this in derived class
			void Hash(const void *input_data, size_t count);

			void SetContext(EVP_MD_CTX *context) { this->context = context; }

		private:

			void UpdateHash(const void *input_data, size_t count)
			{ EVP_DigestUpdate(context, input_data, count); }

			void OutputHash()
			{ EVP_DigestFinal_ex(context, data, NULL); }

			EVP_MD_CTX *context;
			unsigned char *data;
		};

		BaseHasher::BaseHasher(EVP_MD_CTX *context, unsigned char *data) : context(context), data(data) {};

		//Could do this in derived class
		void BaseHasher::Hash(const void *input_data, size_t count)
		{
			
			EVP_DigestInit_ex2(this->context, NULL, NULL);
			UpdateHash(input_data, count);
			OutputHash();
			
		}



	} //namespace pulse::cactuar
} //namespace pulse

export namespace pulse
{

	//Cool Rationale
	//Dont want to over use templates but now am I under-using them?
	//Actually I quite like it as a way to explain that its a distinct hash_type, however in this case thats
	//not fullt true, its actually a sub class, though .... 
	//we can use sub classes too but mainly for bloat right...

	template <HashType hash_type>
	class Hasher//: private cactuar::BaseHasher
	{
	public:
		//Still need to be explicit?
		explicit Hasher();
		~Hasher();

		//using hash_size = static_cast<unsigned char>(hash_type);

		const std::array<unsigned char, static_cast<unsigned char>(hash_type)>
			Hash(const void *input_data, size_t count);
		//{ cactuar::BaseHasher::Hash(input_data, count); return hashed_data; };

	private:
		EVP_MD_CTX *context;
		std::array<unsigned char, static_cast<unsigned char>(hash_type)> hashed_data;
	};
	
	template <HashType hash_type>
	const std::array<unsigned char, static_cast<unsigned char>(hash_type)>
		Hasher<hash_type>::Hash(const void *input_data, size_t count)
	{
		EVP_DigestInit_ex2(this->context, NULL, NULL);
		EVP_DigestUpdate(this->context, input_data, count);
		EVP_DigestFinal_ex(this->context, this->hashed_data.data(), NULL);
		return hashed_data;
	}
	

	template <HashType hash_type>
	Hasher<hash_type>::Hasher() 
		: context(cactuar::GetHashContext(hash_type))
		, hashed_data()
		//, BaseHasher(nullptr, hashed_data.data())
		//, BaseHasher(context, hashed_data.data())
	{
	//	BaseHasher::SetContext(this->context);
	}

	template <HashType hash_type>
	Hasher<hash_type>::~Hasher()
	{
		EVP_MD_CTX_free(this->context);
	}
	
}
/*
* //#include <stdint.h>

#include <pulse/cactuar.h>

*/
/*
typedef struct sha256_hash {
	unsigned char hash[SHA256];
}sha256hash_t;

typedef struct ripemd160_hash {
	unsigned char size = RIPEMD160;
	unsigned char hash[RIPEMD160];
}ripemd160hash_t;
*/

/*
typedef struct hash
{
	int size;
	unsigned char *hash;
} hash_t;
*/

/*
typedef struct hasher hasher_t;


hasher_t* get_default_sha256_hasher();
*/
/** size_t required for compatability with OpenSSL */
//inline void update_hash(hasher_t* hasher, const void* data, size_t count);

/** unsigned int required for compatability with OpenSSL */
//inline void output_hash(hasher_t* hasher, unsigned char* dst, unsigned int* size);


//Potentiall make inline, combo of the two above sans the func calls.
/** size_t, unsigned int required for compatability with OpenSSL */

/*
void hash_data(hasher_t* hasher, const void* src, size_t src_count,
	unsigned char* dst, unsigned int* dst_size);

void free_hasher(hasher_t* hasher);



#include <stdlib.h>

#include <cactuar/crypto.h>

typedef struct hasher 
{
	EVP_MD_CTX* hash_context;
}hasher_t;


//Not thread safe
hasher_t* get_default_sha256_hasher()
{
	static hasher_t* sha256 = NULL;
	if (sha256 == NULL)
	{
		sha256 = (hasher_t*)malloc(sizeof(struct hasher));
		if (sha256 == NULL)
		{
			return NULL;
		}
		sha256->hash_context = EVP_MD_CTX_new();
		EVP_DigestInit_ex2(sha256->hash_context, EVP_sha256(), NULL);
	}
	//TODO: Since first cond will only happen on first call is it better to not use else and 
	//just double init on first instansiation or add extra stack member (hash_type)
	else 
	{
		EVP_DigestInit_ex2(sha256->hash_context, NULL, NULL);
	}

	return sha256;
}

void update_hash(hasher_t* hasher, const void *data, size_t count)
{
	EVP_DigestUpdate(hasher->hash_context, data, count);
}

void output_hash(hasher_t* hasher, unsigned char *dst, unsigned int *size)
{
	EVP_DigestFinal_ex(hasher->hash_context, dst, size);
}

void hash_data(hasher_t* hasher, const void* src, size_t src_count,
	unsigned char* dst, unsigned int* dst_size)
{
	EVP_DigestUpdate(hasher->hash_context, src, src_count);
	EVP_DigestFinal_ex(hasher->hash_context, dst, dst_size);
}

void free_hasher(hasher_t *hasher)
{
	EVP_MD_CTX_free(hasher->hash_context);
	free(hasher);
	hasher = NULL;
}

void free_hash(hash_t *hash)
{
	free(hash->hash);
	free(hash);
}

*/

/*
mdctx = EVP_MD_CTX_new();
EVP_DigestInit_ex2(mdctx, md, NULL);
EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
EVP_DigestFinal_ex(mdctx, md_value, &md_len);
EVP_MD_CTX_free(mdctx);
*/
