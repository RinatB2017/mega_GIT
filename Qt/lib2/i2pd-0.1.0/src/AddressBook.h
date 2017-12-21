#ifndef ADDRESS_BOOK_H__
#define ADDRESS_BOOK_H__

#include <string.h>
#include <string>
#include <map>
#include "base64.h"
#include "util.h"
#include "Identity.h"
#include "Log.h"

namespace i2p
{
namespace data
{
	class AddressBook
	{
		public:

			AddressBook ();
			bool GetIdentHash (const std::string& address, IdentHash& ident);
			const IdentHash * FindAddress (const std::string& address);
			void InsertAddress (const std::string& address, const std::string& base64); // for jump service
		
		private:
	
			void LoadHosts ();
			void LoadHostsFromI2P ();

			std::map<std::string, IdentHash>  m_Addresses;
			bool m_IsLoaded, m_IsDowloading;
	};
}
}

#endif


