
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppServiceList( ) {
		udPBase ret = GetProjectGCService( );
		return ret;
	} // udPBase GetProjectTourAppServiceList

	CHTourAppServiceForm::CHTourAppServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Другие услуги" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblChar( Glib::locale_to_utf8( "Характеристики" ) )
	{
		m_szName = "tourappservice_form";

		m_pData->SetName( "tourappservice" );
		//m_pData->SetGetObject( GetProjectTourAppService );
		m_pData->SetGetObject( GetProjectGCService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHTourAppServiceForm

	CHTourAppServiceForm::CHTourAppServiceForm( const CHTourAppServiceForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppServiceForm

	CHTourAppServiceForm::~CHTourAppServiceForm( ) {
	} // ~CHTourAppServiceForm

	CHTourAppServiceForm& CHTourAppServiceForm::operator =( const CHTourAppServiceForm& objCopy ) {
		return *this;
	} // CHTourAppServiceForm& operator =

	void CHTourAppServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Характеристики
		m_tblInput.attach( m_lblChar, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpChar, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTourAppServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpChar.Reset( );
	} // void Clear

	void CHTourAppServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpChar.SetValue( Glib::locale_to_utf8( obj->get( "char" )->toString( ) ) );
	} // void Load

	udInt CHTourAppServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szChar;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		szChar = m_inpChar.GetValue( );
		if ( szChar.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите характеристики" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpChar.GetEntry( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHTourAppServiceForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName,
						szChar;

		szName = m_inpName.GetValue( );
		szChar = m_inpChar.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "char" )->ini( ConvertString( szChar.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppServiceFormModal::CHTourAppServiceFormModal( ) : CHTourAppServiceForm( ) {
	} // CHTourAppServiceFormModal

	CHTourAppServiceFormModal::CHTourAppServiceFormModal( const CHTourAppServiceFormModal& objCopy ) : CHTourAppServiceForm( ) {
	} // CHTourAppServiceFormModal

	CHTourAppServiceFormModal::~CHTourAppServiceFormModal( ) {
	} // ~CHTourAppServiceFormModal

	CHTourAppServiceFormModal& CHTourAppServiceFormModal::operator =( const CHTourAppServiceFormModal& objCopy ) {
		return *this;
	} // CHTourAppServiceFormModal& operator =

	void CHTourAppServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppServiceForm

	void CHTourAppServiceFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpChar.SetValue( Glib::locale_to_utf8( obj->get( "char" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
