
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHRoute::CHRoute( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_route";
		m_szUpdIndex	= "id";
		m_szName		= "route";
		m_fnGetObject	= GetProjectRoute;
	}

	CHRoute::CHRoute( const CHRoute& objCopy ) {
	}

	CHRoute::~CHRoute( ) {
	}

	CHRoute& CHRoute::operator =( const CHRoute& objCopy ) {
		return *this;
	}

} // namespace UdSDK