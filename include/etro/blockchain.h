#ifndef __ETRO_BLOCKCHAIN_H__
#define __ETRO_BLOCKCHAIN_H__

//CPP Header Only

#include <cstdint>
#include <array>
#include <cstring>

#include <etro/chain.h>

extern "C" {
#include <etro/types.h>
#include <etro/cactuar.h>
}

template<typename T> using mine_func = void(*)(T&, sha256hash_t&);
template<typename T> using hash_func = sha256hash_t&(*)(const T&);

template<typename T> using prev_hash_getter = sha256hash_t&(*)(const T&);
template<typename T> using target_getter = cactaur_t&(*)(T&);

template<typename T, size_t MaxElems>
class Blockchain
{
public:
	Blockchain(T genisis, mine_func<T> m, hash_func<T> h, prev_hash_getter<T> p, target_getter<T> t);
	//virtual ~Blockchain();
	void Add(T elem);
	bool Validate();

private:
	Chain<T, MaxElems> chain;
	mine_func<T> mine;
	hash_func<T> hash;
	prev_hash_getter<T> prev_hash;
	target_getter<T> target;
};

//Might want an additional temp param for difficulty change ... thinggy ...
template<typename T, size_t MaxElems>
Blockchain<T, MaxElems>::Blockchain(T genisis, mine_func<T> m, hash_func<T> h,
	prev_hash_getter<T> p, target_getter<T> t) 
	: mine(m)
	, hash(h)
	, prev_hash(p)
	, target(t)
	, chain(Chain<T, MaxElems>())
{
	chain.Add(genisis);
}

/*
template<typename T, size_t MaxElems>
Blockchain<T, MaxElems>::~Blockchain() {} 
*/

template<typename T, size_t MaxElems>
void Blockchain<T, MaxElems>::Add(T elem)
{
	T prev = chain.GetLast();
	sha256hash_t prev_hash = this->hash(prev);

	//TODO:Update Difficulty

	this->mine(elem, prev_hash);

	//TODO:Set everything else required on block

	this->chain.Add(elem);

}

template<typename T, size_t MaxElems>
bool Blockchain<T, MaxElems>::Validate()
{
	//----------- OLD --------------------------------------------
	// node: = blockchain.head
	// var prevBlock, currBlock* FederatedBlock

	// var i int = 1 //In order to skip genisis in first chain
	//------------------------------------------------------------

	Chain<T, MaxElems>::ChainIterator it = this->chain.begin();
	++it; //In order to skip genisis //< I prefer it++ but since I wrote both and it++ calls ++it ... 

	T prevBlock = *it;
	T currBlock;
	/*
	* hash_func<T> hash;
	* prev_hash_getter<T> prev_hash;
	* target_getter<T> target;
	*/

	while (it != this->chain.end())
	{
		++it;
		currBlock = *it;

		//previousBlock'sHash == current->blockheader->prevhash
		// &&
		//currentBlock'sHash < currentBlocks Target

		if !(
			(std::memcmp(this->hash(prevBlock), this->prev_hash(currBlock), sizeof(sha256hash_t)) == 0)
			&&
			(targetcmp(target(currBlock), this->hash(currBlock)))
			)
		{
			return false;
		}

		prevBlock = currBlock;
	}
	return true;
}




#endif
