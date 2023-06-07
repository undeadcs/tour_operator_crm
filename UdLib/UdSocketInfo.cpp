
#include "includes.h"

namespace UdSDK {

	CUdSocketInfo::CUdSocketInfo( const char* host, udInt iPort, udInt iFamily, udInt iType, udInt iProtocol ) {
		WriteUdLibLog( "CUdSocketInfo::CUdSocketInfo, host='" + string( host ) + "',port=" + toString( iPort ) + "\n" );

		memset( &m_objAddress, 0, sizeof( m_objAddress ) );
		memset( &m_objHost, 0, sizeof( m_objHost ) );

		m_iPort		= iPort;
		m_iFamily	= iFamily;
		m_iType		= iType;
		m_iProtocol	= iProtocol;

		if ( host ) {
			m_szHost = host;
		}
		hostent *phost = gethostbyname( m_szHost.data( ) );
		char* ip = inet_ntoa( *( struct in_addr* ) *phost->h_addr_list );
		m_szIp = ip;

		WriteUdLibLog( "ip='" + m_szIp + "'\n" );

		m_objAddress.sin_family			= iFamily;
		m_objAddress.sin_addr.s_addr	= inet_addr( ip );
		m_objAddress.sin_port			= htons( iPort );
		
		memcpy( &m_objHost, phost, sizeof( m_objHost ) );
	}

	CUdSocketInfo::CUdSocketInfo( const sockaddr_in& addr, udInt iType, udInt iProtocol ) {
		memset( &m_objAddress, 0, sizeof( m_objAddress ) );
		memset( &m_objHost, 0, sizeof( m_objHost ) );

		m_iPort		= ntohs( addr.sin_port );
		m_iFamily	= addr.sin_family;
		m_iType		= iType;
		m_iProtocol	= iProtocol;
		m_szHost	= inet_ntoa( addr.sin_addr );	// там IP, имя хоста надо выгребать отдельно, но пока так

		hostent *phost = gethostbyname( m_szHost.data( ) );
		char* ip = inet_ntoa( *( struct in_addr* ) *phost->h_addr_list );
		m_szIp = ip;

		memset( &m_objAddress, 0, sizeof( m_objAddress ) );
		m_objAddress.sin_family			= addr.sin_family;
		m_objAddress.sin_addr.s_addr	= addr.sin_addr.s_addr;
		m_objAddress.sin_port			= addr.sin_port;

		//memcpy( &m_objAddress, &addr, sizeof( m_objAddress ) );
		memcpy( &m_objHost, phost, sizeof( m_objHost ) );
	}

	CUdSocketInfo::CUdSocketInfo( const CUdSocketInfo& objCopy ) {
		//memset( &m_objAddress, 0, sizeof( m_objAddress ) );
		//memset( &m_objHost, 0, sizeof( m_objHost ) );

		m_iPort		= objCopy.m_iPort;
		m_iFamily	= objCopy.m_iFamily;
		m_iType		= objCopy.m_iType;
		m_iProtocol	= objCopy.m_iProtocol;	

		memcpy( &m_objAddress, &( objCopy.m_objAddress ), sizeof( m_objAddress ) );
		memcpy( &m_objHost, &( objCopy.m_objHost ), sizeof( m_objHost ) );
	}

	CUdSocketInfo::~CUdSocketInfo( ) {
	}

	CUdSocketInfo& CUdSocketInfo::operator =( const CUdSocketInfo& objCopy ) {
		//memset( &m_objAddress, 0, sizeof( m_objAddress ) );
		//memset( &m_objHost, 0, sizeof( m_objHost ) );

		m_iPort		= objCopy.m_iPort;
		m_iFamily	= objCopy.m_iFamily;
		m_iType		= objCopy.m_iType;
		m_iProtocol	= objCopy.m_iProtocol;	

		memcpy( &m_objAddress, &( objCopy.m_objAddress ), sizeof( m_objAddress ) );
		memcpy( &m_objHost, &( objCopy.m_objHost ), sizeof( m_objHost ) );
		return *this;
	}

	udInt CUdSocketInfo::GetPort( ) const {
		return m_iPort;
	} // udInt GetPort

	udInt CUdSocketInfo::GetFamily( ) const {
		return m_iFamily;
	} // udInt GetFamily

	udInt CUdSocketInfo::GetType( ) const {
		return m_iType;
	} // udInt GetType

	sockaddr_in CUdSocketInfo::GetAddr( ) const {
		return m_objAddress;
	} // sockaddr_in& GetAddr

	string CUdSocketInfo::GetHost( ) const {
		return m_szHost;
	} // string GetHost

	string CUdSocketInfo::GetIp( ) const {
		return m_szIp;
	} // string GetIp

	hostent CUdSocketInfo::GetHostEnt( ) const {
		return m_objHost;
	} // hostent GetHostEnt

} // namespace UdSDK