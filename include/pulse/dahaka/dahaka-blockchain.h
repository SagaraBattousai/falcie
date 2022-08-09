#ifndef __DAHAKA_BLOCKCHAIN__
#define __DAHAKA_BLOCKCHAIN__

//module;
#include <cstdint>
#include <cstring>

/*
export module dahaka:blockchain;

import <vector>;
import <concepts>;
import <utility>;

import :chain;
*/

#include <vector>
#include <concepts>
#include <utility>

#include <dahaka/dahaka-chain.h>

//export 
namespace pulse
{
	//Am I going ott with concepts, would probably be easier with inheritance especially since 
	//block is super generic. That said this decouples modules and also does compiler checks :D

	//Should the BlockchainAddable have a static Genisis function or the builder?
	//Lets try builder for now.

	template<typename T>
	concept BlockchainAddable = requires (T block,
		const std::vector<std::byte>&l_hash, std::vector<std::byte> && r_hash)
	{
		{ block.Mine(std::move(r_hash)) };
		{ block.PrevHash() } -> std::convertible_to<const std::vector<std::byte>&>;
		{ block.Hash() } -> std::convertible_to<std::vector<std::byte>>;
		{ block.CompareWithTarget(l_hash) } -> std::convertible_to<bool>;
	};

	//TODO: add V later or maybe leave concepts alone and do blockchain in cactuar etc

	/*
	template<typename T>
	concept BlockchainAddableBuilder = requires (T builder)
	{
		{ builder.Build() } -> BlockchainAddable;
		{ builder.Genisis() } -> BlockchainAddable;

	};
	*/

	template<BlockchainAddable T, std::int64_t UnrolledElems>
	class Blockchain
	{
	public:
		
		Blockchain(T&& genisis, const int hash_size);

		void Add(T&& elem);

		bool Validate();

		const T& GetLast();

		typename Chain<T, UnrolledElems>::ChainIterator begin();
		typename Chain<T, UnrolledElems>::ChainIterator end();

	private:
		Chain<T, UnrolledElems> chain;

		const int hash_size;
	};

	//Might want an additional temp param for difficulty change ... thinggy ...
	//I (Think I) Can finally use a universal ref :D (since (Some) Blocks are non copyable) 
	template<BlockchainAddable T, std::int64_t UnrolledElems>
	Blockchain<T, UnrolledElems>::Blockchain(T&& genisis, const int hash_size)
		: chain(Chain<T, UnrolledElems>())
		, hash_size(hash_size)
	{
		this->chain.Add(std::forward<T>(genisis));
	}

	template<BlockchainAddable T, std::int64_t UnrolledElems>
	void Blockchain<T, UnrolledElems>::Add(T&& elem)
	{
		const T& prev = chain.GetLast();
		auto prev_hash = prev.Hash(); //should be a copy

		//TODO:Update Difficulty

		elem.Mine(std::move(prev_hash));

		//TODO:Set everything else required on block

		this->chain.Add(std::forward<T>(elem));

	}

	template<BlockchainAddable T, std::int64_t UnrolledElems>
	const T& Blockchain<T, UnrolledElems>::GetLast()
	{
		return this->chain.GetLast();
	}

	template<BlockchainAddable T, std::int64_t UnrolledElems>
	bool Blockchain<T, UnrolledElems>::Validate()
	{
		//typename Chain<Block, UnrolledElems>::ChainIterator 
		auto it = this->chain.begin();
		++it; ///< Skip Genisis Block

		T prevBlock = *it;
		T currBlock;

		auto end = this->chain.end();
		while (++it != end) //< Is it acceptable to inc in the while? Is that defined behavouir?
		{
			//++it;
			currBlock = *it;

			//previousBlock'sHash == current->blockheader->prevhash
			// &&
			//currentBlock'sHash < currentBlocks Target

			//auto test = currBlock.Target();

			/*
			std::vector<std::byte> difficulty_array = this->header->target.Expand();

		blockhash_type currHash = this->Hash();

		while (currHash > difficulty_array)
			*/

			/*
			std::memcmp(this->hash(&prevBlock),
					this->previous_hash(&currBlock),
					sizeof(unsigned char) * this->hash_size) == 0
			*/

			if (!(
				prevBlock.Hash() == currBlock.Hash() &&
				currBlock.CompareWithTarget(currBlock.Hash())
				))
			{
				return false;
			}

			prevBlock = currBlock;
		}
		return true;
	}


	template<BlockchainAddable T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator Blockchain<T, UnrolledElems>::begin()
	{
		return this->chain.begin();
	}

	template<BlockchainAddable T, std::int64_t UnrolledElems>
	typename Chain<T, UnrolledElems>::ChainIterator Blockchain<T, UnrolledElems>::end()
	{
		return this->chain.end();
	}

} //namespace pulse

#endif