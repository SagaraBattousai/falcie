#ifndef __CACTUAR_ACCOUNT_H__
#define __CACTUAR_ACCOUNT_H__

#include <cstdint>
#include <vector>
#include <memory>
#include <functional>

#include <cactuar/block.h>

namespace cactuar
{

	class Account
	{
	public:

		//Account(std::vector<std::byte> address);

		virtual std::unique_ptr<cactuar::Transaction> CreateTransaction(cactuar::Transaction::Address, std::int64_t) = 0;

		//TODO: when a blockchain is abs class virtual void SendTransaction() override;

		virtual std::int64_t GetBalance() const = 0;

	private:
		std::vector<std::byte> signature;
	};

}


#endif