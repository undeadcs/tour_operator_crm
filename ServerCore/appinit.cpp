
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CAppInit::CAppInit( ) {
		m_pApp = NULL;
	}

	CAppInit::CAppInit( const CAppInit& objCopy ) {
	}

	CAppInit::~CAppInit( ) {
	}

	CAppInit& CAppInit::operator =( const CAppInit& objCopy ) {
		return *this;
	}

} // namespace UdSDK
