//module;

#ifndef __CACTUAR_CRYPTO__
#define __CACTUAR_CRYPTO__

//Bad to be in header as we want it to be private
//#include <openssl/evp.h>
#include <cstddef>

//export module cactuar:crypto;

#include <vector> //import <vector>;
#include <algorithm> //import <algorithm>;
#include <span> //import <span>;
#include <memory> //import <memory>;
#include <map> //import <map>;
#include <ostream>

typedef struct evp_md_ctx_st EVP_MD_CTX;

//export
namespace pulse
{
	enum class HashAlgorithm : unsigned char { SHA256 = 32, RIPEMD160 = 20 };

	//constexpr funcs are implicitly inline
	constexpr unsigned char AsHashSize(HashAlgorithm hash_algo)
	{
		return static_cast<unsigned char>(hash_algo);
	};

	std::ostream& operator<<(std::ostream&, const HashAlgorithm);

} // namespace pulse

namespace pulse
{
	namespace cactuar
	{
		EVP_MD_CTX* GetHashContext(HashAlgorithm hash_algo);

	} //namespace pulse::cactuar
} //namespace pulse

//export 
namespace pulse
{
	//Could make singleton and then use mutex's
	//template <HashAlgorithm hash_algo>
	class HashFunction
	{
	public:
		HashFunction(HashAlgorithm hash_algo);
		~HashFunction();

		using hash_type = std::vector<std::byte>;

		hash_type operator()(const std::span<const std::byte> input);

		/*
		* void UpdateHash(const void *input_data, size_t count)
		* 
		* void OutputHash()
		*/

	private:
		EVP_MD_CTX *context;
	};


	/*
	//TODO: make concurrent and make a true pool using things such as "in_use" and locks/seamaphores
	//template <HashAlgorithm hash_algo>
	template <HashAlgorithm hash_algo>
	class HashFunctionPool
	{
	public:
		static const HashFunction<hash_algo>& Instance()
		{
			if (instance.get() == nullptr)
			{
				instance.reset(new HashFunctionPool<hash_algo>());
			}
			return (instance->pool)[0];
		};
	private:

		//TODO: Should be auto initialized, else I'll switch to veccy
		//make more than 1 once seamaphores implmented.
		std::array<HashFunction<hash_algo>, 1> pool;

		static inline std::unique_ptr<HashFunctionPool<hash_algo>> instance =
			std::unique_ptr<HashFunctionPool>{ nullptr };
	};
	*/
} //namespace pulse

#endif