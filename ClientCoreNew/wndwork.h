/*
	Tourist application
	Библиотека ядра клиентского приложения
	Рабочее окно
*/
#ifndef TOURIST_CLIENT_CORE_WND_WORK_H
#define TOURIST_CLIENT_CORE_WND_WORK_H

namespace UdSDK {

	class CWndWork : public Gtk::Window {
		udPWndToolbar	m_wndToolbar;
		udPWndMenu		m_wndMenu;
		udPWndModel		m_wndModel;
		Gtk::VBox		m_boxMain;
		Gtk::HBox		m_boxBottom;
		Gtk::ScrolledWindow
						m_wndScroll;

	public:
		CWndWork( );
		CWndWork( const CWndWork& objCopy );

		virtual ~CWndWork( );

		CWndWork& operator =( const CWndWork& objCopy );

		udPWndToolbar	GetWndToolbar( )	const { return m_wndToolbar;	}
		udPWndMenu		GetWndMenu( )		const { return m_wndMenu;		}
		udPWndModel		GetWndModel( )		const { return m_wndModel;		}

	protected:
		bool	OnClose( GdkEventAny* evt );

	}; // class CWndWork

} // namespace UdSDK

#endif
