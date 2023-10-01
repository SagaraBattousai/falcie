
#include <cactuar/cactuar-block.h>
#include <cactuar/cactuar-blockchain.h>
#include <cactuar/cactuar-blockheader.h>
#include <cactuar/cactuar-contract_account.h>
#include <cactuar/cactuar-transaction.h>
#include <cactuar/cactuar-user_account.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

int main(void) {
  using cactuar::Block;
  using cactuar::Blockchain;
  using cactuar::ContractAccount;
  using cactuar::NetworkStructureUpdate;
  using cactuar::Transaction;
  using cactuar::UserAccount;

  Block::Builder block_builder = Block::Builder().WithVersion(0x02);

  Blockchain bc{};

  Block block = block_builder.Build();

  block.AddLocalUpdate(
      std::vector<thoth::Matrix<float>>(1, thoth::Matrix<float>{{3, 3}, 5.0}),
      25);
  block.AddLocalUpdate(
      std::vector<thoth::Matrix<float>>(1, thoth::Matrix<float>{{3, 3}, 7.0}),
      10);

  block.Mine(std::vector<std::byte>{std::byte{1}, std::byte{2}, std::byte{3}});

  const NetworkStructureUpdate& gn = block.GetGlobalUpdate();

  std::cout << gn.delta_weights[0] << std::endl
            << gn.examples_seen << std::endl;

  return 0;
}