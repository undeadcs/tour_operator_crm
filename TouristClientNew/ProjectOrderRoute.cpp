
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectOrderRouteList( ) {
		udPBase ret = GetProjectOrderRoute( );
		return ret;
	} // udPBase GetProjectOrderRouteList

	CHOrderRouteForm::CHOrderRouteForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "orderroute_form";

		m_pData->SetName( "orderroute" );
		m_pData->SetGetObject( GetProjectOrderRoute );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
	} // CHOrderRouteForm

	CHOrderRouteForm::CHOrderRouteForm( const CHOrderRouteForm& objCopy ) : udFormHandler( ) {
	} // CHOrderRouteForm

	CHOrderRouteForm::~CHOrderRouteForm( ) {
	} // ~CHOrderRouteForm

	CHOrderRouteForm& CHOrderRouteForm::operator =( const CHOrderRouteForm& objCopy ) {
		return *this;
	} // CHOrderRouteForm& operator =

	void CHOrderRouteForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// наименование
		m_tblInput.attach( m_lblName, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHOrderRouteForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
	} // void Clear

	void CHOrderRouteForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void Load

	udInt CHOrderRouteForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHOrderRouteForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szName = m_inpName.GetValue( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );

		//m_pData->Send( );
	} // void Send

	




	CHOrderRouteFormModal::CHOrderRouteFormModal( ) : CHOrderRouteForm( ) {
	} // CHOrderRouteFormModal

	CHOrderRouteFormModal::CHOrderRouteFormModal( const CHOrderRouteFormModal& objCopy ) : CHOrderRouteForm( ) {
	} // CHOrderRouteFormModal

	CHOrderRouteFormModal::~CHOrderRouteFormModal( ) {
	} // ~CHOrderRouteFormModal

	CHOrderRouteFormModal& CHOrderRouteFormModal::operator =( const CHOrderRouteFormModal& objCopy ) {
		return *this;
	} // CHOrderRouteFormModal& operator =

	void CHOrderRouteFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHOrderRouteFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHOrderRouteForm

	void CHOrderRouteFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
