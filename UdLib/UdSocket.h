/**
	UndeadCS SDK
	Базовая библиоткеа

	сокет
*/
#ifndef UNDEADCS_SDK_LIBRARY_BASE_SOCKET_H
#define UNDEADCS_SDK_LIBRARY_BASE_SOCKET_H

#include <winsock2.h>

#ifndef WINSOCK_VERSION
#define WINSOCK_VERSION				MAKEWORD( 2, 2 )
#endif
#define GET_SOCKET_ERROR			( WSAGetLastError( ) )
#define CLOSE_SOCKET( hSocket )		{ closesocket( hSocket ); hSocket = INVALID_SOCKET; }

namespace UdSDK {

	class CUdSocket {
	public:
		enum {
			UDSOCKET_SELECT_READ	= 1<<0,
			UDSOCKET_SELECT_WRITE	= 1<<1,
			UDSOCKET_SELECT_ERROR	= 1<<2
		};

	private:
		SOCKET			m_hSocket;	// хэндл сокета
		udPSocketInfo	m_pInfo;	// информация о сокете

	public:
		CUdSocket( );
		CUdSocket( const udSocketInfo& objInfo, SOCKET hSocket = INVALID_SOCKET );
		CUdSocket( const CUdSocket& objCopy );

		virtual ~CUdSocket( );

		CUdSocket& operator =( const CUdSocket& objCopy );

		udInt	Create( );
		udInt	Bind( );
		udInt	Listen( );
		udInt	Connect( );
		udInt	Shutdown( udInt iFlag = SD_BOTH );
		udInt	Close( );
		udPSocket	Accept( );
		//SOCKET	Accept( );
		udInt	Select( udInt iFlags = UDSOCKET_SELECT_READ, udDWord dwMicroSec = INFINITE );
		udInt	Wait( udInt iFlags = UDSOCKET_SELECT_READ, udDWord dwInterval = 0 );

		udInt	Send( const char* data, udInt len );
		udInt	Send( const udPByte data, udInt len );
		udInt	Send( const udBinaryData& data );
		udInt	Send( const udPBinaryData data );

		udInt	SendByte( udByte byte );
		udInt	SendByte( char byte );
		
		udInt	Read( char** data, udInt iBytesLimit = -1 );
		udInt	ReadByte( char* dst );
		udInt	Recv( char* buf, int len, int flags );

		udInt	IoCtl( long cmd, u_long* argp );
		udInt	GetOpt( int level, int optname, char* optval, int* optlen );
		udInt	SetOpt( int level, int optname, const char* optval, int optlen );

		udPSocketInfo	GetInfo( )	const { return m_pInfo;	}

	};  // class CUdSocket

} // namespace UdSDK

#endif
