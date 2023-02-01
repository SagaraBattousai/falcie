
#include <cstdint>
#include <cstddef>
#include <iostream>

#include <vector> //import <vector>;
#include <utility> //import <utility>;

#include <cactuar/cactuar-transaction.h> //import lindzei;
#include <cactuar/cactuar-user_account.h> //import lindzei;
#include <cactuar/cactuar-contract_account.h> //import lindzei;

int main(void)
{

	using lindzei::Federatedblock;
	using lindzei::FederatedTransaction;
	using lindzei::UserAccount;
	using lindzei::ContractAccount;
	//using pulse::NeuralNetwork;
	using lindzei::NetworkUpdate;

	Federatedblock::Builder block_builder = Federatedblock::Builder().WithVersion(0x02);

	pulse::Blockchain<lindzei::Federatedblock, 32> bc{ block_builder.Genisis(), 32 };

	Federatedblock block = block_builder.Build();

	std::vector<std::byte> user_addr = std::vector<std::byte>{ std::byte{0}, std::byte{1}, std::byte{2} };
	
	std::vector < std::byte> contract_addr = std::vector<std::byte>{ std::byte{9}, std::byte{8}, std::byte{7} };

	UserAccount acc{ user_addr };

	auto x = [](const FederatedTransaction& trans) -> FederatedTransaction
	{
		return FederatedTransaction{ trans.GetReceiver(), trans.GetSender(), trans.GetValue() * 2 };
	};

	ContractAccount con{ contract_addr, x };

	FederatedTransaction *ut = new FederatedTransaction{ user_addr, contract_addr, 7 };

	block.AddTransaction(ut);

	bc.Add(std::move(block));

	Federatedblock b2 = block_builder.Build();

	con.ExecuteContract(b2, *ut);

	bc.Add(std::move(b2));




	return 0;
}