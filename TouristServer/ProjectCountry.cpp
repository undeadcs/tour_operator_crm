
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHCountry::CHCountry( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_country";
		m_szUpdIndex	= "id";
		m_szName		= "country";
		m_fnGetObject	= GetProjectCountry;
	}

	CHCountry::CHCountry( const CHCountry& objCopy ) {
	}

	CHCountry::~CHCountry( ) {
	}

	CHCountry& CHCountry::operator =( const CHCountry& objCopy ) {
		return *this;
	}

} // namespace UdSDK
