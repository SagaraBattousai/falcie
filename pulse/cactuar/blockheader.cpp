
//#include <vector>
//#include <ostream>

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

	//Change later when you have time to call constructor
	//Can't be constexpr with gcc but totaly chill with msvc :'(
    Blockheader Blockheader::Genisis(HashAlgorithm hash_algo, std::uint32_t version)
	{
		return Blockheader{
			version,
			Block::GenerateTimestamp(),
			std::vector<std::byte>(AsHashSize(hash_algo)),
			std::vector<std::byte>(AsHashSize(hash_algo)),
			0x00000000,
			0
		};
	}

	std::ostream& operator<<(std::ostream& os, const Blockheader& header)
	{
		os << "Version:          " <<std::dec << header.version << "\n"
			  "Timestamp:        " << Block::AsTimePoint(header.timestamp) << "\n"
			  "Previous Hash:    ";

		//std::Hex in loop incase something happens to IO ??
		for (const std::byte& x : header.prev_hash)
		{
			os << std::hex << static_cast<int>(x);
		}

		os << "\nTransaction Hash: ";

		for (const std::byte& x : header.transaction_hash)
		{
			os << std::hex << static_cast<int>(x);
		}

		os << "\nTarget:           ";

		os << std::hex << header.target << "\n";

		os << "Nonce:            " << header.nonce << std::endl;

		return os;
	}


} // namespace pulse
