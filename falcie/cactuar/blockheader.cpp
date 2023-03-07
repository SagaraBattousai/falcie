#include <utility>

#include <cactuar/cactuar-blockheader.h>
#include <cactuar/cactuar-block.h>
#include <cactuar/cactuar-endianness.h>


namespace cactuar
{
	//constexpr Blockheader::Blockheader(std::uint32_t version, std::int64_t timestamp,
	//	std::vector<std::byte> prev_hash, std::vector<std::byte> transaction_hash,
	//	cactuar::Target target, std::uint32_t nonce)
		

	std::vector<std::byte> Blockheader::State() const
	{
		std::vector<std::byte> bytes(
			4 +
			8 +
			this->prev_hash.size() +
			this->transaction_hash.size() +
			4 +
			4
		);

		auto it = bytes.begin();

		it = cactuar::CopyDataAsBytes(this->version, it);
		it = cactuar::CopyDataAsBytes(this->timestamp, it);

		//TODO: try overloading with vector or pointer?
		it = std::copy_n(reinterpret_cast<const std::byte*>(
			this->prev_hash.data()), this->prev_hash.size(),
			it
		);
		it = std::copy_n(reinterpret_cast<const std::byte*>(
			this->transaction_hash.data()), this->transaction_hash.size(),
			it
		);
		it = cactuar::CopyDataAsBytes(this->target, it);
		it = cactuar::CopyDataAsBytes(this->nonce, it);

		return bytes;
	}

	std::ostream& operator<<(std::ostream& os, const Blockheader& header)
	{
		os << "Version:          " << std::dec << header.version << "\n"
			  //"Timestamp:        " << Blockheader::AsTimePoint(header.timestamp) << "\n"
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


} // namespace cactuar
