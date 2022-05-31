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

export namespace pulse
{
	class Blockheader;

	using hash_type = std::vector<unsigned char>;

	using BlockHasher = std::function<const hash_type(const Blockheader&)>;

	const hash_type PulseHash(const Blockheader&);

	class Blockheader
	{
	public:
		Blockheader(BlockHasher blockhasher = PulseHash, uint32_t version = 0x01);

		using time_rep = uint64_t;
		using time_period = std::micro;
		using time_duration = std::chrono::duration<time_rep, time_period>;

		//Good candidate for inline as just a function call
		inline const hash_type Hash() const { return this->blockhasher(*this); };

		//Am I getting a little too inline happy? Remember it'll force clients to recompile ...
		const hash_type& PrevHash() const { return this->prev_hash; };

		void Mine(const hash_type prev_hash)
		{
			this->prev_hash = prev_hash;
			this->timestamp = GenerateTimestamp();

			//TODO: add error for invalid cactaur values
			std::vector<unsigned char> difficulty_array = ExpandTarget(this->target);

			hash_type currHash = this->Hash();

			while (currHash > difficulty_array)
			{
				this->nonce += 1;
				currHash = this->Hash();
			}
		}

	private:

		static time_rep GenerateTimestamp();

		BlockHasher blockhasher;

		uint32_t version;
		int64_t timestamp;
		hash_type prev_hash;
		hash_type transaction_hash;
		uint32_t target;
		uint32_t nonce; ///< Need overflow semantics, therefore unsigned.
	};

	Blockheader::Blockheader(BlockHasher blockhasher, uint32_t version)
		: blockhasher(blockhasher)
		, version(version)
		, nonce(0)
		//,target????
	{}

	Blockheader::time_rep Blockheader::GenerateTimestamp()
	{
		return std::chrono::duration_cast<Blockheader::time_duration>
			(std::chrono::tai_clock::now().time_since_epoch()).count();
	}

} //namespace pulse

