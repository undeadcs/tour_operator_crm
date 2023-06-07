
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHTRequest::CHTRequest( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_trequest";
		m_szUpdIndex	= "id";
		m_szName		= "trequest";
		m_fnGetObject	= GetProjectTRequest;
	}

	CHTRequest::CHTRequest( const CHTRequest& objCopy ) {
	}

	CHTRequest::~CHTRequest( ) {
	}

	CHTRequest& CHTRequest::operator =( const CHTRequest& objCopy ) {
		return *this;
	}

} // namespace UdSDK
