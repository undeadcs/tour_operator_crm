
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHHostSide::CHHostSide( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_hostside";
		m_szUpdIndex	= "id";
		m_szName		= "hostside";
		m_fnGetObject	= GetProjectHostSide;
	}

	CHHostSide::CHHostSide( const CHHostSide& objCopy ) {
	}

	CHHostSide::~CHHostSide( ) {
	}

	CHHostSide& CHHostSide::operator =( const CHHostSide& objCopy ) {
		return *this;
	}

} // namespace UdSDK