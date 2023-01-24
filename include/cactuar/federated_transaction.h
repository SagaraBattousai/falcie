#ifndef __ORPHAN_FEDERATED_TRANSACTION_H__
#define __ORPHAN_FEDERATED_TRANSACTION_H__

#include <cstddef>
#include <cstdint>

#include <vector>

#include <cactuar/cactuar-transaction.h>

namespace lindzei
{
	class FederatedTransaction : public cactuar::Transaction
	{
	public:

		using Address = cactuar::Transaction::Address;

		FederatedTransaction(
			Address from,
			Address to,
			std::int64_t value);
		
		FederatedTransaction(FederatedTransaction&&);

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