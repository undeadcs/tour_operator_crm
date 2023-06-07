
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHRequestListPick( ) {
		return new CHRequestPick;
	} // udPListHandlerPick GetHRequestListPick

	udPBase GetProjectRequestList( ) {
		udPBase ret = GetProjectRequest( );
		ret->get( "type_" )->ini( wstring( L"" ) );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		ret->get( "manager2_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectRequestList

	CHRequest::CHRequest( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblFltFio( Glib::locale_to_utf8( "ФИО абонента" ) ),
		m_lblFltDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblFltUser( Glib::locale_to_utf8( "Ответственный" ) ),
		m_lblFltOrder( Glib::locale_to_utf8( "Заявка" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "request";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать обращение" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "order" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHRequest::ColModelRequest( );

		m_pData->SetName( "request" );
		m_pData->SetGetObject( GetProjectRequestList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTRequest );
		pFde->SetMark( fltType );
		m_pData->AddExtraData( pFde );
		m_inpFltType.SetDataObject( pFde );
		m_inpFltType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа" ) );
		m_inpFltType.SetAttr( "name" );
		m_inpFltType.SetGetListHandler( GetHTRequestListPick );
		m_inpFltType.SetGetFormHandler( GetHTRequestForm );
		m_inpFltType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование типа обращения" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
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
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( fltUser );
		m_pData->AddExtraData( pFde );
		m_inpFltUser.SetDataObject( pFde );
		m_inpFltUser.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpFltUser.SetAttr( "fio" );
		m_inpFltUser.SetGetListHandler( GetHUserListPick );
		m_inpFltUser.SetGetFormHandler( GetHUserForm );
		m_inpFltUser.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectOrder );
		pFde->SetMark( fltOrder );
		m_pData->AddExtraData( pFde );
		m_inpFltOrder.SetDataObject( pFde );
		m_inpFltOrder.SetWindowTitle( Glib::locale_to_utf8( "Выбор заявки" ) );
		m_inpFltOrder.SetAttr( "employer_" );
		m_inpFltOrder.SetGetListHandler( GetHOrderListPick );
		m_inpFltOrder.SetGetFormHandler( GetHOrderForm );
		m_inpFltOrder.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование заявки" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Дата с
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата по
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Направление
		m_tblFilter.attach( m_lblFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Тип
		m_tblFilter.attach( m_lblFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltType, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Пользователь
		m_tblFilter.attach( m_lblFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Заявка
		m_tblFilter.attach( m_lblFltOrder, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltOrder, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// ФИО абонента
		m_tblFilter.attach( m_lblFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltFio, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHRequest

	CHRequest::CHRequest( const CHRequest& objCopy ) : udListHandler( ) {
		m_szName = "Request";
	} // CHRequest

	CHRequest::~CHRequest( ) {
	} // ~CRequest

	CHRequest& CHRequest::operator =( const CHRequest& objCopy ) {
		return *this;
	} // CHRequest& operator =

	void CHRequest::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата и время" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тип" ), m_pColModel->m_col_type );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Абонент" ), m_pColModel->m_col_fio );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Направление" ), m_pColModel->m_col_direct );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Ответственный" ), m_pColModel->m_col_manager );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHRequest::Load( ) {
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
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_type ] = Glib::locale_to_utf8( obj->get( "type_" )->toString( ) );
			row[ m_pColModel->m_col_fio ] = Glib::locale_to_utf8( obj->get( "fio" )->toString( ) );
			row[ m_pColModel->m_col_direct ] = Glib::locale_to_utf8( obj->get( "direct_" )->toString( ) );
			row[ m_pColModel->m_col_manager ] = Glib::locale_to_utf8( obj->get( "manager2_" )->toString( ) );
		}
	} // void Load

	void CHRequest::FilterUiClear( ) {
		m_inpFltFio.Reset( );
		m_inpFltType.SetTitle( "" );
		m_inpFltDirect.SetTitle( "" );
		m_inpFltUser.SetTitle( "" );
		m_inpFltOrder.SetTitle( "" );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHRequest::FilterUiToObject( ) {
		m_pData->MakeObject( );

		Glib::ustring	szFio;

		szFio = m_inpFltFio.GetValue( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		filter->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "manager" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltUser ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "order" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltOrder ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRequestModeless::CHRequestModeless( ) :
		udListHandlerModeless( ),
		CHRequest( )
	{
	} // CHRequestModeless

	CHRequestModeless::CHRequestModeless( const CHRequestModeless& objCopy ) : CHRequest( ) {
	} // CHRequestModeless

	CHRequestModeless::~CHRequestModeless( ) {
	} // ~CHRequestModeless

	CHRequestModeless& CHRequestModeless::operator =( const CHRequestModeless& objCopy ) {
		return *this;
	} // CHRequestModeless& operator =

	void CHRequestModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHRequest::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHRequestModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "request_form" );
	} // void OnButtonForm

	void CHRequestModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "request_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "request_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRequestPick::CHRequestPick( ) : CHRequest( ) {
	} // CHRequestPick

	CHRequestPick::CHRequestPick( const CHRequestPick& objCopy ) : CHRequest( ) {
	} // CHRequestPick

	CHRequestPick::~CHRequestPick( ) {
	} // ~CHRequestPick

	CHRequestPick& CHRequestPick::operator =( const CHRequestPick& objCopy ) {
		return *this;
	} // CHRequestPick& operator =

	void CHRequestPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHRequest::Init( );
	} // void CHRequestPick

	void CHRequestPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHRequestFormModal handler = new udHRequestFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание обращения" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание обращения" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHRequestPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectRequest( );
				m_pPickedObject->apply( itObj->second, true, true );
			}

			/*Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectRequest( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );*/
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите обращение" ),
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

	void CHRequestPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHRequestPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHRequestFormModal handler = new udHRequestFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование обращения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование обращения" ) );
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

	udPFormHandler GetHRequestForm( ) {
		return new CHRequestFormModal;
	} // udPFormHandler GetHRequestForm

	CHRequestForm::CHRequestForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Обращение" ) ),
		m_lblFio( Glib::locale_to_utf8( "ФИО абонента" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_lblState( Glib::locale_to_utf8( "Состояние" ) ),
		m_lblType( Glib::locale_to_utf8( "Тип" ) ),
		m_lblDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblManager( Glib::locale_to_utf8( "Ответственный" ) ),
		m_lblResult( Glib::locale_to_utf8( "Результат" ) ),
		m_lblStatus( Glib::locale_to_utf8( "Статус" ) ),
		m_inpStatus( udProjectRequest::REQUEST_STATUS_OPENED, udProjectRequest::REQUEST_STATUS_OPENED )
	{
		m_szName = "request_form";

		m_pData->SetName( "request" );
		m_pData->SetGetObject( GetProjectRequest );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblFio.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_inpFio.set_size_request( 250 );

		udPFDEObject pFde = NULL;

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectSRequest );
		pFde->SetMark( ctrlidState );
		m_pData->AddExtraData( pFde );
		m_inpState.SetDataObject( pFde );
		m_inpState.SetWindowTitle( Glib::locale_to_utf8( "Выбор состояния" ) );
		m_inpState.SetAttr( "name" );
		m_inpState.SetGetListHandler( GetHSRequestListPick );
		m_inpState.SetGetFormHandler( GetHSRequestForm );
		m_inpState.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование состояния обращения" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTRequest );
		pFde->SetMark( ctrlidType );
		m_pData->AddExtraData( pFde );
		m_inpType.SetDataObject( pFde );
		m_inpType.SetWindowTitle( Glib::locale_to_utf8( "Выбор типа" ) );
		m_inpType.SetAttr( "name" );
		m_inpType.SetGetListHandler( GetHTRequestListPick );
		m_inpType.SetGetFormHandler( GetHTRequestForm );
		m_inpType.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование тип обращения" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectDirect );
		pFde->SetMark( ctrlidDirect );
		m_pData->AddExtraData( pFde );
		m_inpDirect.SetDataObject( pFde );
		m_inpDirect.SetWindowTitle( Glib::locale_to_utf8( "Выбор направления" ) );
		m_inpDirect.SetAttr( "name" );
		m_inpDirect.SetGetListHandler( GetHDirectListPick );
		m_inpDirect.SetGetFormHandler( GetHDirectForm );
		m_inpDirect.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование направления" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( ctrlidManager );
		m_pData->AddExtraData( pFde );
		m_inpManager.SetDataObject( pFde );
		m_inpManager.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpManager.SetAttr( "fio" );
		m_inpManager.SetGetListHandler( GetHUserListPick );
		m_inpManager.SetGetFormHandler( GetHUserForm );
		m_inpManager.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );

		pFde = new udFDEObject;
		pFde->SetCallback( GetProjectRRequest );
		pFde->SetMark( ctrlidResult );
		m_pData->AddExtraData( pFde );
		m_inpResult.SetDataObject( pFde );
		m_inpResult.SetWindowTitle( Glib::locale_to_utf8( "Выбор результата" ) );
		m_inpResult.SetAttr( "name" );
		m_inpResult.SetGetListHandler( GetHRRequestListPick );
		m_inpResult.SetGetFormHandler( GetHRRequestForm );
		m_inpResult.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование результата обращения" ) );

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpStatus.AddRow( udProjectRequest::REQUEST_STATUS_OPENED, Glib::locale_to_utf8( "Открыто" ) );
		m_inpStatus.AddRow( udProjectRequest::REQUEST_STATUS_CLOSED, Glib::locale_to_utf8( "Закрыто" ) );
		m_inpStatus.AddRow( udProjectRequest::REQUEST_STATUS_REMIND, Glib::locale_to_utf8( "Требует внимания" ) );
		m_inpStatus.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHRequestForm

	CHRequestForm::CHRequestForm( const CHRequestForm& objCopy ) : udFormHandler( ) {
	} // CHRequestForm

	CHRequestForm::~CHRequestForm( ) {
	} // ~CHRequestForm

	CHRequestForm& CHRequestForm::operator =( const CHRequestForm& objCopy ) {
		return *this;
	} // CHRequestForm& operator =

	void CHRequestForm::Init( ) {
		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// ФИО абонента
		m_tblInput.attach( m_lblFio, 0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFio, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK );

		// Состояние
		m_tblInput.attach( m_lblState, 0, 1, 2, 3, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpState, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, 3, 4, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK );

		// Направление
		m_tblInput.attach( m_lblDirect, 0, 1, 4, 5, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDirect, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK );

		// Ответственный
		m_tblInput.attach( m_lblManager, 0, 1, 5, 6, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpManager, 1, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK );

		// Результат
		m_tblInput.attach( m_lblResult, 0, 1, 6, 7, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpResult, 1, 2, 6, 7, Gtk::SHRINK, Gtk::SHRINK );

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Статус
		m_tblInput.attach( m_lblStatus, 0, 1, 7, 8, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpStatus, 1, 2, 7, 8, Gtk::SHRINK, Gtk::SHRINK );
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 9, 10, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHRequestForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpFio.Reset( );
		m_inpComment.Reset( );
		m_inpState.SetTitle( "" );
		m_inpType.SetTitle( "" );
		m_inpDirect.SetTitle( "" );
		m_inpManager.SetTitle( "" );
		m_inpResult.SetTitle( "" );
		//m_inpStatus.set_active( 0 );
		m_inpStatus.Reset( );
	} // void Clear

	void CHRequestForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpFio.SetValue( Glib::locale_to_utf8( obj->get( "fio" )->toString( ) ) );
		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
		m_inpState.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpType.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpDirect.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpManager.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidManager ) )->GetObject( )->get( "fio" )->toString( )
		) );
		m_inpResult.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidResult ) )->GetObject( )->get( "name" )->toString( )
		) );
		if ( obj->get( "status" )->toInt( ) > 0 ) {
			//m_inpStatus.set_active( obj->get( "status" )->toInt( ) - 1 );
			m_inpStatus.SetValue( obj->get( "status" )->toUInt( ) - 1 );
		}
	} // void Load

	udInt CHRequestForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szFio,
						szComment;

		szFio = m_inpFio.GetValue( );
		if ( szFio.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ФИО" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpFio.GetEntry( ) ) );
			return 1;
		}

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите состояние" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpState.GetButton( ) ) );
			return 2;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите тип" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpType.GetButton( ) ) );
			return 3;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите направление" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDirect.GetButton( ) ) );
			return 4;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidManager ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ответственного" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpManager.GetButton( ) ) );
			return 5;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidResult ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите результат" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpResult.GetButton( ) ) );
			return 5;
		}

		return 0;
	} // udInt Validate

	void CHRequestForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Glib::ustring	szFio,
						szComment;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		szFio = m_inpFio.GetValue( );
		szComment = m_inpComment.GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udUInt status = m_inpStatus.LoadValue( ).GetValue( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "created" )->ini( datetime( 0, true ) );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );
		obj->get( "fio" )->ini( ConvertString( szFio.data( ), CP_UTF8 ) );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "status" )->ini( status );
		obj->get( "state" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "type" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidType ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "manager2" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidManager ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "result" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidResult ) )->GetObject( )->get( "id" )->toUInt( )
		);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRequestFormModal::CHRequestFormModal( ) : CHRequestForm( ) {
	} // CHRequestFormModal

	CHRequestFormModal::CHRequestFormModal( const CHRequestFormModal& objCopy ) : CHRequestForm( ) {
	} // CHRequestFormModal

	CHRequestFormModal::~CHRequestFormModal( ) {
	} // ~CHRequestFormModal

	CHRequestFormModal& CHRequestFormModal::operator =( const CHRequestFormModal& objCopy ) {
		return *this;
	} // CHRequestFormModal& operator =

	void CHRequestFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHRequestFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHRequestForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHRequestFormModeless::CHRequestFormModeless( ) : CHRequestForm( ) {
	} // CHRequestFormModeless

	CHRequestFormModeless::CHRequestFormModeless( const CHRequestFormModeless& objCopy ) : CHRequestForm( ) {
	} // CHRequestFormModeless

	CHRequestFormModeless::~CHRequestFormModeless( ) {
	} // ~CHRequestFormModeless

	CHRequestFormModeless& CHRequestFormModeless::operator =( const CHRequestFormModeless& objCopy ) {
		return *this;
	} // CHRequestFormModeless& operator =

	void CHRequestFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "request" );
	} // void OnButtonOk

	void CHRequestFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "request" );
	} // void CHRequestForm

	void CHRequestFormModeless::Init( ) {
		CHRequestForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHRequestFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHRequestFormModeless

} // namespace UdSDK
