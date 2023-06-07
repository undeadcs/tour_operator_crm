
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTplTourListPick( ) {
		return new CHTplTourPick;
	} // udPListHandlerPick GetHTplTourListPick

	udPBase GetProjectTplTourList( ) {
		udPBase ret = GetProjectTplTour( );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTplTourList

	CHTplTour::CHTplTour( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblFltDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblFltTour( Glib::locale_to_utf8( "Тур" ) )
	{
		m_szName = "tpltour";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать шаблон тура" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHTplTour::ColModelTplTour( );

		m_pData->SetName( "tpltour" );
		m_pData->SetGetObject( GetProjectTplTourList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDirect );
		pFde->SetMark( fltDirect );
		m_pData->AddExtraData( pFde );
		m_inpFltDirect.SetDataObject( pFde );
		m_inpFltDirect.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		m_inpFltDirect.SetAttr( "name" );
		m_inpFltDirect.SetGetListHandler( GetHDirectListPick );
		m_inpFltDirect.SetGetFormHandler( GetHDirectForm );
		m_inpFltDirect.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTour );
		pFde->SetMark( fltTour );
		m_pData->AddExtraData( pFde );
		m_inpFltTour.SetDataObject( pFde );
		m_inpFltTour.SetWindowTitle( Glib::locale_to_utf8( "Выбор тура" ) );
		m_inpFltTour.SetAttr( "name" );
		m_inpFltTour.SetGetListHandler( GetHTourListPick );
		m_inpFltTour.SetGetFormHandler( GetHTourForm );
		m_inpFltTour.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование тура" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Направление
		m_tblFilter.attach( m_lblFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Тур
		m_tblFilter.attach( m_lblFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTplTour

	CHTplTour::CHTplTour( const CHTplTour& objCopy ) : udListHandler( ) {
		m_szName = "tpltour";
	} // CHTplTour

	CHTplTour::~CHTplTour( ) {
	} // ~CTplTour

	CHTplTour& CHTplTour::operator =( const CHTplTour& objCopy ) {
		return *this;
	} // CHTplTour& operator =

	void CHTplTour::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Направление" ), m_pColModel->m_col_direct );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Себестоимость" ), m_pColModel->m_col_cost );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Итоговая цена" ), m_pColModel->m_col_price );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHTplTour::Load( ) {
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
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_direct ] = Glib::locale_to_utf8( obj->get( "direct_" )->toString( ) );
			row[ m_pColModel->m_col_cost ] = Glib::locale_to_utf8( obj->get( "cost" )->toString( ) );
			row[ m_pColModel->m_col_price ] = Glib::locale_to_utf8( obj->get( "price" )->toString( ) );
		}
	} // void Load

	void CHTplTour::FilterUiClear( ) {
		m_inpFltName.Reset( );
		m_inpFltDirect.SetTitle( "" );
		m_inpFltTour.SetTitle( "" );
	} // void FilterUiClear

	void CHTplTour::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		filter->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "tour" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltTour ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTplTourModeless::CHTplTourModeless( ) :
		udListHandlerModeless( ),
		CHTplTour( )
	{
	} // CHTplTourModeless

	CHTplTourModeless::CHTplTourModeless( const CHTplTourModeless& objCopy ) : CHTplTour( ) {
	} // CHTplTourModeless

	CHTplTourModeless::~CHTplTourModeless( ) {
	} // ~CHTplTourModeless

	CHTplTourModeless& CHTplTourModeless::operator =( const CHTplTourModeless& objCopy ) {
		return *this;
	} // CHTplTourModeless& operator =

	void CHTplTourModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHTplTour::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTplTourModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "tpltour_form" );
	} // void OnButtonForm

	void CHTplTourModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "tpltour_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "tpltour_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTplTourPick::CHTplTourPick( ) : CHTplTour( ) {
	} // CHTplTourPick

	CHTplTourPick::CHTplTourPick( const CHTplTourPick& objCopy ) : CHTplTour( ) {
	} // CHTplTourPick

	CHTplTourPick::~CHTplTourPick( ) {
	} // ~CHTplTourPick

	CHTplTourPick& CHTplTourPick::operator =( const CHTplTourPick& objCopy ) {
		return *this;
	} // CHTplTourPick& operator =

	void CHTplTourPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTplTour::Init( );
	} // void CHTplTourPick

	void CHTplTourPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHTplTourFormModal handler = new udHTplTourFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание шаблона тура" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание шаблона тура" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTplTourPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectTplTour( );

				itObj->second->apply( m_pPickedObject );
			}
			/*Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTplTour( );
			Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );*/
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите шаблон тура" ),
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

	void CHTplTourPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTplTourPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHTplTourFormModal handler = new udHTplTourFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование шаблона тура" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование шаблона тура" ) );
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

	udPFormHandler GetHTplTourForm( ) {
		return new CHTplTourFormModal;
	} // udPFormHandler GetHTplTourForm

	CHTplTourForm::CHTplTourForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Шаблон тура" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblDaynum( Glib::locale_to_utf8( "Продолжительность" ) ),
		m_lblDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblRoute( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblService( Glib::locale_to_utf8( "Услуги" ) ),
		m_lblApp( Glib::locale_to_utf8( "Приложения" ) ),
		m_btnCreateTour( Glib::locale_to_utf8( "Создать тур" ) )
	{
		m_szName = "tpltour_form";

		m_pData->SetName( "tpltour" );
		m_pData->SetGetObject( GetProjectTplTour );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblName.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpName.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDirect );
		pFde->SetMark( ctrlidDirect );
		m_pData->AddExtraData( pFde );
		m_inpDirect.SetDataObject( pFde );
		m_inpDirect.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		m_inpDirect.SetAttr( "name" );
		m_inpDirect.SetGetListHandler( GetHDirectListPick );
		m_inpDirect.SetGetFormHandler( GetHDirectForm );
		m_inpDirect.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// маршрут
		m_inpRoute.set_size_request( 400, 200 );
		m_pModelFormListRoute = new ColModelFormListRoute;
		m_objRouteModel = Gtk::ListStore::create( *m_pModelFormListRoute );
		
		pTree = m_inpRoute.GetTreeView( );
		pTree->set_model( m_objRouteModel );
		pTree->append_column( Glib::locale_to_utf8( "Город" ), m_pModelFormListRoute->m_col_name );
		m_inpRoute.SetColumnDefaults( );

		m_btnRouteReplace = m_inpRoute.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnRouteReplace->set_sensitive( false );
		m_btnRouteReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListRoute_Replace ) );

		m_btnRouteDel = m_inpRoute.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnRouteDel->set_sensitive( false );
		m_btnRouteDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListRoute_Del ) );

		m_btnRouteAdd = m_inpRoute.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnRouteAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListRoute_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListRoute_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTplTourRouteList );
		pFdeList->SetCallbackSend( GetProjectTplTourRoute );
		pFdeList->SetMark( ctrlidRoute );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// услуги
		m_inpService.set_size_request( 400, 200 );
		m_pModelFormListService = new ColModelFormListService;
		m_objServiceModel = Gtk::ListStore::create( *m_pModelFormListService );
		
		pTree = m_inpService.GetTreeView( );
		pTree->set_model( m_objServiceModel );
		pTree->append_column( Glib::locale_to_utf8( "Услуга" ), m_pModelFormListService->m_col_service );
		pTree->append_column( Glib::locale_to_utf8( "Количество человек" ), m_pModelFormListService->m_col_num );
		pTree->append_column( Glib::locale_to_utf8( "С" ), m_pModelFormListService->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "По" ), m_pModelFormListService->m_col_date2 );
		m_inpService.SetColumnDefaults( );

		m_btnServiceEdit = m_inpService.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnServiceEdit->set_sensitive( false );
		m_btnServiceEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListService_Edit ) );

		m_btnServiceDel = m_inpService.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnServiceDel->set_sensitive( false );
		m_btnServiceDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListService_Del ) );

		m_btnServiceAdd = m_inpService.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnServiceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListService_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListService_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTplTourServiceList );
		pFdeList->SetCallbackSend( GetProjectTplTourService );
		pFdeList->SetMark( ctrlidService );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложения
		m_inpApp.set_size_request( 400, 200 );
		m_pModelFormListApp = new ColModelFormListApp;
		m_objAppModel = Gtk::ListStore::create( *m_pModelFormListApp );

		pTree = m_inpApp.GetTreeView( );
		pTree->set_model( m_objAppModel );
		pTree->append_column( Glib::locale_to_utf8( "Имя файла" ), m_pModelFormListApp->m_col_name );
		m_inpApp.SetColumnDefaults( );

		m_btnAppEdit = m_inpApp.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnAppEdit->set_sensitive( false );
		m_btnAppEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListApp_Edit ) );

		m_btnAppDel = m_inpApp.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnAppDel->set_sensitive( false );
		m_btnAppDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListApp_Del ) );

		m_btnAppReplace = m_inpApp.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnAppReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListApp_Replace ) );
		m_btnAppReplace->set_sensitive( false );

		m_btnAppAdd = m_inpApp.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnAppAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListApp_Add ) );

		m_btnFolder = m_inpApp.AddButton( Glib::locale_to_utf8( "Открыть папку" ) );
		m_btnFolder->signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListApp_Folder ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnFormListApp_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectAttach );
		pFdeList->SetMark( ctrlidApp );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btnCreateTour.signal_clicked( ).connect( sigc::mem_fun( this, &CHTplTourForm::OnButtonCreateTour ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTplTourForm

	CHTplTourForm::CHTplTourForm( const CHTplTourForm& objCopy ) : udFormHandler( ) {
	} // CHTplTourForm

	CHTplTourForm::~CHTplTourForm( ) {
	} // ~CHTplTourForm

	CHTplTourForm& CHTplTourForm::operator =( const CHTplTourForm& objCopy ) {
		return *this;
	} // CHTplTourForm& operator =

	void CHTplTourForm::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложение
		m_szFolderApp = g_pApp->GetUniqueTmpFolder( );
		m_pData->SetHasFiles( true );
		m_pData->SetFolder( m_szFolderApp );
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование
		m_tblInput.attach( m_lblName, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpName, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Пролжительность
		m_tblInput.attach( m_lblDaynum, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDaynum, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Направление
		m_tblInput.attach( m_lblDirect, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDirect, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Маршрут
		m_tblInput.attach( m_lblRoute, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpRoute, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Услуги
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Приложения
		m_tblInput.attach( m_lblApp, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpApp, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnCreateTour, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 0, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHTplTourForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpDaynum.Reset( );
		m_inpDirect.SetTitle( "" );
		m_objRouteModel->clear( );
		m_objServiceModel->clear( );
		m_objAppModel->clear( );
	} // void Clear

	void CHTplTourForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		if ( obj->get( "daynum" )->toUInt( ) > 0 ) {
			m_inpDaynum.SetValue( Glib::locale_to_utf8( obj->get( "daynum" )->toString( ) ) );
		}
		m_inpDirect.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "name" )->toString( )
		) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objRouteModel->append( ) );
				row[ m_pModelFormListRoute->m_col_name		] = Glib::locale_to_utf8( add->get( "city_" )->toString( ) );
				row[ m_pModelFormListRoute->m_col_object	] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objServiceModel->append( ) );
				row[ m_pModelFormListService->m_col_service	] = Glib::locale_to_utf8( add->get( "service_" )->toString( ) );
				row[ m_pModelFormListService->m_col_num		] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
				row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_object	] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objAppModel->append( ) );
				row[ m_pModelFormListApp->m_col_name	] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListApp->m_col_object	] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHTplTourForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szName,
						szDaynum;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		szDaynum = m_inpDaynum.GetValue( );
		if ( szDaynum.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите количество дней" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDaynum.GetEntry( ) ) );
			return 2;
		}

		udPBase obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите направление" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDirect.GetButton( ) ) );
			return 3;
		}

		return 0;
	} // udInt Validate

	void CHTplTourForm::Send( ) {
		Glib::ustring	szName,
						szDaynum;

		szName = m_inpName.GetValue( );
		szDaynum = m_inpDaynum.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "daynum" )->ini( toUInt( szDaynum.data( ) ) );
		obj->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);

		m_pData->Send( ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( ) );
	} // void Send

	void CHTplTourForm::OnButtonCreateTour( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::ScrolledWindow wndScroll;
		udPHTourFormModal handler = new udHTourFormModal( );
		handler->Init( );
		handler->LoadFromTplTour( this );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание тура" ) );

		/*Gtk::Window wnd;
		wnd.set_size_request( 800, 600 );
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание тура" ) );
		//wnd.add( *( handler->GetWindow( ) ) );
		wnd.add( wndScroll );
		wndScroll.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		handler->GetWindow( )->show( );
		wndScroll.show( );
		wnd.show( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );
	} // void OnButtonCreateTour

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTplTourForm::OnFormListService_Select( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnServiceEdit->set_sensitive( true );
			m_btnServiceDel->set_sensitive( true );
		} else {
			m_btnServiceEdit->set_sensitive( false );
			m_btnServiceDel->set_sensitive( false );
		}
	} // void OnFormListService_Select

	void CHTplTourForm::OnFormListService_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTplTourServiceFormModal handler = new udHTplTourServiceFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание услуги" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание услуги" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectTplTourService;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "order" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objServiceModel->append( ) );

			udPFDEObject pFdeService = ( udPFDEObject ) pDa->GetExtraData( udHTplTourServiceForm::ctrlidService );
			if ( pFdeService ) {
				udPBase pService = pFdeService->GetObject( );
				if ( pService ) {
					row[ m_pModelFormListService->m_col_service ] = Glib::locale_to_utf8( pService->get( "name_" )->toString( ) );
				}
			}

			row[ m_pModelFormListService->m_col_num		] = Glib::locale_to_utf8( add->get( "num" )->toString( ) );
			row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListService_Add

	void CHTplTourForm::OnFormListService_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListService->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTplTourServiceFormModal handler = new udHTplTourServiceFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование услуги" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование услуги" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				udPFDEObject pFdeService = ( udPFDEObject ) pDa->GetExtraData( udHTplTourServiceForm::ctrlidService );
				if ( pFdeService ) {
					udPBase pService = pFdeService->GetObject( );
					if ( pService ) {
						row[ m_pModelFormListService->m_col_service ] = Glib::locale_to_utf8( pService->get( "name_" )->toString( ) );
					}
				}

				row[ m_pModelFormListService->m_col_num ] = Glib::locale_to_utf8( edit->get( "num" )->toString( ) );
				row[ m_pModelFormListService->m_col_date1 ] = Glib::locale_to_utf8( edit->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2 ] = Glib::locale_to_utf8( edit->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListService_Edit

	void CHTplTourForm::OnFormListService_Del( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListService->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objServiceModel->erase( i );
		}
	} // void OnFormListService_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTplTourForm::OnFormListApp_Select( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnAppEdit->set_sensitive( true );
			m_btnAppDel->set_sensitive( true );
			m_btnAppReplace->set_sensitive( true );
		} else {
			m_btnAppEdit->set_sensitive( false );
			m_btnAppDel->set_sensitive( false );
			m_btnAppReplace->set_sensitive( false );
		}
	} // void OnFormListApp_Select

	void CHTplTourForm::OnFormListApp_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

		int code = dlgPickFile.run( );

		if ( code == Gtk::RESPONSE_OK ) {
			Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
			wstring szFileName = ConvertString( file->get_basename( ), CP_UTF8 );
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
			udPBase pCurApp = NULL;
			bool bFound = false;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				pCurApp = *i;
				if ( pCurApp->get( "name" )->toWString( ) == szFileName ) {
					bFound = true;
					break;
				}
			}

			if ( bFound ) {
				Gtk::MessageDialog dialog(
					*wndParent,
					Glib::locale_to_utf8( "Файл уже присутствует в списке" ),
					false,
					Gtk::MESSAGE_WARNING,
					Gtk::BUTTONS_OK
				);
				dialog.set_secondary_text(
					Glib::locale_to_utf8( "Файл '" ) +
					file->get_basename( ) +
					Glib::locale_to_utf8( "'\nСмените имя или выберите другой." )
				);
				dialog.run( );
			} else {
				wstring szFilePath;
				if ( file->has_parent( ) ) {
					szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
				}
				udPBase add = new udProjectAttach;
				add->get( "obj" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
				add->get( "type" )->ini( udProjectAttach::ATTACH_TPLTOUR );
				add->get( "name" )->ini( szFileName );
				add->get( "orig" )->ini( szFilePath );

				wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
				Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

				if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
					add->get( "tmp_path" )->ini( wFileTmpPath );
					arrObject->push_back( add );

					Gtk::TreeModel::Row row = *( m_objAppModel->append( ) );

					row[ m_pModelFormListApp->m_col_name	] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListApp->m_col_object	] = add;
				} else {
					udDELETE( add );
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Не удалось скопировать файл" ),
						false,
						Gtk::MESSAGE_ERROR,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text(
						Glib::locale_to_utf8( "Файл '" ) +
						file->get_basename( ) +
						Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
					);
					dialog.run( );
				}
			}
		}
	} // void OnFormListApp_Add

	void CHTplTourForm::OnFormListApp_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListApp->m_col_object ];
			ExtOpenFile( edit->get( "tmp_path" )->toWString( ), false );
		}
	} // void OnFormListApp_Edit

	void CHTplTourForm::OnFormListApp_Del( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListApp->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objAppModel->erase( i );
		}
	} // void OnFormListApp_Del

	void CHTplTourForm::OnFormListApp_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListApp->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
			dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
			dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

			int code = dlgPickFile.run( );

			if ( code == Gtk::RESPONSE_OK ) {
				Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
				wstring szFileName = ConvertString( file->get_basename( ), CP_UTF8 );
				udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( );
				udPBase pCurApp = NULL;
				bool bFound = false;

				for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
					pCurApp = *k;
					if ( pCurApp->get( "name" )->toWString( ) == szFileName ) {
						bFound = true;
						break;
					}
				}

				if ( bFound ) {
					Gtk::MessageDialog dialog(
						*wndParent,
						Glib::locale_to_utf8( "Файл уже присутствует в списке" ),
						false,
						Gtk::MESSAGE_WARNING,
						Gtk::BUTTONS_OK
					);
					dialog.set_secondary_text(
						Glib::locale_to_utf8( "Файл '" ) +
						file->get_basename( ) +
						Glib::locale_to_utf8( "'\nСмените имя или выберите другой." )
					);
					dialog.run( );
				} else {
					wstring szFilePath;
					if ( file->has_parent( ) ) {
						szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
					}

					wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
					Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

					if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
						replace->get( "tmp_path" )->ini( wFileTmpPath );
						replace->get( "name" )->ini( szFileName );
						replace->get( "orig" )->ini( szFilePath );

						Gtk::TreeModel::Row row = *i;

						row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( replace->get( "name" )->toString( ) );
					} else {
						Gtk::MessageDialog dialog(
							*wndParent,
							Glib::locale_to_utf8( "Не удалось скопировать файл" ),
							false,
							Gtk::MESSAGE_ERROR,
							Gtk::BUTTONS_OK
						);
						dialog.set_secondary_text(
							Glib::locale_to_utf8( "Файл '" ) +
							file->get_basename( ) +
							Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
						);
						dialog.run( );
					}
				}
			}
		}
	} // void OnFormListApp_Del

	void CHTplTourForm::OnFormListApp_Folder( ) {
		ExtOpenFolder( m_szFolderApp );
	} // void OnFormListApp_Folder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTplTourForm::OnFormListRoute_Select( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnRouteReplace->set_sensitive( true );
			m_btnRouteDel->set_sensitive( true );
		} else {
			m_btnRouteReplace->set_sensitive( false );
			m_btnRouteDel->set_sensitive( false );
		}
	} // void OnFormListRoute_Select

	void CHTplTourForm::OnFormListRoute_Add( ) {
		m_pData->MakeObject( );

		udPListHandlerPick handler = GetHCityListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать город" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбрать город" ) );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj ) {
			udPBase add = new udProjectTplTourRoute;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "city" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "city_" )->ini( obj->get( "name" )->toWString( ) );
			add->get( "tpltour" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::iterator i = m_objRouteModel->append( );
			Gtk::TreeModel::Row row = *( m_objRouteModel->append( ) );
			Gtk::TreePath objPath = m_objRouteModel->get_path( i );

			row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( add->get( "city_" )->toString( ) );
			row[ m_pModelFormListRoute->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListRoute_Add

	void CHTplTourForm::OnFormListRoute_Del( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListRoute->m_col_object ];
			
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objRouteModel->erase( i );
		}
	} // void OnFormListRoute_Del

	void CHTplTourForm::OnFormListRoute_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListRoute->m_col_object ];
			udPListHandlerPick handler = GetHCityListPick( );
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			handler->BeforeCreate( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать город" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Выбрать город" ) );
			//wnd.add( *( handler->GetWindowPick( ) ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPBase obj = handler->GetObject( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				replace->get( "city" )->ini( obj->get( "id" )->toUInt( ) );
				replace->get( "city_" )->ini( obj->get( "name" )->toWString( ) );

				row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( replace->get( "city_" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListRoute_Replace

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTplTourFormModal::CHTplTourFormModal( ) : CHTplTourForm( ) {
	} // CHTplTourFormModal

	CHTplTourFormModal::CHTplTourFormModal( const CHTplTourFormModal& objCopy ) : CHTplTourForm( ) {
	} // CHTplTourFormModal

	CHTplTourFormModal::~CHTplTourFormModal( ) {
	} // ~CHTplTourFormModal

	CHTplTourFormModal& CHTplTourFormModal::operator =( const CHTplTourFormModal& objCopy ) {
		return *this;
	} // CHTplTourFormModal& operator =

	void CHTplTourFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTplTourFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTplTourForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTplTourFormModeless::CHTplTourFormModeless( ) : CHTplTourForm( ) {
	} // CHTplTourFormModeless

	CHTplTourFormModeless::CHTplTourFormModeless( const CHTplTourFormModeless& objCopy ) : CHTplTourForm( ) {
	} // CHTplTourFormModeless

	CHTplTourFormModeless::~CHTplTourFormModeless( ) {
	} // ~CHTplTourFormModeless

	CHTplTourFormModeless& CHTplTourFormModeless::operator =( const CHTplTourFormModeless& objCopy ) {
		return *this;
	} // CHTplTourFormModeless& operator =

	void CHTplTourFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "tpltour" );
	} // void OnButtonOk

	void CHTplTourFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "tpltour" );
	} // void CHTplTourForm

	void CHTplTourFormModeless::Init( ) {
		CHTplTourForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTplTourFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTplTourFormModeless

} // namespace UdSDK
