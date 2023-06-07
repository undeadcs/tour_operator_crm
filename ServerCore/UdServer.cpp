
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CUdServer::CUdServer( ) {
		m_pSocket = new udSocket( udSocketInfo(
			g_pApp->GetConfig( )->get( "ip" )->toString( ).data( ),
			g_pApp->GetConfig( )->get( "port" )->toInt( )
		) );
	}

	CUdServer::CUdServer( const CUdServer& objCopy ) {
	}

	CUdServer::~CUdServer( ) {
		udDELETE( m_pSocket );
	}

	CUdServer& CUdServer::operator =( const CUdServer& objCopy ) {
		return *this;
	}

	bool CUdServer::Init( ) {
		if ( m_pSocket->Create( ) == 0 ) {
			g_pApp->GetLog( )->Write( "Не удалось создать сокет\n" );
			return false;
		}
		if ( m_pSocket->Bind( ) == 0 ) {
			g_pApp->GetLog( )->Write( "Не удалось привязать сокет\n" );
			return false;
		}
		if ( m_pSocket->Listen( ) == 0 ) {
			g_pApp->GetLog( )->Write( "Не удалось прослушать сокет\n" );
			return false;
		}
		// сбросим буфера, чтобы предыдущие левые данные, не присылались снова
		m_pSocket->Shutdown( );
		delete m_pSocket;

		m_pSocket = new udSocket(
			udSocketInfo( g_pApp->GetConfig( )->get( "ip" )->toString( ).data( ),
			g_pApp->GetConfig( )->get( "port" )->toInt( ) )
		);
		if ( m_pSocket->Create( ) == 0 ) {
			g_pApp->GetLog( )->Write( "Не удалось создать сокет\n" );
			return false;
		}
		if ( m_pSocket->Bind( ) == 0 ) {
			g_pApp->GetLog( )->Write( "Не удалось привязать сокет\n" );
			return false;
		}
		if ( m_pSocket->Listen( ) == 0 ) {
			g_pApp->GetLog( )->Write( "Не удалось прослушать сокет\n" );
			return false;
		}

		return true;
	} // bool Init

	udDWord CUdServer::Start( ) {
		//g_pApp->GetWndServer( )->UpdateText( L"CUdServer::Start" );

		udChar	tmp[ 1024 ];
		udInt	res				= 0;
		SOCKET	hSocketHandle	= INVALID_SOCKET;
		while( !this->isset( "exit" ) ) {
			res = m_pSocket->Select( udSocket::UDSOCKET_SELECT_READ, CUdServer::select_delay );
			switch( res ) {
				case SOCKET_ERROR:
					sprintf_s( tmp, 1024, "UdServer::Start, ошибка сокета, код: %lu\n", GET_SOCKET_ERROR );
					g_pApp->GetLog( )->Write( tmp );
					this->get( "exit" );
					break;

				case 0:
					break;

				default: {
					udPSocket pSocket = m_pSocket->Accept( );
					if ( pSocket ) {
						this->HandleConnection( pSocket );
						pSocket->Close( );
						delete pSocket;
					}
				} break;
			}
		}
		return 0;
	} // udDWord Start

	void CUdServer::HandleConnection( udPSocket pSocket ) {
		//g_pApp->GetWndServer( )->UpdateText( L"CUdServer::HandleConnection" );

		// здесь логинится клиент
		if ( g_pApp->GetInitObj( )->Login( pSocket ) ) {
			string szHandlerName = "";
			udType name( string( "" ) );
			name.read( pSocket );
			szHandlerName = name.toString( );

			//g_pApp->GetWndServer( )->UpdateText( L"Имя обработчика: " + toWString( szHandlerName ) );
			g_pApp->GetLog( )->Write( "handler_name: \"" + szHandlerName + "\"\n" );

			if ( !szHandlerName.empty( ) ) {
				if ( szHandlerName == "#" ) {
				} else {
					udPModelWork model = g_pApp->GetModel( );
					udPStateHandler shandler = model->GetHandler( szHandlerName );
					if ( shandler != NULL ) {
						shandler->ProcRequest( pSocket );
					}
				}
			}
		}
		
		pSocket->Shutdown( );
	} // void HandleConnection

} // namespace UdSDK
