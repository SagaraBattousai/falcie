
#include <vector>
#include <cstddef>

#include <cactuar/cactuar-user_account.h>
#include <cactuar/cactuar-transaction.h>

namespace cactuar
{

	UserAccount::UserAccount(std::vector<std::byte> signature)
		: signature{ signature }
		, balance { 0 }
	{}

	std::unique_ptr<cactuar::Transaction> UserAccount::CreateTransaction(cactuar::Transaction::Address to, std::int64_t amount)
	{
		return std::make_unique<Transaction>(this->signature, to, amount);
	}

	//void UserAccount::SendTransaction() { }

	std::int64_t UserAccount::GetBalance() const
	{
		return this->balance;
	}

}