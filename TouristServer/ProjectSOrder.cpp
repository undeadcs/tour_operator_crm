
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHSOrder::CHSOrder( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_sorder";
		m_szUpdIndex	= "id";
		m_szName		= "sorder";
		m_fnGetObject	= GetProjectSOrder;
	}

	CHSOrder::CHSOrder( const CHSOrder& objCopy ) {
	}

	CHSOrder::~CHSOrder( ) {
	}

	CHSOrder& CHSOrder::operator =( const CHSOrder& objCopy ) {
		return *this;
	}

} // namespace UdSDK
