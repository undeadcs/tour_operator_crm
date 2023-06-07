
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHSRequest::CHSRequest( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_srequest";
		m_szUpdIndex	= "id";
		m_szName		= "srequest";
		m_fnGetObject	= GetProjectSRequest;
	}

	CHSRequest::CHSRequest( const CHSRequest& objCopy ) {
	}

	CHSRequest::~CHSRequest( ) {
	}

	CHSRequest& CHSRequest::operator =( const CHSRequest& objCopy ) {
		return *this;
	}

} // namespace UdSDK
