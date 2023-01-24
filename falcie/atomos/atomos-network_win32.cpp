//module;

#include <winsock2.h>
#include <ws2tcpip.h>

//#include <iphlpapi.h> //If needed MUST be after winsock2.h

#include <memory>

//module atomos:network;
#include <atomos/atomos-network.h>

namespace pulse
{
	/*
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



	*/
}