
#include "includes.h"

namespace UdSDK {

	CDatabaseAccount::CDatabaseAccount( ) {
	}

	CDatabaseAccount::CDatabaseAccount( const string& szHost, const string& szUsername, const string& szPassword, const string& szDatabase ) {
		CServerAccount( szHost, szUsername, szPassword );

		m_szDatabase = szDatabase;
	}

	CDatabaseAccount::~CDatabaseAccount( ) {
		m_szDatabase.clear( );
	}

} // namespace UdSDK
