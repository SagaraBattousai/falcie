module;

#include <cstdint>
#include <cstddef>

export module cactuar:blockheader;

import <vector>;
import <algorithm>;
import <utility>;

import :block;
import :crypto;
import :encoding;
import :target;

export namespace pulse
{
	struct Blockheader
	{
	public:
		std::uint32_t version;
		std::int64_t timestamp;
		std::vector<std::byte> prev_hash;
		std::vector<std::byte> transaction_hash;
		Target target;
		std::uint32_t nonce;

		//I hate giant constructors but somewhat needed for std::make_unique
		constexpr Blockheader(
			std::uint32_t version = 0x01,
			std::int64_t timestamp = -1,
			std::vector<std::byte> prev_hash = std::vector<std::byte>{},
			std::vector<std::byte> transaction_hash = std::vector<std::byte>{},
			Target target = {},
			std::uint32_t nonce = 0);

		constexpr Blockheader(Blockheader&&);

		//constexpr 
		static Blockheader Genisis(HashAlgorithm hash_algo, std::uint32_t version = 0x01);
	};

	constexpr Blockheader::Blockheader(std::uint32_t version, std::int64_t timestamp,
		std::vector<std::byte> prev_hash, std::vector<std::byte> transaction_hash,
		Target target, std::uint32_t nonce)
		: version{ version }
		, timestamp{ timestamp }
		, prev_hash{ prev_hash }
		, transaction_hash{ transaction_hash }
		, target{ target }
		, nonce{ nonce }
	{ }

	constexpr Blockheader::Blockheader(Blockheader&& rhs)
		: version{ std::move(rhs.version) }
		, timestamp{ std::move(rhs.timestamp) }
		, prev_hash{ std::move(rhs.prev_hash) }
		, transaction_hash{ std::move(rhs.transaction_hash) }
		, target{ std::move(rhs.target) }
		, nonce{ std::move(rhs.nonce) }
	{ }


	//Change later when you have time to call constructor
	//constexpr 
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


	constexpr const int BlockheaderSize(HashAlgorithm algo)
	{
		//return sizeof(Blockheader) + 2 * (AsHashSize(algo) - sizeof(std::vector<std::byte>));
		return 20 + 2 * AsHashSize(algo);
		//^^ for some reason I was getting 88 as sizeof(Blockheader) instead of 84
	}

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

} //namespace pulse
