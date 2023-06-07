
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourPriceList( ) {
		udPBase ret = GetProjectTourPrice( );
		return ret;
	} // udPBase GetProjectTourPriceList

	CHTourPriceForm::CHTourPriceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Цена за тур" ) ),
		m_lblAmount( Glib::locale_to_utf8( "Сумма" ) )
	{
		m_szName = "tourprice_form";

		m_pData->SetName( "tourprice" );
		m_pData->SetGetObject( GetProjectTourPrice );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblAmount.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblAmount.set_size_request( 250 );
	} // CHTourPriceForm

	CHTourPriceForm::CHTourPriceForm( const CHTourPriceForm& objCopy ) : udFormHandler( ) {
	} // CHTourPriceForm

	CHTourPriceForm::~CHTourPriceForm( ) {
	} // ~CHTourPriceForm

	CHTourPriceForm& CHTourPriceForm::operator =( const CHTourPriceForm& objCopy ) {
		return *this;
	} // CHTourPriceForm& operator =

	void CHTourPriceForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblAmount, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAmount, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTourPriceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpAmount.Reset( );
	} // void Clear

	void CHTourPriceForm::Load( ) {
	} // void Load

	udInt CHTourPriceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szAmount;

		szAmount = m_inpAmount.GetValue( );
		if ( szAmount.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpAmount.GetEntry( ) ) );
			return 1;
		}

		return 0;
	} // udInt Validate

	void CHTourPriceForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szAmount = m_inpAmount.GetValue( );

		obj->get( "amount" )->ini( ConvertString( szAmount.data( ), CP_UTF8 ) );

		//m_pData->Send( );
	} // void Send

	




	CHTourPriceFormModal::CHTourPriceFormModal( ) : CHTourPriceForm( ) {
	} // CHTourPriceFormModal

	CHTourPriceFormModal::CHTourPriceFormModal( const CHTourPriceFormModal& objCopy ) : CHTourPriceForm( ) {
	} // CHTourPriceFormModal

	CHTourPriceFormModal::~CHTourPriceFormModal( ) {
	} // ~CHTourPriceFormModal

	CHTourPriceFormModal& CHTourPriceFormModal::operator =( const CHTourPriceFormModal& objCopy ) {
		return *this;
	} // CHTourPriceFormModal& operator =

	void CHTourPriceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourPriceFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourPriceForm

	void CHTourPriceFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpAmount.SetValue( Glib::locale_to_utf8( obj->get( "amount" )->toString( ) ) );
	} // void LoadFrom

} // namespace UdSDK
