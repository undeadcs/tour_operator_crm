
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHDirect::CHDirect( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_direct";
		m_szUpdIndex	= "id";
		m_szName		= "direct";
		m_fnGetObject	= GetProjectDirect;
	}

	CHDirect::CHDirect( const CHDirect& objCopy ) {
	}

	CHDirect::~CHDirect( ) {
	}

	CHDirect& CHDirect::operator =( const CHDirect& objCopy ) {
		return *this;
	}

} // namespace UdSDK
