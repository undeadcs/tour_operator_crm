/**
	Tourist application
	Библиотека ядра клиентского приложения
	Приложение
*/
#ifndef TOURIST_CLIENT_CORE_APPLICATION_H
#define TOURIST_CLIENT_CORE_APPLICATION_H

namespace UdSDK {

	/**
	 *	Приложение
	 */
	class CApplication {
		udPAppConfig		m_pConfig;
		udPAppInit			m_pInit;
		udPLog				m_pLog;
		udPWndLogin			m_wndLogin;
		udPWndWork			m_wndWork;
		udPAppUser			m_pUser;
		udPHandlerManager	m_pHandlerManager;
		udPMySql			m_pMySQL;
		udPHandler			m_pHandler;
		STARTUPINFO			m_objSI;		// для запуска mysqld.exe
		PROCESS_INFORMATION m_objPI;	// для запуска mysqld.exe

		typedef map< udDWord, wstring >	map_folders;

		map_folders	m_mapTmpFolder;

	public:
		CApplication( );
		CApplication( const CApplication& objCopy );
		CApplication( udPAppInit pInit );

		virtual ~CApplication( );

		CApplication& operator =( const CApplication& objCopy );

		udInt		Start( );
		wstring		GetUniqueTmpFolder( );
		udPSocket	BeginRequest( bool login = true );

		udPAppUser			GetUser( )				const { return m_pUser;				}
		udPLog				GetLog( )				const { return m_pLog;				}
		udPWndWork			GetWndWork( )			const { return m_wndWork;			}
		udPHandlerManager	GetHandlerManager( )	const { return m_pHandlerManager;	}
		udPDatabase			GetDatabase( )			const { return m_pMySQL;			}
		udPHandler			GetHandler( )			const { return m_pHandler;			}
		udPAppConfig		GetConfig( )			const { return m_pConfig;			}

	private:
		udInt	InitConfig( );
		udInt	InitLog( );
		udInt	InitMySQL( );
		udInt	InitWndLogin( );
		udInt	InitWndWork( );
		udInt	InitHandlerManager( );

		void	OnLoginOk( );
		void	OnLoginCancel( );

		udInt	Login( udPSocket sock );

		void	CheckDrop( );

	}; // class CApplication

} // namespace UdSDK

#endif
