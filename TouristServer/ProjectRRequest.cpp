
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHRRequest::CHRRequest( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_rrequest";
		m_szUpdIndex	= "id";
		m_szName		= "rrequest";
		m_fnGetObject	= GetProjectRRequest;
	}

	CHRRequest::CHRRequest( const CHRRequest& objCopy ) {
	}

	CHRRequest::~CHRRequest( ) {
	}

	CHRRequest& CHRRequest::operator =( const CHRRequest& objCopy ) {
		return *this;
	}

} // namespace UdSDK
