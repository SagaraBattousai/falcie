//module;
#ifndef __ORPHAN_FEDERATED_BLOCK__
#define __ORPHAN_FEDERATED_BLOCK__

//NOTE:- Builder isnt enough to make an abstract base class because they will be so varied 
//(AKA concepts were the right idea). However "fluent" builder is great here!
//Althoughv... It could work .. No because concept is better as we won't use polymorphism
//i.e. using an abstract type pointing to a base ....

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

		//TODO: Hacky fix as array deletes its default constructor if I dont have a constructor!
		Federatedblock();

		//Good candidate for inline as just a function call
		virtual inline blockhash_type Hash() const override 
		{ 
			return this->hash_func(hash_algo, *this); 
		};

		virtual inline const blockhash_type& PrevHash() const override { 
			return this->header.get()->prev_hash; 
		};

		virtual inline const bool CompareWithTarget(const blockhash_type& hash) const override;

		virtual void Mine(blockhash_type&& prev_hash) override;

		virtual inline std::vector<std::byte> State() const override;

		void AddLocalUpdate(NetworkUpdate&& update);

		//constexpr
		template<class... Args >
		void AddLocalUpdate(Args&&... args);

		const NetworkUpdate& GetGlobalUpdate() const;

		friend std::ostream& operator<<(std::ostream& os, const Federatedblock& block);
	

	private:
		Federatedblock(std::uint32_t, pulse::Target, BlockHashFunction, pulse::HashAlgorithm);

		Federatedblock(pulse::Blockheader&&, BlockHashFunction, pulse::HashAlgorithm);

		static Federatedblock Genisis(std::uint32_t = 0x01,
			BlockHashFunction = pulse::PulseHash,
			pulse::HashAlgorithm = pulse::HashAlgorithm::SHA256);

		constexpr static std::uint32_t magic = 0x43616C6F; //Always 0x43616C6F
		//std::uint32_t blocksize;
		std::shared_ptr<pulse::Blockheader> header; //To allow for passing copy into Blockchain
		std::vector<NetworkUpdate> local_updates {};
		NetworkUpdate global_update {};

		pulse::HashAlgorithm hash_algo;
		BlockHashFunction hash_func;

		void CalculateGlobalUpdate();

	public:

		class Builder
		{
		public:

			//These probably don't need to ne inline and may be detrimental but like it in the same file

			Builder& WithVersion(std::uint32_t version)
			{
				this->version = std::move(version);
				return *this;
			};

			Builder& WithTarget(pulse::Target target)
			{
				this->target = std::move(target);
				return *this;
			};

			//Is it a value ...
			Builder& WithHashFunction(BlockHashFunction hash_func)
			{
				//Or does the compiler do this for me?
				this->hash_func = std::move(hash_func);
				return *this;
			};

			Builder& WithHashAlgorithm(pulse::HashAlgorithm hash_algo)
			{
				this->hash_algo = hash_algo;
				return *this;
			};

			Federatedblock Build() const
			{
				return Federatedblock::Federatedblock(version, target, hash_func, hash_algo);
			};

			Federatedblock Genisis() const
			{
				return Federatedblock::Federatedblock(
					pulse::Blockheader::Genisis(hash_algo, version), hash_func, hash_algo);
			};

		private:

			std::uint32_t version{ 0x01 };

			pulse::Target target{ pulse::Target::MinimumDifficulty };

			BlockHashFunction hash_func{ pulse::PulseHash };

			pulse::HashAlgorithm hash_algo{ pulse::HashAlgorithm::SHA256 };
		};
	};



	template< class ...Args >
	void Federatedblock::AddLocalUpdate(Args&&... args)
	{
		//TODO: Is move safe? not forward?
		this->local_updates.emplace_back(std::move(args)...);

		//TODO: For now, probably init block with network stucture (and possibly builder)
		//To enfores all updates to be of a compatible network
		//may need to change to make network_structure part of the block constructor
		//if (this->global_update.network_structure.size() == 0)
		//{
		//	this->global_update = GlobalNetworkUpdate(this->local_updates[0].network_structure);
		//}

	}
} //namespace lindzei

#endif
