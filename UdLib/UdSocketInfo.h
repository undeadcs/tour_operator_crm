/**
	UndeadCS SDK
	Базовая библиоткеа

	информация сокета
*/
#ifndef UNDEADCS_SDK_LIBRARY_BASE_SOCKET_INFO_H
#define UNDEADCS_SDK_LIBRARY_BASE_SOCKET_INFO_H

namespace UdSDK {

	class CUdSocketInfo {
		udInt		m_iPort,		// порт
					m_iFamily,		// семейство
					m_iType,		// тип
					m_iProtocol;	// протокол
		sockaddr_in	m_objAddress;	// информация об адресе
		string		m_szHost,		// хост
					m_szIp;			// IP
		hostent		m_objHost;		// информация о хосте

	public:
		CUdSocketInfo( const char* host = NULL, udInt iPort = 25000, udInt iFamily = AF_INET, udInt iType = SOCK_STREAM, udInt iProtocol = IPPROTO_TCP );
		CUdSocketInfo( const sockaddr_in& addr, udInt iType, udInt iProtocol );
		CUdSocketInfo( const CUdSocketInfo& objCopy );

		~CUdSocketInfo( );

		CUdSocketInfo& operator =( const CUdSocketInfo& objCopy );

		udInt		GetPort( )		const;
		udInt		GetFamily( )	const;
		udInt		GetType( )		const;
		sockaddr_in	GetAddr( )		const;
		string		GetHost( )		const;
		string		GetIp( )		const;
		hostent		GetHostEnt( )	const;

		friend class CUdSocket;

	}; // class CUdSocketInfo

} // namespace UdSDK

#endif
