#ifndef __CACTUAR_USER_ACCOUNT_H__
#define __CACTUAR_USER_ACCOUNT_H__

#include <cstdint>
#include <vector>

#include <cactuar/account.h>

namespace cactuar
{

	class UserAccount : Account
	{
	public:
		UserAccount(std::vector<std::byte> signature);// , std::int64_t balance);

		virtual std::unique_ptr<cactuar::Transaction>  CreateTransaction(cactuar::Transaction::Address, std::int64_t) override;

		//TODO: when a blockchain is abs class virtual void SendTransaction() override;

		virtual std::int64_t GetBalance() const override;

	private:
		std::vector<std::byte> signature;

		std::int64_t balance;



	};

}


#endif