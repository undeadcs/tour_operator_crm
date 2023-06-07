
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppProgramList( ) {
		udPBase ret = GetProjectGCProgram( );
		return ret;
	} // udPBase GetProjectTourAppProgramList

	CHTourAppProgramForm::CHTourAppProgramForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Программа" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "tourappprogram_form";

		m_pData->SetName( "tourappprogram" );
		//m_pData->SetGetObject( GetProjectTourAppProgram );
		m_pData->SetGetObject( GetProjectGCProgram );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHTourAppProgramForm

	CHTourAppProgramForm::CHTourAppProgramForm( const CHTourAppProgramForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppProgramForm

	CHTourAppProgramForm::~CHTourAppProgramForm( ) {
	} // ~CHTourAppProgramForm

	CHTourAppProgramForm& CHTourAppProgramForm::operator =( const CHTourAppProgramForm& objCopy ) {
		return *this;
	} // CHTourAppProgramForm& operator =

	void CHTourAppProgramForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHTourAppProgramForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHTourAppProgramForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "text" )->toString( ) ) );
	} // void Load

	udInt CHTourAppProgramForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHTourAppProgramForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpName.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "text" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppProgramFormModal::CHTourAppProgramFormModal( ) : CHTourAppProgramForm( ) {
	} // CHTourAppProgramFormModal

	CHTourAppProgramFormModal::CHTourAppProgramFormModal( const CHTourAppProgramFormModal& objCopy ) : CHTourAppProgramForm( ) {
	} // CHTourAppProgramFormModal

	CHTourAppProgramFormModal::~CHTourAppProgramFormModal( ) {
	} // ~CHTourAppProgramFormModal

	CHTourAppProgramFormModal& CHTourAppProgramFormModal::operator =( const CHTourAppProgramFormModal& objCopy ) {
		return *this;
	} // CHTourAppProgramFormModal& operator =

	void CHTourAppProgramFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppProgramFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppProgramForm

	void CHTourAppProgramFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "text" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
