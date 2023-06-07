
#include "includes.h"

namespace UdSDK {

	CUdSocketAddress::CUdSocketAddress( udInt iPort, udInt iFamily, udInt iType, udInt iProtocol ) {
		m_iPort		= iPort;
		m_iFamily	= iFamily;
		m_iType		= iType;
		m_iProtocol	= iProtocol;

		memset( &m_objAddress, 0, sizeof( m_objAddress ) );
	}

	CUdSocketAddress::~CUdSocketAddress( ) {
	}

} // namespace UdSDK