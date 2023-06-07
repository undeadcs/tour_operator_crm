
#include "includes.h"
#include "externals.h"

#include <libxml++/libxml++.h>

namespace UdSDK {

	udPListHandlerPick GetHGroupListPick( ) {
		return new CHGroupPick;
	} // udPListHandlerPick GetHGroupListPick

	udPBase GetProjectGroupList( ) {
		udPBase ret = GetProjectGroup( );
		return ret;
	} // udPBase GetProjectGroupList

	CHGroup::CHGroup( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltName( Glib::locale_to_utf8( "Наименование" ) )
	{
		m_szName = "group";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать группу" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHGroup::ColModelGroup( );

		m_pData->SetName( "group" );
		m_pData->SetGetObject( GetProjectGroupList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Наименование
		m_tblFilter.attach( m_lblFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltName, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHGroup

	CHGroup::CHGroup( const CHGroup& objCopy ) : udListHandler( ) {
		m_szName = "group";
	} // CHGroup

	CHGroup::~CHGroup( ) {
	} // ~CGroup

	CHGroup& CHGroup::operator =( const CHGroup& objCopy ) {
		return *this;
	} // CHGroup& operator =

	void CHGroup::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Места" ), m_pColModel->m_col_placenum );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Периодичность" ), m_pColModel->m_col_lperiod );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Шаблон тура" ), m_pColModel->m_col_tpltour );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHGroup::Load( ) {
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
			//row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
			row[ m_pColModel->m_col_placenum ] = Glib::locale_to_utf8( obj->get( "placenum_" )->toString( ) );
			row[ m_pColModel->m_col_lperiod ] = Glib::locale_to_utf8( obj->get( "lperiod_" )->toString( ) );
			row[ m_pColModel->m_col_tpltour ] = Glib::locale_to_utf8( obj->get( "tpltour_" )->toString( ) );
		}
	} // void Load

	void CHGroup::FilterUiClear( ) {
		m_inpFltName.Reset( );
	} // void FilterUiClear

	void CHGroup::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szName;

		szName = m_inpFltName.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupModeless::CHGroupModeless( ) :
		udListHandlerModeless( ),
		CHGroup( )
	{
	} // CHGroupModeless

	CHGroupModeless::CHGroupModeless( const CHGroupModeless& objCopy ) : CHGroup( ) {
	} // CHGroupModeless

	CHGroupModeless::~CHGroupModeless( ) {
	} // ~CHGroupModeless

	CHGroupModeless& CHGroupModeless::operator =( const CHGroupModeless& objCopy ) {
		return *this;
	} // CHGroupModeless& operator =

