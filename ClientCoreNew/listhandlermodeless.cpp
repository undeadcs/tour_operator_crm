
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CListHandlerModeless::CListHandlerModeless( ) :
		m_btnMain( Glib::locale_to_utf8( "На главную" ) )
	{
		m_btnMain.signal_clicked( ).connect( sigc::mem_fun( *this, &CListHandlerModeless::OnButtonMain ) );
	} // CListHandlerModeless

	CListHandlerModeless::CListHandlerModeless( const CListHandlerModeless& objCopy ) {
	} // CListHandlerModeless

	CListHandlerModeless::~CListHandlerModeless( ) {
	} // ~CListHandlerModeless

	CListHandlerModeless& CListHandlerModeless::operator =( const CListHandlerModeless& objCopy ) {
		return *this;
	} // CListHandlerModeless& operator =

	void CListHandlerModeless::OnButtonMain( ) {
		g_pApp->GetHandlerManager( )->SwitchToDefault( );
	} // void OnButtonMain

	void CListHandlerModeless::Init( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( GetModelessName( ), GetModelessWindow( ) );
	} // void Init

	void CListHandlerModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( GetModelessName( ) );
	} // void Show

} // namespace UdSDK