
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppCoverList( ) {
		udPBase ret = GetProjectGCCover( );
		return ret;
	} // udPBase GetProjectTourAppCoverList

	CHTourAppCoverForm::CHTourAppCoverForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Средство размещения" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип страховки" ) ),
		m_lblName( Glib::locale_to_utf8( "Страховщик" ) )
	{
		m_szName = "tourappoccupancy_form";

		m_pData->SetName( "tourappoccupancy" );
		//m_pData->SetGetObject( GetProjectTourAppCover );
		m_pData->SetGetObject( GetProjectGCCover );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblType.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpType.set_size_request( 250 );
	} // CHTourAppCoverForm

	CHTourAppCoverForm::CHTourAppCoverForm( const CHTourAppCoverForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppCoverForm

	CHTourAppCoverForm::~CHTourAppCoverForm( ) {
	} // ~CHTourAppCoverForm

	CHTourAppCoverForm& CHTourAppCoverForm::operator =( const CHTourAppCoverForm& objCopy ) {
		return *this;
	} // CHTourAppCoverForm& operator =

	void CHTourAppCoverForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Тип страховки
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Страховщик
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

	void CHTourAppCoverForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpType.Reset( );
		m_inpName.Reset( );
	} // void Clear

	void CHTourAppCoverForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpType.SetValue( Glib::locale_to_utf8( obj->get( "type" )->toString( ) ) );
		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHTourAppCoverForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szType;

		szType = m_inpType.GetValue( );
		if ( szType.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetEntry( ) ) );
			return 1;
		}

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHTourAppCoverForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szType;

		szType = m_inpType.GetValue( );
		szName = m_inpName.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "type" )->ini( ConvertString( szType.data( ), CP_UTF8 ) );
		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppCoverFormModal::CHTourAppCoverFormModal( ) : CHTourAppCoverForm( ) {
	} // CHTourAppCoverFormModal

	CHTourAppCoverFormModal::CHTourAppCoverFormModal( const CHTourAppCoverFormModal& objCopy ) : CHTourAppCoverForm( ) {
	} // CHTourAppCoverFormModal

	CHTourAppCoverFormModal::~CHTourAppCoverFormModal( ) {
	} // ~CHTourAppCoverFormModal

	CHTourAppCoverFormModal& CHTourAppCoverFormModal::operator =( const CHTourAppCoverFormModal& objCopy ) {
		return *this;
	} // CHTourAppCoverFormModal& operator =

	void CHTourAppCoverFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppCoverFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppCoverForm

	void CHTourAppCoverFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpType.SetValue( Glib::locale_to_utf8( obj->get( "type" )->toString( ) ) );
		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
