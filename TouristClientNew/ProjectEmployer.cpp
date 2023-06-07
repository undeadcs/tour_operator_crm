
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHEmployerListPick( ) {
		return new CHEmployerPick;
	} // udPListHandlerPick GetHEmployerListPick

	CHEmployerPick::CHEmployerPick( ) :
		udListHandlerPick( ),
		udStateHandler( ),
		m_iType( emptypeFiz ),
		m_wndWrap( new udWndHandler ),
		m_wndClient( new udHClientPick ),
		m_wndClientUr( new udHClientUrPick ),
		m_btnClient( Glib::locale_to_utf8( "Физ. лица" ) ),
		m_btnClientUr( Glib::locale_to_utf8( "Юр. лица" ) ),
		m_boxSwitch( Gtk::BUTTONBOX_START, 5 )
	{
		m_boxSwitch.pack_start( m_btnClient, Gtk::PACK_SHRINK );
		m_boxSwitch.pack_start( m_btnClientUr, Gtk::PACK_SHRINK );
		m_wndWrap->pack_start( m_boxSwitch, Gtk::PACK_SHRINK );
		m_wndWrap->show_all( );

		m_btnClient.set_sensitive( false );
		m_btnClient.signal_clicked( ).connect( sigc::bind< udInt >(
			sigc::mem_fun( this, &CHEmployerPick::OnSwitchEmployer ),
			emptypeFiz
		) );
		m_btnClientUr.signal_clicked( ).connect( sigc::bind< udInt >(
			sigc::mem_fun( this, &CHEmployerPick::OnSwitchEmployer ),
			emptypeUr
		) );

		Gtk::Button *btn = NULL;

		btn = m_wndClient->GetButtonOk( );
		btn->signal_clicked( ).connect( sigc::bind< udInt >(
			sigc::mem_fun( this, &CHEmployerPick::OnPickEmployer ),
			emptypeFiz
		) );
		btn = m_wndClientUr->GetButtonOk( );
		btn->signal_clicked( ).connect( sigc::bind< udInt >(
			sigc::mem_fun( this, &CHEmployerPick::OnPickEmployer ),
			emptypeUr
		) );
	} // CHEmployerPick

	CHEmployerPick::CHEmployerPick( const CHEmployerPick& objCopy ) :
		udListHandlerPick( ),
		udStateHandler( )
	{
	} // CHEmployerPick

	CHEmployerPick::~CHEmployerPick( ) {
		udDELETE( m_wndClient );
		udDELETE( m_wndClientUr );
		udDELETE( m_wndWrap );
	} // ~CHEmployerPick

	CHEmployerPick& CHEmployerPick::operator =( const CHEmployerPick& objCopy ) {
		return *this;
	} // CHEmployerPick& operator =

	void CHEmployerPick::Init( ) {
		m_wndClient->Init( );
		m_wndClientUr->Init( );

		udPWndHandler wnd = NULL;

		wnd = m_wndClient->GetWindow( );
		m_wndWrap->add( *wnd );
		wnd->hide( );

		wnd = m_wndClientUr->GetWindow( );
		m_wndWrap->add( *wnd );
		wnd->hide( );

		m_wndClient->GetWindow( )->show( );
	} // void CHEmployerPick

	void CHEmployerPick::Load( ) {
		if ( m_iType == emptypeFiz ) {
			m_wndClient->Load( );
		} else {
			m_wndClientUr->Load( );
		}
	} // void Load

	void CHEmployerPick::Show( ) {
		if ( m_iType == emptypeFiz ) {
			m_wndClient->Show( );
		} else {
			m_wndClientUr->Show( );
		}
	} // void Load

	void CHEmployerPick::Hide( ) {
		if ( m_iType == emptypeFiz ) {
			m_wndClient->Hide( );
		} else {
			m_wndClientUr->Hide( );
		}
	} // void Hide

	void CHEmployerPick::Clear( ) {
		if ( m_iType == emptypeFiz ) {
			m_wndClient->Clear( );
		} else {
			m_wndClientUr->Clear( );
		}
	} // void Clear

	void CHEmployerPick::OnPickEmployer( udInt iType ) {
		udDELETE( m_pPickedObject );
		m_iType = iType;
		if ( m_iType == emptypeFiz ) {
			m_pPickedObject = GetProjectClient( );
			udPBase obj = m_wndClient->GetObject( );
			if ( obj ) {
				obj->apply( m_pPickedObject );
			}
		} else {
			m_pPickedObject = GetProjectClientUr( );
			udPBase obj = m_wndClientUr->GetObject( );
			if ( obj ) {
				obj->apply( m_pPickedObject );
			}
		}
	} // void OnPickEmployer

	void CHEmployerPick::OnSwitchEmployer( udInt iToType ) {
		if ( iToType == emptypeFiz ) {
			m_wndClientUr->Clear( );
			m_wndClientUr->Hide( );
			m_wndClientUr->GetWindow( )->hide( );
			m_wndClient->Show( );
			m_wndClient->Load( );
			m_wndClient->GetWindow( )->show( );
			m_btnClient.set_sensitive( false );
			m_btnClientUr.set_sensitive( true );
		} else {
			m_wndClient->Clear( );
			m_wndClient->Hide( );
			m_wndClient->GetWindow( )->hide( );
			m_wndClientUr->Show( );
			m_wndClientUr->Load( );
			m_wndClientUr->GetWindow( )->show( );
			m_btnClient.set_sensitive( true );
			m_btnClientUr.set_sensitive( false );
		}
	} // void OnSwitchEmployer

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CWgtEmployerPick::CWgtEmployerPick( ) :
		udWgtListPick( ),
		m_iType( emptypeFiz )
	{
		m_szAttr = "fio";
	} // CWgtEmployerPick

	CWgtEmployerPick::CWgtEmployerPick( const CWgtEmployerPick& objCopy ) : udWgtListPick( ) {
		m_szAttr = "fio";
	} // CWgtEmployerPick

	CWgtEmployerPick::~CWgtEmployerPick( ) {
	} // ~CWgtEmployerPick

	CWgtEmployerPick& CWgtEmployerPick::operator =( const CWgtEmployerPick& objCopy ) {
		return *this;
	} // CWgtEmployerPick& operator =

	void CWgtEmployerPick::OnButtonClick( ) {
		udPHEmployerPick handler = new CHEmployerPick;
		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szWindowTitle );

		udPBase obj = handler->GetObject( );
		if ( obj && m_pFde ) {
			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );
			m_iType = handler->GetType( );

			if ( m_iType == emptypeFiz ) {
				m_szAttr = "fio";
			} else {
				m_szAttr = "name";
			}
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( m_szAttr )->toString( ) ) );
			}

			if ( m_pHandler ) {
				m_pHandler->AfterPick( this );
			}
		}

		udDELETE( handler );
	} // void OnButtonClick

	bool CWgtEmployerPick::OnClick( GdkEventButton* evtClick ) {
		if ( !m_pFde || !m_pFde->GetObject( ) || !m_pFde->GetObject( )->get( "id" )->toUInt( ) ) {
			return false;
		}

		if ( m_iType == emptypeFiz ) {
			m_szAttr = "fio";
			m_fnMakeFormHandler = GetHClientForm;
			m_szFormWindowTitle = Glib::locale_to_utf8( "Редактирование клиента" );
		} else {
			m_fnMakeFormHandler = GetHClientUrForm;
			m_szFormWindowTitle = Glib::locale_to_utf8( "Редактирование клиента (юр. лицо)" );
			m_szAttr = "name";
		}

		Gtk::Window *wndParent = ( Gtk::Window* ) get_toplevel( );
		udPFormHandler handler = m_fnMakeFormHandler( );
		handler->Init( );
		handler->GetData( )->SetId( m_pFde->GetObject( )->get( "id" )->toUInt( ) );
		handler->Load( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szFormWindowTitle );

		udPBase obj = handler->GetData( )->GetData( );
		if ( obj ) {
			string log = "modified object, ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( m_szAttr )->toString( ) ) );
			}
		}

		udDELETE( handler );

		return true;
	} // bool OnClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CFDEEmployer::CFDEEmployer( ) : udFDEObject( ) {
		m_fnGetObject = GetProjectClient;
	} // CFDEEmployer

	CFDEEmployer::CFDEEmployer( const CFDEEmployer& objCopy ) {
	} // CFDEEmployer

	CFDEEmployer::~CFDEEmployer( ) {
	} // ~CFDEEmployer

	CFDEEmployer& CFDEEmployer::operator =( const CFDEEmployer& objCopy ) {
		return *this;
	} // CFDEEmployer& operator =

	void CFDEEmployer::Load( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDEEmployer::Load\n" );
		if ( m_pMainObject->get( "emptype" )->toUInt( ) == emptypeFiz ) {
			m_fnGetObject = GetProjectClient;
		} else {
			m_fnGetObject = GetProjectClientUr;
		}
		g_pApp->GetLog( )->Write( "CFDEEmployer::Load, udFDEObject::Load\n" );
		udFDEObject::Load( sock );
		g_pApp->GetLog( )->Write( "CFDEEmployer::Load\n" );
	} // void Load

} // namespace UdSDK
