//module;
#ifndef __CACTUAR_BLOCK_H__
#define __CACTUAR_BLOCK_H__

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

#include <orphan/orphan-keccak.h>

#include <cactuar/cactuar-blockheader.h>
#include <cactuar/cactuar-network_structure_update.h>
#include <cactuar/cactuar-target.h>
#include <cactuar/cactuar-transaction.h>


//export 
namespace cactuar
{

	class Block
	{
	public:

		//TODO: Hacky fix as array deletes its default constructor if I dont have a constructor!
		Block();

		static Block Genisis(std::uint32_t = 0x01);

		// When hash is changed change here too
		constexpr static int hash_size = 32;
		virtual inline orphan::hash_output Hash() const
		{
			return orphan::Keccak256(header.get()->State());
		};

		virtual inline const orphan::hash_output& PrevHash() const
		{
			return this->header.get()->prev_hash;
		};

		virtual inline const bool CompareWithTarget(const orphan::hash_output& hash) const
		{
			return hash < this->header.get()->target;
		}

		virtual void Mine(orphan::hash_output&& prev_hash);

		virtual void ExecuteTransactions();

		virtual void AddTransaction(Transaction* transaction); //are const refs polymorphic? // want a copy right?

		virtual const std::vector<Transaction*> GetTransactions() const;

		void AddLocalUpdate(NetworkStructureUpdate&& update);

		//constexpr
		template<class... Args >
		void AddLocalUpdate(Args&&... args);

		const NetworkStructureUpdate& GetGlobalUpdate() const;

		friend std::ostream& operator<<(std::ostream& os, const Block& block);

	private:
		Block(std::uint32_t, Target);// , cactuar::HashFunction);

		Block(Blockheader&&);

		constexpr static std::uint32_t magic = 0x43616C6F; //Always 0x43616C6F

		std::shared_ptr<Blockheader> header; //To allow for passing copy into Blockchain

		std::vector<NetworkStructureUpdate> local_updates{};

		NetworkStructureUpdate global_update{};

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

			Block Build() const
			{
				return Block(m_version, m_target);// , m_hash_func, m_hash_algo);
			};

			Block Genisis() const
			{
				return Block(Blockheader::Genisis());
			};

		private:

			std::uint32_t m_version{ 0x01 };

			Target m_target{ Target::MinimumDifficulty };
		};
	};



	template< class ...Args >
	void Block::AddLocalUpdate(Args&&... args)
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
} //namespace cactuar

#endif
