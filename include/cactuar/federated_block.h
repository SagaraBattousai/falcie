//module;
#ifndef __ORPHAN_FEDERATED_BLOCK__
#define __ORPHAN_FEDERATED_BLOCK__

//NOTE:- Builder isnt enough to make an abstract base class because they will be so varied 
//(AKA concepts were the right idea). However "fluent" builder is great here!
//Althoughv... It could work .. No because concept is better as we won't use polymorphism
//i.e. using an abstract type pointing to a base ....

#include <cstdint>
#include <cstddef>

#include <vector>
#include <memory>
#include <functional>
#include <utility>
#include <ostream>

#include <pulse/pulse.h> //import pulse;

#include <cactuar/cactuar-keccak.h>

#include <orphan/federated_blockheader.h>
#include <orphan/federated_network.h> //import :federated_network;
#include <orphan/federated_transaction.h>

//export 
namespace lindzei
{

	class Federatedblock : public cactuar::Block
	{
	public:

		//TODO: Hacky fix as array deletes its default constructor if I dont have a constructor!
		Federatedblock();

		// When hash is changed change here too
		constexpr static int hash_size = 32;
		virtual inline cactuar::hash_output Hash() const override 
		{ 
			return cactuar::Keccak256(header->State());
		};

		virtual inline const cactuar::hash_output& PrevHash() const override 
		{ 
			return this->header.get()->prev_hash; 
		};

		virtual inline const bool CompareWithTarget(const cactuar::hash_output& hash) const override
		{
			return hash < this->header.get()->target;
		}

		virtual void Mine(cactuar::hash_output&& prev_hash) override;

		virtual void ExecuteTransactions();

		virtual void AddTransaction(cactuar::Transaction* transaction) override
		{
			this->transactions.push_back(std::ref(*transaction)); //I think this is dangerous AF!!
		}

		virtual const std::vector<std::reference_wrapper<cactuar::Transaction>> GetTransactions() const override
		{
			return transactions;
		}

		void AddLocalUpdate(NetworkUpdate&& update);

		//constexpr
		template<class... Args >
		void AddLocalUpdate(Args&&... args);

		const NetworkUpdate& GetGlobalUpdate() const;

		friend std::ostream& operator<<(std::ostream& os, const Federatedblock& block);

	private:
		Federatedblock(std::uint32_t, cactuar::Target);// , cactuar::HashFunction);

		Federatedblock(Blockheader&&);// , cactuar::HashFunction);

		static Federatedblock Genisis(std::uint32_t = 0x01);

		constexpr static std::uint32_t magic = 0x43616C6F; //Always 0x43616C6F
		//std::uint32_t blocksize;
		std::shared_ptr<Blockheader> header; //To allow for passing copy into Blockchain
		std::vector<NetworkUpdate> local_updates {};
		NetworkUpdate global_update {};

		std::vector< std::reference_wrapper<cactuar::Transaction>> transactions{};

		void CalculateGlobalUpdate();

	public:

		class Builder
		{
		public:

			//These probably don't need to ne inline and may be detrimental but like it in the same file

			Builder& WithVersion(std::uint32_t version)
			{
				this->m_version = std::move(version);
				return *this;
			};

			Builder& WithTarget(cactuar::Target target)
			{
				this->m_target = std::move(target);
				return *this;
			};

			Federatedblock Build() const
			{
				return Federatedblock::Federatedblock(m_version, m_target);// , m_hash_func, m_hash_algo);
			};

			Federatedblock Genisis() const
			{
				return Federatedblock::Federatedblock(Blockheader::Genisis());
			};

		private:

			std::uint32_t m_version{ 0x01 };

			cactuar::Target m_target{ cactuar::Target::MinimumDifficulty };
		};
	};



	template< class ...Args >
	void Federatedblock::AddLocalUpdate(Args&&... args)
	{
		//TODO: Is move safe? not forward? Looks like it isnt safe :)
		this->local_updates.emplace_back(std::forward<Args>(args)...);

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
