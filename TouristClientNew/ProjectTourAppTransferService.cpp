
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourAppTransferServiceList( ) {
		udPBase ret = GetProjectGCTransfer( );
		return ret;
	} // udPBase GetProjectTourAppTransferServiceList

	CHTourAppTransferServiceForm::CHTourAppTransferServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Услуга" ) ),
		m_lblFio( Glib::locale_to_utf8( "ФИО туриста" ) ),
		m_lblTrans( Glib::locale_to_utf8( "Перевозчик" ) ),
		m_lblLevel( Glib::locale_to_utf8( "Уровень сервиса" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Дата убытия" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Дата прибытия" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "tourappservice_form";

		m_pData->SetName( "tourappservice" );
		//m_pData->SetGetObject( GetProjectTourAppTransferService );
		m_pData->SetGetObject( GetProjectGCTransfer );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblFio.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpFio.set_size_request( 250 );
	} // CHTourAppTransferServiceForm

	CHTourAppTransferServiceForm::CHTourAppTransferServiceForm( const CHTourAppTransferServiceForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppTransferServiceForm

	CHTourAppTransferServiceForm::~CHTourAppTransferServiceForm( ) {
	} // ~CHTourAppTransferServiceForm

	CHTourAppTransferServiceForm& CHTourAppTransferServiceForm::operator =( const CHTourAppTransferServiceForm& objCopy ) {
		return *this;
	} // CHTourAppTransferServiceForm& operator =

	void CHTourAppTransferServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// ФИО туриста
		m_tblInput.attach( m_lblFio, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFio, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Перевозчик
		m_tblInput.attach( m_lblTrans, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpTrans, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Уровень сервиса
		m_tblInput.attach( m_lblLevel, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpLevel, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата убытия
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата прибытия
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTourAppTransferServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpFio.Reset( );
		m_inpTrans.Reset( );
		m_inpLevel.Reset( );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
	} // void Clear

	void CHTourAppTransferServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpTrans.SetValue( Glib::locale_to_utf8( obj->get( "trans" )->toString( ) ) );
		m_inpLevel.SetValue( Glib::locale_to_utf8( obj->get( "level" )->toString( ) ) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
	} // void Load

	udInt CHTourAppTransferServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szFio,
						szTrans,
						szLevel;

		szFio = m_inpFio.GetValue( );
		if ( szFio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFio.GetEntry( ) ) );
			return 1;
		}

		szTrans = m_inpTrans.GetValue( );
		if ( szTrans.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите перевозчика" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpTrans.GetEntry( ) ) );
			return 2;
		}

		szLevel = m_inpLevel.GetValue( );
		if ( szLevel.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите уровень сервиса" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpLevel.GetEntry( ) ) );
			return 3;
		}

		return 0;
	} // udInt Validate

	void CHTourAppTransferServiceForm::Send( ) {
		m_pData->MakeObject( );

		Glib::ustring	szFio,
						szTrans,
						szLevel;

		szFio = m_inpFio.GetValue( );
		szTrans = m_inpTrans.GetValue( );
		szLevel = m_inpLevel.GetValue( );

		udPBase obj = m_pData->GetData( );

		obj->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		obj->get( "trans" )->ini( ConvertString( szTrans.data( ), CP_UTF8 ) );
		obj->get( "level" )->ini( ConvertString( szLevel.data( ), CP_UTF8 ) );
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppTransferServiceFormModal::CHTourAppTransferServiceFormModal( ) : CHTourAppTransferServiceForm( ) {
	} // CHTourAppTransferServiceFormModal

	CHTourAppTransferServiceFormModal::CHTourAppTransferServiceFormModal( const CHTourAppTransferServiceFormModal& objCopy ) : CHTourAppTransferServiceForm( ) {
	} // CHTourAppTransferServiceFormModal

	CHTourAppTransferServiceFormModal::~CHTourAppTransferServiceFormModal( ) {
	} // ~CHTourAppTransferServiceFormModal

	CHTourAppTransferServiceFormModal& CHTourAppTransferServiceFormModal::operator =( const CHTourAppTransferServiceFormModal& objCopy ) {
		return *this;
	} // CHTourAppTransferServiceFormModal& operator =

	void CHTourAppTransferServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourAppTransferServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourAppTransferServiceForm

	void CHTourAppTransferServiceFormModal::LoadFrom( udPBase from ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpTrans.SetValue( Glib::locale_to_utf8( obj->get( "trans" )->toString( ) ) );
		m_inpLevel.SetValue( Glib::locale_to_utf8( obj->get( "level" )->toString( ) ) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
	} // void LoadFrom

} // namespace UdSDK
