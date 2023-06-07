
#include "includes.h"

namespace UdSDK {

	CListHandlerPick::CListHandlerPick( ) :
		m_btnOk( new Gtk::Button( "OK" ) ),
		m_btnCancel( new Gtk::Button( Glib::locale_to_utf8( "Отмена" ) ) ),
		m_pPickedObject( NULL )
	{
		m_btnOk->set_name( "TouristFormButtonOk" );
		m_btnOk->set_size_request( 100, 36 );
		m_btnCancel->set_name( "TouristFormButtonCancel" );
		m_btnCancel->set_size_request( 100, 36 );

		m_boxButton.set_spacing( 10 );
		m_boxButton.set_layout( Gtk::BUTTONBOX_CENTER );
		m_boxButton.add( *m_btnOk );
		m_boxButton.add( *m_btnCancel );
		m_boxButton.show_all( );

		m_btnOk->signal_clicked( ).connect( sigc::mem_fun( *this, &CListHandlerPick::OnButtonOk ) );
		m_btnCancel->signal_clicked( ).connect( sigc::mem_fun( *this, &CListHandlerPick::OnButtonCancel ) );
	} // CListHandlerPick

	CListHandlerPick::CListHandlerPick( const CListHandlerPick& objCopy ) {
	} // CListHandlerPick

	CListHandlerPick::~CListHandlerPick( ) {
		udDELETE( m_btnOk );
		udDELETE( m_btnCancel );
		udDELETE( m_pPickedObject );
	} // ~CListHandlerPick

	CListHandlerPick& CListHandlerPick::operator =( const CListHandlerPick& objCopy ) {
		return *this;
	} // CListHandlerPick& operator =

} // namespace UdSDK
