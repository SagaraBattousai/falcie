
#include <cactuar/cactuar-hash_pool.h>
#include <cactuar/internal/cactuar-openssl_wrapper.h>

namespace pulse
{
	namespace cactuar
	{
		std::vector<std::array<EVP_MD_CTX*, 1>>
			HashContextPool::pool{ static_cast<int>(HashAlgorithm::AvailiableHashAlgorithms) };

		HashContextPool& HashContextPool::Instance()
		{
			if (instance.get() == nullptr)
			{
				instance.reset(new HashContextPool());
			}
			return *instance;
		};

		EVP_MD_CTX* HashContextPool::GetContext(HashAlgorithm hash_algo)
		{
			EVP_MD_CTX*& context = pool[static_cast<int>(hash_algo)][0];
			if (context == nullptr)
			{
				//this->pool[static_cast<int>(hash_algo)][0] = GetHashContext(hash_algo);
				context = GetHashContext(hash_algo);
			}
			return context;
		}
	} //namespace pulse::cactuar

} //namespace pulse