
#include "includes.h"

namespace UdSDK {

	CServerAccount::CServerAccount( ) {
	}

	CServerAccount::CServerAccount( const string& szHost, const string& szUsername, const string& szPassword ) {
		m_szHost		= szHost;
		m_szUsername	= szUsername;
		m_szPassword	= szPassword;
	}

	CServerAccount::~CServerAccount( ) {
	}

} // namespace UdSDK
