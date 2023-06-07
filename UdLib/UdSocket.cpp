
#include "includes.h"

namespace UdSDK {

	CUdSocket::CUdSocket( ) {
		m_hSocket	= INVALID_SOCKET;
		m_pInfo		= new udSocketInfo;
	}

	CUdSocket::CUdSocket( const udSocketInfo& objInfo, SOCKET hSocket ) {
		m_hSocket	= hSocket;
		m_pInfo		= new udSocketInfo( objInfo );
	}

	CUdSocket::CUdSocket( const CUdSocket& objCopy ) {
		m_hSocket	= objCopy.m_hSocket;
		*m_pInfo	= *( objCopy.m_pInfo );
	}

	CUdSocket::~CUdSocket( ) {
		if ( m_hSocket != INVALID_SOCKET ) {
			this->Close( );
		}
		udDELETE( m_pInfo );
	}

	CUdSocket& CUdSocket::operator =( const CUdSocket& objCopy ) {
		m_hSocket	= objCopy.m_hSocket;
		*m_pInfo	= *( objCopy.m_pInfo );
		return *this;
	}

	udInt CUdSocket::Create( ) {
		m_hSocket = socket( m_pInfo->m_iFamily, m_pInfo->m_iType, m_pInfo->m_iProtocol );
		if ( m_hSocket == INVALID_SOCKET ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "failed socket, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return 0;
		}

		udChar tmp[ 1024 ];
		sprintf_s( tmp, 1024, "создан сокет, f=%d, t=%d, p=%d\n", m_pInfo->m_iFamily, m_pInfo->m_iType, m_pInfo->m_iProtocol );
		WriteUdLibLog( tmp );

		return 1;
	} // udInt Create

	udInt CUdSocket::Bind( ) {
		WriteUdLibLog( "CUdSocket::Bind, host='" +
			m_pInfo->GetHost( ) + "',ip='" + m_pInfo->GetIp( ) + "',"
			"port=" + toString( m_pInfo->GetPort( ) ) + "\n"
		);

		if ( bind( m_hSocket, ( sockaddr* ) &( m_pInfo->m_objAddress ), sizeof( m_pInfo->m_objAddress ) ) == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "failed binding, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return 0;
		}

		udChar tmp[ 1024 ];
		sprintf_s( tmp, 1024, "присоединили сокет к порту %d\n", m_pInfo->m_iPort );
		WriteUdLibLog( tmp );

		return 1;
	} // udInt Bind

	udInt CUdSocket::Listen( ) {
		if ( listen( m_hSocket, 1 ) == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "listen failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return 0;
		}

		udChar tmp[ 1024 ];
		sprintf_s( tmp, 1024, "поставили сокет прослушивать порт %d\n", m_pInfo->m_iPort );
		WriteUdLibLog( tmp );

		return 1;
	} // udInt Listen

	udInt CUdSocket::Connect( ) {
		WriteUdLibLog( "коннектимся к, host='" +
			m_pInfo->m_szHost + "',ip='" + m_pInfo->m_szIp + "',"
			"port=" + toString( m_pInfo->m_iPort ) + "\n"
		);

		if ( connect( m_hSocket, ( sockaddr* ) &( m_pInfo->m_objAddress ), sizeof( m_pInfo->m_objAddress ) ) == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "connect failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return 0;
		}

		udChar tmp[ 1024 ];
		sprintf_s( tmp, 1024, "присоединились к хосту '%s', на порт %d\n", m_pInfo->m_szHost.data( ), m_pInfo->m_iPort );
		WriteUdLibLog( tmp );

		return 1;
	} // udInt Connect

	udInt CUdSocket::Shutdown( udInt iFlag ) {
		if ( shutdown( m_hSocket, iFlag ) != 0 ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "shutdown failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return 0;
		}

		udChar tmp[ 1024 ];
		sprintf_s( tmp, 1024, "закрыли соединение, флаг %d\n", iFlag );
		WriteUdLibLog( tmp );

		return 1;
	} // udInt Shutdown

	udInt CUdSocket::Close( ) {
		if ( m_hSocket == INVALID_SOCKET ) {
			return 1;
		}
		if ( closesocket( m_hSocket ) == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "closesocket failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return 0;
		}
		m_hSocket = INVALID_SOCKET;

		udChar tmp[ 1024 ];
		sprintf_s( tmp, 1024, "закрыли сокет\n" );
		WriteUdLibLog( tmp );

		return 1;
	} // udInt Close

