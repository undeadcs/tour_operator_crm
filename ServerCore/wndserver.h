/**
	Tourist application
	Ядро сервера

	главное окно сервера
*/
#ifndef TOURIST_APPLICATION_SERVER_CORE_WNDSERVER_H
#define TOURIST_APPLICATION_SERVER_CORE_WNDSERVER_H

namespace UdSDK {

	class CWndServer : public Gtk::Window {
		Gtk::Label*	m_pLabel;

	public:
		CWndServer( );
		CWndServer( const CWndServer& objCopy );

		virtual ~CWndServer( );

		CWndServer& operator =( const CWndServer& objCopy );

		// липовая функция для тестинга
		void	UpdateText( const wstring& text );

	}; // class CWndServer

} // namespace UdSDK

#endif
