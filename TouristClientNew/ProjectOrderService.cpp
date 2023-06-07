
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectOrderServiceList( ) {
		udPBase ret = GetProjectOrderService( );
		ret->get( "type_" )->ini( wstring( L"" ) );
		ret->get( "city_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectOrderServiceList

	CHOrderServiceForm::CHOrderServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Услуга" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Дата \"с\"" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Дата \"по\"" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblCity( Glib::locale_to_utf8( "Город" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "orderservice_form";

		m_pData->SetName( "orderservice" );
		m_pData->SetGetObject( GetProjectOrderService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_size_request( 250 );

		udPFDEObject pFde = NULL;

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTService );
		pFde->SetMark( ctrlidType );
		m_pData->AddExtraData( pFde );
		m_inpType.SetDataObject( pFde );
		m_inpType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа услуги" ) );
		m_inpType.SetAttr( "name" );
		m_inpType.SetGetListHandler( GetHTServiceListPick );
		m_inpType.SetGetFormHandler( GetHTServiceForm );
		m_inpType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа услуги" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCity );
		pFde->SetMark( ctrlidCity );
		m_pData->AddExtraData( pFde );
		m_inpCity.SetDataObject( pFde );
		m_inpCity.SetWindowTitle( Glib::locale_to_utf8( "Выбор города" ) );
		m_inpCity.SetAttr( "name" );
		m_inpCity.SetGetListHandler( GetHCityListPick );
		m_inpCity.SetGetFormHandler( GetHCityForm );
		m_inpCity.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование города" ) );
	} // CHOrderServiceForm

	CHOrderServiceForm::CHOrderServiceForm( const CHOrderServiceForm& objCopy ) : udFormHandler( ) {
	} // CHOrderServiceForm

	CHOrderServiceForm::~CHOrderServiceForm( ) {
	} // ~CHOrderServiceForm

	CHOrderServiceForm& CHOrderServiceForm::operator =( const CHOrderServiceForm& objCopy ) {
		return *this;
	} // CHOrderServiceForm& operator =

	void CHOrderServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Дата "с"
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата "по"
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Город
		m_tblInput.attach( m_lblCity, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCity, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHOrderServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpComment.Reset( );
		m_inpType.SetTitle( "" );
		m_inpCity.SetTitle( "" );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
	} // void Clear

	void CHOrderServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
		m_inpType.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpCity.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity) )->GetObject( )->get( "name" )->toString( )
		) );
	} // void Load

	udInt CHOrderServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetButton( ) ) );
			return 1;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите город" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCity.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHOrderServiceForm::Send( ) {
		Glib::ustring	szComment;

		szComment = m_inpComment.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );
		obj->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "city" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity ) )->GetObject( )->get( "id" )->toUInt( )
		);

		//m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHOrderServiceFormModal::CHOrderServiceFormModal( ) : CHOrderServiceForm( ) {
	} // CHOrderServiceFormModal

	CHOrderServiceFormModal::CHOrderServiceFormModal( const CHOrderServiceFormModal& objCopy ) : CHOrderServiceForm( ) {
	} // CHOrderServiceFormModal

	CHOrderServiceFormModal::~CHOrderServiceFormModal( ) {
	} // ~CHOrderServiceFormModal

	CHOrderServiceFormModal& CHOrderServiceFormModal::operator =( const CHOrderServiceFormModal& objCopy ) {
		return *this;
	} // CHOrderServiceFormModal& operator =

	void CHOrderServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHOrderServiceFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHOrderServiceForm

	void CHOrderServiceFormModal::LoadFrom( udPBase from ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		udPBase	pType	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		udUInt	id		= pType->get( "id" )->toUInt( ),
				type	= obj->get( "type" )->toUInt( );
		
		if ( ( id == 0 ) && ( type > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "tservice", type, GetProjectTService );
			if ( pLoaded ) {
				pType->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		udPBase pCity = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity ) )->GetObject( );
		id = pCity->get( "id" )->toUInt( );
		type = obj->get( "city" )->toUInt( );
		if ( ( id == 0 ) && ( type > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "city", type, GetProjectCity );
			if ( pLoaded ) {
				pCity->apply( pLoaded, true, true );
				udDELETE( pLoaded );
			}
		}

		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
		m_inpType.SetTitle( Glib::locale_to_utf8(
			pType->get( "name" )->toString( )
		) );
		m_inpCity.SetTitle( Glib::locale_to_utf8(
			pCity->get( "name" )->toString( )
		) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
	} // void LoadFrom

} // namespace UdSDK
