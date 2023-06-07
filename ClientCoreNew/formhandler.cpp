
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CFormHandler::CFormHandler( ) :
		udStateHandler( ),
		m_btnOk( Glib::locale_to_utf8( "Сохранить" ) ),
		m_btnCancel( Glib::locale_to_utf8( "Отмена" ) )
	{
		m_btnOk.set_name( "TouristFormButtonOk" );
		m_btnOk.set_size_request( 100, 36 );
		m_btnCancel.set_name( "TouristFormButtonCancel" );
		m_btnCancel.set_size_request( 100, 36 );

		m_wndFormWrap	= new udWndHandler;
		m_pData			= new udDAForm;

		m_btnOk.signal_clicked( ).connect( sigc::mem_fun( this, &CFormHandler::OnButtonOk ) );
		m_btnCancel.signal_clicked( ).connect( sigc::mem_fun( this, &CFormHandler::OnButtonCancel ) );
	} // CFormHandler

	CFormHandler::CFormHandler( const CFormHandler& objCopy ) : udStateHandler( ) {
	} // CFormHandler

	CFormHandler::~CFormHandler( ) {
		g_pApp->GetLog( )->Write( "CFormHandler::~CFormHandler, 1\n" );
		udDELETE( m_pData );
		g_pApp->GetLog( )->Write( "CFormHandler::~CFormHandler, 2\n" );
		udDELETE( m_wndFormWrap );
		g_pApp->GetLog( )->Write( "CFormHandler::~CFormHandler, 3\n" );
	} // ~CFormHandler

	CFormHandler& CFormHandler::operator =( const CFormHandler& objCopy ) {
		return *this;
	} // CFormHandler& operator =

	void CFormHandler::Load( ) {
		g_pApp->GetLog( )->Write( "CFormHandler::Load\n" );
		g_pApp->GetLog( )->Write( "CFormHandler::Load, m_pData->MakeObject\n" );
		m_pData->MakeObject( );
		g_pApp->GetLog( )->Write( "CFormHandler::Load, m_pData->Load\n" );
		m_pData->Load( );
		g_pApp->GetLog( )->Write( "CFormHandler::Load exit\n" );
	} // void Load

	void CFormHandler::Clear( ) {
		m_pData->Clear( );
	} // void Clear

} // namespace UdSDK
