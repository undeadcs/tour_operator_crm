
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppVisaList( ) {
		udPBase ret = GetProjectGCVisa( );
		return ret;
	} // udPBase GetProjectTourAppVisaList

	CHTourAppVisaForm::CHTourAppVisaForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Визовое оформление" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип услуги" ) )
	{
		m_szName = "tourappvisa_form";

		m_pData->SetName( "tourappvisa" );
		//m_pData->SetGetObject( GetProjectTourAppVisa );
		m_pData->SetGetObject( GetProjectGCVisa );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblType.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpType.set_size_request( 250 );
	} // CHTourAppVisaForm

	CHTourAppVisaForm::CHTourAppVisaForm( const CHTourAppVisaForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppVisaForm

	CHTourAppVisaForm::~CHTourAppVisaForm( ) {
	} // ~CHTourAppVisaForm

	CHTourAppVisaForm& CHTourAppVisaForm::operator =( const CHTourAppVisaForm& objCopy ) {
		return *this;
	} // CHTourAppVisaForm& operator =

	void CHTourAppVisaForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Тип услуги
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTourAppVisaForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpType.Reset( );
	} // void Clear

	void CHTourAppVisaForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpType.SetValue( Glib::locale_to_utf8( obj->get( "type" )->toString( ) ) );
	} // void Load

	udInt CHTourAppVisaForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szType;

		szType = m_inpType.GetValue( );
		if ( szType.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHTourAppVisaForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szType;

		szType = m_inpType.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "type" )->ini( ConvertString( szType.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppVisaFormModal::CHTourAppVisaFormModal( ) : CHTourAppVisaForm( ) {
	} // CHTourAppVisaFormModal

	CHTourAppVisaFormModal::CHTourAppVisaFormModal( const CHTourAppVisaFormModal& objCopy ) : CHTourAppVisaForm( ) {
	} // CHTourAppVisaFormModal

	CHTourAppVisaFormModal::~CHTourAppVisaFormModal( ) {
	} // ~CHTourAppVisaFormModal

	CHTourAppVisaFormModal& CHTourAppVisaFormModal::operator =( const CHTourAppVisaFormModal& objCopy ) {
		return *this;
	} // CHTourAppVisaFormModal& operator =

	void CHTourAppVisaFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppVisaFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppVisaForm

	void CHTourAppVisaFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpType.SetValue( Glib::locale_to_utf8( obj->get( "type" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
