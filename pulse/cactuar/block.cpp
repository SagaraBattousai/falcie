
#include <vector>

#include <cactuar/cactuar-block.h>
#include <cactuar/cactuar-crypto.h>
#include <cactuar/cactuar-encoding.h>

//export
namespace pulse
{
	const typename Block::blockhash_type PulseHash(HashAlgorithm hash_algo, const Block& block)
	{
		std::vector<std::byte> bytes = block.State();
		std::vector<std::byte> data =
			EncodeData(std::span<const std::byte>{ bytes.data(), bytes.size() });

		auto hash_func = HashFunction(hash_algo);

		auto firstHash = hash_func(std::span<std::byte>{ data.data(), data.size() });

		return hash_func(std::span<std::byte>{ firstHash.data(), firstHash.size() });
	}
} // namespace pulse