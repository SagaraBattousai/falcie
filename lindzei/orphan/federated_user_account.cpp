

#include <orphan/federated_user_account.h>
#include <orphan/federated_transaction.h>

namespace lindzei
{

	UserAccount::UserAccount(std::vector<std::byte> signature)
		: signature{ signature }
		, balance { 0 }
	{}

	std::unique_ptr<cactuar::Transaction> UserAccount::CreateTransaction(cactuar::Transaction::Address to, std::int64_t amount)
	{
		return std::make_unique<FederatedTransaction>(this->signature, to, amount);
	}

	//void UserAccount::SendTransaction() { }

	std::int64_t UserAccount::GetBalance() const
	{
		return this->balance;
	}

}