
module;

#include <cstdint>

export module cactuar:block;

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
	class Block
	{
	public:

		//using blockhash_type = std::vector<const std::byte>;
		using blockhash_type = std::vector<std::byte>;

		//may make universal ref but for now...
		using BlockHashFunction =
			std::function<const blockhash_type(HashAlgorithm hash_algo, const Block&)>;

		//Uses RVO by returning by value
		virtual blockhash_type Hash() const = 0;
		
		virtual const blockhash_type& PrevHash() const = 0;
	
		//Be careful since my understanding of && is new.
		virtual void Mine(const blockhash_type&& prev_hash) = 0;

		//virtual std::vector<const std::byte> State() const = 0;
		virtual std::vector<std::byte> State() const = 0;


		using time_rep = uint64_t;
		using time_period = std::micro;
		using time_duration = std::chrono::duration<time_rep, time_period>;

		//could make protected as only used by subclasses??!
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
		//return (HashFunctionPool<hash_algo>::Instance())({ data.data(), data.size() });
		//std::array<std::byte, AsHashSize(hash_algo)>y{};
		////std::copy_n(x.get(), AsHashSize(hash_algo), y.data());
		//return y;
	}



} //namespace pulse

