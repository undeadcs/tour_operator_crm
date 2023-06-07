
#include "includes.h"

namespace UdSDK {

	CWndWork::CWndWork( ) : Gtk::Window( ) {
		m_wndToolbar	= new udWndToolbar;
		m_wndMenu		= new udWndMenu;
		m_wndModel		= new udWndModel;

		// настройки окна
		set_size_request( 800, 600 );
		set_border_width( 0 );
		set_name( "Tourist" );

		// добавляем элементы
		m_boxMain.pack_start( *m_wndToolbar, Gtk::PACK_SHRINK, ( guint ) 0 );
		m_boxBottom.pack_start( *m_wndMenu, Gtk::PACK_SHRINK, ( guint ) 0 );
		m_boxBottom.pack_start( *m_wndModel, Gtk::PACK_EXPAND_WIDGET );
		m_boxMain.pack_start( m_boxBottom, Gtk::PACK_EXPAND_WIDGET );
		m_boxMain.set_border_width( 0 );

		m_wndScroll.set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
		m_wndScroll.set_shadow_type( Gtk::SHADOW_NONE );
		m_wndScroll.add( m_boxMain );

		add( m_wndScroll );
		show_all_children( );

		signal_delete_event( ).connect( sigc::mem_fun( *this, &CWndWork::OnClose ) );
	} // CWndWork

	CWndWork::CWndWork( const CWndWork& objCopy ) : Gtk::Window( ) {
	} // CWndWork

	CWndWork::~CWndWork( ) {
		udDELETE( m_wndToolbar );
		udDELETE( m_wndMenu );
		udDELETE( m_wndModel );
	} // CWndWork

	CWndWork& CWndWork::operator =( const CWndWork& objCopy ) {
		return *this;
	} // CWndWork& operator =

	bool CWndWork::OnClose( GdkEventAny* evt ) {
		Gtk::MessageDialog dialog(
			*this,
			Glib::locale_to_utf8( "Вы уверены, что хотите выйти из программы?" ),
			false,
			Gtk::MESSAGE_INFO,
			Gtk::BUTTONS_OK_CANCEL
		);

		return dialog.run( ) != Gtk::RESPONSE_OK;
	} // bool OnClose

} // namespace UdSDK
