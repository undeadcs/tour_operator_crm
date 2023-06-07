
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndServer::CWndServer( ) : Gtk::Window( ) {
		m_pLabel = new Gtk::Label;

		add( *m_pLabel );
		m_pLabel->show( );
	}

	CWndServer::~CWndServer( ) {
		udDELETE( m_pLabel );
	}

	void CWndServer::UpdateText( const wstring& text ) {
		//m_pLabel->set_label( ConvertString( text, CP_UTF8 ) );
	} // void UpdateText

} // namespace UdSDK
