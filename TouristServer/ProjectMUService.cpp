
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHMUService::CHMUService( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_muservice";
		m_szUpdIndex	= "id";
		m_szName		= "muservice";
		m_fnGetObject	= GetProjectMUService;
	}

	CHMUService::CHMUService( const CHMUService& objCopy ) {
	}

	CHMUService::~CHMUService( ) {
	}

	CHMUService& CHMUService::operator =( const CHMUService& objCopy ) {
		return *this;
	}

} // namespace UdSDK
