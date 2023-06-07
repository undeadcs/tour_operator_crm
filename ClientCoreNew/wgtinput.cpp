
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtInput::Entry::Entry( const Glib::ustring& szValue ) :
	Gtk::Entry( ),
	m_szValue( szValue )
	{
		InitValue( );
	} // Entry

	CWgtInput::Entry::Entry( const CWgtInput::Entry& objCopy ) :
	Gtk::Entry( ),
	m_szValue( objCopy.m_szValue )
	{
		InitValue( );
	} // Entry

	CWgtInput::Entry::~Entry( ) {
	} // ~Entry

	CWgtInput::Entry& CWgtInput::Entry::operator =( const CWgtInput::Entry& objCopy ) {
		m_szValue = objCopy.m_szValue;

		InitValue( );

		return *this;
	} // Entry& operator =

	void CWgtInput::Entry::InitValue( ) {
		Glib::RefPtr< Gtk::EntryBuffer > buff = get_buffer( );
		if ( buff ) {
			buff->set_text( m_szValue );
		}
	} // void InitValue

	void CWgtInput::Entry::on_changed( ) {
		m_szValue = get_text( );
	} // void on_changed

	void CWgtInput::Entry::SetValue( const Glib::ustring& szValue ) {
		set_text( szValue );
	} // void SetValue

	CWgtInput::CWgtInput( ) :
	Gtk::EventBox( ),
	m_inpText( new CWgtInput::Entry( ) )
	{
		set_size_request( 200, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText, Gtk::PACK_EXPAND_WIDGET, 2 );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtInput::OnExpose ) );
	} // CWgtInput

	CWgtInput::CWgtInput( const Glib::ustring& szDefault, const Glib::ustring& szValue ) :
	Gtk::EventBox( ),
	m_szDefaultValue( szDefault ),
	m_inpText( new CWgtInput::Entry( szValue ) )
	{
		set_size_request( 200, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtInput::OnExpose ) );
	} // CWgtInput

	CWgtInput::CWgtInput( const CWgtInput& objCopy ) :
	Gtk::EventBox( ),
	m_szDefaultValue( objCopy.m_szDefaultValue ),
	m_inpText( new CWgtInput::Entry( objCopy.m_inpText->GetValue( ) ) )
	{
		set_size_request( 200, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtInput::OnExpose ) );
	} // CWgtInput

	CWgtInput::~CWgtInput( ) {
		udDELETE( m_inpText );
	} // ~CWgtInput

	CWgtInput& CWgtInput::operator =( const CWgtInput& objCopy ) {
		m_szDefaultValue = objCopy.m_szDefaultValue;

		*m_inpText = *( objCopy.m_inpText );

		return *this;
	} // CWgtInput& operator =

	bool CWgtInput::OnExpose( GdkEventExpose* evtExpose ) {
		Gtk::Allocation allocation = get_allocation( );
		int width = allocation.get_width( );
		int height = allocation.get_height( );
		Cairo::RefPtr< Cairo::Context > myContext = get_window( )->create_cairo_context( );

		myContext->save( );

		myContext->set_source_rgb( 0.7, 0.7, 0.7 );
		myContext->set_line_width( 1.0 );
		myContext->move_to( 0, 0 );
		myContext->line_to( width, 0 );
		myContext->line_to( width, height );
		myContext->line_to( 0, height );
		myContext->line_to( 0, 0 );
		myContext->stroke( );

		myContext->restore( );

		return false;
	} // bool on_expose_event
/*
	CWgtInput::CWgtInput( ) : Gtk::EventBox( ), m_inpText( new Gtk::Entry( ) ) {
		set_size_request( 200, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText, Gtk::PACK_EXPAND_WIDGET, 2 );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtInput::OnExpose ) );
	}

	CWgtInput::CWgtInput( const Glib::ustring& szDefault, const Glib::ustring& szValue ) :
		Gtk::EventBox( ),
		m_szDefault( szDefault ),
		m_szValue( szValue ),
		m_inpText( new Gtk::Entry( ) )
	{
		set_size_request( 200, -1 );
		m_inpText->set_has_frame( false );
		m_inpText->set_text( m_szValue );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtInput::OnExpose ) );
	}

	CWgtInput::CWgtInput( const CWgtInput& objCopy ) : Gtk::EventBox( ) {
	}

	CWgtInput::~CWgtInput( ) {
		udDELETE( m_inpText );
	}

	CWgtInput& CWgtInput::operator =( const CWgtInput& objCopy ) {
		return *this;
	} // CWgtInput& operator =

	void CWgtInput::Reset( ) {
		SetValue( m_szDefault );
	} // void Reset

	void CWgtInput::SetValue( const Glib::ustring& szValue ) {
		m_inpText->set_text( szValue );
		m_szValue = szValue;
	} // void SetValue

	void CWgtInput::SetDefaultValue( const Glib::ustring& szDefault ) {
		m_szDefault = szDefault;
	} // void SetDefaultValue

	CWgtInput& CWgtInput::LoadValue( ) {
		m_szValue = m_inpText->get_text( );
		return *this;
	} // CWgtInput& LoadValue

	bool CWgtInput::OnExpose( GdkEventExpose* evtExpose ) {
		Gtk::Allocation allocation = get_allocation( );
		int width = allocation.get_width( );
		int height = allocation.get_height( );
		Cairo::RefPtr< Cairo::Context > myContext = get_window( )->create_cairo_context( );

		myContext->save( );

		myContext->set_source_rgb( 0.7, 0.7, 0.7 );
		myContext->set_line_width( 1.0 );
		myContext->move_to( 0, 0 );
		myContext->line_to( width, 0 );
		myContext->line_to( width, height );
		myContext->line_to( 0, height );
		myContext->line_to( 0, 0 );
		myContext->stroke( );

		myContext->restore( );

		return false;
	} // bool on_expose_event
//*/
} // namesapace UdSDK