	udPSocket CUdSocket::Accept( ) {
		sockaddr_in addr;
		memset( &addr, 0, sizeof( addr ) );
		int size = sizeof( addr );
		SOCKET hSocket = accept( m_hSocket, ( sockaddr* ) &addr, &size );
		if ( hSocket == INVALID_SOCKET ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "accept failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
			return NULL;
		} else {
			udChar tmp[ 1024 ];
			sprintf_s( tmp, 1024, "приняли соединение на новый сокет\n" );
			WriteUdLibLog( tmp );
		}
		if ( size != sizeof( addr ) ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "accept, размер адреса не равен размеру структуры, struct size=%d, final size=%d", sizeof( addr ), size );
			WriteUdLibLog( log );
			return NULL;
		}

		return new udSocket( udSocketInfo( addr, m_pInfo->m_iType, m_pInfo->m_iProtocol ), hSocket );
	} // SOCKET Accept

	udInt CUdSocket::Select( udInt iFlags, udDWord dwMicroSec ) {
		udInt	ret			= 0;
		fd_set	*fdRead		= NULL,
				*fdWrite	= NULL,
				*fdError	= NULL;
		timeval	*pTimeout	= NULL;
		if ( iFlags & UDSOCKET_SELECT_READ ) {
			fdRead = new fd_set;
			fdRead->fd_count = 0;
			FD_SET( m_hSocket, fdRead );
		}
		if ( iFlags & UDSOCKET_SELECT_WRITE ) {
			fdWrite = new fd_set;
			fdWrite->fd_count = 0;
			FD_SET( m_hSocket, fdWrite );
		}
		if ( iFlags & UDSOCKET_SELECT_ERROR ) {
			fdError = new fd_set;
			fdError->fd_count = 0;
			FD_SET( m_hSocket, fdError );
		}
		if ( dwMicroSec != INFINITE ) {
			pTimeout = new timeval;
			pTimeout->tv_sec	= dwMicroSec / 1000;
			pTimeout->tv_usec	= dwMicroSec;
		}
		ret = select( 0, fdRead, fdWrite, fdError, pTimeout );
		if ( ret == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "select failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
		}
		udDELETE( fdRead );
		udDELETE( fdWrite );
		udDELETE( fdError );
		udDELETE( pTimeout );
		return ret;
	} // udInt Select

	udInt CUdSocket::Wait( udInt iFlags, udDWord dwInterval ) {
		udInt ret = 0, res = 0;
		udDWord dwMaxIteration = 1000; // максимум итераций
		while( dwMaxIteration > 0 ) {
			res = this->Select( iFlags, dwInterval );
			if ( res == 1 ) {
				ret = 1;
				break;
			} else if ( res == - 1 ) {
				ret = 0;
				break;
			}
			// res == 0 - продолжим ждать
			--dwMaxIteration;
		}
		return ret;
	} // udInt Wait

	udInt CUdSocket::Send( const char* data, udInt len ) {
		WriteUdLibLog(
			"CUdSocket::Send( const char* data, udInt len ), len=" + toString( len ) + "\n"
		);
		udInt sent = send( m_hSocket, data, len, 0 );
		if ( sent == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "send failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
		} else {
			udChar tmp[ 1024 ];
			sprintf_s( tmp, 1024, "отправили данные, байт: %d\n", sent );
			WriteUdLibLog( tmp );
		}

		return sent;
	} // udInt Send

	udInt CUdSocket::Send( const udPByte data, udInt len ) {
		WriteUdLibLog( "CUdSocket::Send( const udPByte data, udInt len )\n" );
		if ( len > 0 ) {
			return this->Send( ( const char* ) data, len );
		}
		return 0;
	} // udInt Send

	udInt CUdSocket::Send( const udBinaryData& data ) {
		WriteUdLibLog( "CUdSocket::Send( const udBinaryData& data )\n" );
		if ( data.size( ) > 0 ) {
			return this->Send( ( const char* ) data.get( ), data.size( ) );
		}
		return 0;
	} // udInt Send

	udInt CUdSocket::Send( const udPBinaryData data ) {
		WriteUdLibLog( "CUdSocket::Send( const udPBinaryData data )\n" );
		if ( data->size( ) > 0 ) {
			return this->Send( ( const char* ) data->get( ), data->size( ) );
		}
		return 0;
	} // udInt Send

	udInt CUdSocket::SendByte( udByte byte ) {
		char *tmp = new char[ 1 ];
		tmp[ 0 ] = ( char ) byte;
		udInt sent = send( m_hSocket, tmp, 1, 0 );
		if ( sent == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "send failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
		} else {
			udChar tmp[ 1024 ];
			sprintf_s( tmp, 1024, "отправили данные, байт: %d\n", sent );
			WriteUdLibLog( tmp );
		}
		delete [ ] tmp;

		return sent;
	} // udInt SendByte

	udInt CUdSocket::SendByte( char byte ) {
		char *tmp = new char[ 1 ];
		tmp[ 0 ] = byte;
		udInt sent = send( m_hSocket, tmp, 1, 0 );
		if ( sent == SOCKET_ERROR ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "send failed, error code: %d\n", GET_SOCKET_ERROR );
			WriteUdLibLog( log );
		} else {
			udChar tmp[ 1024 ];
			sprintf_s( tmp, 1024, "отправили данные, байт: %d\n", sent );
			WriteUdLibLog( tmp );
		}
		delete [ ] tmp;

		return sent;
	} // udInt SendByte

	udInt CUdSocket::Read( char** data, udInt iBytesLimit ) {
		assert( m_hSocket != INVALID_SOCKET );
		udInt	bytes		= 0,
				res			= 0,
				frame		= 1024,
				capacity	= 0;
		char	byte		= 0;
		char	*_data		= NULL;
		bool	bFinish		= false;

		// создаем фрэйм
		capacity = frame;
		_data = new char[ capacity ];

		while( !bFinish ) {
			byte = 0;
			res = this->ReadByte( &byte );
			if ( res == 1 ) {
				if ( bytes >= capacity ) {
					capacity += frame;
					char *tmp = new char[ capacity ];
					memcpy( tmp, _data, bytes );
					delete [ ] _data;
					_data = tmp;
				}

				_data[ bytes++ ] = byte;

				if ( ( iBytesLimit > 0 ) && ( bytes >= iBytesLimit ) ) {
					bFinish = true;
					break;
				}
			} else {
				/*if ( res == -1 ) { // произошла ошибка при чтении, в буфер возможно запихали мусор
					bytes = 0;
				}*/
				bFinish = true;
				break;
			}
		}

		if ( bytes > 0 ) {
			//WriteUdLibLog( "считано " + UdSDK::toString( bytes ) + "\n" );
			*data = _data;
		} else {
			delete [ ] _data;
		}

		WriteUdLibLog(
			"CUdSocket::Read( char** data, udInt iBytesLimit ), "
			"iBytesLimit=" + toString( iBytesLimit ) + ", "
			"bytes=" + toString( bytes ) + "\n"
		);

		return bytes;
	} // udInt Read

	udInt CUdSocket::ReadByte( char* dst ) {
		char buf[ 2 ];
		udInt read = recv( m_hSocket, buf, 1, 0 );
		switch( read ) {
		//switch( recv( m_hSocket, dst, 1, MSG_WAITALL ) ) {
			case SOCKET_ERROR:
				{
					char log[ 256 ];
					sprintf_s( log, 256, "CUdSocket::ReadByte, ошибка сокета, %lu\r\n", GET_SOCKET_ERROR );
					WriteUdLibLog( log );
					return SOCKET_ERROR;
				}
				break;

			case 0:
				WriteUdLibLog( "CUdSocket::ReadByte, закрыто соединене\n" );
				return 0;
				break;

			case 1:
				*dst = buf[ 0 ];
				return 1;
				break;

			default:
				WriteUdLibLog( "CUdSocket::ReadByte, считано больше чем 1 байт, " + toString( read ) + "\n" );
				return 2;
				break;
		}
		return 1;
	} // udInt ReadByte

	udInt CUdSocket::Recv( char* buf, int len, int flags ) {
		return recv( m_hSocket, buf, len, flags );
	} // udInt Recv

	udInt CUdSocket::IoCtl( long cmd, u_long* argp ) {
		return ioctlsocket( m_hSocket, cmd, argp );
	} // udInt IoCtl

	udInt CUdSocket::GetOpt( int level, int optname, char* optval, int* optlen ) {
		return getsockopt( m_hSocket, level, optname, optval, optlen );
	} // udInt GetOpt

	udInt CUdSocket::SetOpt( int level, int optname, const char* optval, int optlen ) {
		return setsockopt( m_hSocket, level, optname, optval, optlen );
	} // udInt SetOpt

} // namespace UdSDK
