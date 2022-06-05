module;

#include <cstdint>
#include <cstring>

export module cactuar:blockheader;

import <vector>;
//import <functional>;
//import <memory>;

import :block;
import :crypto;
import :target;

export namespace pulse
{
	class Blockheader : public Block
	{
	public:

		Blockheader(std::uint32_t = 0x01, std::uint32_t = 0x21000000, 
			HashAlgorithm = HashAlgorithm::SHA256);

		Blockheader(BlockHashFunction, std::uint32_t = 0x01, 
			std::uint32_t = 0x21000000, HashAlgorithm = HashAlgorithm::SHA256);

		static Blockheader Genisis(std::uint32_t = 0x01, HashAlgorithm = HashAlgorithm::SHA256);

		static Blockheader Genisis(BlockHashFunction hashfunc,
			std::uint32_t = 0x01, HashAlgorithm = HashAlgorithm::SHA256);

		//Good candidate for inline as just a function call
		virtual inline blockhash_type Hash() const override 
		{ return this->hashfunc(hash_algo, *this); };

		virtual const blockhash_type& PrevHash() const override
		{ return this->prev_hash; };

		virtual void Mine(const blockhash_type&& prev_hash) override;

		virtual std::vector<std::byte> State() const override;

	private:

		Blockheader(BlockHashFunction, 
			uint32_t, int64_t&&, 
			blockhash_type&&, blockhash_type&&, 
			uint32_t&&, uint32_t&&, HashAlgorithm);

		Block::BlockHashFunction hashfunc;
		HashAlgorithm hash_algo;

		uint32_t version;
		int64_t timestamp;
		blockhash_type prev_hash;
		blockhash_type transaction_hash;
		Target target;
		uint32_t nonce;

		constexpr const int StateSize() const
		{
			return sizeof(uint32_t) + sizeof(int64_t) + (2 * AsHashSize(this->hash_algo)) +
				sizeof(uint32_t) + sizeof(uint32_t);
		};

		//static constexpr 
	};

} //namespace pulse

