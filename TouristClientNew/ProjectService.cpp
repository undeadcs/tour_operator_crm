
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void CHService::GetServicesById( const map< udUInt, udUInt >& ids, udArrBase& arrResult ) {
		if ( ids.empty( ) ) {
			return;
		}
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( "service" );
			data.add( ( udInt ) opidGet2 );
			data.add( ( udUInt ) 1 );
			data.add( ( udUInt ) ids.size( ) );
			data.add( string( "id" ) );
			data.add( udDAList::ordDesc );
			// добавляем набор ID клиентов
			data.add( ids.size( ) );
			for( map< udUInt, udUInt >::const_iterator i = ids.begin( ); i != ids.end( ); ++i ) {
				data.add( i->first );
			}

			udPMorph filter = new udMorph;
			filter->get( "name" )->ini( wstring( L"" ) );
			filter->get( "name1" )->ini( ( udUInt ) 0 );
			filter->get( "type" )->ini( ( udUInt ) 0 );
			filter->get( "partner" )->ini( ( udUInt ) 0 );
			filter->dump( data );
			udDELETE( filter );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udType total( ( udUInt ) 0 );
				total.read( sock );
				udDataAccess::Read( sock, arrResult, GetProjectServiceList );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
	} // void GetServicesById

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CWgtServicePick::CWgtServicePick( ) : udWgtListPick( ) {
	} // CWgtEmployerPick

	CWgtServicePick::CWgtServicePick( const CWgtServicePick& objCopy ) : udWgtListPick( ) {
	} // CWgtEmployerPick

	CWgtServicePick::~CWgtServicePick( ) {
	} // ~CWgtServicePeriodPick

	CWgtServicePick& CWgtServicePick::operator =( const CWgtServicePick& objCopy ) {
		return *this;
	} // CWgtEmployerPick& operator =

	void CWgtServicePick::OnButtonClick( ) {
		udPListHandlerPick handler = GetHServiceListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), m_szWindowTitle );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( m_szWindowTitle );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj && m_pFde ) {
			m_pFde->Clear( );
			m_pFde->Make( false );
			obj->apply( m_pFde->GetObject( ) );
			//m_pFde->GetObject( )->apply( obj, true, true );
			
			if ( !m_szAttr.empty( ) ) {
				m_lblTitle.set_text( Glib::locale_to_utf8( obj->get( m_szAttr )->toString( ) ) );
			}
		}

		udDELETE( handler );
	} // void OnButtonClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPListHandlerPick GetHServiceListPick( ) {
		return new CHServicePick;
	} // udPListHandlerPick GetHServiceListPick

	udPBase GetProjectServiceList( ) {
		udPBase ret = GetProjectService( );
		ret->get( "name_" )->ini( wstring( L"" ) );
		ret->get( "type_" )->ini( wstring( L"" ) );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		ret->get( "period_" )->ini( wstring( L"" ) );
		ret->get( "price_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectServiceList

	CHService::CHService( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltName1( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblFltPartner( Glib::locale_to_utf8( "Партнер" ) )
	{
		m_szName = "service";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать услугу" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "name1" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "partner" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHService::ColModelService( );

		m_pData->SetName( "service" );
		m_pData->SetGetObject( GetProjectServiceList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectNameService );
		pFde->SetMark( fltName );
		m_pData->AddExtraData( pFde );
		m_inpFltName1.SetDataObject( pFde );
		m_inpFltName1.SetWindowTitle( Glib::locale_to_utf8( "Выбор наименования" ) );
		m_inpFltName1.SetAttr( "name" );
		m_inpFltName1.SetGetListHandler( GetHNameServiceListPick );
		m_inpFltName1.SetGetFormHandler( GetHNameServiceForm );
		m_inpFltName1.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование наименования услуги" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTService );
		pFde->SetMark( fltType );
		m_pData->AddExtraData( pFde );
		m_inpFltType.SetDataObject( pFde );
		m_inpFltType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа" ) );
		m_inpFltType.SetAttr( "name" );
		m_inpFltType.SetGetListHandler( GetHTServiceListPick );
		m_inpFltType.SetGetFormHandler( GetHTServiceForm );
		m_inpFltType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа услуги" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectPartner );
		pFde->SetMark( fltPartner );
		m_pData->AddExtraData( pFde );
		m_inpFltPartner.SetDataObject( pFde );
		m_inpFltPartner.SetWindowTitle( Glib::locale_to_utf8( "Выбор партнера" ) );
		m_inpFltPartner.SetAttr( "name" );
		m_inpFltPartner.SetGetListHandler( GetHPartnerListPick );
		m_inpFltPartner.SetGetFormHandler( GetHPartnerForm );
		m_inpFltPartner.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование партнера" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Наименование
		m_tblFilter.attach( m_lblFltName1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Тип
		m_tblFilter.attach( m_lblFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Партнер
		m_tblFilter.attach( m_lblFltPartner, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltPartner, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHService

	CHService::CHService( const CHService& objCopy ) : udListHandler( ) {
		m_szName = "service";
	} // CHService

	CHService::~CHService( ) {
	} // ~CService

	CHService& CHService::operator =( const CHService& objCopy ) {
		return *this;
	} // CHService& operator =

	void CHService::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тип" ), m_pColModel->m_col_type );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Период" ), m_pColModel->m_col_period );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Партнер" ), m_pColModel->m_col_partner );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Цена" ), m_pColModel->m_col_price );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHService::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		Gtk::TreeModel::iterator itRow;
		Gtk::TreePath objPath;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			itRow = m_objTreeModel->append( );
			objPath = m_objTreeModel->get_path( itRow );
			m_mapObject[ objPath.to_string( ) ] = obj;
			row = *itRow;
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name_" )->toString( ) );
			row[ m_pColModel->m_col_type ] = Glib::locale_to_utf8( obj->get( "type_" )->toString( ) );
			row[ m_pColModel->m_col_period ] = Glib::locale_to_utf8( obj->get( "period_" )->toString( ) );
			row[ m_pColModel->m_col_partner ] = Glib::locale_to_utf8( obj->get( "partner_" )->toString( ) );
			row[ m_pColModel->m_col_price ] = Glib::locale_to_utf8( obj->get( "price_" )->toString( ) );
		}
	} // void Load

	void CHService::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltName1.SetTitle( "" );
		m_inpFltType.SetTitle( "" );
		m_inpFltPartner.SetTitle( "" );
	} // void FilterUiClear

	void CHService::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "name1" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltName ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHServiceModeless::CHServiceModeless( ) :
		udListHandlerModeless( ),
		CHService( )
	{
	} // CHServiceModeless

	CHServiceModeless::CHServiceModeless( const CHServiceModeless& objCopy ) : CHService( ) {
	} // CHServiceModeless

	CHServiceModeless::~CHServiceModeless( ) {
	} // ~CHServiceModeless

	CHServiceModeless& CHServiceModeless::operator =( const CHServiceModeless& objCopy ) {
		return *this;
	} // CHServiceModeless& operator =

	void CHServiceModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHService::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHServiceModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "service_form" );
	} // void OnButtonForm

	void CHServiceModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "service_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "service_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHServicePick::CHServicePick( ) : CHService( ) {
	} // CHServicePick

	CHServicePick::CHServicePick( const CHServicePick& objCopy ) : CHService( ) {
	} // CHServicePick

	CHServicePick::~CHServicePick( ) {
	} // ~CHServicePick

	CHServicePick& CHServicePick::operator =( const CHServicePick& objCopy ) {
		return *this;
	} // CHServicePick& operator =

	void CHServicePick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHService::Init( );
	} // void CHServicePick

	void CHServicePick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHServiceFormModal handler = new udHServiceFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание услуги" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание услуги" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHServicePick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectService( );

				itObj->second->apply( m_pPickedObject );
			}

			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите страну" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( Glib::locale_to_utf8(
				"Щелкните по строке списка и нажмите кнопку \"ОК\""
			) );
			dialog.run( );
		}
	} // void OnButtonOk

	void CHServicePick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHServicePick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHServiceFormModal handler = new udHServiceFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование услуги" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование услуги" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/
			udDELETE( handler );

			Clear( );
			Load( );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPFormHandler GetHServiceForm( ) {
		return new CHServiceFormModal;
	} // udPFormHandler GetHServiceForm

	CHServiceForm::CHServiceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Услуга" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblCountry( Glib::locale_to_utf8( "Страна" ) ),
		m_lblCity( Glib::locale_to_utf8( "Город" ) ),
		m_lblPartner( Glib::locale_to_utf8( "Партнер" ) ),
		m_lblMunit( Glib::locale_to_utf8( "Единица измерения" ) ),
		m_lblPeriod( Glib::locale_to_utf8( "Периоды" ) ),
		m_lblPrice( Glib::locale_to_utf8( "Цены" ) )
	{
		m_szName = "service_form";

		m_pData->SetName( "service" );
		m_pData->SetGetObject( GetProjectService );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_lblName.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblType.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCountry.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCity.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblPartner.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblMunit.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblPeriod.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblPrice.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectNameService );
		pFde->SetMark( ctrlidName );
		m_pData->AddExtraData( pFde );
		m_inpName.SetDataObject( pFde );
		m_inpName.SetWindowTitle( Glib::locale_to_utf8( "Выбор наименования услуги" ) );
		m_inpName.SetAttr( "name" );
		m_inpName.SetGetListHandler( GetHNameServiceListPick );
		m_inpName.SetGetFormHandler( GetHNameServiceForm );
		m_inpName.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование наименования услуги" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTService );
		pFde->SetMark( ctrlidType );
		m_pData->AddExtraData( pFde );
		m_inpType.SetDataObject( pFde );
		m_inpType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа услуги" ) );
		m_inpType.SetAttr( "name" );
		m_inpType.SetGetListHandler( GetHTServiceListPick );
		m_inpType.SetGetFormHandler( GetHTServiceForm );
		m_inpType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа услуги" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCountry );
		pFde->SetMark( ctrlidCountry );
		m_pData->AddExtraData( pFde );
		m_inpCountry.SetDataObject( pFde );
		m_inpCountry.SetWindowTitle( Glib::locale_to_utf8( "Выбор страны" ) );
		m_inpCountry.SetAttr( "name" );
		m_inpCountry.SetGetListHandler( GetHCountryListPick );
		m_inpCountry.SetGetFormHandler( GetHCountryForm );
		m_inpCountry.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование страны" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCity );
		pFde->SetMark( ctrlidCity );
		m_pData->AddExtraData( pFde );
		m_inpCity.SetDataObject( pFde );
		m_inpCity.SetWindowTitle( Glib::locale_to_utf8( "Выбор города" ) );
		m_inpCity.SetAttr( "name" );
		m_inpCity.SetGetListHandler( GetHCityListPick );
		m_inpCity.SetGetFormHandler( GetHCityForm );
		m_inpCity.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование города" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectPartner );
		pFde->SetMark( ctrlidPartner );
		m_pData->AddExtraData( pFde );
		m_inpPartner.SetDataObject( pFde );
		m_inpPartner.SetWindowTitle( Glib::locale_to_utf8( "Выбор партнера" ) );
		m_inpPartner.SetAttr( "name" );
		m_inpPartner.SetGetListHandler( GetHPartnerListPick );
		m_inpPartner.SetGetFormHandler( GetHPartnerForm );
		m_inpPartner.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование партнера" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectMUService );
		pFde->SetMark( ctrlidMunit );
		m_pData->AddExtraData( pFde );
		m_inpMunit.SetDataObject( pFde );
		m_inpMunit.SetWindowTitle( Glib::locale_to_utf8( "Выбор единицы измерения услуги" ) );
		m_inpMunit.SetAttr( "name" );
		m_inpMunit.SetGetListHandler( GetHMUServiceListPick );
		m_inpMunit.SetGetFormHandler( GetHMUServiceForm );
		m_inpMunit.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование единицы измерения услуги" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEList pFdeList = NULL;
		Gtk::TreeView* pTree = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// периоды
		m_inpPeriod.set_size_request( 400, 200 );
		m_pModelFormListPeriod = new ColModelFormListPeriod;
		m_objPeriodModel = Gtk::ListStore::create( *m_pModelFormListPeriod );
		
		pTree = m_inpPeriod.GetTreeView( );
		pTree->set_model( m_objPeriodModel );
		pTree->append_column( Glib::locale_to_utf8( "С" ), m_pModelFormListPeriod->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "По" ), m_pModelFormListPeriod->m_col_date2 );
		m_inpPeriod.SetColumnDefaults( );

		m_btnPeriodEdit = m_inpPeriod.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnPeriodEdit->set_sensitive( false );
		m_btnPeriodEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPeriod_Edit ) );

		m_btnPeriodDel = m_inpPeriod.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnPeriodDel->set_sensitive( false );
		m_btnPeriodDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPeriod_Del ) );

		m_btnPeriodAdd = m_inpPeriod.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnPeriodAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPeriod_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPeriod_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectServicePeriodList );
		pFdeList->SetCallbackSend( GetProjectServicePeriod );
		pFdeList->SetMark( ctrlidPeriod );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// цены
		m_inpPrice.set_size_request( 400, 200 );
		m_pModelFormListPrice = new ColModelFormListPrice;
		m_objPriceModel = Gtk::ListStore::create( *m_pModelFormListPrice );
		
		pTree = m_inpPrice.GetTreeView( );
		pTree->set_model( m_objPriceModel );
		pTree->append_column( Glib::locale_to_utf8( "С" ), m_pModelFormListPrice->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "По" ), m_pModelFormListPrice->m_col_date2 );
		pTree->append_column( Glib::locale_to_utf8( "Количество человек" ), m_pModelFormListPrice->m_col_num );
		pTree->append_column( Glib::locale_to_utf8( "Цена" ), m_pModelFormListPrice->m_col_amount );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListPrice->m_col_currency );
		pTree->append_column( Glib::locale_to_utf8( "Цена \"Р.\"" ), m_pModelFormListPrice->m_col_amount_recommend );
		pTree->append_column( Glib::locale_to_utf8( "Валюта \"Р.\"" ), m_pModelFormListPrice->m_col_currency_recommend );
		m_inpPrice.SetColumnDefaults( );

		m_btnPriceEdit = m_inpPrice.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnPriceEdit->set_sensitive( false );
		m_btnPriceEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPrice_Edit ) );

		m_btnPriceDel = m_inpPrice.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnPriceDel->set_sensitive( false );
		m_btnPriceDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPrice_Del ) );

		m_btnPriceAdd = m_inpPrice.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnPriceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPrice_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHServiceForm::OnFormListPrice_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectServicePriceList );
		//pFdeList->SetCallbackSend( GetProjectServicePrice );
		pFdeList->SetCallbackSend( GetProjectPriceTableItem );
		pFdeList->SetMark( ctrlidPrice );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHServiceForm

	CHServiceForm::CHServiceForm( const CHServiceForm& objCopy ) : udFormHandler( ) {
	} // CHServiceForm

	CHServiceForm::~CHServiceForm( ) {
	} // ~CHServiceForm

	CHServiceForm& CHServiceForm::operator =( const CHServiceForm& objCopy ) {
		return *this;
	} // CHServiceForm& operator =

	void CHServiceForm::Init( ) {
		udInt l = 0, r = 1;
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Страна
		m_tblInput.attach( m_lblCountry, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCountry, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Город
		m_tblInput.attach( m_lblCity, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCity, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Партнер
		m_tblInput.attach( m_lblPartner, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPartner, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Единица измерения
		m_tblInput.attach( m_lblMunit, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpMunit, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Периоды
		m_tblInput.attach( m_lblPeriod, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPeriod, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Цены
		m_tblInput.attach( m_lblPrice, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPrice, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHServiceForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.SetTitle( "" );
		m_inpType.SetTitle( "" );
		m_inpCountry.SetTitle( "" );
		m_inpCity.SetTitle( "" );
		m_inpPartner.SetTitle( "" );
		m_inpMunit.SetTitle( "" );
		m_objPeriodModel->clear( );
		m_objPriceModel->clear( );
	} // void Clear

	void CHServiceForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidName ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpType.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpCountry.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCountry ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpCity.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpPartner.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpMunit.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidMunit ) )->GetObject( )->get( "name" )->toString( )
		) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		map< udUInt, udPBase > mapPeriod;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::iterator itRow;
			Gtk::TreeModel::Row row;
			Gtk::TreePath objPath;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				itRow = m_objPeriodModel->append( );
				objPath = m_objPeriodModel->get_path( itRow );
				row = *itRow;
				row[ m_pModelFormListPeriod->m_col_date1 ] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListPeriod->m_col_date2 ] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

				m_mapObjectToRowPeriod[ objPath.to_string( ) ] = add;

				mapPeriod[ add->get( "index" )->toUInt( ) ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPrice ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::iterator itRow;
			Gtk::TreeModel::Row row;
			Gtk::TreePath objPath;
			map< udUInt, udPBase >::iterator itFind;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				itRow = m_objPriceModel->append( );
				objPath = m_objPriceModel->get_path( itRow );
				row = *itRow;
				//row[ m_pModelFormListPrice->m_col_date1 ] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				//row[ m_pModelFormListPrice->m_col_date2 ] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListPrice->m_col_num ] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
				row[ m_pModelFormListPrice->m_col_amount ] = Glib::locale_to_utf8( add->get( "amount" )->toString( ) );
				row[ m_pModelFormListPrice->m_col_currency ] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				row[ m_pModelFormListPrice->m_col_amount_recommend ] = Glib::locale_to_utf8( add->get( "amount_recommend" )->toString( ) );
				row[ m_pModelFormListPrice->m_col_currency_recommend ] = Glib::locale_to_utf8( add->get( "currency_recommend_" )->toString( ) );

				m_mapObjectToRowPrice[ objPath.to_string( ) ] = add;

				itFind = mapPeriod.find( add->get( "period" )->toUInt( ) );
				if ( itFind != mapPeriod.end( ) ) {
					row[ m_pModelFormListPrice->m_col_date1 ] = Glib::locale_to_utf8( itFind->second->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListPrice->m_col_date2 ] = Glib::locale_to_utf8( itFind->second->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHServiceForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidName ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), "Введите наименование" );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), "Введите тип" );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCountry ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), "Введите страну" );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCountry.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), "Введите город" );
			g_pApp->GetWndWork( )->set_focus( *( m_inpCity.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), "Введите партнера" );
			g_pApp->GetWndWork( )->set_focus( *( m_inpPartner.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidMunit ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), "Введите единицу измерения" );
			g_pApp->GetWndWork( )->set_focus( *( m_inpMunit.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHServiceForm::Send( ) {
		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidName ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "country" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCountry ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "city" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidCity ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "munit" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidMunit ) )->GetObject( )->get( "id" )->toUInt( )
		);

		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHServiceForm::OnFormListPeriod_Select( ) {
		Gtk::TreeModel::iterator i = m_inpPeriod.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnPeriodEdit->set_sensitive( true );
			m_btnPeriodDel->set_sensitive( true );
		} else {
			m_btnPeriodEdit->set_sensitive( false );
			m_btnPeriodDel->set_sensitive( false );
		}
	} // void OnFormListPeriod_Select

	void CHServiceForm::OnFormListPeriod_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHServicePeriodFormModal handler = new udHServicePeriodFormModal;
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание периода" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание периода" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectServicePeriod;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			arrObject->push_back( add );

			add->get( "index" )->ini( GetPeriodLastIndex( ) + 1 );

			Gtk::TreeModel::iterator i = m_objPeriodModel->append( );
			Gtk::TreeModel::Row row = *i;
			Gtk::TreePath objPath = m_objPeriodModel->get_path( i );

			row[ m_pModelFormListPeriod->m_col_date1 ] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListPeriod->m_col_date2 ] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

			m_mapObjectToRowPeriod[ objPath.to_string( ) ] = add;
		}

		udDELETE( handler );
	} // void OnFormListPeriod_Add

	void CHServiceForm::OnFormListPeriod_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpPeriod.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreePath objPath = m_objPeriodModel->get_path( i );
			map< Glib::ustring, udPBase >::iterator j = m_mapObjectToRowPeriod.find( objPath.to_string( ) );
			if ( j != m_mapObjectToRowPeriod.end( ) ) {
				// j->second - наш объект редактирования
				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
				udPHServicePeriodFormModal handler = new udHServicePeriodFormModal;
				handler->Init( );
				handler->LoadFrom( j->second );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование периода" ) );

				/*Gtk::Window wnd;
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование периода" ) );
				wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				wnd.show_all( );
				Gtk::Main::run( wnd );*/

				udPDAForm pDa = handler->GetData( );
				udPBase obj = pDa->GetData( );
				if ( obj ) {
					Gtk::TreeModel::Row row = *i;

					j->second->apply( obj, true, true );

					row[ m_pModelFormListPeriod->m_col_date1 ] = Glib::locale_to_utf8( j->second->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListPeriod->m_col_date2 ] = Glib::locale_to_utf8( j->second->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				}

				udDELETE( handler );
			}
		}
	} // void OnFormListPeriod_Edit

	void CHServiceForm::OnFormListPeriod_Del( ) {
		Gtk::TreeModel::iterator i = m_inpPeriod.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreePath objPath = m_objPeriodModel->get_path( i );
			map< Glib::ustring, udPBase >::iterator j = m_mapObjectToRowPeriod.find( objPath.to_string( ) );
			if ( j != m_mapObjectToRowPeriod.end( ) ) {
				udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );
				udArrBase::iterator toErase = arrObject->end( );

				for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
					if ( *k == j->second ) {
						toErase = k;
						break;
					}
				}

				if ( toErase != arrObject->end( ) ) {
					arrObject->erase( toErase );
				}
			}

			m_objPeriodModel->erase( i );
		}
	} // void OnFormListPeriod_Del

	udUInt CHServiceForm::GetPeriodLastIndex( ) {
		udUInt ret = 0, cur = 0;
		udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );

		for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
			cur = ( *i )->get( "index" )->toUInt( );
			if ( cur > ret ) {
				ret = cur;
			}
		}

		return ret;
	} // udUInt GetPeriodLastIndex

	void CHServiceForm::SavePeriod( udPBase obj ) {
		udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );
		udPBase add = GetProjectServicePeriod( );

		add->apply( obj, true, true );
		arrObject->push_back( add );

		if ( add->get( "index" )->toUInt( ) == 0 ) { // новый период
			add->get( "index" )->ini( GetPeriodLastIndex( ) + 1 );
		} else { // старый период, необходимо многое обновить
		}

		Gtk::TreeModel::iterator i = m_objPeriodModel->append( );
		Gtk::TreeModel::Row row = *i;
		Gtk::TreePath objPath = m_objPeriodModel->get_path( i );

		row[ m_pModelFormListPeriod->m_col_date1 ] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
		row[ m_pModelFormListPeriod->m_col_date2 ] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );

		m_mapObjectToRowPeriod[ objPath.to_string( ) ] = add;
	} // void SavePeriod

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHServiceForm::OnFormListPrice_Select( ) {
		Gtk::TreeModel::iterator i = m_inpPrice.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnPriceEdit->set_sensitive( true );
			m_btnPriceDel->set_sensitive( true );
		} else {
			m_btnPriceEdit->set_sensitive( false );
			m_btnPriceDel->set_sensitive( false );
		}
	} // void OnFormListPrice_Select

	void CHServiceForm::OnFormListPrice_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHServicePriceFormModal handler = new udHServicePriceFormModal( this );
		handler->Init( );
		
		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание периода" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание периода" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			//udPBase add = new udProjectServicePrice;
			udPBase add = new udProjectPriceTableItem;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidPrice );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			arrObject->push_back( add );

			Gtk::TreeModel::iterator i = m_objPriceModel->append( );
			Gtk::TreeModel::Row row = *i;
			Gtk::TreePath objPath = m_objPriceModel->get_path( i );

			udPFDEObject pFdePeriod = ( udPFDEObject ) pDa->GetExtraData( udHServicePriceForm::ctrlidPeriod );
			if ( pFdePeriod ) {
				udPBase pPeriod = pFdePeriod->GetObject( );
				if ( pPeriod ) {
					row[ m_pModelFormListPrice->m_col_date1 ] = Glib::locale_to_utf8( pPeriod->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListPrice->m_col_date2 ] = Glib::locale_to_utf8( pPeriod->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				}
			}

			udPFDEObject pFdeCurrency = ( udPFDEObject ) pDa->GetExtraData( udHServicePriceForm::ctrlidCurrency );
			if ( pFdeCurrency ) {
				udPBase pCurrency = pFdeCurrency->GetObject( );
				if ( pCurrency ) {
					row[ m_pModelFormListPrice->m_col_currency ] = Glib::locale_to_utf8( pCurrency->get( "name" )->toString( ) );
				}
			}

			udPFDEObject pFdeCurrencyRecommend = ( udPFDEObject ) pDa->GetExtraData( udHServicePriceForm::ctrlidCurrencyRecommend );
			if ( pFdeCurrencyRecommend ) {
				udPBase pCurrencyRecommend = pFdeCurrencyRecommend->GetObject( );
				if ( pCurrencyRecommend ) {
					row[ m_pModelFormListPrice->m_col_currency_recommend ] = Glib::locale_to_utf8( pCurrencyRecommend->get( "name" )->toString( ) );
				}
			}

			row[ m_pModelFormListPrice->m_col_num				] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
			row[ m_pModelFormListPrice->m_col_amount			] = Glib::locale_to_utf8( add->get( "amount" )->toString( ) );
			row[ m_pModelFormListPrice->m_col_amount_recommend	] = Glib::locale_to_utf8( add->get( "amount_recommend" )->toString( ) );

			m_mapObjectToRowPrice[ objPath.to_string( ) ] = add;
		}

		udDELETE( handler );
	} // void OnFormListPrice_Add

	void CHServiceForm::OnFormListPrice_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpPrice.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreePath objPath = m_objPriceModel->get_path( i );
			map< Glib::ustring, udPBase >::iterator j = m_mapObjectToRowPrice.find( objPath.to_string( ) );
			if ( j != m_mapObjectToRowPrice.end( ) ) {
				// j->second - наш объект редактирования
				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
				udPHServicePriceFormModal handler = new udHServicePriceFormModal( this );
				handler->Init( );
				handler->LoadFrom( j->second );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование периода" ) );

				/*Gtk::Window wnd;
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование периода" ) );
				wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				wnd.show_all( );
				Gtk::Main::run( wnd );*/

				udPDAForm pDa = handler->GetData( );
				udPBase obj = pDa->GetData( );
				if ( obj ) {
					Gtk::TreeModel::Row row = *i;

					j->second->apply( obj, true, true );

					udPFDEObject pFdePeriod = ( udPFDEObject ) pDa->GetExtraData( udHServicePriceForm::ctrlidPeriod );
					if ( pFdePeriod ) {
						udPBase pPeriod = pFdePeriod->GetObject( );
						if ( pPeriod ) {
							row[ m_pModelFormListPrice->m_col_date1 ] = Glib::locale_to_utf8( pPeriod->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
							row[ m_pModelFormListPrice->m_col_date2 ] = Glib::locale_to_utf8( pPeriod->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
						}
					}

					udPFDEObject pFdeCurrency = ( udPFDEObject ) pDa->GetExtraData( udHServicePriceForm::ctrlidCurrency );
					if ( pFdeCurrency ) {
						udPBase pCurrency = pFdeCurrency->GetObject( );
						if ( pCurrency ) {
							row[ m_pModelFormListPrice->m_col_currency ] = Glib::locale_to_utf8( pCurrency->get( "name" )->toString( ) );
						}
					}

					row[ m_pModelFormListPrice->m_col_num		] = Glib::locale_to_utf8( j->second->get( "num" )->toString( ) );
					row[ m_pModelFormListPrice->m_col_amount	] = Glib::locale_to_utf8( j->second->get( "amount" )->toString( ) );
				}

				udDELETE( handler );
			}
		}
	} // void OnFormListPrice_Edit

	void CHServiceForm::OnFormListPrice_Del( ) {
		Gtk::TreeModel::iterator i = m_inpPrice.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreePath objPath = m_objPriceModel->get_path( i );
			map< Glib::ustring, udPBase >::iterator j = m_mapObjectToRowPrice.find( objPath.to_string( ) );
			if ( j != m_mapObjectToRowPrice.end( ) ) {
				udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPrice ) )->GetArray( );
				udArrBase::iterator toErase = arrObject->end( );

				for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
					if ( *k == j->second ) {
						toErase = k;
						break;
					}
				}

				if ( toErase != arrObject->end( ) ) {
					arrObject->erase( toErase );
				}
			}

			m_objPriceModel->erase( i );
		}
	} // void OnFormListPrice_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHServiceFormModal::CHServiceFormModal( ) : CHServiceForm( ) {
	} // CHServiceFormModal

	CHServiceFormModal::CHServiceFormModal( const CHServiceFormModal& objCopy ) : CHServiceForm( ) {
	} // CHServiceFormModal

	CHServiceFormModal::~CHServiceFormModal( ) {
	} // ~CHServiceFormModal

	CHServiceFormModal& CHServiceFormModal::operator =( const CHServiceFormModal& objCopy ) {
		return *this;
	} // CHServiceFormModal& operator =

	void CHServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHServiceForm




	CHServiceFormModeless::CHServiceFormModeless( ) : CHServiceForm( ) {
	} // CHServiceFormModeless

	CHServiceFormModeless::CHServiceFormModeless( const CHServiceFormModeless& objCopy ) : CHServiceForm( ) {
	} // CHServiceFormModeless

	CHServiceFormModeless::~CHServiceFormModeless( ) {
	} // ~CHServiceFormModeless

	CHServiceFormModeless& CHServiceFormModeless::operator =( const CHServiceFormModeless& objCopy ) {
		return *this;
	} // CHServiceFormModeless& operator =

	void CHServiceFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "service" );
	} // void OnButtonOk

	void CHServiceFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "service" );
	} // void CHServiceForm

	void CHServiceFormModeless::Init( ) {
		CHServiceForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHServiceFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHServiceFormModeless

} // namepsace UdSDK
