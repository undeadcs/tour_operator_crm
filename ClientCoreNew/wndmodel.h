/*
	Tourist application
	Библиотека ядра клиентского приложения
	Окно модели
*/
#ifndef TOURIST_CLIENT_CORE_WND_MODEL_H
#define TOURIST_CLIENT_CORE_WND_MODEL_H

namespace UdSDK {

	class CWndModel : public Gtk::VBox {
		typedef std::map< string, udPWndHandler >	t_mapWndHandler;

		t_mapWndHandler	m_ascWindow;
		udPWndHandler	m_wndCurrent,
						m_wndDefault;

	public:
		CWndModel( );
		CWndModel( const CWndModel& objCopy );

		virtual ~CWndModel( );

		CWndModel& operator =( const CWndModel& objCopy );

		void	SetDefaultWindow( const string& szName );
		void	SetCurrent( const string& szName );
		void	SetWindow( const string& szName, udPWndHandler wndHandler );
		void	SwitchToDefault( );

		udPWndHandler	GetWindow( const string& szName ) const;

		udPWndHandler	GetDefaultWindow( )	const { return m_wndDefault;	}
		udPWndHandler	GetCurrentWindow( )	const { return m_wndCurrent;	}

	}; // class CWndModel

} // namespace UdSDK

#endif
