#ifndef __ETRO_BLOCKCHAIN_H__
#define __ETRO_BLOCKCHAIN_H__

//CPP Header Only

#include <cstdint>
#include <array>

#include <etro/chain.h>

extern "C" {
#include <etro/types.h>
}

template<typename T> using mine_func = void(*)(T&, sha256hash_t&);
template<typename T> using hash_func = sha256hash_t(*)(T&);


template<typename T, size_t MaxElems>
class Blockchain
{
public:
	Blockchain(T genisis, mine_func<T> m, hash_func<T> h);
	//virtual ~Blockchain();
	void Add(T elem);
	bool Validate();

private:
	Chain<T, MaxElems> chain;
	mine_func mine;
	hash_func hash;
};

template<typename T, size_t MaxElems>
Blockchain<T, MaxElems>::Blockchain(T genisis, mine_func<T> m, hash_func<T> h) : 
	mine(m), 
	hash(h)
{
	this->chain = Chain<T, MaxElems>();
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
node: = blockchain.head
var prevBlock, currBlock* FederatedBlock

var i int = 1 //In order to skip genisis in first chain

for node != nil{
	for i < len(node.elements) - 1 {
		prevBlock = node.elements[i]
		currBlock = node.elements[i + 1]

			//Annoying I cant use if's shortcircuit because I need an
			//address to then reference
			prevBlockHash: = prevBlock.Hash()
			currBlockHash : = currBlock.Hash()

			if !(bytes.Equal(prevBlockHash[:],
				currBlock.BlockHeader.Prevhash[:]) &&
				bytes.Compare(currBlockHash[:],
					currBlock.BlockHeader.Target.As256Bit()[:]) == -1) {
				return false
			}
			i++
		}
		i = 0
		node = node.next
}
return true

}




#endif
