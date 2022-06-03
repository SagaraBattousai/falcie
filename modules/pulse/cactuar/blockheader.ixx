module;

#include <cstdint>
#include <cstring>

export module cactuar:blockheader;

import <vector>;
import <array>;
import <span>;
import <chrono>;
import <ratio>;
import <functional>;
import <memory>;

import :block;
import :encoding;
import :crypto;
import :target;

export namespace pulse
{
	template <HashAlgorithm hash_algo>
	class Blockheader
	{
	public:

		using blockhash_type = std::array<std::byte, AsHashSize(hash_algo)>;

		using BlockHashFunction = std::function<const blockhash_type(const Blockheader&)>;

		Blockheader(uint32_t = 0x01);

		Blockheader(BlockHashFunction, uint32_t = 0x01);

		//Good candidate for inline as just a function call
		inline const blockhash_type Hash() const { return this->hashfunc(*this); };

		const blockhash_type& PrevHash() const { return this->prev_hash; };

		void Mine(const blockhash_type prev_hash);

		

		struct BlockheaderState
		{
			//TODO:add constructor back as way way safer!! aka allows make unique
			//temp
			//BlockheaderState(uint32_t version);
			uint32_t version;
			int64_t timestamp;
			blockhash_type prev_hash;
			blockhash_type transaction_hash;
			uint32_t target;
			uint32_t nonce;
		};

		//or reference return and then get address?
		const BlockheaderState* const State() const
		{
			return this->state.get();
		};


	private:
		std::unique_ptr<BlockheaderState> state;

		BlockHashFunction hashfunc;
	};


	/**
	 * @brief Default BlockHashFunction; returns the double hash (of type "hash_algo")
	 * of the Blockheader's BlockheaderState.
	 */
	template <HashAlgorithm hash_algo>
	const typename Blockheader<hash_algo>::blockhash_type PulseHash(const Blockheader<hash_algo>&);

	template <HashAlgorithm hash_algo>
	Blockheader<hash_algo>::Blockheader(uint32_t version)
		: Blockheader(PulseHash<hash_algo>, version)
	{
	}



	template <HashAlgorithm hash_algo>
	Blockheader<hash_algo>::Blockheader(Blockheader<hash_algo>::BlockHashFunction hashfunc, uint32_t version)
		: hashfunc(hashfunc)
		//, state(std::make_unique<BlockheaderState>(version))
		, state(std::unique_ptr<BlockheaderState>(new BlockheaderState{ .version = version, .nonce = 0 }))
	{
	}

	template <HashAlgorithm hash_algo>
	void Blockheader<hash_algo>::Mine(const blockhash_type prev_hash)
	{
		this->prev_hash = prev_hash;
		this->timestamp = Block::GenerateTimestamp();

		//TODO: add error for invalid cactaur values
		std::vector<unsigned char> difficulty_array = ExpandTarget(this->target);

		blockhash_type currHash = this->Hash();

		while (currHash > difficulty_array)
		{
			this->nonce += 1;
			currHash = this->Hash();
		}
	}


	template <HashAlgorithm hash_algo>
	const typename Blockheader<hash_algo>::blockhash_type PulseHash(
		const Blockheader<hash_algo>& header)
	{
		std::span<const std::byte> bytes = std::as_bytes(std::span{ header.State(), 1});

		std::vector<std::byte> data = EncodeData(bytes);

		return (HashFunctionPool<hash_algo>::Instance())({ data.data(), data.size() });
	}



} //namespace pulse

