
#include <cactuar/cactuar-blockchain.h>

namespace cactuar
{

	Blockchain::Blockchain() : chain()
	{
		this->chain.Add(Block::Genisis());
	}

	void Blockchain::Add(Block&& elem)
	{
		/*
		const Block& prev = chain.GetLast();
		auto prev_hash = prev.Hash(); //should be a copy

		//TODO:Update Difficulty

		elem.Mine(std::move(prev_hash));

		//TODO:Set everything else required on block

		this->chain.Add(std::move(elem));
		*/
	}

	const Block& Blockchain::GetLast()
	{
		return this->chain.GetLast();
	}

	bool Blockchain::Validate()
	{
		/*
		//typename Chain<Block, UnrolledElems>::ChainIterator 
		auto it = this->chain.begin();
		++it; ///< Skip Genisis Block

		Block prevBlock = *it;
		Block currBlock;

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
		/*
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
		*/
		return false;
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