//module;
#ifndef __ORPHAN_FEDERATED_BLOCK__
#define __ORPHAN_FEDERATED_BLOCK__

#include <cstdint>
//#include <cstddef>

//export module orphan:federated_block;

#include <vector> //import <vector>;
#include <memory> //import <memory>;
#include <utility> //import <utility>;
#include <ostream>

#include <pulse/pulse.h> //import pulse;

#include <orphan/federated_network.h> //import :federated_network;

//export 
namespace lindzei
{

	class Federatedblock : public pulse::Block
	{
	public:

		Federatedblock(std::uint32_t = 0x01,
			pulse::Target = { 0x21FFFFFF }, //pulse::Target = {},
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

		virtual inline const bool CompareWithTarget(const blockhash_type& hash) const override;

		virtual void Mine(blockhash_type&& prev_hash) override;

		virtual inline std::vector<std::byte> State() const override;

		void AddLocalUpdate(NetworkUpdate&& update);

		//constexpr
		template< class... Args >
		void AddLocalUpdate(Args&&... args);

		const NetworkUpdate& GetGlobalUpdate() const;

		friend std::ostream& operator<<(std::ostream& os, const Federatedblock& block);

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

	

	template< class ...Args >
	void Federatedblock::AddLocalUpdate(Args&&... args)
	{
		this->local_updates.emplace_back(std::move(args)...);

		//TODO: For now, probably init block with network stucture (and possibly builder)
		//To enfores all updates to be of a compatible network
		//may need to change to make network_structure part of the block constructor
		if (this->global_update.network_structure.size() == 0)
		{
			this->global_update = GlobalNetworkUpdate(this->local_updates[0].network_structure);
		}

	}
} //namespace lindzei

#endif
