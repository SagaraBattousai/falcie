
#include <orphan/federated_transaction.h>

namespace lindzei
{
	FederatedTransaction::FederatedTransaction(
		Address from,
		Address to,
		std::int64_t value)
		: from{ from }
		, to{ to }
		, value{ value }
	{}

	FederatedTransaction::FederatedTransaction(FederatedTransaction&& other)
		: from {std::move(other.from)}
		, to {std::move(other.to)}
		, value {std::move(other.value)}
	{}

	FederatedTransaction::Address FederatedTransaction::GetSender() const
	{
		return this->from;
	}
	FederatedTransaction::Address FederatedTransaction::GetReceiver() const
	{
		return this->to;
	}
	std::int64_t FederatedTransaction::GetValue() const
	{
		return this->value;
	}
}