
#include <cactuar/blockchain.h>

namespace cactuar
{

	Blockchain::Blockchain() : chain()
	{
		this->chain.Add(Block::Genisis());
	}

	void Blockchain::Add(Block&& block)
	{
		
		const Block& prev = chain.GetLast();
		auto prev_hash = prev.Hash(); //should be a copy

		//TODO:Update Difficulty

		block.Mine(std::move(prev_hash));

		//TODO:Set everything else required on block

		this->chain.Add(std::move(block));
	}

	const Block& Blockchain::GetLast()
	{
		return this->chain.GetLast();
	}

	bool Blockchain::Validate()
	{
		//typename Chain<Block, UnrolledElems>::ChainIterator 
		auto it = this->chain.begin();
		++it; ///< Skip Genisis Block

		Block prevBlock = *it;
		Block currBlock;

		auto end = this->chain.end();
		while (++it != end) // Usually don't like this but meh
		{
			//currBlock = *it;

			//previousBlock'sHash == current->blockheader->prevhash
			// &&
			//currentBlock'sHash < currentBlocks Target

			if (!(
				prevBlock.Hash() == currBlock.Hash() &&
				currBlock.CompareWithTarget(currBlock.Hash())
				))
			{
				return false;
			}
			//Probably should be move but removed matrix's 
			prevBlock = currBlock;
		}
		return true;
	}


	std::int64_t Blockchain::GetBalance(std::vector<std::byte> addr)
	{
		return 0;
		/*
		auto it = this->chain.begin();
		++it; ///< Skip Genisis Block

		auto end = this->chain.end();
		while (it != end)
		{
			for (auto tran : it->)

			++it;
		}
		*/

	}


	typename Blockchain::chain_t::ChainIterator Blockchain::begin()
	{
		return this->chain.begin();
	}

	typename Blockchain::chain_t::ChainIterator Blockchain::end()
	{
		return this->chain.end();
	}
}