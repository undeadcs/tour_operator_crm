
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void UdBaseEscapeString( char* out, const char* in, size_t len ) {
		g_pApp->GetDatabase( )->EscapeString( out, in, len );
	} // void UdBaseEscapeString

	CApplication::CApplication( HINSTANCE hInstance, udPAppInit pInit ) : udHMsgLoop( ) {
		m_pInstance		= hInstance;
		m_pMySQL		= NULL;
		m_pLog			= NULL;
		m_pHandler		= NULL;
		m_pUser			= NULL;
		m_pConfig		= NULL;
		m_pLogin		= NULL;
		m_pSplash		= NULL;
		m_pWork			= NULL;
		m_pInit			= pInit;
		m_pModel		= NULL;
		m_iState		= CApplication::APP_STATE_CONSTRUCTED;
		m_pSync			= NULL;

		ZeroMemory( &m_objMsg,	sizeof( MSG ) );
		ZeroMemory( &m_objSI,	sizeof( STARTUPINFO ) );
		ZeroMemory( &m_objPI,	sizeof( PROCESS_INFORMATION ) );

		m_objSI.cb = sizeof( STARTUPINFO );
	}

	CApplication::CApplication( const CApplication& objApp ) {
	}

	CApplication::~CApplication( ) {
		if ( !m_mapTmpFolder.empty( ) ) {
			for( map_folders::iterator i = m_mapTmpFolder.begin( ); i != m_mapTmpFolder.end( ); ++i ) {
				DirectoryClear( i->second );
				RemoveDirectory( i->second.data( ) );
			}
		}
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
		WSACleanup( );
		if ( m_pSync ) {
			delete m_pSync;
			m_pSync = NULL;
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
		if ( !m_arrFont.empty( ) ) {
			m_pLog->Write( "Очищаем массив шрифтов\n" );
			for( udAscFont::iterator i = m_arrFont.begin( ); i != m_arrFont.end( ); ++i ) {
				delete i->second;
			}
			m_arrFont.clear( );
		}
		if ( !m_arrImage.empty( ) ) {
			m_pLog->Write( "Очищаем массив изображений\n" );
			for( udAscImage::iterator i = m_arrImage.begin( ); i != m_arrImage.end( ); ++i ) {
				delete i->second;
			}
			m_arrImage.clear( );
		}
		if ( !m_arrBitmap.empty( ) ) {
			m_pLog->Write( "Очищаем массив битмапов\n" );
			for( udAscBitmap::iterator i = m_arrBitmap.begin( ); i != m_arrBitmap.end( ); ++i ) {
				delete i->second;
			}
			m_arrBitmap.clear( );
		}
		if ( m_pLogin ) {
			m_pLog->Write( "Удаляем объект окна входа\n" );
			delete m_pLogin;
			m_pLogin = NULL;
		}
		if ( m_pSplash ) {
			m_pLog->Write( "Удаляем объект окна загрузки\n" );
			delete m_pSplash;
			m_pSplash = NULL;
		}
		if ( m_pWork ) {
			m_pLog->Write( "Удаляем объект рабочего окна\n" );
			delete m_pWork;
			m_pWork = NULL;
		}
		m_pLog->Write( "Завершаем GDI+\n" );
		GdiplusShutdown( m_pToken );
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
		if ( !m_arrCursor.empty( ) ) {
			for( map< string, HCURSOR >::iterator i = m_arrCursor.begin( ); i != m_arrCursor.end( ); ++i ) {
				DeleteObject( ( HGDIOBJ ) i->second );
			}
			m_arrCursor.clear( );
		}

		m_pInstance = NULL;
	}

	udInt CApplication::InitConfig( ) {
		m_iState = CApplication::APP_STATE_INITCONFIG;
		udInt	iScreenWidth	= GetSystemMetrics( SM_CXSCREEN ),
				iScreenHeight	= GetSystemMetrics( SM_CYSCREEN );
		m_pConfig = new udAppConfig;

		udPStdFile config = NULL;
		if ( fopen_s( &config, "client_config.txt", "rb" ) == 0 ) {
			//udPLog log = new udLog;
			//log->Open( "out.txt", "wb" );

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
					//log->Write( name += ", " + value + "\n" );
					name = "";
					value = "";
				}
			}

			fclose( config );
			//log->Close( );
			//delete log;
		}

		/*m_pConfig->get( "database_host"		)->ini( string( "localhost" ) );
		m_pConfig->get( "database_user"		)->ini( string( "root" ) );
		m_pConfig->get( "database_password"	)->ini( string( "" ) );
		m_pConfig->get( "database_dbname"	)->ini( string( "test_client_1" ) );
		m_pConfig->get( "log_path"			)->ini( string( "log.txt" ) );
		m_pConfig->get( "log_mode"			)->ini( string( "wb" ) );
		m_pConfig->get( "server_host"		)->ini( string( "127.0.0.1" ) );
		m_pConfig->get( "window_title"		)->ini( wstring( L"ИСУ БМТ Приморье" ) );
		m_pConfig->get( "wndwork_min_width"		)->ini( ( udLong ) 800 );
		m_pConfig->get( "wndwork_min_height"	)->ini( ( udLong ) 600 );
		m_pConfig->get( "wndwork_max_width"		)->ini( ( udLong ) 1280 );
		m_pConfig->get( "wndwork_max_height"	)->ini( ( udLong ) 1024 );
		m_pConfig->get( "mysql_path"		)->ini( wstring( L"mysql\\bin\\mysqld.exe" ) );*/

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

		return 1;
	} // udInt InitConfig

	udInt CApplication::InitLog( ) {
		m_iState = CApplication::APP_STATE_INITLOG;
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
			Sleep( 3000 );
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

	udInt CApplication::InitGraphic( ) {
		INITCOMMONCONTROLSEX objControls = {
			sizeof( INITCOMMONCONTROLSEX ),
			// смотрим бинарные значения констант, некоторые перекрывают другие
			/*ICC_ANIMATE_CLASS |	// входит в WIN95
			ICC_BAR_CLASSES |*/		// входит в WIN95
			ICC_COOL_CLASSES |
			ICC_DATE_CLASSES |
			//ICC_HOTKEY_CLASS |	// входит в WIN95
			ICC_INTERNET_CLASSES |
			//ICC_LINK_CLASS |	// под Vista, XP не пашет
			//ICC_LISTVIEW_CLASSES |	// входит в WIN95
			ICC_NATIVEFNTCTL_CLASS |
			ICC_PAGESCROLLER_CLASS |
			//ICC_PROGRESS_CLASS |	// входит в WIN95
			//ICC_STANDARD_CLASSES |	// под Vista, XP не пашет
			/*ICC_TAB_CLASSES |		// входит в WIN95
			ICC_TREEVIEW_CLASSES |
			ICC_UPDOWN_CLASS |*/	// входит в WIN95
			ICC_USEREX_CLASSES |
			ICC_WIN95_CLASSES |
			0
		};
		if ( InitCommonControlsEx( &objControls ) == FALSE ) {
			return 0;
		}

		if ( GdiplusStartup( &m_pToken, &m_objGdiInp, NULL ) ) {
			return 0;
		}

		m_arrBitmap[ "icon" ] = this->GetBitmap( "sprite.png" )->Clone( Rect( 0, 18, 16, 16 ), PixelFormatDontCare );

		return 1;
	} // udInt InitGraphic

	udInt CApplication::InitWindowLogin( ) {
		m_pLogin = new udWndLogin( m_pInstance );
		m_pLogin->get( "title"	)->ini( m_pConfig->get( "window_title" )->toWString( ) );
		m_pLogin->get( "x"		)->ini( m_pConfig->get( "wndlogin_x" )->toInt( ) );
		m_pLogin->get( "y"		)->ini( m_pConfig->get( "wndlogin_y" )->toInt( ) );
		m_pLogin->get( "width"	)->ini( ( udInt ) 276 );
		m_pLogin->get( "height"	)->ini( ( udInt ) 191 );
		m_pLogin->get( "style"	)->ini( ( udDWord ) ( WS_POPUP | WS_SYSMENU ) );

		bool ret = m_pLogin->Create( );
		if ( ret ) {
			CenterOneWindowAboveParent( m_pLogin->GetHWnd( ) );
		}
		return ( udInt ) ret;
	} // udInt InitWindow

	udInt CApplication::InitWindowSplash( ) {
		m_pSplash = new udWndSplash( m_pInstance );
		m_pSplash->get( "title"		)->ini( m_pConfig->get( "window_title" )->toWString( ) );
		m_pSplash->get( "x"			)->ini( m_pConfig->get( "wndsplash_x" )->toInt( ) );
		m_pSplash->get( "y"			)->ini( m_pConfig->get( "wndsplash_y" )->toInt( ) );
		m_pSplash->get( "width"		)->ini( ( udInt ) 499 );
		m_pSplash->get( "height"	)->ini( ( udInt ) 398 );
		m_pSplash->get( "style"		)->ini( ( udDWord ) ( WS_POPUP | WS_SYSMENU ) );

		HICON hIconApp = NULL;
		m_arrBitmap[ "icon" ]->GetHICON( &hIconApp );
		m_pSplash->SetIcon( hIconApp );

		bool ret = m_pSplash->Create( );
		if ( ret ) {
			CenterOneWindowAboveParent( m_pSplash->GetHWnd( ) );
		}
		return ( udInt ) ret;
	} // udInt InitWindowSplash

	udInt CApplication::InitWindowWork( ) {
		m_pSplash->SetState( L"Инициализация рабочего окна", 80.0f );

		udDWord style	= WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
				exstyle	= WS_EX_APPWINDOW;

		m_pWork = new udWndWork( m_pInstance );
		m_pWork->get( "title"		)->ini( m_pConfig->get( "window_title" )->toWString( ) );
		m_pWork->get( "x"			)->ini( ( udInt ) 0 );
		m_pWork->get( "y"			)->ini( ( udInt ) 0 );
		m_pWork->get( "width"		)->ini( ( udInt ) 800 );
		m_pWork->get( "height"		)->ini( ( udInt ) 600 );
		m_pWork->get( "style"		)->ini( style );
		m_pWork->get( "exstyle"		)->ini( exstyle );

		if ( m_pConfig->isset( "wndwork_min_width" ) ) {
			m_pWork->get( "min-width"	)->ini( m_pConfig->get( "wndwork_min_width" )->toLong( ) );
		}
		if ( m_pConfig->isset( "wndwork_min_height" ) ) {
			m_pWork->get( "min-height"	)->ini( m_pConfig->get( "wndwork_min_height" )->toLong( ) );
		}
		if ( m_pConfig->isset( "wndwork_max_width" ) ) {
			m_pWork->get( "max-width" )->ini( m_pConfig->get( "wndwork_max_width" )->toLong( ) );
		}
		if ( m_pConfig->isset( "wndwork_max_height" ) ) {
			m_pWork->get( "max-height" )->ini( m_pConfig->get( "wndwork_max_height" )->toLong( ) );
		}

		return ( udInt ) m_pWork->Create( );
	} // udInt InitWindowWork

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
		if ( !this->InitGraphic( ) ) {
			mb::err( L"Не удалось инициализировать графический модуль" );
			return 1;
		}
		if ( !this->InitWindowLogin( ) ) {
			mb::err( L"Не удалось создать окно формы входа" );
			return 1;
		}

		m_arrCursor[ "arrow"	] = LoadCursor( NULL, IDC_ARROW	);
		m_arrCursor[ "hand"		] = LoadCursor( NULL, IDC_HAND	);

		HICON hIconApp = NULL;
		m_arrBitmap[ "icon" ]->GetHICON( &hIconApp );
		SendMessage(
			m_pLogin->GetHWnd( ), WM_SETICON, ( WPARAM ) ICON_SMALL,
			( LPARAM ) hIconApp
		);

		m_pHandler	= new udHandler( m_pMySQL );
		m_pSync = new udDatabaseSync;
		m_iState = CApplication::APP_STATE_LOGIN;
		m_pLogin->Show( );
		m_pLogin->Update( );
		MessageLoop( this );
		//this->MainLoop( );

		if ( m_pUser ) { // значит клиент залогинился, показываем сплэш
			if ( !this->InitWindowSplash( ) ) {
				mb::err( L"Не удалось создать окно Splash" );
				return 1;
			}

			/*SendMessage(
				m_pSplash->GetHWnd( ), WM_SETICON, ( WPARAM ) ICON_SMALL,
				( LPARAM ) hIconApp
			);*/
			m_iState = CApplication::APP_STATE_SPLASH;
			m_pSplash->Show( );
			m_pSplash->Update( );
			m_pInit->LoadFonts( m_pSplash );
			m_pInit->LoadImages( m_pSplash );

			if ( !this->InitWindowWork( ) ) {
				mb::err( L"Не удалось создать рабочее окно" );
				return 1;
			}

			m_pModel = new udModelWork( m_pWork );

			udPWndToolbar	pToolbar	= m_pWork->GetToolbar( );
			udPWndMenu		pMenu		= m_pWork->GetMenu( );
			udPWndModel		pWndModel	= m_pWork->GetWndModel( );

			m_pInit->InitToolbar( m_pSplash, pToolbar );
			m_pInit->InitMenu( m_pSplash, pMenu );
			m_pInit->InitModel( m_pSplash, m_pModel );
			//m_pInit->InitSync( m_pSync );
			m_pSync->Init( );
			//m_pSync->Sync( );

			pToolbar->SetModel( m_pModel );
			pMenu->SetModel( m_pModel );
			pWndModel->SetModel( m_pModel );

			pToolbar->Show( );
			pMenu->Show( );
			pWndModel->Show( );
			
			SendMessage(
				m_pWork->GetHWnd( ), WM_SETICON, ( WPARAM ) ICON_SMALL,
				( LPARAM ) hIconApp
			);

			m_pSplash->Hide( );
			m_pSplash->Destroy( );
			// показываем модель
			m_pModel->Show( );
			CenterOneWindowAboveParent( m_pWork->GetHWnd( ) );
			MessageLoop( this );
			//this->MainLoop( );
		}

		return 0;
	} // udInt Start

	HCURSOR CApplication::GetCursor( string index ) {
		map< string, HCURSOR >::iterator i = m_arrCursor.find( index );
		if ( i != m_arrCursor.end( ) ) {
			return i->second;
		}
		return NULL;
	} // HCURSOR GetCursor

	udInt CApplication::Login( ) {
		assert( m_pLogin );
		udInt		ret	= 0;
		udPMorph	pData	= m_pLogin->GetData( );
		if ( pData ) {
			ret = m_pInit->Login( m_pLogin, pData->get( "login" )->toWString( ), pData->get( "password" )->toWString( ) );
			if ( ret == 1 ) {
				g_pApp->SwitchState( udApplication::APP_STATE_EXIT );
			}
			delete pData;
		}

		return ret;
	} // udInt Login

	udInt CApplication::Exit( ) {
		this->SwitchState( CApplication::APP_STATE_EXIT );
		//PostQuitMessage( 0 );
		return 0;
	} // udInt Exit

	void CApplication::SwitchState( CApplication::appState iState ) {
		switch( m_iState ) {
			case CApplication::APP_STATE_LOGIN:
				m_pLogin->Hide( );
				m_pLogin->Destroy( );
				m_iState = CApplication::APP_STATE_EXIT;
				break;

			case CApplication::APP_STATE_SPLASH:
				if ( iState == CApplication::APP_STATE_EXIT ) {
					m_pSplash->Hide( );
					m_pSplash->Destroy( );
					m_iState = CApplication::APP_STATE_EXIT;
				}
				break;

			case CApplication::APP_STATE_START:
				break;

			case CApplication::APP_STATE_WORK:
				if ( iState == CApplication::APP_STATE_EXIT ) {
					m_iState = CApplication::APP_STATE_EXIT;
				}
				break;

			default:
				break;
		}
	} // void SwitchState

	bool CApplication::IsFinish( ) {
		return m_iState == CApplication::APP_STATE_EXIT;
	} // bool IsFinish

	int CApplication::GetMessage( LPMSG pMsg ) {
		BOOL bGotMsg = ::GetMessage( pMsg, NULL, 0, 0 );
		if ( bGotMsg == 0 ) {
			return udHMsgLoop::MsgLoopQuit;
		} else if ( bGotMsg == -1 ) {
			return udHMsgLoop::MsgLoopError;
		}
		return udHMsgLoop::MsgLoopMessage;
	} // int GetMessage

	void CApplication::OnQuit( ) {
		m_pLog->Write( "CApplication::OnQuit\n" );
	} // void OnQuit

	void CApplication::OnError( ) {
		m_pLog->Write( "CApplication::OnError, code=" + toString( GetLastError( ) ) + "\n" );
	} // void OnError

	void CApplication::ProcMessage( LPMSG pMsg ) {
		if ( !this->CheckDialogMessage( pMsg ) ) {
			::TranslateMessage( pMsg );
			::DispatchMessage( pMsg );
		}
	} // void ProcMessage

	/*void CApplication::MainLoop( ) {
		BOOL bGotMsg = FALSE;

		ZeroMemory( &m_objMsg, sizeof( MSG ) );

		while( m_iState != CApplication::APP_STATE_EXIT ) {
			bGotMsg = GetMessage( &m_objMsg, NULL, 0, 0 );
			if ( bGotMsg == 0 ) {
				break;
			} else if ( bGotMsg == -1 ) {
				m_pLog->Write( "was error\n" );
				break;
			} else {
				if ( !this->CheckDialogMessage( ) ) {
					::TranslateMessage( &m_objMsg );
					::DispatchMessage( &m_objMsg );
				}
			}
		}
	} // void MainLoop*/

	bool CApplication::CheckDialogMessage( LPMSG pMsg ) {
		switch( m_iState ) {
			case CApplication::APP_STATE_LOGIN:
				if ( IsDialogMessage( m_pLogin->GetHWnd( ), pMsg ) ) {
					return true;
				}
				break;

			case CApplication::APP_STATE_WORK:
				/*if ( IsDialogMessage( m_arrUi[ "form_work/area_menu" ]->GetHWnd( ), &m_objMsg ) ) {
					return true;
				}
				if ( IsDialogMessage( m_arrUi[ "form_work/area_work" ]->GetHWnd( ), &m_objMsg ) ) {
					return true;
				}*/
				break;

			default:
				break;
		}

		return false;
	} // bool CheckDialogMessage

	udPImage CApplication::GetImage( string szFilename ) {
		udPImage pImage = NULL;
		udAscImage::iterator i = m_arrImage.find( szFilename );
		if ( i != m_arrImage.end( ) ) {
			pImage = i->second;
		} else {
			wstring szFilenameW = ConvertString( szFilename );
			pImage = new Image( szFilenameW.data( ) );
			m_arrImage[ szFilename ] = pImage;
		}
		return pImage;
	} // udPImage GetImage

	udPBitmap CApplication::GetBitmap( string szFilename ) {
		udPBitmap pBitmap = NULL;
		udAscBitmap::iterator i = m_arrBitmap.find( szFilename );
		if ( i != m_arrBitmap.end( ) ) { // картинка была подгружена
			pBitmap = i->second;
		} else { // пытаемся загрузить
			wstring szFilenameW = ConvertString( szFilename );
			pBitmap = new Bitmap( szFilenameW.data( ) );
			m_arrBitmap[ szFilename ] = pBitmap;
		}

		return pBitmap;
	} // udPImage GetImage

	udPFont CApplication::GetFont( wstring szFamily, udUInt iSize, FontStyle iStyle ) {
		udPFont	pFont		= NULL;
		string	szFamilyA	= ConvertString( szFamily ),
				szIndex		= "";
		udChar	tmp[ 1024 ];
		udAscFont::iterator i;

		sprintf_s( tmp, 1024, "%u", iSize );
		szIndex = szFamilyA + string( " " ) + string( tmp ) + string( " " );
		sprintf_s( tmp, 1024, "%d", iStyle );
		szIndex += tmp;

		if ( ( i = m_arrFont.find( szIndex ) ) != m_arrFont.end( ) ) { // шрифт найден в коллекции
			pFont = i->second;
		} else { // шрифт не найден, добавляем его
			pFont = new Font( szFamily.data( ), ( REAL ) iSize, iStyle, UnitPixel );
			m_arrFont[ szIndex ] = pFont;
		}

		return pFont;
	} // udPFont GetFont

	HFONT CApplication::GetHFont( HWND hWnd, wstring szFamily, udUInt iSize, FontStyle iStyle ) {
		Graphics	objGdi( hWnd );
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;
		udPFont		pFont	= this->GetFont( szFamily, iSize, iStyle );

		pFont->GetLogFontW( &objGdi, &objLogFont );
		hFont = CreateFontIndirect( &objLogFont );

		return hFont;
	} // HFONT GetHFont

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

} // namespace UdSDK
