#ifndef __CACTUAR_CRYPTO_POOL_H__
#define __CACTUAR_CRYPTO_POOL_H__

#include <vector>
#include <array>
#include <memory>

#include <cactuar/cactuar-crypto.h>


namespace pulse
{
	namespace cactuar
	{
		//Remember Templates must be defined in the header (come on, basic stuff).
		//TODO: make concurrent and make a true pool using things such as "in_use" and locks/seamaphores
		template<class PoolClass, typename Context, CrytographicTypeEnum Type>
		class ContextPool
		{
		public:

			static ContextPool& Instance();

			static Context* GetContext(Type type);

		private:

			virtual Context* GetContext_impl(Type) = 0;

		protected:
			ContextPool() : pool{ GetEnumCount<Type>() } {}

			//Despite that there can only be one, won't help with the abstract stuff.
			std::vector<std::array<Context*, 1>> pool;

			//Also protected? TODO:
			// 
			//inline as multiple definitions (but all are the same and all are in different translation units) (C++17)
			static inline std::unique_ptr<ContextPool> instance =
				std::unique_ptr<ContextPool>{ nullptr };
		};

		template<class PoolClass, typename Context, CrytographicTypeEnum Type>
		typename ContextPool<PoolClass, Context, Type>::ContextPool& ContextPool<PoolClass, Context, Type>::Instance()
		{
			if (instance.get() == nullptr)
			{
				instance.reset(new PoolClass());
			}
			return *instance;
		}

		template<class PoolClass, typename Context, CrytographicTypeEnum Type>
		Context* ContextPool<PoolClass, Context, Type>::GetContext(Type type) 
		{ 
			Context*& context = Instance().pool[static_cast<int>(type)][0];
			if (context == nullptr)
			{
				context = Instance().GetContext_impl(type);
			}
			return context;
		}

	} //namespace pulse::cactuar 
} //namespace pulse


#endif // !__CACTUAR_HASH_POOL_H__
