
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndMenu::CWndMenu( ) : Gtk::EventBox( ) {
		set_size_request( 244, -1 );
		set_name( "TouristMenu" );

		m_boxContent.set_spacing( 5 );
		m_boxContent.set_border_width( 25 );

		Gtk::EventBox::add( m_boxContent );
		show_all_children( );
	} // CWndMenu

	CWndMenu::CWndMenu( const CWndMenu& objCopy ) : Gtk::EventBox( ) {
	} // CWndMenu

	CWndMenu::~CWndMenu( ) {
		for( t_vctButton::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
			delete *i;
		}
		for( t_vctSeparator::iterator i = m_arrSeparator.begin( ); i != m_arrSeparator.end( ); ++i ) {
			delete *i;
		}
	} // CWndMenu

	CWndMenu& CWndMenu::operator =( const CWndMenu& objCopy ) {
		return *this;
	} // CWndMenu& operator =

	Gtk::Button* CWndMenu::add( const Glib::ustring& szLabel, const string& szHandlerName, guint spacing ) {
		Gtk::Button *btn = new Gtk::Button( szLabel );
		btn->set_name( "TouristMenuButton" + toString( m_arrButton.size( ) ) );
		btn->set_size_request( 189, 43 );
		//btn->set_label( szLabel );
		m_arrButton.push_back( btn );
		m_boxContent.pack_start( *btn, false, false );//Gtk::PACK_SHRINK, spacing );
		btn->signal_clicked( ).connect( sigc::bind< string >(
			sigc::mem_fun( g_pApp->GetHandlerManager( ), &CHandlerManager::SetState ),
			szHandlerName
		) );
		btn->show( );
		return btn;
	} // void add

	void CWndMenu::add_sep( guint height ) {
		t_sep* sep = new t_sep( );
		sep->set_size_request( -1, height );
		m_arrSeparator.push_back( sep );
		m_boxContent.pack_start( *sep, Gtk::PACK_SHRINK );
		sep->show( );
	} // void add_sep

} // namespace UdSDK
