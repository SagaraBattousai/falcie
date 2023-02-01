#ifndef __CACTUAR_ACCOUNT_H__
#define __CACTUAR_ACCOUNT_H__

#include <cstdint>
#include <vector>
#include <memory>
#include <functional>

#include <cactuar/cactuar-block.h>

namespace cactuar
{

	class Account
	{
	public:

		using ContractCode = std::function<FederatedTransaction(const FederatedTransaction&)>; //wont work as value only lref ??

		ContractAccount(std::vector<std::byte>, ContractCode);

		virtual std::unique_ptr<cactuar::Transaction> CreateTransaction(cactuar::Transaction::Address, std::int64_t) override;

		//TODO: when a blockchain is abs class virtual void SendTransaction() override;

		virtual std::int64_t GetBalance() const override;

		//template<std::int64_t UnrolledElems> //Cant be virtul for now
		void ExecuteContract(Federatedblock, const FederatedTransaction&);//pulse::Blockchain<Federatedblock, UnrolledElems>);

	private:
		std::vector<std::byte> signature;
		ContractCode code;
	};

}


#endif