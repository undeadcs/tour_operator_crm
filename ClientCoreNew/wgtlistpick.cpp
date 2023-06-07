
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtListPick::CWgtListPick( ) :
		Gtk::HBox( ),
		m_btnList( new Gtk::Button( "" ) ),
		m_lblTitle( "" ),
		m_fnMakeHandler( NULL ),
		m_fnMakeFormHandler( NULL ),
		m_pFde( NULL ),
		m_pHandler( NULL )
	{
		m_btnList->set_name( "TouristWgtListPickButton" );
		m_btnList->set_size_request( 25, 22 );

		m_lblTitle.set_name( "TouristWgtListPickLabel" );
		m_lblTitle.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_CENTER );
		m_lblTitle.set_ellipsize( Pango::ELLIPSIZE_END );
		m_lblTitle.set_size_request( 171, 22 );

		m_boxLabelWrap.set_border_width( 2 );
		m_boxLabelWrap.pack_start( m_lblTitle, Gtk::PACK_EXPAND_WIDGET, 5 );

		m_boxLabel.add( m_boxLabelWrap );
		m_boxLabel.set_name( "TouristWgtListPickWrap" );
		m_boxLabel.signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtListPick::OnExpose ) );
		m_boxLabel.add_events( Gdk::BUTTON_PRESS_MASK );
		m_boxLabel.signal_button_press_event( ).connect( sigc::mem_fun( this, &CWgtListPick::OnClick ) );

		m_btnList->signal_clicked( ).connect( sigc::mem_fun( this, &CWgtListPick::OnButtonClick ) );

		pack_start( m_boxLabel, Gtk::PACK_SHRINK, ( guint ) 0 );
		pack_start( *m_btnList, Gtk::PACK_SHRINK, ( guint ) 0 );
		show_all_children( );
	} // CWgtListPick

	CWgtListPick::CWgtListPick( const CWgtListPick& objCopy ) : Gtk::HBox( ) {
	} // CWgtListPick

	CWgtListPick::~CWgtListPick( ) {
	} // ~CWgtListPick

	CWgtListPick& CWgtListPick::operator =( const CWgtListPick& objCopy ) {
		return *this;
	} // CWgtListPick& operator =

	void CWgtListPick::OnButtonClick( ) {
		if ( !m_fnMakeHandler ) {
			g_pApp->GetLog( )->Write( "CWgtListPick::OnButtonClick, no make handler callback\n" );
			return;
		}

		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		udPListHandlerPick handler = m_fnMakeHandler( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szWindowTitle );

		udPBase obj = handler->GetObject( );
		if ( obj && m_pFde ) {
			string log = "picked object, ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			m_pFde->Clear( );
			m_pFde->Make( false );
			m_pFde->GetObject( )->apply( obj, true, true );
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( m_szAttr )->toString( ) ) );
			}

			if ( m_pHandler ) {
				m_pHandler->AfterPick( this );
			}
		}

		udDELETE( handler );
	} // void OnButtonClick

	bool CWgtListPick::OnExpose( GdkEventExpose* evtExpose ) {
		Gtk::Allocation allocation = m_boxLabel.get_allocation( );
		int width = allocation.get_width( );
		int height = allocation.get_height( );
		Cairo::RefPtr< Cairo::Context > myContext = m_boxLabel.get_window( )->create_cairo_context( );

		myContext->save( );

		myContext->set_source_rgb( 0.7, 0.7, 0.7 );
		myContext->set_line_width( 2.0 );
		myContext->move_to( width, height - 1 );
		myContext->line_to( 1, height - 1 );
		myContext->line_to( 1, 1 );
		myContext->line_to( width, 1 );
		myContext->stroke( );

		myContext->restore( );

		return false;
	} // bool OnExpose

	bool CWgtListPick::OnClick( GdkEventButton* evtClick ) {
		if ( !m_pFde || !m_pFde->GetObject( ) || !m_pFde->GetObject( )->get( "id" )->toUInt( ) || !m_fnMakeFormHandler ) {
			return false;
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

} // namespace UdSDK
