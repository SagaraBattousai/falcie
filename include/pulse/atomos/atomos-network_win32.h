//module;
#ifndef __ATOMOS_NETWORK_WIN32_H__
#define __ATOMOS_NETWORK_WIN32_H__

#include <winsock2.h>
#include <ws2tcpip.h> //May not need in header

//#include <iphlpapi.h> //If needed MUST be after winsock2.h

#include <memory>
#include <iostream>

//module atomos:network;
#include <atomos/atomos-network.h>

//	} //namespace pulse::anima


namespace pulse
{
		//Could be singelton init now that I think of it ....
		class WinP2PNode : public P2PNode
		{
		public:
			WinP2PNode();

		private:
			int InitWinsock();

			static std::unique_ptr<WSADATA> wsock_info;
			static int startup_result;

		};

	

		struct Socket
		{
		};

} //namespace pulse


namespace pulse
{
	/*
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

	*/

}

#endif
