
#ifndef APPLICATION_H
#define APPLICATION_H

namespace UdSDK {

	/**
	 *	Приложение
	 */
	class CApplication : public udHMsgLoop {
	public:
		typedef enum appState {
			APP_STATE_CONSTRUCTED,	// был вызван конструктор и все
			APP_STATE_INITCONFIG,	// инициализация конфига
			APP_STATE_INITLOG,		// инициализация лога
			APP_STATE_INITMYSQL,	// инициализация MySQL
			APP_STATE_START,		// старт приложения
			APP_STATE_LOGIN,		// вход
			APP_STATE_SPLASH,		// загрузка данных и показа окна splash
			APP_STATE_LOADFONT,		// загрузка шрифтов
			APP_STATE_LOADIMAGE,	// загрузка картинок
			APP_STATE_INITTOOLBAR,	// инициализация тулбара
			APP_STATE_INITMENU,		// инициализация меню
			APP_STATE_INITWORK,		// инициализация рабочей области
			APP_STATE_INITMODEL,	// инициализация модели
			APP_STATE_WORK,			// рабочий режим
			APP_STATE_EXIT			// выход
		} appState;

	private:
		HINSTANCE		m_pInstance;	// экземпляр приложения
		udPAppConfig	m_pConfig;		// конфиг
		udPMySql		m_pMySQL;		// бд
		udPLog			m_pLog;			// лог
		appState		m_iState;		// текущее состояние
		MSG				m_objMsg;		// сообщение из очереди
		udPHandler		m_pHandler;		// обработчик объектов
		udPBase			m_pUser;		// пользователь системы
		udAscFont		m_arrFont;		// шрифты, используемые приложением
		udAscImage		m_arrImage;		// картинки, используемые приложением
		udAscBitmap		m_arrBitmap;	// картинки
		udPAppInit		m_pInit;		// объект инициализации
		udPWndLogin		m_pLogin;		// окно входа
		udPWndSplash	m_pSplash;		// окно загрузки ( splash )
		udPWndWork		m_pWork;		// окно работы
		udPModelWork	m_pModel;		// модель работы приложения
		ULONG_PTR		m_pToken;		// для GDI+
		GdiplusStartupInput	m_objGdiInp;// для GDI+
		STARTUPINFO		m_objSI;		// для запуска mysqld.exe
		PROCESS_INFORMATION m_objPI;	// для запуска mysqld.exe
		udAscHCursor	m_arrCursor;
		WSADATA			m_objWinSockData;	// для инициализации сокетов

		udPDatabaseSync	m_pSync;	// синхронизатор базы

		typedef map< udDWord, wstring >	map_folders;

		map_folders	m_mapTmpFolder;	// набор папок, занятых во временной

	public:
		CApplication( HINSTANCE hInstance, udPAppInit pInit );
		CApplication( const CApplication& objApp );

		virtual ~CApplication( );

		udInt	Start( );
		udInt	Login( );
		udInt	Exit( );

		udPFont		GetFont( wstring szFamily = L"Tahoma", udUInt iSize = 12, FontStyle iStyle = FontStyleRegular );
		HFONT		GetHFont( HWND hWnd, wstring szFamily = L"Tahoma", udUInt iSize = 12, FontStyle iStyle = FontStyleRegular );
		udPImage	GetImage( string szFilename );
		udPBitmap	GetBitmap( string szFilename );
		HCURSOR		GetCursor( string index );

		void	SetUser( udPBase user ) { m_pUser = user; }

		wstring	GetUniqueTmpFolder( );

		udPLog			GetLog( )		const { return m_pLog;		}
		udPDatabase		GetDatabase( )	const { return m_pMySQL;	}
		udPHandler		GetHandler( )	const { return m_pHandler;	}
		HINSTANCE		GetHInstance( )	const { return m_pInstance;	}
		udPBase			GetUser( )		const { return m_pUser;		}
		udPModelWork	GetModel( )		const { return m_pModel;	}
		udPAppConfig	GetConfig( )	const { return m_pConfig;	}
		udPDatabaseSync	GetSync( )		const { return m_pSync;		}
		udPAppInit		GetInitObj( )	const { return m_pInit;		}

		virtual bool	IsFinish( );
		virtual int		GetMessage( LPMSG pMsg );
		virtual void	OnQuit( );
		virtual void	OnError( );
		virtual void	ProcMessage( LPMSG pMsg );

	private:
		void	SwitchState( CApplication::appState iState );
		udInt	InitConfig( );
		udInt	InitLog( );
		udInt	InitMySQL( );
		udInt	InitGraphic( );
		udInt	InitWindowLogin( );
		udInt	InitWindowSplash( );
		udInt	InitWindowWork( );
		//void	MainLoop( );
		bool	CheckDialogMessage( LPMSG pMsg );
		void	CheckDrop( );

	}; // class CApplication

} // namespace UdSDK

#endif
