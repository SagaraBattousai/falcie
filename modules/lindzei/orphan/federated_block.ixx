module;

#include <cstdint>
//#include <cstddef>

export module orphan:federated_block;

import <vector>;
import <memory>;
import <utility>;

import pulse;

import :federated_network;

export namespace lindzei
{
	class Federatedblock : public pulse::Block
	{
	public:

		Federatedblock(std::uint32_t = 0x01, pulse::Target = { 0x21FFFFFF }, //pulse::Target = {},
			BlockHashFunction = pulse::PulseHash,
			pulse::HashAlgorithm = pulse::HashAlgorithm::SHA256);

		Federatedblock(pulse::Blockheader&&,
			BlockHashFunction = pulse::PulseHash,
			pulse::HashAlgorithm = pulse::HashAlgorithm::SHA256);

		static Federatedblock Genisis(std::uint32_t = 0x01,
			BlockHashFunction = pulse::PulseHash,
			pulse::HashAlgorithm = pulse::HashAlgorithm::SHA256);

		//Good candidate for inline as just a function call
		virtual inline blockhash_type Hash() const override
		{
			return this->hash_func(hash_algo, *this);
		};

		virtual inline const blockhash_type& PrevHash() const override
		{
			return this->header.get()->prev_hash;
		};

		virtual const bool CompareWithTarget(const blockhash_type& hash) const override
		{
			return hash < this->header.get()->target;
		};

		virtual void Mine(const blockhash_type&& prev_hash) override;

		virtual inline std::vector<std::byte> State() const override
		{
			return pulse::BlockheaderState(this->header.get(), this->hash_algo);
		};

		void AddLocalUpdate(NetworkUpdate&& update);

		//constexpr
		template< class... Args >
		void AddLocalUpdate(Args&&... args);

		const NetworkUpdate& GetGlobalUpdate() const;

	private:
		std::uint32_t magic; //Always 0x43616C6F
		//std::uint32_t blocksize;
		std::shared_ptr<pulse::Blockheader> header; //To allow for passing copy into Blockchain
		std::vector<NetworkUpdate> local_updates;
		NetworkUpdate global_update;

		pulse::HashAlgorithm hash_algo;
		BlockHashFunction hash_func;

		void CalculateGlobalUpdate();

	};

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

	//void Federatedblock::Mine(typename const Block::blockhash_type&& prev_hash)
	void Federatedblock::Mine(const blockhash_type&& prev_hash)
	{
		this->header->prev_hash = std::move(prev_hash);
		//this->header->timestamp = pulse::Block::GenerateTimestamp();
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

	template< class ...Args >
	void Federatedblock::AddLocalUpdate(Args&&... args)
	{
		this->local_updates.emplace_back(std::move(args));

		//TODO: For now, probably init block with network stucture (and possibly builder)
		//To enfores all updates to be of a compatible network
		//may need to change to make network_structure part of the block constructor
		if (this->global_update.network_structure.size() == 0)
		{
			this->global_update = GlobalNetworkUpdate(this->local_updates[0].network_structure);
		}

	}

	const NetworkUpdate& Federatedblock::GetGlobalUpdate() const
	{
		return this->global_update;
	}

	void Federatedblock::CalculateGlobalUpdate()
	{		
		for (const NetworkUpdate& update: this->local_updates)
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



}