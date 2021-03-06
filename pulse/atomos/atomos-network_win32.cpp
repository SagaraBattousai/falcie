//module;

#ifdef _WIN32 

#include <winsock2.h>
#include <ws2tcpip.h>

//#include <iphlpapi.h> //If needed MUST be after winsock2.h


#include <memory>
#include <iostream>

//module atomos:network;
#include <atomos/atomos-network.h>

namespace pulse
{
	namespace atomos
	{
		//Could be singelton init now that I think of it ....
		class InitWinsock
		{
		public:
			static InitWinsock* Init();

		private:
			InitWinsock();
			static inline std::unique_ptr<InitWinsock> instance{ nullptr };

			WSADATA wsock_info;
			int result;
		};

		InitWinsock* InitWinsock::Init()
		{
			if (InitWinsock::instance.get() == nullptr)
			{
				InitWinsock::instance.reset(new InitWinsock());
			}
			return InitWinsock::instance.get();
		}

		InitWinsock::InitWinsock()
		{
			this->result = WSAStartup(MAKEWORD(2, 2), &(this->wsock_info));
			if (this->result != 0)
			{
				std::cerr << "WSAStartup failed with: " << this->result << std::endl;
			}
		}

		struct Socket
		{
		};

	} //namespace pulse::anima
} //namespace pulse


namespace pulse
{

	//may cause Errors
	std::unique_ptr<atomos::Socket> NewSocket()
	{
		std::unique_ptr soc = std::make_unique<atomos::Socket>();

		atomos::InitWinsock::Init();


		return soc;
	}

	int f()
	{
		return 32;
	}



}

#endif
