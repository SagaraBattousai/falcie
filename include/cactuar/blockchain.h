#ifndef __CACTUAR_BLOCKCHAIN_H__
#define __CACTUAR_BLOCKCHAIN_H__

//module;
#include <cstdint>
#include <cstring>

#include <vector>
#include <concepts>
#include <utility>

#include <dahaka/dahaka-chain.h>

#include <cactuar/cactuar-block.h>

namespace cactuar
{
	
	class Blockchain
	{
	public:
		static constexpr int ChainUnrolledLength = 32; // 64

		using chain_t = dahaka::Chain<Block, ChainUnrolledLength>;

		Blockchain();

		void Add(Block&& elem);

		bool Validate();

		std::int64_t GetBalance(std::vector<std::byte> addr);

		const Block& GetLast();

		typename chain_t::ChainIterator begin();
		typename chain_t::ChainIterator end();

	private:
		chain_t chain;
	};

} //namespace cactuar

#endif