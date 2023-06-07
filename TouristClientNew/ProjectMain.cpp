
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHMain::CHMain( ) :
		udStateHandlerEmpty( ),
		m_lblHeader( Glib::locale_to_utf8( "Здравствуйте." ) )
	{
		m_szName = "main";
		m_wndEmpty->add( m_lblHeader );
		m_wndEmpty->add( m_lblUserName );
		m_wndEmpty->show_all_children( );
	} // CProjectMain

	CHMain::CHMain( const CHMain& objCopy ) {
	} // CProjectMain

	CHMain::~CHMain( ) {
	} // ~CProjectMain

	CHMain& CHMain::operator =( const CHMain& objCopy ) {
		return *this;
	} // CProjectMain& operator =

	void CHMain::Load( ) {
		udPAppUser pUser = g_pApp->GetUser( );

		m_lblUserName.set_text( Glib::locale_to_utf8( ConvertString(
			pUser->get( "login" )->toWString( ) + L", " +
			pUser->get( "fio" )->toWString( )
		) ) );
	} // void Load

} // namespace UdSDK
