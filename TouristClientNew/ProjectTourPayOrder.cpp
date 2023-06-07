
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHTourPayOrderListPick( ) {
		return new CHTourPayOrderPick;
	} // udPListHandlerPick GetHTourPayOrderListPick

	udPBase GetProjectTourPayOrderList( ) {
		udPBase ret = GetProjectTourPayOrder( );
		ret->get( "tour_" )->ini( wstring( L"" ) );
		ret->get( "employer_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourPayOrderList

	CHTourPayOrder::CHTourPayOrder( ) :
		udListHandler( ),
		udListHandlerBtnForm( ),
		m_lblFltDate1( Glib::locale_to_utf8( "Дата с" ) ),
		m_lblFltDate2( Glib::locale_to_utf8( "по" ) ),
		m_lblFltTour( Glib::locale_to_utf8( "Тур" ) ),
		m_lblFltState( Glib::locale_to_utf8( "Состояние" ) ),
		m_inpFltDate1( datetime( ), datetime( ) ),
		m_inpFltDate2( datetime( ), datetime( ) )
	{
		m_szName = "tourpayorder";

		m_btnForm.set_label( Glib::locale_to_utf8( "Создать указание на оплату" ) );

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( ) );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
		filter->get( "state" )->ini( ( udInt ) 0 );

		m_pRowModel = m_pColModel = new CHTourPayOrder::ColModelTourPayOrder( );

		m_pData->SetName( "tourpayorder" );
		m_pData->SetGetObject( GetProjectTourPayOrderList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
		m_inpFltState.AddRow( 0, Glib::locale_to_utf8( "Все" ) );
		m_inpFltState.AddRow( udProjectTourPayOrder::stOpened + 1, Glib::locale_to_utf8( "Только открытые" ) );
		m_inpFltState.AddRow( udProjectTourPayOrder::stClosed + 1, Glib::locale_to_utf8( "Только закрытые" ) );
		m_inpFltState.Reset( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Дата создания
		m_tblFilter.attach( m_lblFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate1, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Дата создания
		m_tblFilter.attach( m_lblFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltDate2, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Тур
		m_tblFilter.attach( m_lblFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltTour, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Состояние
		m_tblFilter.attach( m_lblFltState, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltState, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTourPayOrder

	CHTourPayOrder::CHTourPayOrder( const CHTourPayOrder& objCopy ) : udListHandler( ) {
		m_szName = "tourpayorder";
	} // CHTourPayOrder

	CHTourPayOrder::~CHTourPayOrder( ) {
	} // ~CTourPayOrder

	CHTourPayOrder& CHTourPayOrder::operator =( const CHTourPayOrder& objCopy ) {
		return *this;
	} // CHTourPayOrder& operator =

	void CHTourPayOrder::Init( ) {
		m_hbbTop.pack_start( m_btnForm, false, false, 0 );
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата создания" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Тур" ), m_pColModel->m_col_tour );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Состояние" ), m_pColModel->m_col_state );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Заказчик" ), m_pColModel->m_col_employer );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHTourPayOrder::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		udInt state = 0;

		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pColModel->m_col_tour ] = Glib::locale_to_utf8( obj->get( "tour_" )->toString( ) );

			state = obj->get( "state" )->toInt( );
			if ( state == udProjectTourPayOrder::stOpened ) {
				row[ m_pColModel->m_col_state ] = Glib::locale_to_utf8( "Открыто" );
			} else if ( state == udProjectTourPayOrder::stClosed ) {
				row[ m_pColModel->m_col_state ] = Glib::locale_to_utf8( "Закрыто" );
			}

			row[ m_pColModel->m_col_employer ] = Glib::locale_to_utf8( obj->get( "employer_" )->toString( ) );
		}
	} // void Load

	void CHTourPayOrder::FilterUiClear( ) {
		m_inpFltDate1.Reset( );
		m_inpFltDate2.Reset( );
		m_inpFltTour.SetTitle( "" );
		m_inpFltState.Reset( );
	} // void FilterUiClear

	void CHTourPayOrder::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "date1" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "date2" )->ini( m_inpFltDate1.GetValue( ) );
		filter->get( "tour" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltTour ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "state" )->ini( m_inpFltState.LoadValue( ).GetValue( ) );
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPayOrderModeless::CHTourPayOrderModeless( ) :
		udListHandlerModeless( ),
		CHTourPayOrder( )
	{
	} // CHTourPayOrderModeless

	CHTourPayOrderModeless::CHTourPayOrderModeless( const CHTourPayOrderModeless& objCopy ) : CHTourPayOrder( ) {
	} // CHTourPayOrderModeless

	CHTourPayOrderModeless::~CHTourPayOrderModeless( ) {
	} // ~CHTourPayOrderModeless

	CHTourPayOrderModeless& CHTourPayOrderModeless::operator =( const CHTourPayOrderModeless& objCopy ) {
		return *this;
	} // CHTourPayOrderModeless& operator =

	void CHTourPayOrderModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHTourPayOrder::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHTourPayOrderModeless::OnButtonForm( ) {
		g_pApp->GetHandlerManager( )->SetState( "tourpayorder_form" );
	} // void OnButtonForm

	void CHTourPayOrderModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "tourpayorder_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "tourpayorder_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPayOrderPick::CHTourPayOrderPick( ) : CHTourPayOrder( ) {
	} // CHTourPayOrderPick

	CHTourPayOrderPick::CHTourPayOrderPick( const CHTourPayOrderPick& objCopy ) : CHTourPayOrder( ) {
	} // CHTourPayOrderPick

	CHTourPayOrderPick::~CHTourPayOrderPick( ) {
	} // ~CHTourPayOrderPick

	CHTourPayOrderPick& CHTourPayOrderPick::operator =( const CHTourPayOrderPick& objCopy ) {
		return *this;
	} // CHTourPayOrderPick& operator =

	void CHTourPayOrderPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHTourPayOrder::Init( );
	} // void CHTourPayOrderPick

	void CHTourPayOrderPick::OnButtonForm( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		udPHTourPayOrderFormModal handler = new udHTourPayOrderFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание указания на оплату" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание страны" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );

		Clear( );
		Load( );
	} // void OnButtonForm

	void CHTourPayOrderPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectTourPayOrder( );
			//Glib::ustring szName = row[ m_pColModel->m_col_name ];
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			//m_pPickedObject->get( "name" )->ini( ConvertString( szName.data( ), CP_UTF8 ) );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите указание на оплату" ),
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

	void CHTourPayOrderPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHTourPayOrderPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHTourPayOrderFormModal handler = new udHTourPayOrderFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование указания на оплату" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование указания на оплату" ) );
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

	CHTourPayOrderForm::CHTourPayOrderForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Указание на оплату" ) ),
		m_lblTour( Glib::locale_to_utf8( "Тур" ) ),
		m_lblState( Glib::locale_to_utf8( "Состояние" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Суммы" ) ),
		m_inpState( udProjectTourPayOrder::stOpened, udProjectTourPayOrder::stOpened ),
		m_iState( -1 ),
		m_iTour( 0 ),
		m_btnOpenPersAcc( Glib::locale_to_utf8( "Лицевой счет" ) )
	{
		m_szName = "tourpayorder_form";

		m_pData->SetName( "tourpayorder" );
		m_pData->SetGetObject( GetProjectTourPayOrder );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblSumm.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpState.AddRow( udProjectTourPayOrder::stOpened, Glib::locale_to_utf8( "Открыто" ) );
		m_inpState.AddRow( udProjectTourPayOrder::stClosed, Glib::locale_to_utf8( "Закрыто" ) );
		m_inpState.Reset( );
		/*m_pColModelState = new CHTourPayOrderForm::ColModelState;
		m_objStateModel = Gtk::ListStore::create( *m_pColModelState );

		Gtk::TreeModel::Row row;

		row = *( m_objStateModel->append( ) );
		row[ m_pColModelState->m_col_code ] = udProjectTourPayOrder::stOpened;
		row[ m_pColModelState->m_col_name ] = Glib::locale_to_utf8( "Открыто" );

		row = *( m_objStateModel->append( ) );
		row[ m_pColModelState->m_col_code ] = udProjectTourPayOrder::stClosed;
		row[ m_pColModelState->m_col_name ] = Glib::locale_to_utf8( "Закрыто" );

		m_inpState.set_model( m_objStateModel );

		m_inpState.pack_start( m_pColModelState->m_col_name );
		m_inpState.set_active( 0 );*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectTour );
		pFde->SetMark( ctrlidTour );
		m_pData->AddExtraData( pFde );
		m_inpTour.SetDataObject( pFde );
		m_inpTour.SetWindowTitle( Glib::locale_to_utf8( "Выбор тура" ) );
		m_inpTour.SetAttr( "name" );
		m_inpTour.SetGetListHandler( GetHTourListPick );
		m_inpTour.SetGetFormHandler( GetHTourForm );
		m_inpTour.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование тура" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpSumm.set_size_request( 400, 200 );
		m_pModelFormListSumm = new ColModelFormListSumm;
		m_objSummModel = Gtk::ListStore::create( *m_pModelFormListSumm );
		
		pTree = m_inpSumm.GetTreeView( );
		pTree->set_model( m_objSummModel );
		pTree->append_column( Glib::locale_to_utf8( "Сумма" ), m_pModelFormListSumm->m_col_summ );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListSumm->m_col_currency );
		//pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListSumm->m_col_type );
		m_inpSumm.SetColumnDefaults( );

		m_btnSummAdd = m_inpSumm.AddButton( Glib::locale_to_utf8( "Зачислить" ) );
		m_btnSummAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourPayOrderForm::OnFormListSumm_Add ) );

		m_btnSummBack = m_inpSumm.AddButton( Glib::locale_to_utf8( "Снять" ) );
		m_btnSummBack->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourPayOrderForm::OnFormListSumm_Back ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectPersAccSummList );
		pFdeList->SetCallbackSend( GetProjectFinanceSumm );
		pFdeList->SetMark( ctrlidSumm );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btnOpenPersAcc.signal_clicked( ).connect( sigc::mem_fun( this, &CHTourPayOrderForm::OnButtonOpenPersAcc ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTourPayOrderForm

	CHTourPayOrderForm::CHTourPayOrderForm( const CHTourPayOrderForm& objCopy ) : udFormHandler( ) {
	} // CHTourPayOrderForm

	CHTourPayOrderForm::~CHTourPayOrderForm( ) {
	} // ~CHTourPayOrderForm

	CHTourPayOrderForm& CHTourPayOrderForm::operator =( const CHTourPayOrderForm& objCopy ) {
		return *this;
	} // CHTourPayOrderForm& operator =

	void CHTourPayOrderForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		if ( !m_iTour ) {
			// Тур
			m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			m_tblInput.attach( m_inpTour, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		if ( m_iState < 0 ) {
			// Состояние
			m_tblInput.attach( m_lblState, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
			m_tblInput.attach( m_inpState, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		}

		// Суммы
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnOpenPersAcc, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHTourPayOrderForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpTour.SetTitle( "" );
		//m_inpState.set_active( 0 );
		m_inpState.Reset( );
		m_objSummModel->clear( );

		m_btnOpenPersAcc.hide( );
	} // void Clear

	void CHTourPayOrderForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		m_inpTour.SetTitle( Glib::locale_to_utf8(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidTour ) )->GetObject( )->get( "name" )->toString( )
		) );
		if ( obj->get( "state" )->toInt( ) > 0 ) {
			//m_inpState.set_active( obj->get( "state" )->toInt( ) - 1 );
			m_inpState.SetValue( obj->get( "state" )->toUInt( ) - 1 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidSumm ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objSummModel->append( ) );
				row[ m_pModelFormListSumm->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListSumm->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListSumm->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListSumm->m_col_object		] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if ( obj->get( "tour" )->toUInt( ) ) {
			m_btnOpenPersAcc.show( );
		} else {
			m_btnOpenPersAcc.hide( );
		}
	} // void Load

	udInt CHTourPayOrderForm::Validate( ) {
		return 0;
	} // udInt Validate

	void CHTourPayOrderForm::Send( ) {
		m_pData->MakeObject( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//Gtk::TreeModel::Row row;
		//row = *( m_inpState.get_active( ) );
		udUInt state = m_inpState.LoadValue( ).GetValue( );//row[ m_pColModelState->m_col_code ];
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );

		obj->get( "created" )->ini( datetime( 0, true ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if ( m_iTour ) {
			obj->get( "tour" )->ini( m_iTour );
		} else {
			obj->get( "tour" )->ini(
				( ( udPFDEObject ) m_pData->GetExtraData( ctrlidTour ) )->GetObject( )->get( "id" )->toUInt( )
			);
		}
		if ( m_iState >= 0 ) {
			obj->get( "state" )->ini( m_iState );
		} else {
			obj->get( "state" )->ini( state );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pData->Send( );
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourPayOrderForm::OnFormListSumm_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourPayOrderFinanceSummAddFormModal handler = new udHTourPayOrderFinanceSummAddFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Зачислить" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Зачислить" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		bool bLoad = false;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			string log = "\nsumm add: ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidSumm );
			udPArrBase arrObject = pFde->GetArray( );
			bool bAdd = false;
			udPLog __log = g_pApp->GetLog( );

			if ( m_pModelFormListSumm->size( ) ) {
				udPBase obj1 = NULL;
				bool found = false;
				udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
				udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
				udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );

				Gtk::TreeModel::Children arrRows = m_objSummModel->children( );
				Gtk::TreeModel::Row row;

				for( Gtk::TreeModel::iterator i = arrRows.begin( ); i != arrRows.end( ); ++i ) {
					row = *i;
					obj1 = row[ m_pModelFormListSumm->m_col_object ];
					c1 = obj1->get( "currency" )->toUInt( );
					t1 = obj1->get( "type" )->toInt( );

					if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
						s1 = obj1->get( "summ" )->toFloat( );
						obj1->get( "summ" )->ini( s1 + s2 );
						row[ m_pModelFormListSumm->m_col_summ	] = Glib::locale_to_utf8( obj1->get( "summ" )->toString( ) );
						found = true;
						break;
					}
				}

				if ( !found ) { // если не найдена, то добавляем
					bAdd = true;
				}
			} else { // список пуст
				bAdd = true;
			}

			if ( bAdd ) {
				udPBase add = new udProjectFinanceSumm;
				add->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
				add->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
				add->get( "type" )->ini( obj->get( "type" )->toInt( ) );
				add->get( "at" )->ini( udProjectFinanceSumm::SummAtTourPayOrder );
				add->get( "obj" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );

				arrObject->push_back( add );

				Gtk::TreeModel::Row row = *( m_objSummModel->append( ) );

				udPFDEObject pFdeCurrency = ( udPFDEObject ) pDa->GetExtraData( udHFinanceSummAddForm::ctrlidCurrency );
				if ( pFdeCurrency ) {
					udPBase pCurrency = pFdeCurrency->GetObject( );
					if ( pCurrency ) {
						row[ m_pModelFormListSumm->m_col_currency ] = Glib::locale_to_utf8( pCurrency->get( "name" )->toString( ) );
					}
				}

				row[ m_pModelFormListSumm->m_col_summ	] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				//row[ m_pModelFormListSumm->m_col_type	] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListSumm->m_col_object	] = add;
			}
		}

		udDELETE( handler );
	} // void OnFormListFinal_Add

	void CHTourPayOrderForm::OnFormListSumm_Back( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourPayOrderFinanceSummBackFormModal handler = new udHTourPayOrderFinanceSummBackFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Снять" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Снять" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		bool bLoad = false;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			string log = "\nsumm back: ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidSumm );
			udPArrBase arrObject = pFde->GetArray( );

			if ( m_pModelFormListSumm->size( ) ) {
				udPBase obj1 = NULL;
				udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
				udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
				udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );

				Gtk::TreeModel::Children arrRows = m_objSummModel->children( );
				Gtk::TreeModel::Row row;

				for( Gtk::TreeModel::iterator i = arrRows.begin( ); i != arrRows.end( ); ++i ) {
					row = *i;
					obj1 = row[ m_pModelFormListSumm->m_col_object ];
					c1 = obj1->get( "currency" )->toUInt( );
					t1 = obj1->get( "type" )->toInt( );

					if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
						s1 = obj1->get( "summ" )->toFloat( );
						if ( s1 < s2 ) {
						} else {
							obj1->get( "summ" )->ini( s1 - s2 );
							row[ m_pModelFormListSumm->m_col_summ ] = Glib::locale_to_utf8( obj1->get( "summ" )->toString( ) );
						}
						break;
					}
				}
			}
		}

		udDELETE( handler );
	} // void OnFormListFinal_Back

	void CHTourPayOrderForm::OnButtonOpenPersAcc( ) {
		m_pData->MakeObject( );
		/**
		 *	Лицевой счет определяется как сочетание заказчика и компании продавца тура
		 */
		udPBase obj = m_pData->GetData( );
		Glib::ustring msg;
		udUInt iTourId = 0;
		udPBase pPersAcc = NULL;

		if ( obj->get( "tour" )->toUInt( ) ) {
			iTourId = obj->get( "tour" )->toUInt( );
		} else if ( m_iTour ) {
			iTourId = m_iTour;
		}

		if ( iTourId ) {
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidGetPersAcc );
				data.add( iTourId );

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );

				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) { // необходимо считать объекты
					pPersAcc = new udProjectPersAcc;
					if ( !pPersAcc->read( sock ) ) {
						udDELETE( pPersAcc );
						msg = Glib::locale_to_utf8( "Ошибки при загрузке лицевого счета" );
					}
				} else {
					switch( code ) {
						case 1: msg = Glib::locale_to_utf8( "Ошибки при выборке тура" );			break;
						case 2: msg = Glib::locale_to_utf8( "Отсутствует тур" );					break;
						case 3: msg = Glib::locale_to_utf8( "Ошибка в коде типа заказчика" );		break;
						case 4: msg = Glib::locale_to_utf8( "Ошибки при выборке лицевого счета" );	break;
						case 5: msg = Glib::locale_to_utf8( "Отсутствует лицевой счет" );			break;
						default: break;
					}
				}

				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );

				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		if ( !msg.empty( ) ) {
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), msg );
		}

		if ( pPersAcc ) {
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHPersAccFormModal handler = new udHPersAccFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( pPersAcc->get( "id" )->toUInt( ) );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование лицевого счета" ) );

			udDELETE( handler );

			udDELETE( pPersAcc );
		}
	} // void OnButtonOpenPersAcc

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPayOrderFormModal::CHTourPayOrderFormModal( ) : CHTourPayOrderForm( ) {
	} // CHTourPayOrderFormModal

	CHTourPayOrderFormModal::CHTourPayOrderFormModal( const CHTourPayOrderFormModal& objCopy ) : CHTourPayOrderForm( ) {
	} // CHTourPayOrderFormModal

	CHTourPayOrderFormModal::~CHTourPayOrderFormModal( ) {
	} // ~CHTourPayOrderFormModal

	CHTourPayOrderFormModal& CHTourPayOrderFormModal::operator =( const CHTourPayOrderFormModal& objCopy ) {
		return *this;
	} // CHTourPayOrderFormModal& operator =

	void CHTourPayOrderFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHTourPayOrderFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHTourPayOrderForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPayOrderFormModeless::CHTourPayOrderFormModeless( ) : CHTourPayOrderForm( ) {
	} // CHTourPayOrderFormModeless

	CHTourPayOrderFormModeless::CHTourPayOrderFormModeless( const CHTourPayOrderFormModeless& objCopy ) : CHTourPayOrderForm( ) {
	} // CHTourPayOrderFormModeless

	CHTourPayOrderFormModeless::~CHTourPayOrderFormModeless( ) {
	} // ~CHTourPayOrderFormModeless

	CHTourPayOrderFormModeless& CHTourPayOrderFormModeless::operator =( const CHTourPayOrderFormModeless& objCopy ) {
		return *this;
	} // CHTourPayOrderFormModeless& operator =

	void CHTourPayOrderFormModeless::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		g_pApp->GetHandlerManager( )->SetState( "tourpayorder" );
	} // void OnButtonOk

	void CHTourPayOrderFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "tourpayorder" );
	} // void CHTourPayOrderForm

	void CHTourPayOrderFormModeless::Init( ) {
		CHTourPayOrderForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHTourPayOrderFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHTourPayOrderFormModeless

} // namespace UdSDK
