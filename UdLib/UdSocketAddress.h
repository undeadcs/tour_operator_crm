/**
	UndeadCS SDK
	Базовая библиоткеа

	адрес сокета
*/
#ifndef UNDEADCS_SDK_LIBRARY_BASE_SOCKET_ADDRESS_H
#define UNDEADCS_SDK_LIBRARY_BASE_SOCKET_ADDRESS_H

namespace UdSDK {

	class CUdSocketAddress {
		udInt		m_iPort,		// порт
					m_iFamily,		// семейство
					m_iType,		// тип
					m_iProtocol;	// протокол
		sockaddr_in	m_objAddress;	// информация об адресе

	public:
		CUdSocketAddress( udInt iPort = 25000, udInt iFamily = AF_INET, udInt iType = SOCK_STREAM, udInt iProtocol = IPPROTO_TCP );

		~CUdSocketAddress( );

	}; // class CUdSocketAddress

} // namespace UdSDK

#endif
