//module;

#ifndef __CACTUAR_BLOCK__
#define __CACTUAR_BLOCK__

#include <cstdint>

//export module cactuar:block;

#include <vector> //import <vector>;
#include <span> //import <span>;
#include <chrono> //import <chrono>;
#include <ratio> //import <ratio>;
#include <functional> //import <functional>;

#include <cactuar/cactuar-encoding.h> //import :encoding;
#include <cactuar/cactuar-crypto.h> //import :crypto;
#include <cactuar/cactuar-target.h> //import :target;

//export 
namespace pulse
{
	/**
	 * @brief Interface for blocks that can be added to the blockchain. 
	 *
	 * Note, it is not essential to implement this inorder to add to the blockchain;
	 * however, by implementing this interface, it is garunteed to be addable to the blockchain.
	 * 
	*/
	class Block
	{
	public:

		using blockhash_type = std::vector<std::byte>;

		using time_rep = std::int64_t; //Maybe make private
		using time_period = std::micro; //Maybe make private
		using time_duration = std::chrono::duration<time_rep, time_period>;
		using time_clock = std::chrono::system_clock; //std::chrono::tai_clock;
		using time_point = std::chrono::time_point<time_clock, time_duration>;

		//may make universal ref but for now...
		using BlockHashFunction =
			std::function<const blockhash_type(HashAlgorithm hash_algo, const Block&)>;

		virtual blockhash_type Hash() const = 0;
		
		virtual const blockhash_type& PrevHash() const = 0;
		
		//TODO: Could make <=> but for now just true iff arg is < target
		virtual const bool CompareWithTarget(const blockhash_type&) const = 0;

		//Be careful since my understanding of && is new.
		virtual void Mine(blockhash_type&& prev_hash) = 0;

		virtual std::vector<std::byte> State() const = 0;

		static time_rep GenerateTimestamp()
		{
			return std::chrono::duration_cast<time_duration>
				(time_clock::now().time_since_epoch()).count();
		};

		//TODO: May need optimising
		static time_point AsTimePoint(const std::int64_t& timestamp)
		{
			return time_point(time_duration(timestamp));
		}


	};

	/**
	 * @brief Default BlockHashFunction; returns the double hash (of type "hash_algo")
	 * of the Blockheader's BlockheaderState.
	 */
	const typename Block::blockhash_type PulseHash(HashAlgorithm hash_algo, const Block& block);

} //namespace pulse

#endif
