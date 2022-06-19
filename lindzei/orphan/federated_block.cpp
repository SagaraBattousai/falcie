
#include <vector> //import <vector>;
#include <string>

#include <pulse/pulse.h> //import pulse;
#include <cactuar/cactuar-target.h>

#include <orphan/federated_block.h>

//export
namespace lindzei
{
	Federatedblock::Federatedblock(std::uint32_t version, pulse::Target target,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
		: Federatedblock(pulse::Blockheader{
			version,
			-1,
			std::vector<std::byte>{AsHashSize(hash_algo)},
			std::vector<std::byte>{AsHashSize(hash_algo)},
			target,
			0
			}, hash_func, hash_algo)
	{
	}

	Federatedblock::Federatedblock(pulse::Blockheader&& header,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
		: magic{ 0x43616C6F }
		//, header{ std::move(header) }
		//, header{ std::make_unique<pulse::Blockheader>(std::move(header)) }
		, header{ std::make_shared<pulse::Blockheader>(std::move(header)) }
		, local_updates{ std::vector<NetworkUpdate>{} }
		, global_update{ }// std::make_unique<NetworkUpdate>() }
		, hash_algo{ hash_algo }
		, hash_func{ hash_func }
	{}

	Federatedblock Federatedblock::Genisis(std::uint32_t version,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
	{
		return Federatedblock(
			pulse::Blockheader::Genisis(hash_algo, version), hash_func, hash_algo);
	}

	void Federatedblock::Mine(blockhash_type&& prev_hash)
	{
		this->header->prev_hash = std::move(prev_hash);
		this->header->timestamp = Block::GenerateTimestamp();

		//TODO: add error for invalid cactaur values
		std::vector<std::byte> difficulty_array = this->header->target.Expand();

		blockhash_type currHash = this->Hash();

		while (currHash > difficulty_array)
		{
			this->header->nonce += 1;
			currHash = this->Hash();
		}

		CalculateGlobalUpdate();

	}

	//I think this can be rvalue since we dont need it after its added.
	void Federatedblock::AddLocalUpdate(NetworkUpdate&& update)
	{
		//TODO: For now, probably init block with network stucture (and possibly builder)
		//To enfores all updates to be of a compatible network
		//may need to change to make network_structure part of the block constructor
		if (this->global_update.network_structure.size() == 0)
		{
			this->global_update = GlobalNetworkUpdate(update.network_structure);
		}

		this->local_updates.push_back(std::move(update));
	}


	inline const bool Federatedblock::CompareWithTarget(const Federatedblock::blockhash_type& hash) const
	{
		return hash < this->header.get()->target;
	}

	inline std::vector<std::byte> Federatedblock::State() const
	{
		return pulse::BlockheaderState(this->header.get(), this->hash_algo);
	}

	const NetworkUpdate& Federatedblock::GetGlobalUpdate() const
	{
		return this->global_update;
	}

	void Federatedblock::CalculateGlobalUpdate()
	{
		for (const NetworkUpdate& update : this->local_updates)
		{
			this->global_update.examples_seen += update.examples_seen;

			for (std::int64_t i = 0;
				i < static_cast<std::int64_t>(this->global_update.network_structure.size()) - 1;
				i++)
			{
				this->global_update.delta_weights[i] += update.examples_seen * update.delta_weights[i];
			}
		}

		for (std::int64_t i = 0;
			i < static_cast<std::int64_t>(this->global_update.network_structure.size()) - 1;
			i++)
		{
			this->global_update.delta_weights[i] *= (1.f / global_update.examples_seen);
		}
	}


	std::ostream& operator<<(std::ostream& os, const Federatedblock& block)
	{
		if (block.header) //i.e. not genisis
		{
			os << std::string(68, '-') << "\n";
			os << *block.header;
			os << std::string(68, '-') << "\n";
		}

		os << "Magic:               " << block.magic;
		//std::uint32_t blocksize;
		os << "\nExample Seen:        " << block.global_update.examples_seen;
		os << "\nHash Algorithm Used: " << block.hash_algo << std::endl;

		return os;
	}

} //namespace lindzei