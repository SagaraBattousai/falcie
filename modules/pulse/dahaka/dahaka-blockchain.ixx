
module;
#include <cstdint>
#include <cstring>

export module dahaka:blockchain;

template<typename Block, typename HashAlgorithm>
using MineBlock = void(*)(Block *const, const HashAlgorithm* const);

template<typename Block, typename HashAlgorithm>
using HashBlock = HashAlgorithm * (*)(const Block *const);

template<typename Block>
using GetBlockTarget = uint32_t(*)(const Block* const);

template<typename HashAlgorithm>
using CheckHashTarget = int(*)(const uint32_t* const, const HashAlgorithm* const);

template<typename T, std::size_t UnrolledElems>
class Chain;

export namespace pulse {

	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	class Blockchain
	{
	public:
		Blockchain(Block genisis,
			MineBlock<Block, HashAlgorithm> mine,
			HashBlock<Block, HashAlgorithm> hash,
			HashBlock<Block, HashAlgorithm> previous_hash,
			GetBlockTarget<Block> target,
			CheckHashTarget<HashAlgorithm> target_check,
			int hash_size);

		void Add(Block elem);
		bool Validate();
		Block GetLast();
		typename Chain<Block, UnrolledElems>::ChainIterator begin();
		typename Chain<Block, UnrolledElems>::ChainIterator end();

	private:
		Chain<Block, UnrolledElems> chain;
		MineBlock<Block, HashAlgorithm> mine;
		HashBlock<Block, HashAlgorithm> hash;
		HashBlock<Block, HashAlgorithm> previous_hash;
		GetBlockTarget<Block> target;
		CheckHashTarget<HashAlgorithm> target_check;

		int hash_size;
	};


	//Might want an additional temp param for difficulty change ... thinggy ...
	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	Blockchain<Block, HashAlgorithm, UnrolledElems>::Blockchain(Block genisis,
		MineBlock<Block, HashAlgorithm> mine,
		HashBlock<Block, HashAlgorithm> hash,
		HashBlock<Block, HashAlgorithm> previous_hash,
		GetBlockTarget<Block> target,
		CheckHashTarget<HashAlgorithm> target_check,
		int hash_size)
		: mine(mine)
		, hash(hash)
		, previous_hash(previous_hash)
		, target(target)
		, target_check(target_check)
		, hash_size(hash_size)
		, chain(Chain<Block, UnrolledElems>())
	{
		chain.Add(genisis);
	}

	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	void Blockchain<Block, HashAlgorithm, UnrolledElems>::Add(Block elem)
	{
		Block prev = chain.GetLast();
		unsigned char *prev_hash = this->hash(&prev);

		//TODO:Update Difficulty

		this->mine(&elem, prev_hash);

		//TODO:Set everything else required on block

		this->chain.Add(elem);

	}

	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	Block Blockchain<Block, HashAlgorithm, UnrolledElems>::GetLast()
	{
		return this->chain.GetLast();
	}

	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	bool Blockchain<Block, HashAlgorithm, UnrolledElems>::Validate()
	{
		//----------- OLD --------------------------------------------
		// node: = blockchain.head
		// var prevBlock, currBlock* FederatedBlock

		// var i int = 1 //In order to skip genisis in first chain
		//------------------------------------------------------------

		typename Chain<Block, UnrolledElems>::ChainIterator it = this->chain.begin();
		++it; //In order to skip genisis //< I prefer it++ but since I wrote both and it++ calls ++it ... 

		Block prevBlock = *it;
		Block currBlock;
		/*
		* HashBlock<Block> hash;
		* HashBlock<Block> prev_hash;
		* GetBlockTarget<Block> target;
		*/
		auto end = this->chain.end();
		while (++it != end) //< Is it acceptable to inc in the while? Is that defined behavouir?
		{
			//++it;
			currBlock = *it;

			//previousBlock'sHash == current->blockheader->prevhash
			// &&
			//currentBlock'sHash < currentBlocks Target

			uint32_t test = this->target(&currBlock);

			if (!(
				std::memcmp(this->hash(&prevBlock),
					this->previous_hash(&currBlock),
					sizeof(unsigned char) * this->hash_size) == 0
				&&
				this->target_check(&test, this->hash(&currBlock)) < 0
				))
			{
				return false;
			}

			prevBlock = currBlock;
		}
		return true;
	}


	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	typename Chain<Block, UnrolledElems>::ChainIterator Blockchain<Block, HashAlgorithm, UnrolledElems>::begin()
	{
		return this->chain.begin();
	}

	template<typename Block, typename HashAlgorithm, std::int64_t UnrolledElems>
	typename Chain<Block, UnrolledElems>::ChainIterator Blockchain<Block, HashAlgorithm, UnrolledElems>::end()
	{
		return this->chain.end();
	}

} //namespace pulse



