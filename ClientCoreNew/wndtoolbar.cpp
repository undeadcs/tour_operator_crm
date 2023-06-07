
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndToolbar::CWndToolbar( ) : Gtk::EventBox( ) {
		set_size_request( -1, 94 );
		set_name( "TouristToolbar" );

		m_boxContent.set_spacing( 5 );
		m_boxContent.set_border_width( 10 );

		Gtk::EventBox::add( m_boxContent );
		show_all_children( );
	} // CWndToolbar

	CWndToolbar::CWndToolbar( const CWndToolbar& objCopy ) : Gtk::EventBox( ) {
	} // CWndToolbar

	CWndToolbar::~CWndToolbar( ) {
	} // ~CWndToolbar

	CWndToolbar& CWndToolbar::operator =( const CWndToolbar& objCopy ) {
		for( t_vctButton::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
			delete *i;
		}
		return *this;
	} // CWndToolbar& operator =

	Gtk::Button* CWndToolbar::add( const Glib::ustring& szLabel, const string& szHandlerName, guint spacing ) {
		Gtk::Button *btn = new Gtk::Button;
		//btn->set_name( "TouristToolbarButton" );
		btn->set_relief( Gtk::RELIEF_NONE );
		btn->set_label( szLabel );
		m_arrButton.push_back( btn );
		m_boxContent.pack_start( *btn, Gtk::PACK_SHRINK, spacing );
		if ( !szHandlerName.empty( ) ) {
			btn->signal_clicked( ).connect( sigc::bind< string >(
				sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
				szHandlerName
			) );
		}
		btn->show( );
		return btn;
	} // void add

} // namespace UdSDK
