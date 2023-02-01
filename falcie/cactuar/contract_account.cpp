

#include <cactuar/cactuar-contract_account.h>
#include <cactuar/cactuar-transaction.h>

namespace cactuar
{

	ContractAccount::ContractAccount(std::vector<std::byte> signature, ContractCode code)
		: signature{ signature }
		, code { code }
	{}

	std::unique_ptr<Transaction> ContractAccount::CreateTransaction(Transaction::Address to, std::int64_t amount)
	{
		return std::make_unique<Transaction>(this->signature, to, amount);
	}

	//template<std::int64_t UnrolledElems> //Cant be virtul for now
	void ContractAccount::ExecuteContract(Block block, const Transaction& transaction)//pulse::Blockchain<Federatedblock, UnrolledElems>)
	{
		//block.AddTransaction(std::make_unique<Transaction>(this->code(transaction)));
		Transaction *t = new Transaction(this->code(transaction)); //Super mem leak!!
		block.AddTransaction(t);
	}

	std::int64_t ContractAccount::GetBalance() const
	{
		return 0;
	}

}