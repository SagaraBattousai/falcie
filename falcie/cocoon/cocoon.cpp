
#include <cstdint>
#include <cstddef>
#include <iostream>

#include <vector>
#include <utility>

#include <cactuar/cactuar-transaction.h>
#include <cactuar/cactuar-user_account.h>
#include <cactuar/cactuar-contract_account.h>
#include <cactuar/cactuar-blockheader.h>
#include <cactuar/cactuar-block.h>
#include <cactuar/cactuar-blockchain.h>

int main(void)
{

	using cactuar::Block;
	using cactuar::Blockchain;
	using cactuar::Transaction;
	using cactuar::UserAccount;
	using cactuar::ContractAccount;
	using cactuar::NetworkStructureUpdate;

	Block::Builder block_builder = Block::Builder().WithVersion(0x02);

	Blockchain bc();

	Block block = block_builder.Build();





	/*
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
	*/

	//std::cout << "Header = " << cactuar::Blockheader::Genisis() << std::endl;



	return 0;
}