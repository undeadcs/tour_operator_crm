
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndLogin::CWndLogin( ) :
		Gtk::Window( ),
		m_lblLogin( Glib::locale_to_utf8( "Логин" ), 0.0f, 0.0f ),
		m_lblPassword( Glib::locale_to_utf8( "Пароль" ), 0.0f, 0.0f )
	{
		//set_decorated( false );
		set_name( "TouristLogin" );
		set_title( Glib::locale_to_utf8( "ИСУ БМТ Приморье" ) );
		set_resizable( false );
		set_modal( true );

		m_lblLogin.set_name( "TouristLoginLabel" );
		m_lblPassword.set_name( "TouristLoginLabel" );

		m_btnOk			= new Gtk::Button( Glib::locale_to_utf8( "OK" ) );
		m_btnCancel		= new Gtk::Button( Glib::locale_to_utf8( "Отмена" ) );
		m_inpLogin		= new udWgtInput;
		m_inpPassword	= new udWgtInput;

		m_btnOk->set_name( "TouristLoginButton" );
		m_btnCancel->set_name( "TouristLoginButton" );
		m_inpPassword->GetEntry( )->set_visibility( false );
		m_boxWindow.set_spacing( 5 );
		m_boxWindow.set_border_width( 20 );

		m_boxWindow.add( m_lblLogin );
		m_boxWindow.add( *m_inpLogin );
		m_boxWindow.add( m_lblPassword );
		m_boxWindow.add( *m_inpPassword );

		m_boxButton.set_layout( Gtk::BUTTONBOX_CENTER );
		m_boxButton.set_spacing( 20 );
		m_boxButton.add( *m_btnOk );
		m_boxButton.add( *m_btnCancel );

		m_boxWindow.add( m_boxButton );
		add( m_boxWindow );

		m_inpLogin->GetEntry( )->signal_activate( ).connect( sigc::mem_fun( this, &CWndLogin::OnActivateEntry ) );
		m_inpPassword->GetEntry( )->signal_activate( ).connect( sigc::mem_fun( this, &CWndLogin::OnActivateEntry ) );

		show_all( );
	} // CWndLogin

	CWndLogin::~CWndLogin( ) {
		udDELETE( m_btnOk );
		udDELETE( m_btnCancel );
		udDELETE( m_inpLogin );
		udDELETE( m_inpPassword );
	} // ~CWndLogin

	void CWndLogin::OnActivateEntry( ) {
		m_btnOk->clicked( );
	} // void OnActivateEntry

} // namesapce UdSDK
