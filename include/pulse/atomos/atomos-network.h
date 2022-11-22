
//module;
#ifndef __ATOMOS_NETWORK__
#define __ATOMOS_NETWORK__

#include <memory>
#include <iostream>

//export module atomos:network;

//export 
namespace pulse
{
	//abstract class
	class P2PNode
	{
		
	};

	//Will use -> #ifdef _WIN32 to check os when called ...? Can a dll/library do this??
	std::unique_ptr<P2PNode> MakeP2PNode(); //For auto os detection 
} //namespace pulse

#endif