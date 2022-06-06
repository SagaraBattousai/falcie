
module;

#include <cstdint>

export module cactuar:block;

import <vector>;
import <span>;
import <chrono>;
import <ratio>;
import <functional>;

import :encoding;
import :crypto;
import :target;

export namespace pulse
{
	class Block
	{
	public:

		using blockhash_type = std::vector<std::byte>;

		using time_rep = std::int64_t;
		using time_period = std::micro;
		using time_duration = std::chrono::duration<time_rep, time_period>;

		//may make universal ref but for now...
		using BlockHashFunction =
			std::function<const blockhash_type(HashAlgorithm hash_algo, const Block&)>;

		virtual blockhash_type Hash() const = 0;
		
		virtual const blockhash_type& PrevHash() const = 0;
		
		//TODO: Could make <=> but for now just true iff arg is < target
		virtual const bool CompareWithTarget(const blockhash_type&) const = 0;

		//Be careful since my understanding of && is new.
		virtual void Mine(const blockhash_type&& prev_hash) = 0;

		virtual std::vector<std::byte> State() const = 0;

		static time_rep GenerateTimestamp()
		{
			return std::chrono::duration_cast<time_duration>
				(std::chrono::tai_clock::now().time_since_epoch()).count();
		};
	};

	/**
	 * @brief Default BlockHashFunction; returns the double hash (of type "hash_algo")
	 * of the Blockheader's BlockheaderState.
	 */
	const typename Block::blockhash_type PulseHash(HashAlgorithm hash_algo, const Block& block)
	{
		std::vector<std::byte> bytes = block.State();
		std::vector<std::byte> data =
			EncodeData(std::span<const std::byte>{ bytes.data(), bytes.size() });

		return HashFunction(hash_algo)(std::span<std::byte>{ data.data(), data.size() });
	}



} //namespace pulse

