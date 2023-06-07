
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTouristListListPick( ) {
		return new CHTouristListPick;
	} // udPListHandlerPick GetHTouristListListPick

	udPBase GetProjectTouristListList( ) {
		udPBase ret = GetProjectTouristList( );
		ret->get( "leader_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTouristListList

	CHTouristList::CHTouristList( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата выезда с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltClient( Glib::locale_to_utf8( "Руководитель" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "touristlist";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать список туристов" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "leader" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHTouristList::ColModelTouristList( );

		m_pData->SetName( "touristlist" );
		m_pData->SetGetObject( GetProjectTouristListList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( fltClient );
		m_pData->AddExtraData( pFde );
		m_inpFltClient.SetDataObject( pFde );
		m_inpFltClient.SetWindowTitle( Glib::locale_to_utf8( "Выбор клиента" ) );
		m_inpFltClient.SetAttr( "fio" );
		m_inpFltClient.SetGetListHandler( GetHClientListPick );
		m_inpFltClient.SetGetFormHandler( GetHClientForm );
		m_inpFltClient.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование клиента" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Дата выезда с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата выезда по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Руководитель
		m_tblFilter.attach( m_lblFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTouristList

	CHTouristList::CHTouristList( const CHTouristList& objCopy ) : udListHandler( ) {
		m_szName = "touristlist";
	} // CHTouristList

	CHTouristList::~CHTouristList( ) {
	} // ~CTouristList

	CHTouristList& CHTouristList::operator =( const CHTouristList& objCopy ) {
		return *this;
	} // CHTouristList& operator =

	void CHTouristList::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата выезда" ), m_pColModel->m_col_date1 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата въезда" ), m_pColModel->m_col_date2 );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Руководитель" ), m_pColModel->m_col_leader );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHTouristList::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_date1 ] = Glib::locale_to_utf8( obj->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_date2 ] = Glib::locale_to_utf8( obj->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_leader ] = Glib::locale_to_utf8( obj->get( "leader_" )->toString( ) );
		}
	} // void Load

	void CHTouristList::FilterUiClear( ) {
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
		m_inpFltClient.SetTitle( "" );
	} // void FilterUiClear

	void CHTouristList::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "leader" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltClient ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTouristListModeless::CHTouristListModeless( ) :
		udListHandlerModeless( ),
		CHTouristList( )
	{
	} // CHTouristListModeless

	CHTouristListModeless::CHTouristListModeless( const CHTouristListModeless& objCopy ) : CHTouristList( ) {
	} // CHTouristListModeless

	CHTouristListModeless::~CHTouristListModeless( ) {
	} // ~CHTouristListModeless

	CHTouristListModeless& CHTouristListModeless::operator =( const CHTouristListModeless& objCopy ) {
		return *this;
	} // CHTouristListModeless& operator =

	void CHTouristListModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHTouristList::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTouristListModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "touristlist_form" );
	} // void OnButtonForm

	void CHTouristListModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "touristlist_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "touristlist_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTouristListPick::CHTouristListPick( ) : CHTouristList( ) {
	} // CHTouristListPick

	CHTouristListPick::CHTouristListPick( const CHTouristListPick& objCopy ) : CHTouristList( ) {
	} // CHTouristListPick

	CHTouristListPick::~CHTouristListPick( ) {
	} // ~CHTouristListPick

	CHTouristListPick& CHTouristListPick::operator =( const CHTouristListPick& objCopy ) {
		return *this;
	} // CHTouristListPick& operator =

	void CHTouristListPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTouristList::Init( );
	} // void CHTouristListPick

	void CHTouristListPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHTouristListFormModal handler = new udHTouristListFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание списка туристов" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание списка туристов" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTouristListPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTouristList( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите список туристов" ),
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

	void CHTouristListPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTouristListPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHTouristListFormModal handler = new udHTouristListFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование списка туристов" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование списка туристов" ) );
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

	CHTouristListForm::CHTouristListForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Список туристов" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Дата выезда" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Дата въезда" ) ),
		m_lblExit( Glib::locale_to_utf8( "Пункт выезда" ) ),
		m_lblEnter( Glib::locale_to_utf8( "Пункт въезда" ) ),
		m_lblHost( Glib::locale_to_utf8( "Принимающая сторона" ) ),
		m_lblLeader( Glib::locale_to_utf8( "Руководитель группы" ) ),
		m_lblRoute( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblClient( Glib::locale_to_utf8( "Клиенты" ) ),
		m_btnPrint( Glib::locale_to_utf8( "Распечатать" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) )
	{
		m_szName = "touristlist_form";

		m_pData->SetName( "touristlist" );
		m_pData->SetGetObject( GetProjectTouristList );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblDate1.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTransferPoint );
		pFde->SetMark( ctrlidExit );
		m_pData->AddExtraData( pFde );
		m_inpExit.SetDataObject( pFde );
		m_inpExit.SetWindowTitle( Glib::locale_to_utf8( "Выбор пункта перехода" ) );
		m_inpExit.SetAttr( "name" );
		m_inpExit.SetGetListHandler( GetHTransferPointListPick );
		m_inpExit.SetGetFormHandler( GetHTransferPointForm );
		m_inpExit.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пункта перехода" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTransferPoint );
		pFde->SetMark( ctrlidEnter );
		m_pData->AddExtraData( pFde );
		m_inpEnter.SetDataObject( pFde );
		m_inpEnter.SetWindowTitle( Glib::locale_to_utf8( "Выбор пункта перехода" ) );
		m_inpEnter.SetAttr( "name" );
		m_inpEnter.SetGetListHandler( GetHTransferPointListPick );
		m_inpEnter.SetGetFormHandler( GetHTransferPointForm );
		m_inpEnter.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пункта перехода" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectHostSide );
		pFde->SetMark( ctrlidHost );
		m_pData->AddExtraData( pFde );
		m_inpHost.SetDataObject( pFde );
		m_inpHost.SetWindowTitle( Glib::locale_to_utf8( "Выбор принимающей стороны" ) );
		m_inpHost.SetAttr( "name" );
		m_inpHost.SetGetListHandler( GetHHostSideListPick );
		m_inpHost.SetGetFormHandler( GetHHostSideForm );
		m_inpHost.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование принимающей стороны" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectRoute );
		pFde->SetMark( ctrlidRoute );
		m_pData->AddExtraData( pFde );
		m_inpRoute.SetDataObject( pFde );
		m_inpRoute.SetWindowTitle( Glib::locale_to_utf8( "Выбор маршрута" ) );
		m_inpRoute.SetAttr( "name" );
		m_inpRoute.SetGetListHandler( GetHRouteListPick );
		m_inpRoute.SetGetFormHandler( GetHRouteForm );
		m_inpRoute.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование маршрута" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( ctrlidLeader );
		m_pData->AddExtraData( pFde );
		m_inpLeader.SetDataObject( pFde );
		m_inpLeader.SetWindowTitle( Glib::locale_to_utf8( "Выбор руководителя группы" ) );
		m_inpLeader.SetAttr( "fio" );
		m_inpLeader.SetGetListHandler( GetHTouristListLeaderListPick );
		m_inpLeader.SetTouristListForm( this );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Клиенты
		m_inpClient.set_size_request( 400, 200 );
		m_pModelFormListClient = new ColModelFormListClient;
		m_objClientModel = Gtk::ListStore::create( *m_pModelFormListClient );
		
		pTree = m_inpClient.GetTreeView( );
		pTree->set_model( m_objClientModel );
		pTree->append_column( Glib::locale_to_utf8( "ФИО" ), m_pModelFormListClient->m_col_fio );
		m_inpClient.SetColumnDefaults( );

		m_btnClientReplace = m_inpClient.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnClientReplace->set_sensitive( false );
		m_btnClientReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHTouristListForm::OnFormListClient_Replace ) );

		m_btnClientDel = m_inpClient.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnClientDel->set_sensitive( false );
		m_btnClientDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTouristListForm::OnFormListClient_Del ) );

		m_btnClientAdd = m_inpClient.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnClientAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTouristListForm::OnFormListClient_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTouristListForm::OnFormListClient_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourClientList );
		pFdeList->SetCallbackSend( GetProjectTourClient );
		pFdeList->SetMark( ctrlidClient );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btnPrint.signal_clicked( ).connect( sigc::mem_fun( this, &CHTouristListForm::OnButtonPrint ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTouristListForm

	CHTouristListForm::CHTouristListForm( const CHTouristListForm& objCopy ) : udFormHandler( ) {
	} // CHTouristListForm

	CHTouristListForm::~CHTouristListForm( ) {
	} // ~CHTouristListForm

	CHTouristListForm& CHTouristListForm::operator =( const CHTouristListForm& objCopy ) {
		return *this;
	} // CHTouristListForm& operator =

	void CHTouristListForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Дата выезда
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Дата въезда
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Пункт выезда
		m_tblInput.attach( m_lblExit, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpExit, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Пункт въезда
		m_tblInput.attach( m_lblEnter, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpEnter, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Принимающая сторона
		m_tblInput.attach( m_lblHost, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpHost, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Маршрут
		m_tblInput.attach( m_lblRoute, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpRoute, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Руководитель группы
		m_tblInput.attach( m_lblLeader, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpLeader, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Клиенты
		m_tblInput.attach( m_lblClient, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpClient, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnPrint, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHTouristListForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpExit.SetTitle( "" );
		m_inpEnter.SetTitle( "" );
		m_inpHost.SetTitle( "" );
		m_inpRoute.SetTitle( "" );
		m_inpLeader.SetTitle( "" );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
		m_objClientModel->clear( );
	} // void Clear

	void CHTouristListForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
		m_inpExit.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidExit ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpEnter.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEnter ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpHost.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidHost ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpRoute.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidRoute ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpLeader.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidLeader ) )->GetObject( )->get( "fio" )->toString( )
		) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objClientModel->append( ) );
				row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
				row[ m_pModelFormListClient->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHTouristListForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidExit ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите пункты выезда" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpExit.GetButton( ) ) );
			return 1;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEnter ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите пункты въезда" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEnter.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidHost ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите принимающую сторону" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpHost.GetButton( ) ) );
			return 3;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidRoute ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите маршрут" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpRoute.GetButton( ) ) );
			return 4;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidLeader ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите руководителя" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpLeader.GetButton( ) ) );
			return 5;
		}

		return 0;
	} // udInt Validate

	void CHTouristListForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "exit" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidExit ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "enter" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEnter ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "host" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidHost ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "leader" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidLeader ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "route" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidRoute ) )->GetObject( )->get( "id" )->toUInt( )
		);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTouristListForm::OnFormListClient_Select( ) {
		Gtk::TreeModel::iterator i = m_inpClient.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnClientReplace->set_sensitive( true );
			m_btnClientDel->set_sensitive( true );
		} else {
			m_btnClientReplace->set_sensitive( false );
			m_btnClientDel->set_sensitive( false );
		}
	} // void OnFormListClient_Select

	void CHTouristListForm::OnFormListClient_Add( ) {
		m_pData->MakeObject( );

		udPListHandlerPick handler = GetHClientListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать клиента" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбрать клиента" ) );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj ) {
			udPBase add = new udProjectTouristListLink;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidClient );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "client" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "fio" )->ini( obj->get( "fio" )->toWString( ) );
			add->get( "list" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objClientModel->append( ) );

			row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
			row[ m_pModelFormListClient->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListClient_Add

	void CHTouristListForm::OnFormListClient_Del( ) {
		Gtk::TreeModel::iterator i = m_inpClient.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListClient->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );
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

			m_objClientModel->erase( i );
		}
	} // void OnFormListClient_Del

	void CHTouristListForm::OnFormListClient_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpClient.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListClient->m_col_object ];
			udPListHandlerPick handler = GetHClientListPick( );
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			handler->BeforeCreate( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать клиента" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Выбрать клиента" ) );
			//wnd.add( *( handler->GetWindowPick( ) ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPBase obj = handler->GetObject( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				replace->get( "client" )->ini( obj->get( "id" )->toUInt( ) );
				replace->get( "fio" )->ini( obj->get( "fio" )->toWString( ) );

				row[ m_pModelFormListClient->m_col_fio ] = Glib::locale_to_utf8( replace->get( "fio" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListClient_Replace

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 *	По объекту клиента физ лица, строит узел
	 */
	static udPXmlNode GetClientNode( const wstring& name, udPXmlDoc doc, udPBase obj ) {
		udPXmlNode	ret = doc->CreateNode( name ),
					node1 = NULL,
					node2 = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// ФИО
		node1 = doc->CreateNode( L"fio" );
		node1->SetValue( L"<![CDATA[" + obj->get( "fio" )->toWString( ) + L"]]>" );
		ret->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// ФИО латиницей
		node1 = doc->CreateNode( L"fiolatin" );
		node1->SetValue( L"<![CDATA[" + obj->get( "fiolatin" )->toWString( ) + L"]]>" );
		ret->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// дата рождения
		node1 = doc->CreateNode( L"born" );

		node2 = doc->CreateNode( L"date" );
		node2->SetValue( L"<![CDATA[" + obj->get( "born" )->toTime( ).toWString( "%d.%m.%Y" ) + L"]]>" );
		node1->InsertChild( node2 );

		node2 = doc->CreateNode( L"year" );
		node2->SetValue( L"<![CDATA[" + obj->get( "born" )->toTime( ).toWString( "%Y" ) + L"]]>" );
		node1->InsertChild( node2 );

		node2 = doc->CreateNode( L"month" );
		node2->SetValue( L"<![CDATA[" + obj->get( "born" )->toTime( ).toWString( "%m" ) + L"]]>" );
		node1->InsertChild( node2 );

		node2 = doc->CreateNode( L"day" );
		node2->SetValue( L"<![CDATA[" + obj->get( "born" )->toTime( ).toWString( "%d" ) + L"]]>" );
		node1->InsertChild( node2 );
		
		ret->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// место рождения
		node2 = doc->CreateNode( L"bornplace" );
		node2->SetValue( L"<![CDATA[" + obj->get( "bornplace" )->toWString( ) + L"]]>" );
		ret->InsertChild( node2 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// место рождения латиницей
		node2 = doc->CreateNode( L"bornplacelatin" );
		node2->SetValue( L"<![CDATA[" + obj->get( "bornplacelatin" )->toWString( ) + L"]]>" );
		ret->InsertChild( node2 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // udPXmlNode GetClientNode

	void CHTouristListForm::OnButtonPrint( ) {
		udPXmlDoc doc = new udXmlDoc;
		string xml;
		wstring folder = g_pApp->GetUniqueTmpFolder( );
		udPMorph decl = NULL;
		udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
		udPBase obj = NULL;
		udPArrBase items = NULL;
		//////////////////////////////////////////////////////////////////////////////////
		decl = new udMorph;
		decl->get( "version" )->ini( wstring( L"1.0" ) );
		decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
		doc->AddDeclaration( L"xml", decl );
		udDELETE( decl );
		//////////////////////////////////////////////////////////////////////////////////
		decl = new udMorph;
		decl->get( "type" )->ini( wstring( L"text/xsl" ) );
		decl->get( "href" )->ini( wstring( L"touristlist_out.xsl" ) );
		doc->AddDeclaration( L"xml-stylesheet", decl );
		udDELETE( decl );
		//////////////////////////////////////////////////////////////////////////////////
		obj = GetProjectTouristList( );
		obj->apply( m_pData->GetData( ), true, true );
		node = obj->toXml( doc );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"exit_txt" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidExit ) )->GetObject( )->get( "name" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"exit_txtlatin" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidExit ) )->GetObject( )->get( "namelatin" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"enter_txt" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEnter ) )->GetObject( )->get( "name" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"enter_txtlatin" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEnter ) )->GetObject( )->get( "namelatin" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"host_txt" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidHost ) )->GetObject( )->get( "name" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"host_txtlatin" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidHost ) )->GetObject( )->get( "namelatin" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"route_txt" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidRoute ) )->GetObject( )->get( "name" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"route_txtlatin" );
		node1->SetValue( L"<![CDATA[" +
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidRoute ) )->GetObject( )->get( "namelatin" )->toWString( )
			+ L"]]>" );
		node->InsertChild( node1 );
		//////////////////////////////////////////////////////////////////////////////////
		node->InsertChild( GetClientNode( L"leader", doc, ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidLeader ) )->GetObject( ) ) );
		//////////////////////////////////////////////////////////////////////////////////
		node1 = doc->CreateNode( L"tourist_list" );
		node->InsertChild( node1 );
		items = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidClient ) )->GetArray( );

		udPArrBase pClients = NULL;
		map< udUInt, udUInt > ids;
		udUInt id = 0;
		for( udArrBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			id = ( *i )->get( "client" )->toUInt( );
			ids[ id ] = id;
			g_pApp->GetLog( )->Write( toString( id ) + "\n" );
		}
		pClients = LoadClients( ids );

		if ( pClients && !pClients->empty( ) ) {
			for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
				node1->InsertChild( GetClientNode( L"tourist", doc, *i ) );
			}
		}

		ClearArrBase( pClients );
		udDELETE( pClients );
		//////////////////////////////////////////////////////////////////////////////////
		node->SetAttribute( L"object_title", L"Список туристов" );
		doc->SetRoot( node );
		doc->toString( xml );
		WriteToFile( folder + L"\\out.xml", xml );
		CopyFile( L".\\xsl\\touristlist_out.xsl", ( folder + wstring( L"\\touristlist_out.xsl" ) ).data( ), FALSE );
		ExtOpenFile( folder + L"\\out.xml" );
		//////////////////////////////////////////////////////////////////////////////////
		xml.clear( );
		udDELETE( obj );
		udDELETE( doc );
		//////////////////////////////////////////////////////////////////////////////////
	} // void OnButtonPrint

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTouristListFormModal::CHTouristListFormModal( ) : CHTouristListForm( ) {
	} // CHTouristListFormModal

	CHTouristListFormModal::CHTouristListFormModal( const CHTouristListFormModal& objCopy ) : CHTouristListForm( ) {
	} // CHTouristListFormModal

	CHTouristListFormModal::~CHTouristListFormModal( ) {
	} // ~CHTouristListFormModal

	CHTouristListFormModal& CHTouristListFormModal::operator =( const CHTouristListFormModal& objCopy ) {
		return *this;
	} // CHTouristListFormModal& operator =

	void CHTouristListFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTouristListFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTouristListForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTouristListFormModeless::CHTouristListFormModeless( ) : CHTouristListForm( ) {
	} // CHTouristListFormModeless

	CHTouristListFormModeless::CHTouristListFormModeless( const CHTouristListFormModeless& objCopy ) : CHTouristListForm( ) {
	} // CHTouristListFormModeless

	CHTouristListFormModeless::~CHTouristListFormModeless( ) {
	} // ~CHTouristListFormModeless

	CHTouristListFormModeless& CHTouristListFormModeless::operator =( const CHTouristListFormModeless& objCopy ) {
		return *this;
	} // CHTouristListFormModeless& operator =

	void CHTouristListFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "touristlist" );
	} // void OnButtonOk

	void CHTouristListFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "touristlist" );
	} // void CHTouristListForm

	void CHTouristListFormModeless::Init( ) {
		CHTouristListForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTouristListFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTouristListFormModeless

} // namespace UdSDK
