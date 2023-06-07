
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void UdBaseEscapeString( char* out, const char* in, size_t len ) {
		g_pApp->GetDatabase( )->EscapeString( out, in, len );
	} // void UdBaseEscapeString

	CApplication::CApplication( HINSTANCE hInstance, udPAppInit pInit ) {
		m_hInstance		= hInstance;
		m_pMySQL		= NULL;
		m_pLog			= NULL;
		m_pHandler		= NULL;
		m_pUser			= NULL;
		m_pConfig		= NULL;
		m_pInit			= pInit;
		m_pModel		= new udModelWork;
		m_wndServer		= NULL;

		ZeroMemory( &m_objSI,	sizeof( STARTUPINFO ) );
		ZeroMemory( &m_objPI,	sizeof( PROCESS_INFORMATION ) );

		m_objSI.cb = sizeof( STARTUPINFO );

		m_pServer	= NULL;
	}

	CApplication::CApplication( const CApplication& objApp ) {
	}

	CApplication::~CApplication( ) {
		if ( m_pModel ) {
			m_pLog->Write( "Удаляем модель\n" );
			delete m_pModel;
			m_pModel = NULL;
		}
		if ( m_pUser ) {
			m_pLog->Write( "Удаляем объект учетки\n" );
			delete m_pUser;
			m_pUser = NULL;
		}
		if ( !m_arrConnectionHandlers.empty( ) ) {
			m_pServer->get( "exit" );
			DWORD dwRes = 0;
			udChar tmp[ 256 ];
			for( map< string, HANDLE >::iterator i = m_arrConnectionHandlers.begin( ); i != m_arrConnectionHandlers.end( ); ++i ) {
				dwRes = WaitForSingleObject( i->second, INFINITE );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
				switch( dwRes ) {
					case WAIT_ABANDONED:
						m_pLog->Write( i->first + " WAIT_ABANDONED\n" );
						break;

					case WAIT_OBJECT_0:
						m_pLog->Write( i->first + " WAIT_OBJECT_0\n" );
						break;

					case WAIT_TIMEOUT:
						m_pLog->Write( i->first + " WAIT_TIMEOUT\n" );
						break;

					case WAIT_FAILED:
						m_pLog->Write( i->first + " WAIT_FAILED\n" );
						sprintf_s( tmp, 256, "error code: %lu\n", GetLastError( ) );
						m_pLog->Write( tmp );
						break;
				}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
				TerminateThread( i->second, 0 );
				CloseHandle( i->second );
			}
		}
		WSACleanup( );
		if ( m_pServer ) {
			delete m_pServer;
			m_pServer = NULL;
		}
		if ( m_pHandler ) {
			m_pLog->Write( "Удаляем обработчик объектов\n" );
			delete m_pHandler;
			m_pHandler = NULL;
		}
		if ( m_pMySQL ) {
			m_pLog->Write( "Удаляем объект работы с СУБД\n" );
			delete m_pMySQL;
			m_pMySQL = NULL;
		}
		if ( m_objPI.hProcess ) {
			m_pLog->Write( "Завершаем процесс СУБД\n" );
			TerminateProcess( m_objPI.hProcess, 0 );
			CloseHandle( m_objPI.hProcess );
			CloseHandle( m_objPI.hThread );
		}
		if ( m_wndServer ) {
			delete m_wndServer;
			m_wndServer = NULL;
		}
		if ( m_pInit ) {
			m_pLog->Write( "Удаляем объект инициализации\n" );
			delete m_pInit;
			m_pInit = NULL;
		}
		if ( m_pLog ) {
			m_pLog->Write( "Удаляем объект лога\n" );
			m_pLog->Close( );
			delete m_pLog;
			m_pLog = NULL;
		}
		if ( m_pConfig ) {
			delete m_pConfig;
			m_pConfig = NULL;
		}

		m_hInstance = NULL;
	}

	udInt CApplication::InitConfig( ) {
		udInt	iScreenWidth	= GetSystemMetrics( SM_CXSCREEN ),
				iScreenHeight	= GetSystemMetrics( SM_CYSCREEN );
		m_pConfig = new udAppConfig;

		udPStdFile config = NULL;
		if ( fopen_s( &config, "server_config.txt", "rb" ) == 0 ) {
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
		
		/*m_pConfig->get( "database_host"		)->ini( string( "localhost" ) );
		m_pConfig->get( "database_user"		)->ini( string( "root" ) );
		m_pConfig->get( "database_password"	)->ini( string( "" ) );
		m_pConfig->get( "database_dbname"	)->ini( string( "test_server_1" ) );
		m_pConfig->get( "log_path"			)->ini( string( "server_log.txt" ) );
		m_pConfig->get( "log_mode"			)->ini( string( "wb" ) );
		m_pConfig->get( "window_title"		)->ini( wstring( L"ИСУ БМТ Приморье" ) );*/

		m_pConfig->get( "lang"				)->ini( string( "ru" ) );
		m_pConfig->get( "screen_width"		)->ini( iScreenWidth );
		m_pConfig->get( "screen_height"		)->ini( iScreenHeight );
		/*m_pConfig->get( "wndlogin_x"		)->ini( ( iScreenWidth / 2 ) - 120 );
		m_pConfig->get( "wndlogin_y"		)->ini( ( iScreenHeight / 2 ) - 120 );
		m_pConfig->get( "wndsplash_x"		)->ini( ( iScreenWidth / 2 ) - 300 );
		m_pConfig->get( "wndsplash_y"		)->ini( ( iScreenHeight / 2 ) - 300 );
		m_pConfig->get( "wndwork_min_width"		)->ini( ( udLong ) 800 );
		m_pConfig->get( "wndwork_min_height"	)->ini( ( udLong ) 600 );
		m_pConfig->get( "wndwork_max_width"		)->ini( ( udLong ) 1280 );
		m_pConfig->get( "wndwork_max_height"	)->ini( ( udLong ) 1024 );*/
		//m_pConfig->get( "mysql_path"		)->ini( wstring( L"mysql\\bin\\mysqld.exe" ) );

		udWChar dir[ 1024 ];
		memset( dir, 0, sizeof( dir ) );
		GetCurrentDirectory( 1024, dir );
		wstring _dir( dir ), tmp( L"" );
		
		m_pConfig->get( "workdir" )->ini( _dir );
		tmp = _dir + L"\\tmp_server";
		CreateDirectory( tmp.data( ), NULL );
		m_pConfig->get( "temporary_dir" )->ini( tmp  );
		tmp = _dir + L"\\files_server";
		CreateDirectory( tmp.data( ), NULL );
		m_pConfig->get( "files_dir" )->ini( tmp );

		return 1;
	} // udInt InitConfig

	udInt CApplication::InitLog( ) {
		m_pLog = new udLog;
		if ( m_pLog->Open( m_pConfig->get( "log_path" )->toString( ), m_pConfig->get( "log_mode" )->toString( ).data( ) ) == udLog::UDERROR_OK ) {
			g_pLog = m_pLog;
			return 1;
		}
		return 0;
	}

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
			//m_pMySQL->SetCharacterSet( "utf8" );
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
				//m_pMySQL->SetCharacterSet( "utf8" );
				return 1;
			}
		}
		/*udMySql::cmysqlError iErr = m_pMySQL->Create( host, user, password, dbname, port );
		if ( iErr == udMySql::UDSDK_MYSQL_ERROR_OK ) {
			m_pMySQL->SetCharacterSet( "utf8" );
			return 1;
		} else {
			CreateProcess(
				szPath.data( ), NULL, NULL, NULL, FALSE,
				DETACHED_PROCESS,
				NULL, NULL, &m_objSI, &m_objPI
			);
			CloseHandle( m_objPI.hProcess );
			CloseHandle( m_objPI.hThread );
			m_objPI.hProcess = NULL;
			m_objPI.hThread = NULL;
			Sleep( 3000 );
			if ( m_pMySQL->Create( host, user, password, dbname, port ) == udMySql::UDSDK_MYSQL_ERROR_OK ) {
				m_pMySQL->SetCharacterSet( "utf8" );
				return 1;
			}
		}*/
		return 0;
	} // udInt InitMySQL

	udInt CApplication::Start( ) {
		if ( !m_pInit ) {
			mb::err( L"Отсутствует объект инициализации" );
			return 1;
		}
		m_pInit->SetApp( this );
		if ( !this->InitConfig( ) ) {
			mb::err( L"Отсутствует файл настроек" );
			return 1;
		}
		if ( !this->InitLog( ) ) {
			mb::err( L"Не удалось открыть файл журнала" );
			return 1;
		}
		if ( !this->InitMySQL( ) ) {
			mb::err( L"Отсутствует сервер баз данных" );
			return 1;
		}
		this->CheckDrop( );
		if ( !this->InitWindowServer( ) ) {
			mb::err( L"Не удалось инициализировать окно" );
			return 1;
		}
		if ( !this->InitChildThread( ) ) {
			mb::err( L"Не удалось инициализировать серверные модули" );
			return 1;
		}

		m_pHandler = new udHandler( m_pMySQL );
		m_pModel = new udModelWork;
		m_pInit->InitModel( m_pModel );

		Gtk::Main::run( *m_wndServer );

		return 0;
	} // udInt Start

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

	udInt CApplication::InitWindowServer( ) {
		m_wndServer = new udWndServer;
		m_wndServer->set_size_request( 800, 600 );
		return 1;
	} // udInt InitWindowServer

	static DWORD WINAPI udServerThreadProc( LPVOID pParam ) {
		udPServer server = ( udPServer ) pParam;
		if ( !server->Init( ) ) {
			return 1;
		}
		return server->Start( );
	}

	udInt CApplication::InitChildThread( ) {
		if ( WSAStartup( WINSOCK_VERSION, &m_objWinSockData ) != 0 ) {
			return 0;
		}
		m_pServer = new udServer;
		DWORD id = 0;
		m_arrConnectionHandlers[ "server1" ] =
			CreateThread( NULL, 0, udServerThreadProc, ( LPVOID ) m_pServer, NULL, &id );
		m_arrCHId[ "server1" ] = id;
		return 1;
	} // udInt InitChildThread

} // namespace UdSDK
