
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void UdBaseEscapeString( char* out, const char* in, size_t len ) {
		g_pApp->GetDatabase( )->EscapeString( out, in, len );
	} // void UdBaseEscapeString

	CApplication::CApplication( ) :
		m_pInit( NULL ),
		m_pConfig( NULL ),
		m_wndLogin( NULL ),
		m_pLog( NULL ),
		m_wndWork( NULL ),
		m_pUser( NULL ),
		m_pHandlerManager( NULL ),
		m_pMySQL( NULL ),
		m_pHandler( NULL )
	{
	} // CApplication

	CApplication::CApplication( const CApplication& objCopy ) {
	} // CApplication

	CApplication::CApplication( udPAppInit pInit ) :
		m_pInit( pInit ),
		m_pConfig( NULL ),
		m_wndLogin( NULL ),
		m_pLog( NULL ),
		m_wndWork( NULL ),
		m_pUser( NULL ),
		m_pHandlerManager( NULL ),
		m_pMySQL( NULL ),
		m_pHandler( NULL )
	{
	} // CApplication

	CApplication::~CApplication( ) {
		udDELETE( m_pHandlerManager );
		udDELETE( m_pUser );
		udDELETE( m_wndWork );
		udDELETE( m_wndLogin );
		udDELETE( m_pInit );
		udDELETE( m_pHandler );
		udDELETE( m_pMySQL );
		udDELETE( m_pLog );
		udDELETE( m_pConfig );
	} // ~CApplication

	CApplication& CApplication::operator =( const CApplication& objCopy ) {
		return *this;
	} // CApplication& operator =

	udInt CApplication::Start( ) {
		if ( InitConfig( ) || InitLog( ) || InitWndLogin( ) ) {
			return 1;
		}

		Gtk::Main::run( *m_wndLogin );

		if ( m_pUser ) {
			InitMySQL( );
			m_pHandler = new udHandler( m_pMySQL );
			CheckDrop( );

			if ( InitWndWork( ) || InitHandlerManager( ) ) {
				return 1;
			}

			if ( m_pInit ) {
				m_pInit->InitToolbar( );
				m_pInit->InitMenu( );
				m_pInit->InitModel( );
			}

			udPStateHandler pHandler = m_pHandlerManager->GetDefaultHandler( );
			if ( pHandler ) {
				pHandler->Load( );
				pHandler->Show( );
			}

			Gtk::Main::run( *m_wndWork );
		}

		return 0;
	} // udInt Start

	udInt CApplication::InitConfig( ) {
		m_pConfig = new udAppConfig;

		// считываем конфиг
		udPStdFile config = NULL;
		if ( fopen_s( &config, "client_config.txt", "rb" ) == 0 ) {
			char line[ 1024 ];
			int i = 0, len = 0;
			string name = "", value = "";
			memset( line, 0, 1024 );
			while( !feof( config ) ) {
				if ( fgets( line, 1024, config ) != NULL ) {
					len = strlen( line );
					for( i = 0; i < len; ++i ) {
						if ( line[ i ] == '=' ) {
							break;
						}
						name += line[ i ];
					}
					++i;
					for( ; i < len; ++i ) {
						if ( line[ i ] == '\r' || line[ i ] == '\n' ) {
							break;
						}
						value += line[ i ];
					}
					if ( !name.empty( ) ) {
						m_pConfig->get( name )->ini( value );
					}
					name = "";
					value = "";
				}
			}

			fclose( config );
		}

		udInt	iScreenWidth	= GetSystemMetrics( SM_CXSCREEN ),
				iScreenHeight	= GetSystemMetrics( SM_CYSCREEN );

		m_pConfig->get( "lang"				)->ini( string( "ru" ) );
		m_pConfig->get( "screen_width"		)->ini( iScreenWidth );
		m_pConfig->get( "screen_height"		)->ini( iScreenHeight );
		m_pConfig->get( "wndlogin_x"		)->ini( ( iScreenWidth / 2 ) - 120 );
		m_pConfig->get( "wndlogin_y"		)->ini( ( iScreenHeight / 2 ) - 120 );
		m_pConfig->get( "wndsplash_x"		)->ini( ( iScreenWidth / 2 ) - 300 );
		m_pConfig->get( "wndsplash_y"		)->ini( ( iScreenHeight / 2 ) - 300 );

		udWChar dir[ 1024 ];
		memset( dir, 0, sizeof( dir ) );
		GetCurrentDirectory( 1024, dir );
		wstring _dir( dir ), tmp( L"" );

		m_pConfig->get( "workdir" )->ini( _dir );
		tmp = _dir + L"\\tmp";
		CreateDirectory( tmp.data( ), NULL );
		m_pConfig->get( "temporary_dir" )->ini( tmp  );
		tmp = _dir + L"\\files";
		CreateDirectory( tmp.data( ), NULL );
		m_pConfig->get( "files_dir" )->ini( tmp );

		return 0;
	} // udInt InitConfig

	udInt CApplication::InitLog( ) {
		m_pLog = new udLog;
		if ( m_pLog->Open( m_pConfig->get( "log_path" )->toString( ), m_pConfig->get( "log_mode" )->toString( ).data( ) ) == udLog::UDERROR_OK ) {
			return 0;
		}
		return 1;
	} // udInt InitLog

	udInt CApplication::InitWndLogin( ) {
		m_wndLogin = new udWndLogin;
		m_wndLogin->GetButtonOk( )->signal_clicked( ).connect( sigc::mem_fun( *this, &CApplication::OnLoginOk ) );
		m_wndLogin->GetButtonCancel( )->signal_clicked( ).connect( sigc::mem_fun( *this, &CApplication::OnLoginCancel ) );
		return 0;
	} // udInt InitWndLogin

	udInt CApplication::InitMySQL( ) {
		wstring szPath = L"mysql\\bin\\mysqld.exe";
		if ( m_pConfig->isset( "mysql_path" ) ) {
			szPath = m_pConfig->get( "mysql_path" )->toWString( );
		}
		//
		m_pMySQL = new udMySql;
		string	host		= m_pConfig->get( "database_host"		)->toString( ),
				user		= m_pConfig->get( "database_user"		)->toString( ),
				password	= m_pConfig->get( "database_password"	)->toString( ),
				dbname		= m_pConfig->get( "database_dbname"		)->toString( );
		udUInt	port		= m_pConfig->get( "database_port"		)->toUInt( );
		// коннектимся к нему
		m_pMySQL->SetAccData( host, user, password, dbname );
		m_pMySQL->SetPortNumber( port );
		m_pMySQL->SetCharSet( "utf8" );
		if ( m_pMySQL->Connect( true ) == udMySql::UDSDK_MYSQL_ERROR_OK ) {
			return 1;
		} else {
			m_pMySQL->Close( );
			CreateProcess(
				szPath.data( ), NULL, NULL, NULL, FALSE,
				DETACHED_PROCESS,
				NULL, NULL, &m_objSI, &m_objPI
			);
			CloseHandle( m_objPI.hProcess );
			CloseHandle( m_objPI.hThread );
			m_objPI.hProcess = NULL;
			m_objPI.hThread = NULL;
			Sleep( 4000 );
			if ( m_pMySQL->Connect( true ) == udMySql::UDSDK_MYSQL_ERROR_OK ) {
				return 1;
			}
		}
		return 0;
	} // udInt InitMySQL

	void CApplication::OnLoginOk( ) {
		Glib::ustring	szLogin		= m_wndLogin->GetInputLogin( )->GetValue( ),
						szPassword	= m_wndLogin->GetInputPassword( )->GetValue( );
		if ( szLogin.empty( ) ) {
			mb::err( *m_wndLogin, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Заполните поле \"Логин\"" ) );
			m_wndLogin->set_focus( *( m_wndLogin->GetInputLogin( )->GetEntry( ) ) );
		} else if ( szPassword.empty( ) ) {
			mb::err( *m_wndLogin, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Заполните поле \"Пароль\"" ) );
			m_wndLogin->set_focus( *( m_wndLogin->GetInputPassword( )->GetEntry( ) ) );
		} else {
			bool show_error = false;
			udPSocket sock = BeginRequest( false );
			if ( sock ) {
				m_pLog->Write( "подсоединились к серверу...\n" );
				// производим операции
				udBinaryData data;
				data.add( string( "" ) );
				data.add( ConvertString( szLogin.data( ) ) );
				data.add( ConvertString( szPassword.data( ) ) );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					// ждем ответа - ID сессии, данные клиента
					udType id( string( "" ) );
					id.read( sock );
					m_pLog->Write( "получили ID сессии '" + id.toString( ) + "'\n" );
					m_pUser = new udAppUser;
					m_pUser->read( sock );
					m_pUser->get( "session_id" )->ini( id.toString( ) );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );
					m_wndLogin->hide( );
				} else {
					m_pLog->Write( "не удалось залогиниться\n" );
					show_error = true;
				}
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				m_pLog->Write( "не удалось подсоединиться к серверу...\n" );
				mb::err( *m_wndLogin, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Отсутствует соединение с сервером" ) );
			}
			if ( show_error ) {
				mb::err( *m_wndLogin, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Данный пользователь не существует или был введен неверный пароль." ) );
				m_wndLogin->set_focus( *( m_wndLogin->GetInputLogin( )->GetEntry( ) ) );
			}
		}
	} // void OnLoginOk

	void CApplication::OnLoginCancel( ) {
		m_wndLogin->hide( );
	} // void OnLoginCancel

	udInt CApplication::InitWndWork( ) {
		m_wndWork = new udWndWork;
		return 0;
	} // udInt InitWndWork

	udInt CApplication::InitHandlerManager( ) {
		m_pHandlerManager = new udHandlerManager;
		return 0;
	} // udInt InitHandlerManager

	udPSocket CApplication::BeginRequest( bool login ) {
		string			host	= m_pConfig->get( "server_host" )->toString( );
		udInt			port	= m_pConfig->get( "server_port" )->toInt( ),
						clport	= m_pConfig->get( "client_port" )->toInt( );
		udPSocket		sock	= new udSocket( udSocketInfo( host.data( ), port ) );

		if ( ( sock->Create( ) == 1 ) && ( sock->Connect( ) == 1 ) ) {
			if ( login ) {
				if ( Login( sock ) ) {
					return sock;
				}
			} else {
				return sock;
			}
		}
		delete sock;

		return NULL;
	} // udPSocket BeginRequest

	udInt CApplication::Login( udPSocket sock ) {
		udBinaryData data;
		data.add( m_pUser->get( "session_id" )->toString( ) );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		if ( code == 80 ) {
			return 1;
		}
		return 0;
	} // udInt Login

	wstring CApplication::GetUniqueTmpFolder( ) {
		wstring ret = m_pConfig->get( "temporary_dir" )->toWString( );
		if ( m_mapTmpFolder.empty( ) ) {
			ret += L"\\0";
			m_mapTmpFolder[ 0 ] = ret;
		} else {
			udDWord dwIndex = ( --m_mapTmpFolder.end( ) )->first + 1;
			ret += L"\\" + toWString( dwIndex );
			m_mapTmpFolder[ dwIndex ] = ret;
		}
		CreateDirectory( ret.data( ), NULL );
		return ret;
	} // wstring GetUniqueTmpFolder

	void CApplication::CheckDrop( ) {
		FILE *pFileDrop = NULL;
		if ( fopen_s( &pFileDrop, "drop.txt", "rb" ) == 0 ) { // файл присутствует
			size_t len = 0;
			char tmp[ 1024 ];
			string query = "";
			while( !feof( pFileDrop ) ) {
				if ( fgets( tmp, 1024, pFileDrop ) != NULL ) {
					len = strlen( tmp );
					if ( tmp[ len - 1 ] == '\n' ) {
						tmp[ len - 1 ] = 0;
					}
					if ( tmp[ len - 2 ] == '\r' ) {
						tmp[ len - 2 ] = 0;
					}
					query = "DROP TABLE IF EXISTS `" + string( tmp ) + "`";
					m_pMySQL->Query( query );
				}
			}
			fclose( pFileDrop );
			remove( "drop.txt" );
		}
	} // void CheckDrop

} // namespace UdSDK
