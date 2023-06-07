/*
	Tourist application
	Библиотека ядра клиентского приложения
	Модель работы приложения
*/
#ifndef TOURIST_CLIENT_CORE_HANDLER_MANAGER_H
#define TOURIST_CLIENT_CORE_HANDLER_MANAGER_H

namespace UdSDK {

	class CHandlerManager {
		typedef map< string, udPStateHandler >	t_ascStateHandler;

		udPStateHandler		m_pDefaultHandler,
							m_pCurrentHandler;
		t_ascStateHandler	m_ascHandler;

	public:
		CHandlerManager( );
		CHandlerManager( const CHandlerManager& objCopy );

		virtual ~CHandlerManager( );

		CHandlerManager& operator =( const CHandlerManager& objCopy );

		void	Init( );
		virtual void	SetState( const string& szName );
		void	SetHandler( const string& szName, udPStateHandler pHandler );
		void	SetDefaultHandler( const string& szName );
		void	SwitchToDefault( );

		udPStateHandler	GetHandler( const string& szName ) const;

		udPStateHandler	GetDefaultHandler( )	const { return m_pDefaultHandler;	}
		udPStateHandler	GetCurrentHandler( )	const { return m_pCurrentHandler;	}

	}; // class CHandlerManager

} // namespace UdSDK

#endif
