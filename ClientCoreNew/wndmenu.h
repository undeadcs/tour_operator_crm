/*
	Tourist application
	Библиотека ядра клиентского приложения
	Меню
*/
#ifndef TOURIST_CLIENT_CORE_WND_MENU_H
#define TOURIST_CLIENT_CORE_WND_MENU_H

namespace UdSDK {

	class CWndMenu : public Gtk::EventBox {
		typedef vector< Gtk::Button* >	t_vctButton;
		typedef Gtk::Label t_sep;
		typedef vector< t_sep* > t_vctSeparator;

		t_vctButton		m_arrButton;
		t_vctSeparator	m_arrSeparator;
		Gtk::VBox		m_boxContent;

	public:
		CWndMenu( );
		CWndMenu( const CWndMenu& objCopy );

		virtual ~CWndMenu( );

		CWndMenu& operator =( const CWndMenu& objCopy );

		virtual Gtk::Button*	add( const Glib::ustring& szLabel, const string& szHandlerName, guint spacing = 0 );

		void	add_sep( guint height );

	}; // class CWndMenu

} // namespace UdSDK

#endif
