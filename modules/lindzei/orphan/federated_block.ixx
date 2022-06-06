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
		//Federatedblock(std::uint32_t = 0x01, pulse::HashAlgorithm = pulse::HashAlgorithm::SHA256);

		Federatedblock(std::uint32_t = 0x01, pulse::Target = {},
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

	
		//NetworkUpdate new_global_update(int64_t num_layers, int64_t * network_dims); ///<Should probably be hidden;

		//int AddLocalUpdate(Federatedblock *block, NetworkUpdate *update);

		//void GetGlobalUpdate(Federatedblock *block, std::vector<NetworkUpdate> update);

	private:
		std::uint32_t magic; //Always 0x43616C6F
		//std::uint32_t blocksize;
		//std::unique_ptr<pulse::Blockheader> header;
		std::shared_ptr<pulse::Blockheader> header;
		std::vector<NetworkUpdate> local_updates;
		//std::unique_ptr<NetworkUpdate> global_update;

		pulse::HashAlgorithm hash_algo;
		BlockHashFunction hash_func;

		//int CalculateGlobalUpdate();

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
	{}

	Federatedblock::Federatedblock(pulse::Blockheader&& header,
		BlockHashFunction hash_func, pulse::HashAlgorithm hash_algo)
		: magic{ 0x43616C6F }
		//, header{ std::move(header) }
		//, header{ std::make_unique<pulse::Blockheader>(std::move(header)) }
		, header{ std::make_shared<pulse::Blockheader>(std::move(header)) }
		, local_updates{ std::vector<NetworkUpdate>{} }
		//, global_update{ std::make_unique<NetworkUpdate>() }
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

		//CalculateGlobalUpdate();

	}












	NetworkUpdate new_global_update(int64_t num_layers, int64_t *network_dims)
	{
		return NetworkUpdate{};
	}

	int AddLocalUpdate(Federatedblock *block, NetworkUpdate *update) { return 0; }

	void GetGlobalUpdate(Federatedblock *block, std::vector<NetworkUpdate> update) {}

	int CalculateGlobalUpdate(Federatedblock *block) { return 0; }



}