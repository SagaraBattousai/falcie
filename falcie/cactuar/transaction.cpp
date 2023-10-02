
#include <cactuar/transaction.h>

namespace cactuar
{
	Transaction::Transaction(
		Address from,
		Address to,
		std::int64_t value)
		: from{ from }
		, to{ to }
		, value{ value }
	{}

	Transaction::Transaction(Transaction&& other)
		: from {std::move(other.from)}
		, to {std::move(other.to)}
		, value {std::move(other.value)}
	{}

	Transaction::Address Transaction::GetSender() const
	{
		return this->from;
	}
	Transaction::Address Transaction::GetReceiver() const
	{
		return this->to;
	}
	std::int64_t Transaction::GetValue() const
	{
		return this->value;
	}
}
