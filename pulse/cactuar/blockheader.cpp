
#include <vector>

#include <cactuar/cactuar-blockheader.h>
#include <cactuar/cactuar-crypto.h>
#include <cactuar/cactuar-encoding.h>

//export
namespace pulse
{
	std::vector<std::byte> BlockheaderState(const Blockheader* const header, HashAlgorithm algo)
	{
		std::vector<std::byte> bytes(BlockheaderSize(algo));
		auto it = bytes.begin();

		//it = std::copy_n(reinterpret_cast<const std::byte*>(&header.version), sizeof(uint32_t), it);
		it = CopyDataAsBytes(header->version, it);
		it = CopyDataAsBytes(header->timestamp, it);
		//TODO: try overloading with vector or pointer?
		it = std::copy_n(reinterpret_cast<const std::byte*>(header->prev_hash.data()), AsHashSize(algo), it);
		it = std::copy_n(reinterpret_cast<const std::byte*>(header->transaction_hash.data()), AsHashSize(algo), it);
		it = CopyDataAsBytes(header->target, it);
		it = CopyDataAsBytes(header->nonce, it);

		return bytes;
	}
} // namespace pulse