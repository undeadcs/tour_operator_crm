/*
	Tourist application
	Библиотека ядра клиентского приложения
	Окно тулбара
*/
#ifndef TOURIST_CLIENT_CORE_WND_TOOLBAR_H
#define TOURIST_CLIENT_CORE_WND_TOOLBAR_H

namespace UdSDK {

	class CWndToolbar : public Gtk::EventBox {
		typedef vector< Gtk::Button* >	t_vctButton;

		t_vctButton	m_arrButton;
		Gtk::HBox	m_boxContent;

	public:
		CWndToolbar( );
		CWndToolbar( const CWndToolbar& objCopy );

		virtual ~CWndToolbar( );

		CWndToolbar& operator =( const CWndToolbar& objCopy );

		virtual Gtk::Button*	add( const Glib::ustring& szLabel, const string& szHandlerName, guint spacing = 0 );

	}; // class CWndToolbar

} // namespace UdSDK

#endif
