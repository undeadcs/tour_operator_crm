
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHSTour::CHSTour( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_stour";
		m_szUpdIndex	= "id";
		m_szName		= "stour";
		m_fnGetObject	= GetProjectSTour;
	}

	CHSTour::CHSTour( const CHSTour& objCopy ) {
	}

	CHSTour::~CHSTour( ) {
	}

	CHSTour& CHSTour::operator =( const CHSTour& objCopy ) {
		return *this;
	}

} // namespace UdSDK