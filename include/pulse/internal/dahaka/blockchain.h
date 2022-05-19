#ifndef __PULSE_DAHAKA_BLOCKCHAIN_H__
#define __PULSE_DAHAKA_BLOCKCHAIN_H__

/*-------------------------------------------
* C++ Code only (Template code)
---------------------------------------------*/

#include <cstdint>
#include <cstring>

#include "chain.h" //<- hate "" but seems like I must all of a sudden for some reason,
				   // despite working in old configuration.
				   // It does work of course but get horrible warnings


template<typename Block, typename HashType>
using MineBlock = void(*)(Block *const, const HashType* const);

template<typename Block, typename HashType> 
using HashBlock = HashType*(*)(const Block *const);

template<typename Block> 
using GetBlockTarget = uint32_t(*)(const Block* const);

template<typename HashType>
using CheckHashTarget = int(*)(const uint32_t* const, const HashType* const);


/*
template<typename Block> using MineBlock = void(*)(Block&, sha256hash_t&);
template<typename Block> using HashBlock = sha256hash_t&(*)(const Block&);

template<typename Block> using HashBlock = sha256hash_t&(*)(const Block&);
template<typename Block> using GetBlockTarget = cactaur_t&(*)(Block&);
*/

template<typename Block, typename HashType, std::int64_t UnrolledElems>
class Blockchain
{
public:
	Blockchain(Block genisis, 
		MineBlock<Block, HashType> mine, 
		HashBlock<Block, HashType> hash, 
		HashBlock<Block, HashType> previous_hash, 
		GetBlockTarget<Block> target,
		CheckHashTarget<HashType> target_check,
		int hash_size);

	void Add(Block elem);
	bool Validate();
	Block GetLast();
	typename Chain<Block, UnrolledElems>::ChainIterator begin();
	typename Chain<Block, UnrolledElems>::ChainIterator end();

private:
	Chain<Block, UnrolledElems> chain;
	MineBlock<Block, HashType> mine;
	HashBlock<Block, HashType> hash;
	HashBlock<Block, HashType> previous_hash;
	GetBlockTarget<Block> target;
	CheckHashTarget<HashType> target_check;

	int hash_size;
};

//Might want an additional temp param for difficulty change ... thinggy ...
template<typename Block, typename HashType, std::int64_t UnrolledElems>
Blockchain<Block, HashType, UnrolledElems>::Blockchain(Block genisis,
	MineBlock<Block, HashType> mine, 
	HashBlock<Block, HashType> hash,
	HashBlock<Block, HashType> previous_hash,
	GetBlockTarget<Block> target,
	CheckHashTarget<HashType> target_check,
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

template<typename Block, typename HashType, std::int64_t UnrolledElems>
void Blockchain<Block, HashType, UnrolledElems>::Add(Block elem)
{
	Block prev = chain.GetLast();
	unsigned char *prev_hash = this->hash(&prev);

	//TODO:Update Difficulty

	this->mine(&elem, prev_hash);

	//TODO:Set everything else required on block

	this->chain.Add(elem);

}

template<typename Block, typename HashType, std::int64_t UnrolledElems>
Block Blockchain<Block, HashType, UnrolledElems>::GetLast()
{
	return this->chain.GetLast();
}

template<typename Block, typename HashType, std::int64_t UnrolledElems>
bool Blockchain<Block, HashType, UnrolledElems>::Validate()
{
	//----------- OLD --------------------------------------------
	// node: = blockchain.head
	// var prevBlock, currBlock* FederatedBlock

	// var i int = 1 //In order to skip genisis in first chain
	//------------------------------------------------------------

	Chain<Block, UnrolledElems>::ChainIterator it = this->chain.begin();
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


template<typename Block, typename HashType, std::int64_t UnrolledElems>
typename Chain<Block, UnrolledElems>::ChainIterator Blockchain<Block, HashType, UnrolledElems>::begin()
{
	return this->chain.begin();
}

template<typename Block, typename HashType, std::int64_t UnrolledElems>
typename Chain<Block, UnrolledElems>::ChainIterator Blockchain<Block, HashType, UnrolledElems>::end()
{
	return this->chain.end();
}


#endif
