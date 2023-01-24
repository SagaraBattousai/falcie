
//For Debug
#include <iostream>

#include <vector>
#include <string>
#include <numeric>
#include <execution>

#include <pulse/pulse.h>
#include <cactuar/cactuar-target.h>

#include <orphan/federated_block.h>

//export
namespace lindzei
{
	//TODO: Hacky fix as array deletes its default constructor if I dont have a constructor!
	//This is a bit of a problem as it leave member variables uninitalised
	Federatedblock::Federatedblock()
	{

	}

	//TODO: Decide, Probably could leave default constructors....

	Federatedblock::Federatedblock(std::uint32_t version, cactuar::Target target)
		: Federatedblock(Blockheader{
			version,
			-1,
			std::vector<std::byte>(Federatedblock::hash_size),
			std::vector<std::byte>(Federatedblock::hash_size),
			target,
			0
			})
	{
	}

	Federatedblock::Federatedblock(Blockheader&& header)
		: header{ std::make_shared<Blockheader>(std::move(header)) }
		// ^^^may beable to go back to unique, we'll see
		, local_updates{ std::vector<NetworkUpdate>{} }
		, global_update{ }// std::make_unique<NetworkUpdate>() }
	{}

	Federatedblock Federatedblock::Genisis(std::uint32_t version)
	{
		return Federatedblock(Blockheader::Genisis(version));
	}

	void Federatedblock::Mine(cactuar::hash_output&& prev_hash)
	{
		this->header->prev_hash = std::move(prev_hash);
		this->header->timestamp = Block::GenerateTimestamp();

		//TODO: add error for invalid cactaur values
		std::vector<std::byte> difficulty_array = this->header->target.Expand();

		cactuar::hash_output currHash = this->Hash();

		while (currHash > difficulty_array)
		{
			this->header->nonce += 1;
			currHash = this->Hash();
		}

		ExecuteTransactions();

		CalculateGlobalUpdate();
	}

	void Federatedblock::ExecuteTransactions()
	{
		for (auto tran : this->transactions)
		{
			if (tran.get().GetReceiver()[0] == std::byte{ 9 }) //if sent to contract
			{
				//Get Contract from Address
				//Execute Contract
			}
		}
	}


	//I think this can be rvalue since we dont need it after its added.
	void Federatedblock::AddLocalUpdate(NetworkUpdate&& update)
	{
		//TODO: For now, probably init block with network stucture (and possibly builder)
		//To enfores all updates to be of a compatible network
		//may need to change to make network_structure part of the block constructor
		/*
		if (this->global_update.network_structure.size() == 0)
		{
			this->global_update = GlobalNetworkUpdate(update.network_structure);
		}
		*/
		this->local_updates.push_back(std::move(update));
	}

	const NetworkUpdate& Federatedblock::GetGlobalUpdate() const
	{
		return this->global_update;
	}

	void Federatedblock::CalculateGlobalUpdate()
	{

		if (this->local_updates.size() == 0)
		{
			return;
		}

		this->global_update.delta_weights.reserve(this->local_updates.size());

		this->global_update.examples_seen =
			std::accumulate(
				this->local_updates.begin(), this->local_updates.end(), (std::int64_t)0,
				[](std::int64_t acc, const NetworkUpdate& elem) {return std::move(acc) + elem.examples_seen; }
		);

		//TODO: RedoALL

		
		for (const pulse::Matrix<float>& weight : this->local_updates[0].delta_weights)
		{
			global_update.delta_weights.push_back(
				weight * (float(this->local_updates[0].examples_seen) / global_update.examples_seen));
		}
		
		for (std::size_t ui = 1; ui < local_updates.size(); ui++)
		{
			const NetworkUpdate& update = local_updates[ui];

			for (std::int64_t wi = 0; wi < (std::int64_t) update.delta_weights.size(); wi++)
			{
				this->global_update.delta_weights[wi] += 
					update.delta_weights[wi] * ( float(update.examples_seen) / this->global_update.examples_seen);
				
				////////////////////////////////////////////////////////////////////////////
				//DEBUG:
				/*
				auto guit = this->global_update.delta_weights[wi].begin();
				auto luit = update.delta_weights[wi].begin();
				for (std::int64_t wii = 0;
					wii < (std::int64_t)this->global_update.delta_weights[wi].TotalSize();
					wii++)
				{
					std::cout << guit[wii] << " + ( "
						<< luit[wii] << " * "
						<< float(update.examples_seen) << " / " << this->global_update.examples_seen
						<< " = ";

					guit[wii] += luit[wii] * (float(update.examples_seen) / this->global_update.examples_seen);

					//	<< std::endl;

					std::cout << guit[wii] << std::endl;
				}
				*/
				//END DEBUG:
				////////////////////////////////////////////////////////////////////////////
			
			}
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
		os << "\nExample Seen:        " << block.global_update.examples_seen << std::endl;

		return os;
	}

} //namespace lindzei