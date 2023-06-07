
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndModel::CWndModel( ) :
		Gtk::VBox( ),
		m_wndCurrent( NULL ),
		m_wndDefault( NULL )
	{
	} // CWndModel

	CWndModel::CWndModel( const CWndModel& objCopy ) :
		Gtk::VBox( ),
		m_wndCurrent( NULL ),
		m_wndDefault( NULL )
	{
	} // CWndModel

	CWndModel::~CWndModel( ) {
		for( t_mapWndHandler::iterator i = m_ascWindow.begin( ); i != m_ascWindow.end( ); ++i ) {
			delete i->second;
		}
	} // CWndModel

	CWndModel& CWndModel::operator =( const CWndModel& objCopy ) {
		return *this;
	} // CWndModel& operator =

	void CWndModel::SetDefaultWindow( const string& szName ) {
		t_mapWndHandler::iterator i = m_ascWindow.find( szName );
		if ( i != m_ascWindow.end( ) ) {
			m_wndDefault = i->second;
		}
	} // void SetDefaultWindow

	void CWndModel::SetWindow( const string& szName, udPWndHandler wndHandler ) {
		t_mapWndHandler::iterator i = m_ascWindow.find( szName );
		if ( i != m_ascWindow.end( ) ) {
			g_pApp->GetLog( )->Write( "window named '" + szName + "' already exists\n" );
			return;
		}
		m_ascWindow[ szName ] = wndHandler;
		wndHandler->hide( );
		pack_start( *wndHandler, Gtk::PACK_EXPAND_WIDGET );
	} // void SetWindow

	void CWndModel::SwitchToDefault( ) {
		if ( !m_wndDefault ) {
			return;
		}
		if ( m_wndCurrent ) {
			m_wndCurrent->hide( );
		}
		m_wndCurrent = m_wndDefault;
		m_wndCurrent->show( );
	} // void SwitchToDefault

	void CWndModel::SetCurrent( const string& szName ) {
		t_mapWndHandler::iterator i = m_ascWindow.find( szName );
		if ( i == m_ascWindow.end( ) ) {
			g_pApp->GetLog( )->Write( "window named '" + szName + "' not found\n" );
			return;
		}
		if ( m_wndCurrent == i->second ) {
			return;
		}
		if ( m_wndCurrent ) {
			m_wndCurrent->hide( );
		}
		m_wndCurrent = i->second;
		m_wndCurrent->show( );
	} // void SetCurrent

	udPWndHandler CWndModel::GetWindow( const string& szName ) const {
		t_mapWndHandler::const_iterator i = m_ascWindow.find( szName );
		if ( i != m_ascWindow.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPWndHandler GetWindow

} // namespace UdSDK
