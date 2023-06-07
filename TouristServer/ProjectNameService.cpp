
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHNameService::CHNameService( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_nameservice";
		m_szUpdIndex	= "id";
		m_szName		= "nameservice";
		m_fnGetObject	= GetProjectNameService;
	}

	CHNameService::CHNameService( const CHNameService& objCopy ) {
	}

	CHNameService::~CHNameService( ) {
	}

	CHNameService& CHNameService::operator =( const CHNameService& objCopy ) {
		return *this;
	}

} // namespace UdSDK