
//module;
#ifndef __ATOMOS_NETWORK__
#define __ATOMOS_NETWORK__

#ifdef _WIN32 
#include <winsock2.h>
#include <ws2tcpip.h>

//#include <iphlpapi.h> //If needed MUST be after winsock2.h

#endif

#include <memory>
#include <iostream>

//export module atomos:network;

//export 
namespace pulse
{
	int f();

} //namespace pulse

#endif