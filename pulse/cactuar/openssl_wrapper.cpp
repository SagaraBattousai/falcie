//module;

#include <vector>
#include <span>


//For Signing
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>

//Only needed in the static case with modules, which we can't use at the moment :(
//#if _WIN32
//#include <openssl/applink.c>
//#endif

//module cactuar:openssl_wraper; //private module
#include <cactuar/internal/cactuar-openssl_wrapper.h>

namespace pulse
{
	//Think I may be able to eliminate this file
	

		///////////////////////////////////////////////////////////////////
		//////////Eliptical Curves/////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		
		
	} //namespace pulse::cactuar
} //namespace pulse