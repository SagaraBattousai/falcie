

#include <orphan/federated_contract_account.h>
#include <orphan/federated_transaction.h>

namespace lindzei
{

	ContractAccount::ContractAccount(std::vector<std::byte> signature, ContractCode code)
		: signature{ signature }
		, code { code }
	{}

	std::unique_ptr<cactuar::Transaction> ContractAccount::CreateTransaction(cactuar::Transaction::Address to, std::int64_t amount)
	{
		return std::make_unique<FederatedTransaction>(this->signature, to, amount);
	}

	//template<std::int64_t UnrolledElems> //Cant be virtul for now
	void ContractAccount::ExecuteContract(Federatedblock block, const FederatedTransaction& transaction)//pulse::Blockchain<Federatedblock, UnrolledElems>)
	{
		//block.AddTransaction(std::make_unique<FederatedTransaction>(this->code(transaction)));
		FederatedTransaction *t = new FederatedTransaction(this->code(transaction)); //Super mem leak!!
		block.AddTransaction(t);
	}

	std::int64_t ContractAccount::GetBalance() const
	{
		return 0;
	}

}