
#ifndef APPLICATION_H
#define APPLICATION_H

namespace UdSDK {

	//DWORD WINAPI udServerThreadProc( LPVOID pParam );

	class CApplication {
		HINSTANCE		m_hInstance;	// экземпляр приложения
		udPAppConfig	m_pConfig;		// конфиг
		udPMySql		m_pMySQL;		// бд
		udPLog			m_pLog;			// лог
		udPHandler		m_pHandler;		// обработчик объектов
		udPSystemUser	m_pUser;		// пользователь системы
		udPAppInit		m_pInit;		// объект инициализации
		udPModelWork	m_pModel;		// модель работы приложения
		STARTUPINFO		m_objSI;		// для запуска mysqld.exe
		PROCESS_INFORMATION m_objPI;	// для запуска mysqld.exe
		udPWndServer	m_wndServer;	// главное окно сервера

		map< string, HANDLE >	m_arrConnectionHandlers;	// трэды, обрабатывающие клиентские соединения
		map< string, DWORD >	m_arrCHId;					// ID этих трэдов

		udPServer	m_pServer;
		WSADATA		m_objWinSockData;

	public:
		CApplication( HINSTANCE hInstance, udPAppInit pInit );
		CApplication( const CApplication& objApp );

		virtual ~CApplication( );

		udInt	Start( );

		void	SetUser( udPSystemUser user ) { m_pUser = user; }

		udPLog			GetLog( )		const { return m_pLog;		}
		udPDatabase		GetDatabase( )	const { return m_pMySQL;	}
		udPHandler		GetHandler( )	const { return m_pHandler;	}
		HINSTANCE		GetHInstance( )	const { return m_hInstance;	}
		udPSystemUser	GetUser( )		const { return m_pUser;		}
		udPModelWork	GetModel( )		const { return m_pModel;	}
		udPAppConfig	GetConfig( )	const { return m_pConfig;	}
		udPWndServer	GetWndServer( )	const { return m_wndServer;	}
		udPAppInit		GetInitObj( )	const { return m_pInit;		}

	private:
		udInt	InitConfig( );
		udInt	InitLog( );
		udInt	InitMySQL( );
		udInt	InitWindowServer( );
		udInt	InitChildThread( );
		void	CheckDrop( );

	}; // class CApplication

} // namespace UdSDK

#endif
