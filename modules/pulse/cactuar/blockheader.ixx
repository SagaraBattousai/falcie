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

import :encoding;
import :crypto;
import :target;

namespace pulse
{
	namespace cactuar
	{
		using time_rep = uint64_t;
		using time_period = std::micro;
		using time_duration = std::chrono::duration<time_rep, time_period>;
	} //namespace pulse::cactuar

} //namespace pulse

export namespace pulse
{
	cactuar::time_rep GenerateTimestamp()
	{
		return std::chrono::duration_cast<cactuar::time_duration>
			(std::chrono::tai_clock::now().time_since_epoch()).count();
	}

	template <HashAlgorithm hash_algo>
	class Blockheader;

	template <HashAlgorithm hash_algo>
	using blockhash_type = std::array<std::byte, AsHashSize(hash_algo)>;
	//using blockhash_type = std::array<unsigned char, AsHashSize(hash_algo)>;

	template <HashAlgorithm hash_algo>
	using BlockHashFunction = std::function<const blockhash_type<hash_algo>(const Blockheader<hash_algo>&)>;

	/**
	 * @brief Default BlockHashFunction; returns the double hash (of type "hash_algo")
	 * of the Blockheader's BlockheaderState.
	 *
	 */
	template <HashAlgorithm hash_algo>
	const blockhash_type<hash_algo> PulseHash(const Blockheader<hash_algo>&);

	template <HashAlgorithm hash_algo>
	class Blockheader
	{
	public:
		Blockheader(BlockHashFunction<hash_algo> = PulseHash<hash_algo>, uint32_t = 0x01);

		//Good candidate for inline as just a function call
		inline const blockhash_type<hash_algo> Hash() const { return this->hashfunc(*this); };

		//const hash_array& PrevHash() const { return this->prev_hash; };
		/*
		void Mine(const hash_array prev_hash)
		{
			this->prev_hash = prev_hash;
			this->timestamp = GenerateTimestamp();

			//TODO: add error for invalid cactaur values
			std::vector<unsigned char> difficulty_array = ExpandTarget(this->target);

			hash_array currHash = this->HashFunction();

			while (currHash > difficulty_array)
			{
				this->nonce += 1;
				currHash = this->HashFunction();
			}
		}
		*/
		//private:
			//TODO: ^^handle this differently.
		struct BlockheaderState
		{
			//TODO:add constructor back as way way safer!! aka allows make unique
			//temp
			//BlockheaderState(uint32_t version);
			uint32_t version;
			int64_t timestamp;
			blockhash_type<hash_algo> prev_hash;
			blockhash_type<hash_algo> transaction_hash;
			uint32_t target;
			uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
		};

		// Works as a raw pointer but not (yet) as unique
		std::unique_ptr<BlockheaderState> state;
		//BlockheaderState *state;
		BlockHashFunction<hash_algo> hashfunc;

	public:
		//TODO: handle this differently. (probably not forward delc in public space)
		//header almost certainly less than 2gb in size
		static constexpr int state_size = sizeof(BlockheaderState);
	};
/*
	template <HashAlgorithm hash_algo>
	Blockheader<hash_algo>::BlockheaderState::BlockheaderState(uint32_t version) : version(version), nonce(0) {};
	*/

	template <HashAlgorithm hash_algo>
	Blockheader<hash_algo>::Blockheader(BlockHashFunction<hash_algo> hashfunc, uint32_t version)
		: hashfunc(hashfunc)
		//, state(std::make_unique<BlockheaderState>(version))
		//, state(std::make_unique<BlockheaderState>(BlockheaderState{ .version = version, .nonce = 0 }))
		, state(std::unique_ptr<BlockheaderState>(new BlockheaderState{ .version = version, .nonce = 0 }))
	{
	}


	//may need base ..... One day, remember 80/20 rule //actually don't think I can do base
	template <HashAlgorithm hash_algo>
	const blockhash_type<hash_algo> PulseHash(const Blockheader<hash_algo>& header)
	{
		//hash_array hash(HashAlgorithm::SHA256);
		//HashFunction<hash_algo> blockheader_hashfunc{};

		//may need to copy due to encoding
		//std::span<unsigned char, Blockheader<hash_algo>::state_size> data{ header.state.get(), Blockheader<hash_algo>::state_size};

		//std::span<const std::byte, 88> s{ reinterpret_cast<const std::byte*>(header.state.get()), 88 };


		std::span<std::byte> data = std::as_writable_bytes(
			std::span<Blockheader<hash_algo>::BlockheaderState>{header.state.get(), 1}
		);
		//std::array<unsigned char, Blockheader<hash_algo>::state_size> data{};
		

		//std::memcpy(data.data(), header.state.get(), Blockheader<hash_algo>::state_size);
		//std::memcpy(data.data(), reinterpret_cast<unsigned char*>(bs), Blockheader<hash_algo>::state_size);

		//EncodeData({ data });

		//std::span<unsigned char, Blockheader<hash_algo>::state_size>
			//data(header.state.get());// , Blockheader<hash_algo>::state_size);

		//EncodeData(data);


		//Change to operator() instead of HashFunction ?!!
		//return  HashFunction<hash_algo>{}({ data.data(), data.size() });
		return  HashFunction<hash_algo>{}(data);
		//return  HashFunction<hash_algo>{}({ data.data(), Blockheader<hash_algo>::state_size });
	}



} //namespace pulse

