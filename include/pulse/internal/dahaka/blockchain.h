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
using mine_func = void(*)(Block *const, const HashType* const);

template<typename Block, typename HashType> 
using get_hash = HashType(*)(const Block *const);

template<typename Block> 
using get_target = uint32_t(*)(const Block* const)

/*
template<typename Block> using mine_func = void(*)(Block&, sha256hash_t&);
template<typename Block> using get_hash = sha256hash_t&(*)(const Block&);

template<typename Block> using get_hash = sha256hash_t&(*)(const Block&);
template<typename Block> using get_target = cactaur_t&(*)(Block&);
*/

template<typename Block, typename HashType, std::int64_t UnrolledElems>
class Blockchain
{
public:
	Blockchain(Block genisis, 
		mine_func mine, 
		get_hash hash, 
		get_hash previous_hash, 
		get_target target);

	void Add(Block elem);
	bool Validate();
	typename Chain<Block, UnrolledElems>::ChainIterator begin();
	typename Chain<Block, UnrolledElems>::ChainIterator end();

private:
	Chain<Block, UnrolledElems> chain;
	mine_func mine;
	get_hash hash;
	get_hash previous_hash;
	get_target target;
};

//Might want an additional temp param for difficulty change ... thinggy ...
template<typename Block, typename HashType, std::int64_t UnrolledElems>
Blockchain<Block, HashType, UnrolledElems>::Blockchain(Block genisis,
	mine_func<Block, HashType> mine, 
	get_hash<Block, HashType> hash,
	get_hash<Block, HashType> previous_hash,
	get_target<Block> target)
	: mine(m)
	, hash(h)
	, previous_hash(p)
	, target(t)
	, chain(Chain<Block, UnrolledElems>())
{
	chain.Add(genisis);
}

template<typename Block, typename HashType, std::int64_t UnrolledElems>
void Blockchain<Block, HashType, UnrolledElems>::Add(Block elem)
{
	Block prev = chain.GetLast();
	sha256hash_t prev_hash = this->hash(&prev);

	//TODO:Update Difficulty

	this->mine(&elem, &prev_hash);

	//TODO:Set everything else required on block

	this->chain.Add(elem);

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
	* get_hash<Block> hash;
	* get_hash<Block> prev_hash;
	* get_target<Block> target;
	*/
	auto end = this->chain.end();
	while (++it != end) //< Is it acceptable to inc in the while? Is that defined behavouir?
	{
		//++it;
		currBlock = *it;

		//previousBlock'sHash == current->blockheader->prevhash
		// &&
		//currentBlock'sHash < currentBlocks Target

		auto test = (this->target(&currBlock));

		if (!(
			(std::memcmp(&(this->hash(&prevBlock)), &(this->prev_hash(&currBlock)), sizeof(sha256hash_t)) == 0)
			&&
			(targetcmp(&test, &(this->hash(&currBlock))) < 0)
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
