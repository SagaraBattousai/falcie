#ifndef __PULSE_DAHAKA_BLOCKCHAIN_H__
#define __PULSE_DAHAKA_BLOCKCHAIN_H__

/*-------------------------------------------
* C++ Code only (Template code)
---------------------------------------------*/

#include <cstdint>
#include <cstring>

#include <pulse/types.h>
#include <pulse/chain.h>

template<typename T> using mine_func = void(*)(T *const, const sha256hash_t* const);
template<typename T> using hash_func = sha256hash_t(*)(const T *const);

template<typename T> using prev_hash_getter = sha256hash_t(*)(const T *const);
template<typename T> using target_getter = cactuar_t(*)(const T* const);

/*
template<typename T> using mine_func = void(*)(T&, sha256hash_t&);
template<typename T> using hash_func = sha256hash_t&(*)(const T&);

template<typename T> using prev_hash_getter = sha256hash_t&(*)(const T&);
template<typename T> using target_getter = cactaur_t&(*)(T&);
*/


template<typename T, std::size_t UnrolledElems>
class Blockchain
{
public:
	Blockchain(T genisis, mine_func<T> m, hash_func<T> h, prev_hash_getter<T> p, target_getter<T> t);
	//virtual ~Blockchain();
	void Add(T elem);
	bool Validate();
	typename Chain<T, UnrolledElems>::ChainIterator begin();
	typename Chain<T, UnrolledElems>::ChainIterator end();

private:
	Chain<T, UnrolledElems> chain;
	mine_func<T> mine;
	hash_func<T> hash;
	prev_hash_getter<T> prev_hash;
	target_getter<T> target;
};

//Might want an additional temp param for difficulty change ... thinggy ...
template<typename T, std::size_t UnrolledElems>
Blockchain<T, UnrolledElems>::Blockchain(T genisis, mine_func<T> m, hash_func<T> h,
	prev_hash_getter<T> p, target_getter<T> t) 
	: mine(m)
	, hash(h)
	, prev_hash(p)
	, target(t)
	, chain(Chain<T, UnrolledElems>())
{
	chain.Add(genisis);
}

/*
template<typename T, std::size_t UnrolledElems>
Blockchain<T, UnrolledElems>::~Blockchain() {} 
*/

template<typename T, std::size_t UnrolledElems>
void Blockchain<T, UnrolledElems>::Add(T elem)
{
	T prev = chain.GetLast();
	sha256hash_t prev_hash = this->hash(&prev);

	//TODO:Update Difficulty

	this->mine(&elem, &prev_hash);

	//TODO:Set everything else required on block

	this->chain.Add(elem);

}

template<typename T, std::size_t UnrolledElems>
bool Blockchain<T, UnrolledElems>::Validate()
{
	//----------- OLD --------------------------------------------
	// node: = blockchain.head
	// var prevBlock, currBlock* FederatedBlock

	// var i int = 1 //In order to skip genisis in first chain
	//------------------------------------------------------------

	Chain<T, UnrolledElems>::ChainIterator it = this->chain.begin();
	++it; //In order to skip genisis //< I prefer it++ but since I wrote both and it++ calls ++it ... 

	T prevBlock = *it;
	T currBlock;
	/*
	* hash_func<T> hash;
	* prev_hash_getter<T> prev_hash;
	* target_getter<T> target;
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

template<typename T, std::size_t UnrolledElems>
typename Chain<T, UnrolledElems>::ChainIterator Blockchain<T, UnrolledElems>::begin()
{
	return this->chain.begin();
}

template<typename T, std::size_t UnrolledElems>
typename Chain<T, UnrolledElems>::ChainIterator Blockchain<T, UnrolledElems>::end()
{
	return this->chain.end();
}




#endif
