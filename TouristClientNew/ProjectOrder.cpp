
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHOrderListPick( ) {
		return new CHOrderPick;
	} // udPListHandlerPick GetHOrderListPick

	udPBase GetProjectOrderList( ) {
		udPBase ret = GetProjectOrder( );
		ret->get( "manager2_" )->ini( wstring( L"" ) );
		ret->get( "employer_" )->ini( wstring( L"" ) );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		ret->get( "status_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectOrderList

	CHOrder::CHOrder( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltUser( Glib::locale_to_utf8( "Пользователь" ) ),
		m_lblFltAmount1( Glib::locale_to_utf8( "Количество мест от" ) ),
		m_lblFltAmount2( Glib::locale_to_utf8( "до" ) ),
		m_lblFltEmployer( Glib::locale_to_utf8( "Заказчик" ) ),
		m_lblFltDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblFltRequest( Glib::locale_to_utf8( "Обращение" ) ),
		m_lblFltTour( Glib::locale_to_utf8( "Тур" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "order";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать заявку" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "amount1" )->ini( ( udDWord ) 0 );
		filter->get( "amount2" )->ini( ( udDWord ) 1000000 );
		filter->get( "employer" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "request" )->ini( ( udUInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );

		m_pRowModel = m_pColModel = new CHOrder::ColModelOrder( );

		m_pData->SetName( "order" );
		m_pData->SetGetObject( GetProjectOrderList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
		pFde->SetCallback( GetProjectRequest );
		pFde->SetMark( fltRequest );
		m_pData->AddExtraData( pFde );
		m_inpFltRequest.SetDataObject( pFde );
		m_inpFltRequest.SetWindowTitle( Glib::locale_to_utf8( "Выбор обращения" ) );
		m_inpFltRequest.SetAttr( "fio" );
		m_inpFltRequest.SetGetListHandler( GetHRequestListPick );
		m_inpFltRequest.SetGetFormHandler( GetHRequestForm );
		m_inpFltRequest.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование обращения" ) );
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
		udPFDEEmployer pFdeEmployer = new udFDEEmployer;
		pFdeEmployer->SetMark( fltEmployer );
		m_pData->AddExtraData( pFdeEmployer );
		m_inpFltEmployer = new udWgtEmployerPick;
		m_inpFltEmployer->SetDataObject( pFdeEmployer );
		m_inpFltEmployer->SetWindowTitle( Glib::locale_to_utf8( "Выбор заказчика" ) );
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

		// Количество человек с
		m_tblFilter.attach( m_lblFltAmount1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltAmount1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Количество человек по
		m_tblFilter.attach( m_lblFltAmount2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltAmount2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Пользователь
		m_tblFilter.attach( m_lblFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltUser, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Направление
		m_tblFilter.attach( m_lblFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDirect, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Заказчик
		m_tblFilter.attach( m_lblFltEmployer, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( *m_inpFltEmployer, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Обращение
		m_tblFilter.attach( m_lblFltRequest, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltRequest, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Тур
		m_tblFilter.attach( m_lblFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHOrder

	CHOrder::CHOrder( const CHOrder& objCopy ) : udListHandler( ) {
		m_szName = "order";
	} // CHOrder

	CHOrder::~CHOrder( ) {
	} // ~COrder

	CHOrder& CHOrder::operator =( const CHOrder& objCopy ) {
		return *this;
	} // CHOrder& operator =

	void CHOrder::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата и время" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Ответственный" ), m_pColModel->m_col_manager );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Статус" ), m_pColModel->m_col_status );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Количество мест" ), m_pColModel->m_col_amount );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Заказчик" ), m_pColModel->m_col_employer );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Направление" ), m_pColModel->m_col_direct );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHOrder::Load( ) {
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
			row[ m_pColModel->m_col_manager ] = Glib::locale_to_utf8( obj->get( "manager2_" )->toString( ) );
			row[ m_pColModel->m_col_status ] = Glib::locale_to_utf8( obj->get( "status_" )->toString( ) );
			row[ m_pColModel->m_col_amount ] = Glib::locale_to_utf8( obj->get( "amount" )->toString( ) );
			row[ m_pColModel->m_col_employer ] = Glib::locale_to_utf8( obj->get( "employer_" )->toString( ) );
			row[ m_pColModel->m_col_direct ] = Glib::locale_to_utf8( obj->get( "direct_" )->toString( ) );
		}
	} // void Load

	void CHOrder::FilterUiClear( ) {
		m_inpFltUser.SetTitle( "" );
		m_inpFltDirect.SetTitle( "" );
		m_inpFltRequest.SetTitle( "" );
		m_inpFltTour.SetTitle( "" );
		m_inpFltEmployer->SetTitle( "" );
		m_inpFltAmount1.Reset( );
		m_inpFltAmount2.Reset( );
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
	} // void FilterUiClear

	void CHOrder::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udDWord		dwAmount1,
					dwAmount2;

		dwAmount1 = toDWord( m_inpFltAmount1.GetValue( ).data( ) );
		dwAmount2 = toDWord( m_inpFltAmount2.GetValue( ).data( ) );
		
		udPMorph filter = m_pData->GetFilter( );
		
		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate2.GetValue( ) );
		filter->get( "amount1" )->ini( dwAmount1 );
		filter->get( "amount2" )->ini( dwAmount2 );
		filter->get( "manager" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltUser ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "request" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltRequest ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "tour" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltTour ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "emptype" )->ini( m_inpFltEmployer->GetType( ) );
		filter->get( "employer" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltEmployer ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHOrderModeless::CHOrderModeless( ) :
		udListHandlerModeless( ),
		CHOrder( )
	{
	} // CHOrderModeless

	CHOrderModeless::CHOrderModeless( const CHOrderModeless& objCopy ) : CHOrder( ) {
	} // CHOrderModeless

	CHOrderModeless::~CHOrderModeless( ) {
	} // ~CHOrderModeless

	CHOrderModeless& CHOrderModeless::operator =( const CHOrderModeless& objCopy ) {
		return *this;
	} // CHOrderModeless& operator =

	void CHOrderModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHOrder::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHOrderModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "order_form" );
	} // void OnButtonForm

	void CHOrderModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "order_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "order_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHOrderPick::CHOrderPick( ) : CHOrder( ) {
	} // CHOrderPick

	CHOrderPick::CHOrderPick( const CHOrderPick& objCopy ) : CHOrder( ) {
	} // CHOrderPick

	CHOrderPick::~CHOrderPick( ) {
	} // ~CHOrderPick

	CHOrderPick& CHOrderPick::operator =( const CHOrderPick& objCopy ) {
		return *this;
	} // CHOrderPick& operator =

	void CHOrderPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHOrder::Init( );
	} // void CHOrderPick

	void CHOrderPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHOrderFormModal handler = new udHOrderFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание заявки" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание заявки" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHOrderPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			map< Glib::ustring, udPBase >::iterator itObj = m_mapObject.find( m_objTreeModel->get_path( itSelected ).to_string( ) );
			if ( itObj != m_mapObject.end( ) ) {
				m_pPickedObject = GetProjectOrder( );
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
				Glib::locale_to_utf8( "Выберите заявку" ),
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

	void CHOrderPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHOrderPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHOrderFormModal handler = new udHOrderFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование заявки" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование заявки" ) );
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

	udPFormHandler GetHOrderForm( ) {
		return new CHOrderFormModal;
	} // udPFormHandler GetHOrderForm

	CHOrderForm::CHOrderForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Заявка" ) ),
		m_lblManager( Glib::locale_to_utf8( "Ответственный" ) ),
		m_lblStatus( Glib::locale_to_utf8( "Статус" ) ),
		m_lblState( Glib::locale_to_utf8( "Состояние" ) ),
		m_lblAmount( Glib::locale_to_utf8( "Количество человек" ) ),
		m_lblEmployer( Glib::locale_to_utf8( "Заказчик" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Желаемая дата отъезда" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Желаемая дата приезда" ) ),
		m_lblDirect( Glib::locale_to_utf8( "Направление" ) ),
		m_lblComment( Glib::locale_to_utf8( "Комментарий" ) ),
		m_lblRoute( Glib::locale_to_utf8( "Маршрут" ) ),
		m_lblService( Glib::locale_to_utf8( "Примерные услуги" ) ),
		m_lblApp( Glib::locale_to_utf8( "Приложения" ) ),
		m_lblDocument( Glib::locale_to_utf8( "Документ по умолчанию" ) ),
		m_inpDate1( datetime( 0, true ), datetime( 0, true ) ),
		m_inpDate2( datetime( 0, true ), datetime( 0, true ) ),
		m_inpStatus( udProjectRequest::REQUEST_STATUS_OPENED, udProjectRequest::REQUEST_STATUS_OPENED )
	{
		m_szName = "order_form";

		m_pData->SetName( "order" );
		m_pData->SetGetObject( GetProjectOrder );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_lblManager.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblStatus.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblState.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblAmount.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblEmployer.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDate2.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDirect.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblComment.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblRoute.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblService.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblApp.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDocument.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectUser );
		pFde->SetMark( ctrlidManager );
		m_pData->AddExtraData( pFde );
		m_inpManager.SetDataObject( pFde );
		m_inpManager.SetWindowTitle( Glib::locale_to_utf8( "Выбор пользователя" ) );
		m_inpManager.SetAttr( "fio" );
		m_inpManager.SetGetListHandler( GetHUserListPick );
		m_inpManager.SetGetFormHandler( GetHUserForm );
		m_inpManager.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование пользователя" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectSOrder );
		pFde->SetMark( ctrlidState );
		m_pData->AddExtraData( pFde );
		m_inpState.SetDataObject( pFde );
		m_inpState.SetWindowTitle( Glib::locale_to_utf8( "Выбор состояния заявки" ) );
		m_inpState.SetAttr( "name" );
		m_inpState.SetGetListHandler( GetHSOrderListPick );
		m_inpState.SetGetFormHandler( GetHSOrderForm );
		m_inpState.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование состояния заявки" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEEmployer pFdeEmployer = new udFDEEmployer;
		pFdeEmployer->SetMark( ctrlidEmployer );
		m_pData->AddExtraData( pFdeEmployer );
		m_inpEmployer = new udWgtEmployerPick;
		m_inpEmployer->SetDataObject( pFdeEmployer );
		m_inpEmployer->SetWindowTitle( Glib::locale_to_utf8( "Выбор заказчика" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
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
		m_inpStatus.AddRow( udProjectRequest::REQUEST_STATUS_OPENED, Glib::locale_to_utf8( "Открыта" ) );
		m_inpStatus.AddRow( udProjectRequest::REQUEST_STATUS_CLOSED, Glib::locale_to_utf8( "Закрыта" ) );
		m_inpStatus.AddRow( udProjectRequest::REQUEST_STATUS_REMIND, Glib::locale_to_utf8( "Требует внимания" ) );
		m_inpStatus.Reset( );
		/*m_pColModelStatus = new CHOrderForm::ColModelStatus;
		m_objStatusModel = Gtk::ListStore::create( *m_pColModelStatus );

		Gtk::TreeModel::Row row;

		row = *( m_objStatusModel->append( ) );
		row[ m_pColModelStatus->m_col_code ] = udProjectRequest::REQUEST_STATUS_OPENED;
		row[ m_pColModelStatus->m_col_name ] = Glib::locale_to_utf8( "Открыта" );

		row = *( m_objStatusModel->append( ) );
		row[ m_pColModelStatus->m_col_code ] = udProjectRequest::REQUEST_STATUS_CLOSED;
		row[ m_pColModelStatus->m_col_name ] = Glib::locale_to_utf8( "Закрыта" );

		row = *( m_objStatusModel->append( ) );
		row[ m_pColModelStatus->m_col_code ] = udProjectRequest::REQUEST_STATUS_REMIND;
		row[ m_pColModelStatus->m_col_name ] = Glib::locale_to_utf8( "Требует внимания" );

		m_inpStatus.set_model( m_objStatusModel );
		m_inpStatus.pack_start( m_pColModelStatus->m_col_name );
		m_inpStatus.set_active( 0 );*/
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
		pTree->append_column( Glib::locale_to_utf8( "Наименование" ), m_pModelFormListRoute->m_col_name );
		m_inpRoute.SetColumnDefaults( );

		m_btnRouteEdit = m_inpRoute.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnRouteEdit->set_sensitive( false );
		m_btnRouteEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListRoute_Edit ) );

		m_btnRouteDel = m_inpRoute.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnRouteDel->set_sensitive( false );
		m_btnRouteDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListRoute_Del ) );

		m_btnRouteAdd = m_inpRoute.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnRouteAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListRoute_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListRoute_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectOrderRouteList );
		pFdeList->SetCallbackSend( GetProjectOrderRoute );
		pFdeList->SetMark( ctrlidRoute );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// услуги
		m_inpService.set_size_request( 400, 200 );
		m_pModelFormListService = new ColModelFormListService;
		m_objServiceModel = Gtk::ListStore::create( *m_pModelFormListService );
		
		pTree = m_inpService.GetTreeView( );
		pTree->set_model( m_objServiceModel );
		pTree->append_column( Glib::locale_to_utf8( "С" ), m_pModelFormListService->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "По" ), m_pModelFormListService->m_col_date2 );
		pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListService->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Город" ), m_pModelFormListService->m_col_city );
		m_inpService.SetColumnDefaults( );

		m_btnServiceEdit = m_inpService.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnServiceEdit->set_sensitive( false );
		m_btnServiceEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListService_Edit ) );

		m_btnServiceDel = m_inpService.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnServiceDel->set_sensitive( false );
		m_btnServiceDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListService_Del ) );

		m_btnServiceAdd = m_inpService.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnServiceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListService_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListService_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectOrderServiceList );
		pFdeList->SetCallbackSend( GetProjectOrderService );
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
		m_btnAppEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListApp_Edit ) );

		m_btnAppDel = m_inpApp.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnAppDel->set_sensitive( false );
		m_btnAppDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListApp_Del ) );

		m_btnAppReplace = m_inpApp.AddButton( Glib::locale_to_utf8( "Заменить" ) );
		m_btnAppReplace->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListApp_Replace ) );
		m_btnAppReplace->set_sensitive( false );

		m_btnAppAdd = m_inpApp.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnAppAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListApp_Add ) );

		m_btnFolder = m_inpApp.AddButton( Glib::locale_to_utf8( "Открыть папку" ) );
		m_btnFolder->signal_clicked( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListApp_Folder ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHOrderForm::OnFormListApp_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectAttach );
		pFdeList->SetMark( ctrlidApp );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClDoc );
		pFde->SetMark( ctrlidDocument );
		m_pData->AddExtraData( pFde );
		m_inpDocument.SetDataObject( pFde );
		m_inpDocument.SetWindowTitle( Glib::locale_to_utf8( "Выбор документа заказчика" ) );
		//m_inpDocument.SetAttr( "client_" );
		m_inpDocument.SetGetListHandler( GetHEmployerDocListPick );
		m_inpDocument.SetHandler( this );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHOrderForm

	CHOrderForm::CHOrderForm( const CHOrderForm& objCopy ) : udFormHandler( ) {
	} // CHOrderForm

	CHOrderForm::~CHOrderForm( ) {
		udDELETE( m_inpEmployer );
	} // ~CHOrderForm

	CHOrderForm& CHOrderForm::operator =( const CHOrderForm& objCopy ) {
		return *this;
	} // CHOrderForm& operator =

	void CHOrderForm::Init( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// приложение
		m_szFolderApp = g_pApp->GetUniqueTmpFolder( );
		m_pData->SetHasFiles( true );
		m_pData->SetFolder( m_szFolderApp );
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Заказчик
		m_tblInput.attach( m_lblEmployer, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( *m_inpEmployer, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Документ заказчика
		m_tblInput.attach( m_lblDocument, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDocument, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Ответственный
		m_tblInput.attach( m_lblManager, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpManager, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Статус
		m_tblInput.attach( m_lblStatus, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpStatus, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Состояние
		m_tblInput.attach( m_lblState, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpState, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Направление
		m_tblInput.attach( m_lblDirect, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDirect, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Количество человек
		m_tblInput.attach( m_lblAmount, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpAmount, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Желаемая дата отъезда
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Желаемая дата приезда
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Маршрут
		m_tblInput.attach( m_lblRoute, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL, 10 );
		m_tblInput.attach( m_inpRoute, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Услуги
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL, 10 );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Приложения
		m_tblInput.attach( m_lblApp, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL, 10 );
		m_tblInput.attach( m_inpApp, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 0, 2, l, r, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	void CHOrderForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpEmployer->SetTitle( "" );
		m_inpManager.SetTitle( "" );
		//m_inpStatus.set_active( 0 );
		m_inpStatus.Reset( );
		m_inpState.SetTitle( "" );
		m_inpDirect.SetTitle( "" );
		m_objRouteModel->clear( );
		m_inpAmount.Reset( );
		m_inpComment.Reset( );
		m_inpDate1.Reset( );
		m_inpDate2.Reset( );
		m_objAppModel->clear( );
		m_objServiceModel->clear( );
		m_inpDocument.SetTitle( "" );
	} // void Clear

	void CHOrderForm::Load( ) {
		m_pData->MakeObject( );
		( ( udPFDEEmployer ) m_pData->GetExtraData( ctrlidEmployer ) )->SetMainObject( m_pData->GetData( ) );

		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		if ( obj->get( "amount" )->toUInt( ) > 0 ) {
			m_inpAmount.SetValue( Glib::locale_to_utf8( obj->get( "amount" )->toString( ) ) );
		}
		m_inpComment.SetValue( Glib::locale_to_utf8( obj->get( "comment" )->toString( ) ) );
		if ( obj->get( "status" )->toInt( ) > 0 ) {
			//m_inpStatus.set_active( obj->get( "status" )->toInt( ) - 1 );
			m_inpStatus.SetValue( obj->get( "status" )->toUInt( ) - 1 );
		}

		string szAttr = "fio";
		if ( obj->get( "emptype" )->toInt( ) == emptypeUr ) {
			szAttr = "name";
		}
		m_inpEmployer->SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( )->get( szAttr )->toString( )
		) );

		udPBase doc = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) )->GetObject( );
		if ( doc ) {
			m_inpDocument.SetTitle( doc->get( "series" )->toString( ) + " " + doc->get( "number" )->toString( ) );
		}

		m_inpManager.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidManager ) )->GetObject( )->get( "fio" )->toString( )
		) );
		m_inpState.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpDirect.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "name" )->toString( )
		) );
		m_inpDate1.SetValue( obj->get( "date1" )->toTime( ) );
		m_inpDate2.SetValue( obj->get( "date2" )->toTime( ) );
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
				row[ m_pModelFormListRoute->m_col_name		] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
				row[ m_pModelFormListRoute->m_col_object	] = add;
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
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objServiceModel->append( ) );
				row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_type	] = Glib::locale_to_utf8( add->get( "type_" )->toString( ) );
				row[ m_pModelFormListService->m_col_city	] = Glib::locale_to_utf8( add->get( "city_" )->toString( ) );
				row[ m_pModelFormListService->m_col_object	] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHOrderForm::Validate( ) {
		Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Glib::ustring	szAmount,
						szComment;

		szAmount = m_inpAmount.GetValue( );
		if ( szAmount.empty( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите количество человек" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpAmount.GetEntry( ) ) );
			return 1;
		}

		/*szComment = m_inpComment.LoadValue( ).GetValue( );
		if ( szComment.empty( ) ) {
			return 2;
		}*/

		udPBase obj = NULL;
		
		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите заказчика" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpEmployer->GetButton( ) ) );
			return 3;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidManager ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите ответственного" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpManager.GetButton( ) ) );
			return 4;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите состояние" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpState.GetButton( ) ) );
			return 5;
		}

		obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( );
		if ( !obj || !obj->get( "id" )->toUInt( ) ) {
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите направление" ) );
			g_pApp->GetWndWork( )->set_focus( *( m_inpDirect.GetButton( ) ) );
			return 6;
		}

		return 0;
	} // udInt Validate

	void CHOrderForm::Send( ) {
		Glib::ustring	szAmount,
						szComment;

		szAmount = m_inpAmount.GetValue( );
		szComment = m_inpComment.GetValue( );

		udUInt status = m_inpStatus.LoadValue( ).GetValue( );

		m_pData->MakeObject( );

		udPBase obj = m_pData->GetData( );

		obj->get( "created" )->ini( datetime( 0, true ) );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );
		obj->get( "amount" )->ini( toUInt( szAmount.data( ) ) );
		obj->get( "comment" )->ini( ConvertString( szComment.data( ), CP_UTF8 ) );
		obj->get( "status" )->ini( status );
		obj->get( "emptype" )->ini( m_inpEmployer->GetType( ) );
		obj->get( "employer" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "document" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDocument ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "manager2" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidManager ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "state" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidState ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "direct" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidDirect ) )->GetObject( )->get( "id" )->toUInt( )
		);
		obj->get( "date1" )->ini( m_inpDate1.GetValue( ) );
		obj->get( "date2" )->ini( m_inpDate2.GetValue( ) );

		m_pData->Send( ( ( udPFDEList ) m_pData->GetExtraData( ctrlidApp ) )->GetArray( ) );
	} // void Send

	bool CHOrderForm::BeforeCreatePickHandler( udPWgtEmployerDocPick wgt, udPHEmployerDocPick handler ) {
		assert( wgt );
		assert( handler );

		m_pData->MakeObject( );

		udPBase employer = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( );
		if ( employer && ( employer->get( "id" )->toUInt( ) > 0 ) ) {
			handler->SetEmployerId( employer->get( "id" )->toUInt( ) );
			return true;
		} else {
			mb::err( *( g_pApp->GetWndWork( ) ), Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Сначала выберите заказчика" ) );
		}
		return false;
	} // bool BeforeCreatePickHandler

	udPHClDocFormModal CHOrderForm::GetEditHandler( udPWgtEmployerDocPick wgt ) {
		assert( wgt );

		m_pData->MakeObject( );

		udPBase employer = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidEmployer ) )->GetObject( );
		if ( employer && ( employer->get( "id" )->toUInt( ) > 0 ) ) {
			return new CHClDocFormModal( employer->get( "id" )->toUInt( ), false, true );
		}
		return NULL;
	} // udPHClDocFormModal GetEditHandler

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHOrderForm::OnFormListRoute_Select( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnRouteEdit->set_sensitive( true );
			m_btnRouteDel->set_sensitive( true );
		} else {
			m_btnRouteEdit->set_sensitive( false );
			m_btnRouteDel->set_sensitive( false );
		}
	} // void OnFormListRoute_Select

	void CHOrderForm::OnFormListRoute_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHOrderRouteFormModal handler = new udHOrderRouteFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание маршрута" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание маршрута" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectOrderRoute;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidRoute );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "order" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objRouteModel->append( ) );

			row[ m_pModelFormListRoute->m_col_name		] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
			row[ m_pModelFormListRoute->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListRoute_Add

	void CHOrderForm::OnFormListRoute_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpRoute.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListRoute->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHOrderRouteFormModal handler = new udHOrderRouteFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование маршрута" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование маршрута" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListRoute->m_col_name ] = Glib::locale_to_utf8( edit->get( "name" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListRoute_Edit

	void CHOrderForm::OnFormListRoute_Del( ) {
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
			} else {
				g_pApp->GetLog( )->Write( "CHOrderForm::OnFormListRoute_Del, (1) not found\n" );
			}

			m_objRouteModel->erase( i );
		}
	} // void OnFormListRoute_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHOrderForm::OnFormListApp_Select( ) {
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

	void CHOrderForm::OnFormListApp_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		Gtk::FileChooserDialog dlgPickFile( *wndParent, Glib::locale_to_utf8( "Выберите файл" ) );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

		int code = dlgPickFile.run( );

		g_pApp->GetLog( )->Write( "code=" + toString( code ) + "\n" );

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
				add->get( "type" )->ini( udProjectAttach::ATTACH_ORDER );
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

	void CHOrderForm::OnFormListApp_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpApp.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListApp->m_col_object ];
			ExtOpenFile( edit->get( "tmp_path" )->toWString( ), false );
		}
	} // void OnFormListApp_Edit

	void CHOrderForm::OnFormListApp_Del( ) {
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

	void CHOrderForm::OnFormListApp_Replace( ) {
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

	void CHOrderForm::OnFormListApp_Folder( ) {
		ExtOpenFolder( m_szFolderApp );
	} // void OnFormListApp_Folder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHOrderForm::OnFormListService_Select( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnServiceEdit->set_sensitive( true );
			m_btnServiceDel->set_sensitive( true );
		} else {
			m_btnServiceEdit->set_sensitive( false );
			m_btnServiceDel->set_sensitive( false );
		}
	} // void OnFormListService_Select

	void CHOrderForm::OnFormListService_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHOrderServiceFormModal handler = new udHOrderServiceFormModal( );
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
			udPBase add = new udProjectOrderService;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "order" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::iterator i = m_objServiceModel->append( );
			Gtk::TreeModel::Row row = *i;
			Gtk::TreePath objPath = m_objServiceModel->get_path( i );

			udPFDEObject pFdeType = ( udPFDEObject ) pDa->GetExtraData( udHOrderServiceForm::ctrlidType );
			if ( pFdeType ) {
				udPBase pType = pFdeType->GetObject( );
				if ( pType ) {
					row[ m_pModelFormListService->m_col_type ] = Glib::locale_to_utf8( pType->get( "name" )->toString( ) );
				}
			}
			
			udPFDEObject pFdeCity = ( udPFDEObject ) pDa->GetExtraData( udHOrderServiceForm::ctrlidCity );
			if ( pFdeCity ) {
				udPBase pCity = pFdeCity->GetObject( );
				if ( pCity ) {
					row[ m_pModelFormListService->m_col_city ] = Glib::locale_to_utf8( pCity->get( "name" )->toString( ) );
				}
			}

			row[ m_pModelFormListService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListService->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListService_Add

	void CHOrderForm::OnFormListService_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListService->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHOrderServiceFormModal handler = new udHOrderServiceFormModal( );
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

				udPFDEObject pFdeType = ( udPFDEObject ) pDa->GetExtraData( udHOrderServiceForm::ctrlidType );
				if ( pFdeType ) {
					udPBase pType = pFdeType->GetObject( );
					if ( pType ) {
						row[ m_pModelFormListService->m_col_type ] = Glib::locale_to_utf8( pType->get( "name" )->toString( ) );
					}
				}
				
				udPFDEObject pFdeCity = ( udPFDEObject ) pDa->GetExtraData( udHOrderServiceForm::ctrlidCity );
				if ( pFdeCity ) {
					udPBase pCity = pFdeCity->GetObject( );
					if ( pCity ) {
						row[ m_pModelFormListService->m_col_city ] = Glib::locale_to_utf8( pCity->get( "name" )->toString( ) );
					}
				}

				row[ m_pModelFormListService->m_col_date1 ] = Glib::locale_to_utf8( edit->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListService->m_col_date2 ] = Glib::locale_to_utf8( edit->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListService_Edit

	void CHOrderForm::OnFormListService_Del( ) {
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

	




	CHOrderFormModal::CHOrderFormModal( ) : CHOrderForm( ) {
	} // CHOrderFormModal

	CHOrderFormModal::CHOrderFormModal( const CHOrderFormModal& objCopy ) : CHOrderForm( ) {
	} // CHOrderFormModal

	CHOrderFormModal::~CHOrderFormModal( ) {
	} // ~CHOrderFormModal

	CHOrderFormModal& CHOrderFormModal::operator =( const CHOrderFormModal& objCopy ) {
		return *this;
	} // CHOrderFormModal& operator =

	void CHOrderFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHOrderFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHOrderForm




	CHOrderFormModeless::CHOrderFormModeless( ) : CHOrderForm( ) {
	} // CHOrderFormModeless

	CHOrderFormModeless::CHOrderFormModeless( const CHOrderFormModeless& objCopy ) : CHOrderForm( ) {
	} // CHOrderFormModeless

	CHOrderFormModeless::~CHOrderFormModeless( ) {
	} // ~CHOrderFormModeless

	CHOrderFormModeless& CHOrderFormModeless::operator =( const CHOrderFormModeless& objCopy ) {
		return *this;
	} // CHOrderFormModeless& operator =

	void CHOrderFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "order" );
	} // void OnButtonOk

	void CHOrderFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "order" );
	} // void CHOrderForm

	void CHOrderFormModeless::Init( ) {
		CHOrderForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHOrderFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHOrderFormModeless

} // namespace UdSDK
