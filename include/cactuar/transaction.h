#ifndef __CACTUAR_TRANSACTION_H__
#define __CACTUAR_TRANSACTION_H__

#include <cstddef>
#include <cstdint>

#include <vector>

namespace cactuar
{
	class Transaction
	{
	public:
		using Address = std::vector<std::byte>;

		Transaction(
			Address from,
			Address to,
			std::int64_t value);
		
		Transaction(Transaction&&);

		virtual ~Transaction() = default;

		virtual Address GetSender() const;
		virtual Address GetReceiver() const;
		virtual std::int64_t GetValue() const;

	private:
		Address from;
		Address to;
		std::int64_t value;

	};
}

#endif
