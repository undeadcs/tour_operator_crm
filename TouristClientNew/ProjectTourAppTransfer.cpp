
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppTransferList( ) {
		udPBase ret = GetProjectGCTransfer2( );
		return ret;
	} // udPBase GetProjectTourAppTransferList

	CHTourAppTransferForm::CHTourAppTransferForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Средство размещения" ) ),
		m_lblRoute( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) )
	{
		m_szName = "tourapptransfer_form";

		m_pData->SetName( "tourapptransfer" );
		//m_pData->SetGetObject( GetProjectTourAppTransfer );
		m_pData->SetGetObject( GetProjectGCTransfer2 );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblRoute.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpRoute.set_size_request( 250 );
	} // CHTourAppTransferForm

	CHTourAppTransferForm::CHTourAppTransferForm( const CHTourAppTransferForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppTransferForm

	CHTourAppTransferForm::~CHTourAppTransferForm( ) {
	} // ~CHTourAppTransferForm

	CHTourAppTransferForm& CHTourAppTransferForm::operator =( const CHTourAppTransferForm& objCopy ) {
		return *this;
	} // CHTourAppTransferForm& operator =

	void CHTourAppTransferForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Маршрут
		m_tblInput.attach( m_lblRoute, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpRoute, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
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

	void CHTourAppTransferForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpRoute.Reset( );
		m_inpType.Reset( );
	} // void Clear

	void CHTourAppTransferForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpRoute.SetValue( Glib::locale_to_utf8( obj->get( "route" )->toString( ) ) );
		m_inpType.SetValue( Glib::locale_to_utf8( obj->get( "type" )->toString( ) ) );
	} // void Load

	udInt CHTourAppTransferForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szRoute,
						szType;

		szRoute = m_inpRoute.GetValue( );
		if ( szRoute.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите маршрут" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpRoute.GetEntry( ) ) );
			return 1;
		}

		szType = m_inpType.GetValue( );
		if ( szType.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetEntry( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHTourAppTransferForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szRoute,
						szType;

		szRoute = m_inpRoute.GetValue( );
		szType = m_inpType.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "route" )->ini( ConvertString( szRoute.data( ), CP_UTF8 ) );
		obj->get( "type" )->ini( ConvertString( szType.data( ), CP_UTF8 ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppTransferFormModal::CHTourAppTransferFormModal( ) : CHTourAppTransferForm( ) {
	} // CHTourAppTransferFormModal

	CHTourAppTransferFormModal::CHTourAppTransferFormModal( const CHTourAppTransferFormModal& objCopy ) : CHTourAppTransferForm( ) {
	} // CHTourAppTransferFormModal

	CHTourAppTransferFormModal::~CHTourAppTransferFormModal( ) {
	} // ~CHTourAppTransferFormModal

	CHTourAppTransferFormModal& CHTourAppTransferFormModal::operator =( const CHTourAppTransferFormModal& objCopy ) {
		return *this;
	} // CHTourAppTransferFormModal& operator =

	void CHTourAppTransferFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppTransferFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppTransferForm

	void CHTourAppTransferFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpRoute.SetValue( Glib::locale_to_utf8( obj->get( "route" )->toString( ) ) );
		m_inpType.SetValue( Glib::locale_to_utf8( obj->get( "type" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