	void CHGroupModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHGroup::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHGroupModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "group_form" );
	} // void OnButtonForm

	void CHGroupModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "group_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "group_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupPick::CHGroupPick( ) : CHGroup( ) {
	} // CHGroupPick

	CHGroupPick::CHGroupPick( const CHGroupPick& objCopy ) : CHGroup( ) {
	} // CHGroupPick

	CHGroupPick::~CHGroupPick( ) {
	} // ~CHGroupPick

	CHGroupPick& CHGroupPick::operator =( const CHGroupPick& objCopy ) {
		return *this;
	} // CHGroupPick& operator =

	void CHGroupPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHGroup::Init( );
	} // void CHGroupPick

	void CHGroupPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHGroupFormModal handler = new udHGroupFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание группы" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание группы" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHGroupPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectGroup( );
				//m_pPickedObject->apply( itObj->second, true, true );
				itObj->second->apply( m_pPickedObject );
			}
			/*Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectOrder( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );*/
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите группу" ),
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

	void CHGroupPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHGroupPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHGroupFormModal handler = new udHGroupFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование группы" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование группы" ) );
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

	udPFormHandler GetHGroupForm( ) {
		return new CHGroupFormModal;
	} // udPFormHandler GetHGroupForm

	CHGroupForm::CHGroupForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Группа" ) ),
		m_lblName( Glib::locale_to_utf8( "Наименование" ) ),
		m_lblDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_lblApp( Glib::locale_to_utf8( "Приложения" ) ),
		m_lblTour( Glib::locale_to_utf8( "Туры" ) ),
		m_lblPeriod( Glib::locale_to_utf8( "Периодичность" ) ),
		m_lblTplTour( Glib::locale_to_utf8( "Шаблоны туров" ) ),
		m_btnExport( Glib::locale_to_utf8( "Экспорт данных туристов" ) )
	{
		m_szName = "group_form";

		m_pData->SetName( "group" );
		m_pData->SetGetObject( GetProjectGroup );

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
		// туры
		m_inpTour.set_size_request( 400, 200 );
		m_pModelFormListTour = new ColModelFormListTour;
		m_objTourModel = Gtk::ListStore::create( *m_pModelFormListTour );
		
		pTree = m_inpTour.GetTreeView( );
		pTree->set_model( m_objTourModel );
		pTree->append_column( Glib::locale_to_utf8( "Тур" ), m_pModelFormListTour->m_col_name );
		pTree->append_column( Glib::locale_to_utf8( "Период" ), m_pModelFormListTour->m_col_date1 );
		m_inpTour.SetColumnDefaults( );

		m_btnTourEdit = m_inpTour.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnTourEdit->set_sensitive( false );
		m_btnTourEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTour_Edit ) );

		m_btnTourDel = m_inpTour.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnTourDel->set_sensitive( false );
		m_btnTourDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTour_Del ) );

		m_btnTourAdd = m_inpTour.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnTourAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTour_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTour_Select ) );
		pTree->signal_row_activated( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTour_Activate ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectGroupTourList );
		pFdeList->SetCallbackSend( GetProjectGroupTour );
		pFdeList->SetMark( ctrlidTour );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// периоды
		m_inpPeriod.set_size_request( 400, 200 );
		m_pModelFormListPeriod = new ColModelFormListPeriod;
		m_objPeriodModel = Gtk::ListStore::create( *m_pModelFormListPeriod );
		
		pTree = m_inpPeriod.GetTreeView( );
		pTree->set_model( m_objPeriodModel );
		pTree->append_column( Glib::locale_to_utf8( "Дата начала" ), m_pModelFormListPeriod->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "Дата окончания" ), m_pModelFormListPeriod->m_col_date2 );
		pTree->append_column( Glib::locale_to_utf8( "Минимум человек" ), m_pModelFormListPeriod->m_col_min );
		pTree->append_column( Glib::locale_to_utf8( "Максимум человек" ), m_pModelFormListPeriod->m_col_max );
		pTree->append_column( Glib::locale_to_utf8( "Руководитель" ), m_pModelFormListPeriod->m_col_client );
		m_inpPeriod.SetColumnDefaults( );

		m_btnPeriodEdit = m_inpPeriod.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnPeriodEdit->set_sensitive( false );
		m_btnPeriodEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListPeriod_Edit ) );

		m_btnPeriodDel = m_inpPeriod.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnPeriodDel->set_sensitive( false );
		m_btnPeriodDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListPeriod_Del ) );

		m_btnPeriodAdd = m_inpPeriod.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnPeriodAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListPeriod_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListPeriod_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectGroupPeriodList );
		pFdeList->SetCallbackSend( GetProjectGroupPeriod );
		pFdeList->SetMark( ctrlidPeriod );
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
		m_btnAppEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListApp_Edit ) );

		m_btnAppDel = m_inpApp.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnAppDel->set_sensitive( false );
		m_btnAppDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListApp_Del ) );

		m_btnAppReplace = m_inpApp.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnAppReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListApp_Replace ) );
		m_btnAppReplace->set_sensitive( false );

		m_btnAppAdd = m_inpApp.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnAppAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListApp_Add ) );

		m_btnFolder = m_inpApp.AddButton( Glib::locale_to_utf8( "Открыть папку" ) );
		m_btnFolder->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListApp_Folder ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListApp_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectAttach );
		pFdeList->SetMark( ctrlidApp );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// шаблоны туров
		m_inpTplTour.set_size_request( 400, 200 );
		m_pModelFormListTplTour = new ColModelFormListTplTour;
		m_objTplTourModel = Gtk::ListStore::create( *m_pModelFormListTplTour );
		
		pTree = m_inpTplTour.GetTreeView( );
		pTree->set_model( m_objTplTourModel );
		pTree->append_column( Glib::locale_to_utf8( "Наименование" ), m_pModelFormListTplTour->m_col_name );
		m_inpTplTour.SetColumnDefaults( );

		m_btnTplTourReplace = m_inpTplTour.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnTplTourReplace->set_sensitive( false );
		m_btnTplTourReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTplTour_Replace ) );

		m_btnTplTourDel = m_inpTplTour.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnTplTourDel->set_sensitive( false );
		m_btnTplTourDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTplTour_Del ) );

		m_btnTplTourAdd = m_inpTplTour.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnTplTourAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTplTour_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHGroupForm::OnFormListTplTour_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectGroupTplTourList );
		pFdeList->SetCallbackSend( GetProjectGroupTplTour );
		pFdeList->SetMark( ctrlidTplTour );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btnExport.signal_clicked( ).connect( sigc::mem_fun( this, &CHGroupForm::OnButtonExportClick ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHGroupForm

	CHGroupForm::CHGroupForm( const CHGroupForm& objCopy ) : udFormHandler( ) {
	} // CHGroupForm

	CHGroupForm::~CHGroupForm( ) {
	} // ~CHGroupForm

	CHGroupForm& CHGroupForm::operator =( const CHGroupForm& objCopy ) {
		return *this;
	} // CHGroupForm& operator =

	void CHGroupForm::Init( ) {
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

		// Направление
		m_tblInput.attach( m_lblDirect, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDirect, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Приложения
		m_tblInput.attach( m_lblApp, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpApp, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Туры
		m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpTour, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Периоды
		m_tblInput.attach( m_lblPeriod, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPeriod, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Шаблоны туров
		m_tblInput.attach( m_lblTplTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpTplTour, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnExport, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHGroupForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpName.Reset( );
		m_inpDirect.SetTitle( "" );
		m_inpComment.Reset( );
		m_objAppModel->clear( );
		m_objPeriodModel->clear( );
		m_objTplTourModel->clear( );
		m_objTourModel->clear( );
	} // void Clear

	void CHGroupForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpName.SetValue( Glib::locale_to_utf8( obj->get( "name" )->toString( ) ) );
		m_inpDirect.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		map< udUInt, udPBase > mapPeriod;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objPeriodModel->append( ) );
				row[ m_pModelFormListPeriod->m_col_date1 ] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListPeriod->m_col_date2 ] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListPeriod->m_col_min ] = Glib::locale_to_utf8( add->get( "min" )->toString( ) );
				row[ m_pModelFormListPeriod->m_col_max ] = Glib::locale_to_utf8( add->get( "max" )->toString( ) );
				row[ m_pModelFormListPeriod->m_col_client ] = Glib::locale_to_utf8( add->get( "client_" )->toString( ) );
				row[ m_pModelFormListPeriod->m_col_object ] = add;

				mapPeriod[ add->get( "index" )->toUInt( ) ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;
			map< udUInt, udPBase >::iterator itFind;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objTourModel->append( ) );
				row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListTour->m_col_object ] = add;

				itFind = mapPeriod.find( add->get( "period" )->toUInt( ) );
				if ( itFind != mapPeriod.end( ) ) {
					row[ m_pModelFormListTour->m_col_date1 ] = Glib::locale_to_utf8( itFind->second->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTplTour ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objTplTourModel->append( ) );
				row[ m_pModelFormListTplTour->m_col_name ] = Glib::locale_to_utf8( add->get( "tpltour_" )->toString( ) );
				row[ m_pModelFormListTplTour->m_col_object ] = add;
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
				row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListApp->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHGroupForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring szName;

		szName = m_inpName.GetValue( );
		if ( szName.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите наименование" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpName.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите направление" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDirect.GetButton( ) ) );
			return 2;
		}

		return 0;
	} // udInt Validate

	void CHGroupForm::Send( ) {
		Glib::ustring	szName,
						szComment;
		
		szName = m_inpName.GetValue( );
		szComment = m_inpComment.GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
		obj->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );

		m_pData->Send( ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( ) );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHGroupForm::OnFormListApp_Select( ) {
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

	void CHGroupForm::OnFormListApp_Add( ) {
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
				add->get( "type" )->ini( udProjectAttach::ATTACH_GROUP );
				add->get( "name" )->ini( szFileName );
				add->get( "orig" )->ini( szFilePath );

				wstring wFileTmpPath = m_szFolderApp + L"\\" + szFileName;
				Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

				if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
					add->get( "tmp_path" )->ini( wFileTmpPath );
					arrObject->push_back( add );

					Gtk::TreeModel::iterator i = m_objAppModel->append( );
					Gtk::TreeModel::Row row = *i;
					Gtk::TreePath objPath = m_objAppModel->get_path( i );

					row[ m_pModelFormListApp->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListApp->m_col_object] = add;
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

	void CHGroupForm::OnFormListApp_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListApp->m_col_object ];
			ExtOpenFile( edit->get( "tmp_path" )->toWString( ), false );
		}
	} // void OnFormListApp_Edit

	void CHGroupForm::OnFormListApp_Del( ) {
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

	void CHGroupForm::OnFormListApp_Replace( ) {
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

	void CHGroupForm::OnFormListApp_Folder( ) {
		ExtOpenFolder( m_szFolderApp );
	} // void OnFormListApp_Folder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHGroupForm::OnFormListPeriod_Select( ) {
		Gtk::TreeModel::iterator i = m_inpPeriod.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnPeriodEdit->set_sensitive( true );
			m_btnPeriodDel->set_sensitive( true );
		} else {
			m_btnPeriodEdit->set_sensitive( false );
			m_btnPeriodDel->set_sensitive( false );
		}
	} // void OnFormListPeriod_Select

	void CHGroupForm::OnFormListPeriod_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHGroupPeriodFormModal handler = new udHGroupPeriodFormModal;
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
			udPBase add = new udProjectGroupPeriod;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			arrObject->push_back( add );

			add->get( "group" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			add->get( "index" )->ini( GetPeriodLastIndex( ) + 1 );

			Gtk::TreeModel::Row row = *( m_objPeriodModel->append( ) );

			udPFDEObject pFdeClient = ( udPFDEObject ) pDa->GetExtraData( udHGroupPeriodForm::ctrlidClient );
			if ( pFdeClient ) {
				udPBase pClient = pFdeClient->GetObject( );
				if ( pClient ) {
					row[ m_pModelFormListPeriod->m_col_client ] = Glib::locale_to_utf8( pClient->get( "fio" )->toString( ) );
				}
			}

			row[ m_pModelFormListPeriod->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListPeriod->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListPeriod->m_col_min		] = Glib::locale_to_utf8( add->get( "min" )->toString( ) );
			row[ m_pModelFormListPeriod->m_col_max		] = Glib::locale_to_utf8( add->get( "max" )->toString( ) );
			row[ m_pModelFormListPeriod->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListPeriod_Add

	void CHGroupForm::OnFormListPeriod_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpPeriod.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListPeriod->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHGroupPeriodFormModal handler = new udHGroupPeriodFormModal;
			handler->Init( );
			handler->LoadFrom( edit );

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

				edit->apply( obj, true, true );

				udPFDEObject pFdeClient = ( udPFDEObject ) pDa->GetExtraData( udHGroupPeriodForm::ctrlidClient );
				if ( pFdeClient ) {
					udPBase pClient = pFdeClient->GetObject( );
					if ( pClient ) {
						row[ m_pModelFormListPeriod->m_col_client ] = Glib::locale_to_utf8( pClient->get( "fio" )->toString( ) );
					}
				}

				row[ m_pModelFormListPeriod->m_col_date1	] = Glib::locale_to_utf8( edit->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListPeriod->m_col_date2	] = Glib::locale_to_utf8( edit->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListPeriod->m_col_min		] = Glib::locale_to_utf8( edit->get( "min" )->toString( ) );
				row[ m_pModelFormListPeriod->m_col_max		] = Glib::locale_to_utf8( edit->get( "max" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListPeriod_Edit

	void CHGroupForm::OnFormListPeriod_Del( ) {
		Gtk::TreeModel::iterator i = m_inpPeriod.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListPeriod->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del  ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objPeriodModel->erase( i );
		}
	} // void OnFormListPeriod_Del

	udUInt CHGroupForm::GetPeriodLastIndex( ) {
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

	void CHGroupForm::SavePeriod( udPHGroupPeriodForm pForm ) {
		udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidPeriod ) )->GetArray( );
		udPBase obj	= pForm->GetData( )->GetData( ),
				add = GetProjectGroupPeriod( );

		add->apply( obj, true, true );
		arrObject->push_back( add );

		if ( add->get( "index" )->toUInt( ) == 0 ) { // новый период
			add->get( "index" )->ini( GetPeriodLastIndex( ) + 1 );
		} else { // старый период, необходимо многое обновить
		}

		Gtk::TreeModel::Row row = *( m_objPeriodModel->append( ) );

		udPFDEObject pFdeClient = ( udPFDEObject ) pForm->GetData( )->GetExtraData( udHGroupPeriodForm::ctrlidClient );
		if ( pFdeClient ) {
			udPBase pClient = pFdeClient->GetObject( );
			if ( pClient ) {
				row[ m_pModelFormListPeriod->m_col_client ] = Glib::locale_to_utf8( pClient->get( "fio" )->toString( ) );
			}
		}

		row[ m_pModelFormListPeriod->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
		row[ m_pModelFormListPeriod->m_col_date2	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
		row[ m_pModelFormListPeriod->m_col_min		] = Glib::locale_to_utf8( add->get( "min" )->toString( ) );
		row[ m_pModelFormListPeriod->m_col_max		] = Glib::locale_to_utf8( add->get( "max" )->toString( ) );
		row[ m_pModelFormListPeriod->m_col_object	] = add;
	} // void SavePeriod

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHGroupForm::OnFormListTplTour_Select( ) {
		Gtk::TreeModel::iterator i = m_inpTplTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnTplTourReplace->set_sensitive( true );
			m_btnTplTourDel->set_sensitive( true );
		} else {
			m_btnTplTourReplace->set_sensitive( false );
			m_btnTplTourDel->set_sensitive( false );
		}
	} // void OnFormListTplTour_Select

	void CHGroupForm::OnFormListTplTour_Add( ) {
		m_pData->MakeObject( );

		udPListHandlerPick handler = GetHTplTourListPick( );
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		handler->BeforeCreate( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать шаблон тура" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Выбрать шаблон тура" ) );
		//wnd.add( *( handler->GetWindowPick( ) ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPBase obj = handler->GetObject( );
		if ( obj ) {
			udPBase add = new udProjectGroupTplTour;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidTplTour );
			udPArrBase arrObject = pFde->GetArray( );

			add->get( "group" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			add->get( "tpltour" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "name" )->ini( obj->get( "name" )->toWString( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objTplTourModel->append( ) );

			row[ m_pModelFormListTplTour->m_col_name ] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
			row[ m_pModelFormListTplTour->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListTplTour_Add

	void CHGroupForm::OnFormListTplTour_Del( ) {
		Gtk::TreeModel::iterator i = m_inpTplTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListTplTour->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTplTour ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del  ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
			}

			m_objTplTourModel->erase( i );
		}
	} // void OnFormListTplTour_Del

	void CHGroupForm::OnFormListTplTour_Replace( ) {
		Gtk::TreeModel::iterator i = m_inpTplTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase replace = row[ m_pModelFormListTplTour->m_col_object ];
			udPListHandlerPick handler = GetHTplTourListPick( );
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			handler->BeforeCreate( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Выбрать шаблон тура" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Выбрать шаблон тура" ) );
			//wnd.add( *( handler->GetWindowPick( ) ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPBase obj = handler->GetObject( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				replace->get( "tpltour" )->ini( obj->get( "id" )->toUInt( ) );
				replace->get( "name" )->ini( obj->get( "name" )->toWString( ) );

				row[ m_pModelFormListTplTour->m_col_name ] = Glib::locale_to_utf8( replace->get( "name" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListTplTour_Replace

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHGroupForm::OnFormListTour_Select( ) {
		Gtk::TreeModel::iterator i = m_inpTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnTourEdit->set_sensitive( true );
			m_btnTourDel->set_sensitive( true );
		} else {
			m_btnTourEdit->set_sensitive( false );
			m_btnTourDel->set_sensitive( false );
		}
	} // void OnFormListTour_Select

	void CHGroupForm::OnFormListTour_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTourModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			Gtk::TreeModel::Row row = *i;
			udPBase obj = row[ m_pModelFormListTour->m_col_object ];

			if ( obj ) {
				string log = "\n";
				obj->dump( log );
				g_pApp->GetLog( )->Write( log );

				Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
				Gtk::ScrolledWindow wndScroll;
				udPHTourFormModal handler = new udHTourFormModal( );
				handler->Init( );
				handler->GetData( )->SetId( obj->get( "tour" )->toUInt( ) );
				handler->Load( );

				ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование тура" ) );

				/*Gtk::Window wnd;
				wnd.set_size_request( 800, 600 );
				wnd.set_modal( true );
				wnd.set_title( Glib::locale_to_utf8( "Редактирование тура" ) );
				//wnd.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				//wnd.show_all( );
				wnd.add( wndScroll );
				wndScroll.add( *( handler->GetWindow( ) ) );
				wnd.set_transient_for( *wndParent );
				handler->GetWindow( )->show( );
				wndScroll.show( );
				wnd.show( );
				Gtk::Main::run( wnd );*/

				udPBase tour = handler->GetData( )->GetData( );
				if ( tour ) {
					obj->get( "name" )->ini( tour->get( "name" )->toWString( ) );

					row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
				}

				udDELETE( handler );
			}
		}
	} // void OnFormListClient_Activate

	void CHGroupForm::OnFormListTour_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHGroupTourFormModal handler = new udHGroupTourFormModal( this );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание связи с туром" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание связи с туром" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGroupTour;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidTour );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			arrObject->push_back( add );

			Gtk::TreeModel::iterator i = m_objTourModel->append( );
			Gtk::TreeModel::Row row = *i;
			Gtk::TreePath objPath = m_objTourModel->get_path( i );

			udPFDEObject pFdePeriod = ( udPFDEObject ) pDa->GetExtraData( udHGroupTourForm::ctrlidPeriod );
			if ( pFdePeriod ) {
				udPBase pPeriod = pFdePeriod->GetObject( );
				if ( pPeriod ) {
					row[ m_pModelFormListTour->m_col_date1 ] = Glib::locale_to_utf8( pPeriod->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				}
			}

			udPFDEObject pFdeTour = ( udPFDEObject ) pDa->GetExtraData( udHGroupTourForm::ctrlidTour );
			if ( pFdeTour ) {
				udPBase pTour = pFdeTour->GetObject( );
				if ( pTour ) {
					row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( pTour->get( "name" )->toString( ) );
				}
			}

			row[ m_pModelFormListTour->m_col_object ] = add;
		}

		udDELETE( handler );
	} // void OnFormListTour_Add

	void CHGroupForm::OnFormListTour_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListTour->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHGroupTourFormModal handler = new udHGroupTourFormModal( this );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование связи с туром" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование связи с туром" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				udPFDEObject pFdePeriod = ( udPFDEObject ) pDa->GetExtraData( udHGroupTourForm::ctrlidPeriod );
				if ( pFdePeriod ) {
					udPBase pPeriod = pFdePeriod->GetObject( );
					if ( pPeriod ) {
						row[ m_pModelFormListTour->m_col_date1 ] = Glib::locale_to_utf8( pPeriod->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
					}
				}

				udPFDEObject pFdeTour = ( udPFDEObject ) pDa->GetExtraData( udHGroupTourForm::ctrlidTour );
				if ( pFdeTour ) {
					udPBase pTour = pFdeTour->GetObject( );
					if ( pTour ) {
						row[ m_pModelFormListTour->m_col_name ] = Glib::locale_to_utf8( pTour->get( "name" )->toString( ) );
					}
				}
			}

			udDELETE( handler );
		}
	} // void OnFormListTour_Edit

	void CHGroupForm::OnFormListTour_Del( ) {
		Gtk::TreeModel::iterator i = m_inpTour.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListTour->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
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

			m_objTourModel->erase( i );
		}
	} // void OnFormListTour_Del

	static wstring BornToRuDate( const wstring& szBorn ) {
		wstring ret( L"" );

		if ( szBorn.length( ) == 10 ) {
			ret = szBorn.substr( 8, 2 ) + L"." +
				szBorn.substr( 5, 2 ) + L"." +
				szBorn.substr( 0, 4 );
		}

		return ret;
	} // wstring BornToRuDate

	enum {
		splitFioCurrentLastName,
		splitFioCurrentFirstName,
		splitFioCurrentSecondName
	};

	static void SplitFio( const wstring& szFio, wstring& szLastName, wstring& szFirstName, wstring& szSecondName ) {
		udWChar ch, lastch;
		udInt current = splitFioCurrentLastName;

		for( wstring::const_iterator i = szFio.begin( ); i != szFio.end( ); ++i ) {
			ch = *i;

			if ( ch == L' ' ) {
				if ( lastch != ch ) { // чтобы исключить повторные пробелы
					++current;
				}
			} else {
				switch( current ) {
					case splitFioCurrentLastName:	szLastName += ch;	break;
					case splitFioCurrentFirstName:	szFirstName += ch;	break;
					case splitFioCurrentSecondName:	szSecondName += ch;	break;
					default:
						break;
				}
			}

			lastch = ch;
		}
	} // void SplitFio

	void CHGroupForm::OnButtonExportClick( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPBase pPeriod = NULL;
		udPHGroupPeriodPick hPick = new udHGroupPeriodPick;
		hPick->SetGroupForm( this );
		hPick->BeforeCreate( );

		ModalWindow( wndParent, hPick->GetWindow( ), Glib::locale_to_utf8( "Выбор периода" ) );

		udPBase obj = hPick->GetObject( );
		if ( obj ) {
			pPeriod = new udProjectGroupPeriod;
			obj->apply( pPeriod );
		}

		udDELETE( hPick );

		if ( pPeriod ) {
			udArrBase arrTourist, arrDocument;
			// 1 обращение к серверу за списком туристов
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				// ListRequest
				udBinaryData data;
				data.add( string( "group" ) );
				data.add( ( udInt ) opidGetTouristList );
				data.add( pPeriod->get( "group" )->toUInt( ) );
				data.add( pPeriod->get( "index" )->toUInt( ) );

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );

				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code > 0 ) { // необходимо считать объекты
					udDataAccess::Read( sock, arrTourist, GetProjectClient );
					udDataAccess::Read( sock, arrDocument, GetProjectClDoc );
				}

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );

				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}

			if ( !arrTourist.empty( ) ) {
				// 2 составление документа
				wstring szFilename	= g_pApp->GetUniqueTmpFolder( ) + L"\\out.xml",
						tmp			= L"",
						fio			= L"",
						born		= L"",
						szLastName, szFirstName, szSecondName;
				udPBase tourist		= NULL,
						document	= NULL;
				udUInt	iDocId		= 0;

				tmp = L"<?xml version=\"1.0\" encoding=\"Windows-1251\"?>\n<list name=\"people\" title=\"Персональные данные\" id_lbd=\"279\" count=\"1\">\n";

				for( udArrBase::iterator i = arrTourist.begin( ); i != arrTourist.end( ); ++i ) {
					document = NULL;
					tourist = *i;

					iDocId = tourist->get( "doc" )->toUInt( );
					if ( iDocId ) {
						for( udArrBase::iterator j = arrDocument.begin( ); j != arrDocument.end( ); ++j ) {
							if ( iDocId == ( *j )->get( "id" )->toUInt( ) ) {
								document = *j;
								break;
							}
						}
					}

					fio = tourist->get( "fio" )->toWString( );
					SplitFio( fio, szLastName, szFirstName, szSecondName );

					tmp += L"\t<pr_people id=\"\" "
						L"f=\"" + szLastName + L"\" i=\"" + szFirstName + L"\" o=\"" + szSecondName + L"\" ";

					szLastName = szFirstName = szSecondName = L"";

					fio = tourist->get( "fiolatin" )->toWString( );
					SplitFio( fio, szLastName, szFirstName, szSecondName );

					tmp += L"ef=\"" + szLastName + L"\" ei=\"" + szFirstName + L"\" "
						L"birthplace=\"" + tourist->get( "bornplacelatin" )->toWString( ) + L"/" + tourist->get( "bornplace" )->toWString( ) +
						L"\" birthdate=\"" + BornToRuDate( tourist->get( "born" )->toWString( ) ) + L"\" "
						L"addr=\"" + tourist->get( "laddr" )->toWString( ) + L"\" ";

					if ( document ) {
						tmp += L"doc_series=\"" + document->get( "series" )->toWString( ) + L"\" "
							L"doc_number=\"" + document->get( "number" )->toWString( ) + L"\" "
							L"doc_date_stop=\"" + document->get( "enddate" )->toWString( L"%d.%m.%Y" ) + L"\" "
							L"doc_date_start=\"" + document->get( "issuedate" )->toWString( L"%d.%m.%Y" ) + L"\" ";
					} else {
						tmp += L"doc_series=\"\" doc_number=\"\" doc_date_stop=\"\" doc_date_start=\"\" ";
					}

					tmp += L"email=\"" + tourist->get( "lemail" )->toWString( ) + L"\" "
						L"is_male=\"" + ( tourist->get( "sex" )->toUInt( ) == udProjectClient::SEX_MAN ? L"1" : L"0" ) + L"\" "
						L"uid=\"\" "
						L"phone=\"" + tourist->get( "lphone" )->toWString( ) + L"\" "
						L"is_actual=\"1\">\n"
						L"\t\t<nationality id=\"643\" ref=\"dc_oksm\" name=\"РОССИЯ\"/>\n"
						L"\t\t<doc_country id=\"643\" ref=\"dc_oksm\" name=\"РОССИЯ\"/>\n"
						L"\t\t<dc_doc_type id=\"1\" name=\"Общегражданский заграничный паспорт\"/>\n"
						L"\t\t<pr_black ref_pr_people=\"\" id=\"\" uid=\"\" hcode=\"\" date_in=\"\" ref_st_org_structure=\"\" info=\"\"/>\n"
						L"\t</pr_people>\n";

					szLastName = szFirstName = szSecondName = L"";
				}

				tmp += L"</list>";

				WriteToFile( szFilename, Glib::convert( ConvertString( tmp, CP_UTF8 ), "Windows-1251", "UTF-8" ) );
				ExtOpenFile( szFilename );
				
				ClearArrBase( arrTourist );
			}
		}
	} // void OnButtonExportClick

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupFormModal::CHGroupFormModal( ) : CHGroupForm( ) {
	} // CHGroupFormModal

	CHGroupFormModal::CHGroupFormModal( const CHGroupFormModal& objCopy ) : CHGroupForm( ) {
	} // CHGroupFormModal

	CHGroupFormModal::~CHGroupFormModal( ) {
	} // ~CHGroupFormModal

	CHGroupFormModal& CHGroupFormModal::operator =( const CHGroupFormModal& objCopy ) {
		return *this;
	} // CHGroupFormModal& operator =

	void CHGroupFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHGroupFormModal::OnButtonCancel( ) {
		m_pData->Clear( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHGroupForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHGroupFormModeless::CHGroupFormModeless( ) : CHGroupForm( ) {
	} // CHGroupFormModeless

	CHGroupFormModeless::CHGroupFormModeless( const CHGroupFormModeless& objCopy ) : CHGroupForm( ) {
	} // CHGroupFormModeless

	CHGroupFormModeless::~CHGroupFormModeless( ) {
	} // ~CHGroupFormModeless

	CHGroupFormModeless& CHGroupFormModeless::operator =( const CHGroupFormModeless& objCopy ) {
		return *this;
	} // CHGroupFormModeless& operator =

	void CHGroupFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "group" );
	} // void OnButtonOk

	void CHGroupFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "group" );
	} // void CHGroupForm

	void CHGroupFormModeless::Init( ) {
		CHGroupForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHGroupFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHGroupFormModeless

} // namespace UdSDK
