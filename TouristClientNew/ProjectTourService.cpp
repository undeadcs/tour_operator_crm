
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourServiceList( ) {
		udPBase ret = GetProjectTourService( );
		ret->get( "service_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourServiceList

	CHTourServiceForm::CHTourServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Услуга" ) ),
		m_lblService( Glib::locale_to_utf8( "Услуга" ) ),
		m_lblNum( Glib::locale_to_utf8( "Количество" ) ),
		m_lblDate1( Glib::locale_to_utf8( "С" ) ),
		m_lblDate2( Glib::locale_to_utf8( "По" ) ),
		m_lblReady( Glib::locale_to_utf8( "Готовность" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) ),
		m_inpReady( 0, 0 )
	{
		m_szName = "tourservice_form";

		m_pData->SetName( "tourservice" );
		m_pData->SetGetObject( GetProjectTourService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblService.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblService.set_size_request( 250 );

		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectService );
		pFde->SetMark( ctrlidService );
		m_pData->AddExtraData( pFde );
		m_inpService.SetDataObject( pFde );
		m_inpService.SetWindowTitle( Glib::locale_to_utf8( "Выбор услуги" ) );
		m_inpService.SetAttr( "name_" );
		m_inpService.SetGetListHandler( GetHServiceListPick );
		m_inpService.SetGetFormHandler( GetHServiceForm );
		m_inpService.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование услуги" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpReady.AddRow( 0, Glib::locale_to_utf8( "Не готова" ) );
		m_inpReady.AddRow( 1, Glib::locale_to_utf8( "Готова" ) );
		m_inpReady.Reset( );
		/*m_pColModelReady = new CHTourServiceForm::ColModelReady;
		m_objReadyModel = Gtk::ListStore::create( *m_pColModelReady );

		Gtk::TreeModel::Row row;

		row = *( m_objReadyModel->append( ) );
		row[ m_pColModelReady->m_col_code ] = 0;
		row[ m_pColModelReady->m_col_name ] = Glib::locale_to_utf8( "Не готова" );

		row = *( m_objReadyModel->append( ) );
		row[ m_pColModelReady->m_col_code ] = 1;
		row[ m_pColModelReady->m_col_name ] = Glib::locale_to_utf8( "Готова" );

		m_inpReady.set_model( m_objReadyModel );

		m_inpReady.pack_start( m_pColModelReady->m_col_name );
		m_inpReady.set_active( 0 );*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTourServiceForm

	CHTourServiceForm::CHTourServiceForm( const CHTourServiceForm& objCopy ) : udFormHandler( ) {
	} // CHTourServiceForm

	CHTourServiceForm::~CHTourServiceForm( ) {
	} // ~CHTourServiceForm

	CHTourServiceForm& CHTourServiceForm::operator =( const CHTourServiceForm& objCopy ) {
		return *this;
	} // CHTourServiceForm& operator =

	void CHTourServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Услуга
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Количество
		m_tblInput.attach( m_lblNum, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpNum, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата "с"
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата "по"
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Готовность
		m_tblInput.attach( m_lblReady, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpReady, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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

	void CHTourServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpNum.Reset( );
		m_inpService.SetTitle( "" );
		//m_inpReady.set_active( 0 );
		m_inpReady.Reset( );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
	} // void Clear

	void CHTourServiceForm::Load( ) {
	} // void Load

	udInt CHTourServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szNum;

		szNum = m_inpNum.GetValue( );
		if ( szNum.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите количество" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpNum.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidService ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите услугу" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpService.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHTourServiceForm::Send( ) {
		m_pData->MakeObject( );

		udUInt ready = m_inpReady.LoadValue( ).GetValue( );

		udPBase obj = m_pData->GetData( );
		Glib::ustring szNum = m_inpNum.GetValue( );

		obj->get( "num" )->ini( ConvertString( szNum.data( ), CP_UTF8 ) );
		obj->get( "service" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidService ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );
		obj->get( "ready" )->ini( ready );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourServiceFormModal::CHTourServiceFormModal( ) : CHTourServiceForm( ) {
	} // CHTourServiceFormModal

	CHTourServiceFormModal::CHTourServiceFormModal( const CHTourServiceFormModal& objCopy ) : CHTourServiceForm( ) {
	} // CHTourServiceFormModal

	CHTourServiceFormModal::~CHTourServiceFormModal( ) {
	} // ~CHTourServiceFormModal

	CHTourServiceFormModal& CHTourServiceFormModal::operator =( const CHTourServiceFormModal& objCopy ) {
		return *this;
	} // CHTourServiceFormModal& operator =

	void CHTourServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourServiceFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourServiceForm

	void CHTourServiceFormModal::LoadFrom( udPBase from ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->apply( from, true, true );

		udPBase	pService	= ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidService ) )->GetObject( );
		udUInt	id			= pService->get( "id" )->toUInt( ),
				key			= obj->get( "service" )->toUInt( );
		
		if ( ( id == 0 ) && ( key > 0 ) ) {
			udPBase pLoaded = udDataAccess::LoadById( "service", key, GetProjectService );
			if ( pLoaded ) {
				pService->apply( pLoaded, true, true );

				udUInt idName = pLoaded->get( "name" )->toUInt( );
				if ( idName > 0 ) {
					udPBase pName = udDataAccess::LoadById( "nameservice", idName, GetProjectNameService );
					if ( pName ) {
						pService->get( "name_" )->ini( pName->get( "name" )->toWString( ) );
						udDELETE( pName );
					}
				}
				
				udDELETE( pLoaded );
			}
		}

		m_inpService.SetTitle( Glib::locale_to_utf8(
			pService->get( "name_" )->toString( )
		) );

		m_inpNum.SetValue( Glib::locale_to_utf8( obj->get( "num" )->toString( ) ) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );

		if ( obj->get( "ready" )->toUInt( ) ) {
			m_inpReady.set_active( obj->get( "ready" )->toUInt( ) );
		}
	} // void LoadFrom

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace UdSDK
