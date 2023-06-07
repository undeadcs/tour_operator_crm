
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHTransferPoint::CHTransferPoint( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_transferpoint";
		m_szUpdIndex	= "id";
		m_szName		= "transferpoint";
		m_fnGetObject	= GetProjectTransferPoint;
	}

	CHTransferPoint::CHTransferPoint( const CHTransferPoint& objCopy ) {
	}

	CHTransferPoint::~CHTransferPoint( ) {
	}

	CHTransferPoint& CHTransferPoint::operator =( const CHTransferPoint& objCopy ) {
		return *this;
	}

} // namespace UdSDK