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
		Federatedblock(std::uint32_t version = 0x01);// , pulse::HashAlgorithm = HashAlgorithm::SHA256);

		/*
		static inline Federatedblock Genisis(std::uint32_t version = 0x01, 
			HashAlgorithm hash_algo = HashAlgorithm::SHA256)
		{ return pulse::Blockheader::Genisis() }

		static Federatedblock Genisis(BlockHashFunction hashfunc,
			std::uint32_t = 0x01, HashAlgorithm = HashAlgorithm::SHA256);
			*/
		NetworkUpdate new_global_update(int64_t num_layers, int64_t *network_dims); ///<Should probably be hidden;

		int AddLocalUpdate(Federatedblock *block, NetworkUpdate *update);

		void GetGlobalUpdate(Federatedblock *block, std::vector<NetworkUpdate> update);


		//Good candidate for inline as just a function call
		virtual inline blockhash_type Hash() const override
		{
			return this->header.Hash();
		};

		virtual inline const blockhash_type& PrevHash() const override
		{
			return this->header.PrevHash();
		};

		virtual inline void Mine(const blockhash_type&& prev_hash) override
		{
			this->header.Mine(std::move(prev_hash));
			//CalculateGlobalUpdate();
		};

		virtual inline std::vector<std::byte> State() const override
		{
			return this->header.State();
		};



	private:
		std::uint32_t magic; //Always 0x43616C6F
		//std::uint32_t blocksize;
		//TODO: make unique_ptr (probably?)
		pulse::Blockheader header;
		std::vector<NetworkUpdate> local_updates;
		std::unique_ptr<NetworkUpdate> global_update;

		int CalculateGlobalUpdate();


	};

	Federatedblock::Federatedblock(std::uint32_t version)
		: magic { 0x43616C6F }
		, header {pulse::Blockheader(version) }
	{}

	NetworkUpdate new_global_update(int64_t num_layers, int64_t *network_dims)
	{
		return NetworkUpdate{};
	}

	int AddLocalUpdate(Federatedblock *block, NetworkUpdate *update) { return 0; }

	void GetGlobalUpdate(Federatedblock *block, std::vector<NetworkUpdate> update){}

	int CalculateGlobalUpdate(Federatedblock *block) { return 0; }



}