#ifndef __CACTUAR_TRANSACTION_H__
#define __CACTUAR_TRANSACTION_H__

#include <cstddef>
#include <cstdint>

#include <vector>

class Transaction
{
public:
	//May need to promote to outside class
	using Address = std::vector<std::byte>;

	Transaction(Address from,
		Address to,
		std::int64_t value);

private:
	Address from;
	Address to;
	std::int64_t value;


};



#endif