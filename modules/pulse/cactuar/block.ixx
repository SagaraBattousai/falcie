
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
	//template <HashAlgorithm hash_algo>
	class Block
	{
	public:

		using time_rep = uint64_t;
		using time_period = std::micro;
		using time_duration = std::chrono::duration<time_rep, time_period>;

		static time_rep GenerateTimestamp()
		{
			return std::chrono::duration_cast<time_duration>
				(std::chrono::tai_clock::now().time_since_epoch()).count();
		};

		//using BlockHashFunction = std::function<const blockhash_type(const BlockheaderState&)>;


		//Good candidate for inline as just a function call
		//inline const blockhash_type Hash() const
		
		//const blockhash_type& PrevHash() const
	
		//void Mine(const blockhash_type prev_hash);

	};


	/**
	 * @brief Default BlockHashFunction; returns the double hash (of type "hash_algo")
	 * of the Blockheader's BlockheaderState.
	 */
	/*template <HashAlgorithm hash_algo>
	const typename Blockheader<hash_algo>::blockhash_type PulseHash(const typename Blockheader<hash_algo>::BlockheaderState&);
*/
	/*
	template <HashAlgorithm hash_algo>
	const typename Blockheader<hash_algo>::blockhash_type PulseHash(const typename Blockheader<hash_algo>::BlockheaderState& state)
	{
		std::span<const std::byte> bytes = std::as_bytes(std::span{ &state, 1 });

		std::vector<std::byte> data = EncodeData(bytes);

		return (HashFunctionPool<hash_algo>::Instance())({ data.data(), data.size() });
	}

	*/

} //namespace pulse

