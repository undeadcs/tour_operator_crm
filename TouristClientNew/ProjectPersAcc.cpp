
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPListHandlerPick GetHPersAccListPick( ) {
		return new CHPersAccPick;
	} // udPListHandlerPick GetHPersAccListPick

	udPBase GetProjectPersAccList( ) {
		udPBase ret = GetProjectPersAcc( );
		ret->get( "name" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectPersAccList

	CHPersAcc::CHPersAcc( ) :
		udListHandler( ),
		m_lblFltCompany( Glib::locale_to_utf8( "Компания" ) ),
		m_lblFltPartner( Glib::locale_to_utf8( "Партнер" ) ),
		m_lblFltClient( Glib::locale_to_utf8( "Клиент (физ./юр)" ) )
	{
		m_szName = "persacc";

		m_pRowModel = m_pColModel = new CHPersAcc::ColModelPersAcc( );

		m_pData->SetName( "persacc" );
		m_pData->SetGetObject( GetProjectPersAccList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPMorph filter = m_pData->GetFilter( );
		filter->get( "company" )->ini( ( udUInt ) 0 );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCompany );
		pFde->SetMark( fltCompany );
		m_pData->AddExtraData( pFde );
		m_inpFltCompany.SetDataObject( pFde );
		m_inpFltCompany.SetWindowTitle( Glib::locale_to_utf8( "Выбор компании" ) );
		m_inpFltCompany.SetAttr( "name" );
		m_inpFltCompany.SetGetListHandler( GetHCompanyListPick );
		m_inpFltCompany.SetGetFormHandler( GetHCompanyForm );
		m_inpFltCompany.SetFormWindowTitle( Glib::locale_to_utf8( "Редактирование компании" ) );
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
		udPFDEEmployer pFdeClient = new udFDEEmployer;
		pFdeClient->SetMark( fltClient );
		m_pData->AddExtraData( pFdeClient );
		m_inpFltClient = new udWgtEmployerPick;
		m_inpFltClient->SetDataObject( pFdeClient );
		m_inpFltClient->SetWindowTitle( Glib::locale_to_utf8( "Выбор клиента" ) );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// размеры контролов
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// расположение контролов
		udInt	col1 = 0, col2 = 1,
				row1 = 0, row2 = 1;
		
		// Компания
		m_tblFilter.attach( m_lblFltCompany, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltCompany, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// Партнер
		m_tblFilter.attach( m_lblFltPartner, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_inpFltPartner, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		// кнопки
		m_tblFilter.attach( m_btnFilter, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( m_btnReset, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );

		col1 = 0;
		col2 = 1;
		++row1;
		++row2;

		// Клиент
		m_tblFilter.attach( m_lblFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK );
		m_tblFilter.attach( *m_inpFltClient, col1++, col2++, row1, row2, Gtk::SHRINK, Gtk::SHRINK, 2, 1 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHPersAcc

	CHPersAcc::CHPersAcc( const CHPersAcc& objCopy ) : udListHandler( ) {
		m_szName = "persacc";
	} // CHPersAcc

	CHPersAcc::~CHPersAcc( ) {
		udDELETE( m_inpFltClient );
	} // ~CPersAcc

	CHPersAcc& CHPersAcc::operator =( const CHPersAcc& objCopy ) {
		return *this;
	} // CHPersAcc& operator =

	void CHPersAcc::Init( ) {
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Наименование" ), m_pColModel->m_col_name );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHPersAcc::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_name ] = Glib::locale_to_utf8( obj->get( "name" )->toString( ) );
		}
	} // void Load

	void CHPersAcc::FilterUiClear( ) {
		m_inpFltClient->SetTitle( "" );
		m_inpFltCompany.SetTitle( "" );
		m_inpFltPartner.SetTitle( "" );
	} // void FilterUiClear

	void CHPersAcc::FilterUiToObject( ) {
		m_pData->MakeObject( );

		udPMorph filter = m_pData->GetFilter( );

		filter->get( "company" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltCompany ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "emptype" )->ini( m_inpFltClient->GetType( ) );
		filter->get( "client" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltClient ) )->GetObject( )->get( "id" )->toUInt( )
		);
		filter->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( fltPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
	} // void FilterUiToObject

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPersAccModeless::CHPersAccModeless( ) :
		udListHandlerModeless( ),
		CHPersAcc( )
	{
	} // CHPersAccModeless

	CHPersAccModeless::CHPersAccModeless( const CHPersAccModeless& objCopy ) : CHPersAcc( ) {
	} // CHPersAccModeless

	CHPersAccModeless::~CHPersAccModeless( ) {
	} // ~CHPersAccModeless

	CHPersAccModeless& CHPersAccModeless::operator =( const CHPersAccModeless& objCopy ) {
		return *this;
	} // CHPersAccModeless& operator =

	void CHPersAccModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHPersAcc::Init( );
		udListHandlerModeless::Init( );
	} // void Init

	void CHPersAccModeless::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			udPFormHandler handler = ( udPFormHandler ) g_pApp->GetHandlerManager( )->GetHandler( "persacc_form" );
			Gtk::TreeModel::Row row = *i;
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );

			g_pApp->GetHandlerManager( )->SetState( "persacc_form" );
		}
	} // void OnRowActivated

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPersAccPick::CHPersAccPick( ) : CHPersAcc( ) {
	} // CHPersAccPick

	CHPersAccPick::CHPersAccPick( const CHPersAccPick& objCopy ) : CHPersAcc( ) {
	} // CHPersAccPick

	CHPersAccPick::~CHPersAccPick( ) {
	} // ~CHPersAccPick

	CHPersAccPick& CHPersAccPick::operator =( const CHPersAccPick& objCopy ) {
		return *this;
	} // CHPersAccPick& operator =

	void CHPersAccPick::Init( ) {
		m_wndListWrap->pack_end( m_boxButton, false, false, 0 );
		CHPersAcc::Init( );
	} // void CHPersAccPick

	void CHPersAccPick::OnButtonOk( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
		Gtk::TreeModel::iterator itSelected = m_objTreeView.get_selection( )->get_selected( );
		if ( itSelected ) {
			Gtk::TreeModel::Row row = *itSelected;
			m_pPickedObject = GetProjectPersAcc( );
			m_pPickedObject->get( "id" )->ini( row[ m_pColModel->m_col_id ] );
			( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
		} else {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Выберите лицевой счет" ),
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

	void CHPersAccPick::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndListWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	void CHPersAccPick::OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) {
		Gtk::TreeModel::iterator i = m_objTreeModel->get_iter( path.to_string( ) );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndListWrap->get_toplevel( );
			udPHPersAccFormModal handler = new udHPersAccFormModal( );
			handler->Init( );
			handler->GetData( )->SetId( row[ m_pColModel->m_col_id ] );
			handler->Load( );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование лицевого счета" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование лицевого счета" ) );
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

	CFDEPersAccObject::CFDEPersAccObject( ) :
		udFormDataExtra( ),
		m_pData( NULL )
	{
	} // CFDEPersAccObject

	CFDEPersAccObject::CFDEPersAccObject( const CFDEPersAccObject& objCopy ) : udFormDataExtra( ) {
	} // CFDEPersAccObject

	CFDEPersAccObject::~CFDEPersAccObject( ) {
	} // ~CFDEPersAccObject

	CFDEPersAccObject& CFDEPersAccObject::operator =( const CFDEPersAccObject& objCopy ) {
		return *this;
	} // CFDEPersAccObject& operator =

	void CFDEPersAccObject::Load( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDEPersAccObject::Load, enter\n" );
		if ( m_pData ) {
			udInt type		= m_pData->GetData( )->get( "type" )->toInt( );
			udPFDEObject	pFdeObject1 = ( udPFDEObject ) m_pData->GetExtraData( udHPersAccForm::ctrlidObject1 ),
							pFdeObject2 = ( udPFDEObject ) m_pData->GetExtraData( udHPersAccForm::ctrlidObject2 );

			if ( type == udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) {
				g_pApp->GetLog( )->Write( "CFDEPersAccObject::Load, client <=> company\n" );
				pFdeObject1->SetCallback( GetProjectClient );
				pFdeObject2->SetCallback( GetProjectCompany );
			} else if ( type == udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) {
				g_pApp->GetLog( )->Write( "CFDEPersAccObject::Load, clientur <=> company\n" );
				pFdeObject1->SetCallback( GetProjectClientUr );
				pFdeObject2->SetCallback( GetProjectCompany );
			} else if ( type = udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) {
				g_pApp->GetLog( )->Write( "CFDEPersAccObject::Load, company <=> partner\n" );
				pFdeObject1->SetCallback( GetProjectCompany );
				pFdeObject2->SetCallback( GetProjectPartner );
			} else {
				g_pApp->GetLog( )->Write( "CFDEPersAccObject::Load, unknown type=" + toString( type ) + "\n" );
				pFdeObject1->SetCallback( NULL );
				pFdeObject2->SetCallback( NULL );
			}
		}
		g_pApp->GetLog( )->Write( "CFDEPersAccObject::Load, exit\n" );
	} // void Load

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	udPBase GetProjectPersAccSummList( ) {
		udPBase ret = GetProjectFinanceSumm( );
		ret->get( "currency_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectPersAccSummList

	udPBase GetProjectPersAccOperationList( ) {
		udPBase ret = GetProjectFinanceOperation( );
		return ret;
	} // udPBase GetProjectPersAccOperationList

	udPFormHandler GetHPersAccForm( ) {
		return new CHPersAccFormModal;
	} // udPFormHandler GetHPersAccForm

	CHPersAccForm::CHPersAccForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Лицевой счет" ) ),
		m_lblObject1( Glib::locale_to_utf8( "" ) ),
		m_lblObject2( Glib::locale_to_utf8( "" ) ),
		m_inpObject1( Glib::locale_to_utf8( "" ) ),
		m_inpObject2( Glib::locale_to_utf8( "" ) ),
		m_lblSumm( Glib::locale_to_utf8( "Суммы" ) ),
		m_lblOperation( Glib::locale_to_utf8( "Операции" ) )
	{
		m_btnCancel.set_label( Glib::locale_to_utf8( "Назад" ) );

		m_szName = "persacc_form";

		m_pData->SetName( "persacc" );
		m_pData->SetGetObject( GetProjectPersAcc );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblObject1.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblObject1.set_size_request( 250 );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEPersAccObject pFdePersAccObject = new udFDEPersAccObject;
		pFdePersAccObject->SetMark( ctrlidPersAccConfig );
		pFdePersAccObject->SetDataObject( m_pData );
		m_pData->AddExtraData( pFdePersAccObject );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
		pFde->SetCallback( GetProjectClient );
		pFde->SetMark( ctrlidObject1 );
		m_pData->AddExtraData( pFde );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/*udPFDEObject*/ pFde = new udFDEObject;
		pFde->SetCallback( GetProjectCompany );
		pFde->SetMark( ctrlidObject2 );
		m_pData->AddExtraData( pFde );
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
		m_btnSummAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHPersAccForm::OnFormListSumm_Add ) );

		m_btnSummBack = m_inpSumm.AddButton( Glib::locale_to_utf8( "Снять" ) );
		m_btnSummBack->signal_clicked( ).connect( sigc::mem_fun( this, &CHPersAccForm::OnFormListSumm_Back ) );

		m_btnSummConvert = m_inpSumm.AddButton( Glib::locale_to_utf8( "Конвертировать" ) );
		m_btnSummConvert->signal_clicked( ).connect( sigc::mem_fun( this, &CHPersAccForm::OnFormListSumm_Convert ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectPersAccSummList );
		pFdeList->SetCallbackSend( GetProjectFinanceSumm );
		pFdeList->SetMark( ctrlidSumm );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpOperation.set_size_request( 400, 200 );
		m_pModelFormListOperation = new ColModelFormListOperation;
		m_objOperationModel = Gtk::ListStore::create( *m_pModelFormListOperation );
		
		pTree = m_inpOperation.GetTreeView( );
		pTree->set_model( m_objOperationModel );
		pTree->append_column( Glib::locale_to_utf8( "Дата" ), m_pModelFormListOperation->m_col_created );
		pTree->append_column( Glib::locale_to_utf8( "Тип операции" ), m_pModelFormListOperation->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Тип денег" ), m_pModelFormListOperation->m_col_money );
		m_inpOperation.SetColumnDefaults( );
		
		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectPersAccOperationList );
		pFdeList->SetCallbackSend( GetProjectFinanceOperation );
		pFdeList->SetMark( ctrlidOperation );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHPersAccForm

	CHPersAccForm::CHPersAccForm( const CHPersAccForm& objCopy ) : udFormHandler( ) {
	} // CHPersAccForm

	CHPersAccForm::~CHPersAccForm( ) {
	} // ~CHPersAccForm

	CHPersAccForm& CHPersAccForm::operator =( const CHPersAccForm& objCopy ) {
		return *this;
	} // CHPersAccForm& operator =

	void CHPersAccForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Наименование объекта 1
		m_tblInput.attach( m_lblObject1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpObject1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Наименование объекта 1
		m_tblInput.attach( m_lblObject2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpObject2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Суммы
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Операции
		m_tblInput.attach( m_lblOperation, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpOperation, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHPersAccForm::Clear( ) {
		udFormHandler::Clear( );

		m_lblObject1.set_text( "" );
		m_lblObject2.set_text( "" );
		m_inpObject1.set_text( "" );
		m_inpObject2.set_text( "" );
		m_objSummModel->clear( );
		m_objOperationModel->clear( );
	} // void Clear

	void CHPersAccForm::Load( ) {
		udFormHandler::Load( );

		udPBase obj = m_pData->GetData( );

		udInt type		= obj->get( "type" )->toInt( );
		udPFDEObject	pFdeObject1 = ( udPFDEObject ) m_pData->GetExtraData( udHPersAccForm::ctrlidObject1 ),
						pFdeObject2 = ( udPFDEObject ) m_pData->GetExtraData( udHPersAccForm::ctrlidObject2 );

		if ( type == udProjectPersAcc::PERS_ACC_CLIENT_COMPANY ) {
			m_lblObject1.set_text( Glib::locale_to_utf8( "Клиент (физ. лицо)" ) );
			m_lblObject2.set_text( Glib::locale_to_utf8( "Компания" ) );
			m_inpObject1.set_text( Glib::locale_to_utf8( pFdeObject1->GetObject( )->get( "fio" )->toString( ) ) );
			m_inpObject2.set_text( Glib::locale_to_utf8( pFdeObject2->GetObject( )->get( "name" )->toString( ) ) );
		} else if ( type == udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY ) {
			m_lblObject1.set_text( Glib::locale_to_utf8( "Клиент (юр. лицо)" ) );
			m_lblObject2.set_text( Glib::locale_to_utf8( "Компания" ) );
			m_inpObject1.set_text( Glib::locale_to_utf8( pFdeObject1->GetObject( )->get( "name" )->toString( ) ) );
			m_inpObject2.set_text( Glib::locale_to_utf8( pFdeObject2->GetObject( )->get( "name" )->toString( ) ) );
		} else if ( type = udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) {
			m_lblObject1.set_text( Glib::locale_to_utf8( "Компания" ) );
			m_lblObject2.set_text( Glib::locale_to_utf8( "Партнер" ) );
			m_inpObject1.set_text( Glib::locale_to_utf8( pFdeObject1->GetObject( )->get( "name" )->toString( ) ) );
			m_inpObject2.set_text( Glib::locale_to_utf8( pFdeObject2->GetObject( )->get( "name" )->toString( ) ) );
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
				row[ m_pModelFormListSumm->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objOperationModel->append( ) );
				row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListOperation->m_col_money		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetMoneyTypeName( add->get( "money" )->toInt( ) ) ) );
				row[ m_pModelFormListOperation->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	udInt CHPersAccForm::Validate( ) {
		return 1;
	} // udInt Validate

	void CHPersAccForm::Send( ) {
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHPersAccForm::OnFormListSumm_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummAddFormModal handler = new udHFinanceSummAddFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Зачисление на счет" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Зачисление на счет" ) );
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
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummAdd( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( "persacc" );
				data.add( ( udInt ) opidSummAdd );
				data.add( m_pData->GetData( )->get( "id" )->toUInt( ) );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// считываем данные
				//////////////////////////////////////////////////////////////////////////////////////////////
				bLoad = true;
				udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidSumm );
				m_objSummModel->clear( );
				pFdeList->Clear( );
				pFdeList->Load( sock );
				/*udPFDEList*/ pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation );
				m_objOperationModel->clear( );
				pFdeList->Clear( );
				pFdeList->Load( sock );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( handler );

		if ( bLoad ) {
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
					row[ m_pModelFormListSumm->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objOperationModel->append( ) );
					row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_money		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetMoneyTypeName( add->get( "money" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OnFormListSumm_Add

	void CHPersAccForm::OnFormListSumm_Back( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummBackFormModal handler = new udHFinanceSummBackFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Снятие со счета" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Снятие со счета" ) );
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
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummBack( );
				pObjWrite->apply( obj, true, true );

				string log = "\nsumm back: ";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( "persacc" );
				data.add( ( udInt ) opidSummBack );
				data.add( m_pData->GetData( )->get( "id" )->toUInt( ) );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				//sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				//sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					//msg = L"Не достаточно средств для снятия";
				}
				//////////////////////////////////////////////////////////////////////////////////////////////
				bLoad = true;
				udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidSumm );
				m_objSummModel->clear( );
				pFdeList->Clear( );
				pFdeList->Load( sock );
				/*udPFDEList*/ pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation );
				m_objOperationModel->clear( );
				pFdeList->Clear( );
				pFdeList->Load( sock );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( handler );

		if ( bLoad ) {
			//////////////////////////////////////////////////////////////////////////////////////////////////
			udPArrBase arrObject = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////////////
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
					row[ m_pModelFormListSumm->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objOperationModel->append( ) );
					row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_money		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetMoneyTypeName( add->get( "money" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OnFormListSumm_Back

	void CHPersAccForm::OnFormListSumm_Convert( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummConvertFormModal handler = new udHFinanceSummConvertFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Конвертация" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Конвертация" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		bool bLoad = false;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			string log = "\nsumm convert: ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummConvert( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( "persacc" );
				data.add( ( udInt ) opidSummConvert );
				data.add( m_pData->GetData( )->get( "id" )->toUInt( ) );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				//sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				//sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					//msg = L"Не достаточно средств для конвертации";
				} else if ( code == 1 ) {
					//msg = L"Отсутствует валютная пара для конвертации";
				} else if ( code == 2 ) {
					//msg = L"Не установлен курс валютной пары";
				}
				//////////////////////////////////////////////////////////////////////////////////////////////
				bLoad = true;
				udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidSumm );
				m_objSummModel->clear( );
				pFdeList->Clear( );
				pFdeList->Load( sock );
				/*udPFDEList*/ pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation );
				m_objOperationModel->clear( );
				pFdeList->Clear( );
				pFdeList->Load( sock );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( handler );

		if ( bLoad ) {
			//////////////////////////////////////////////////////////////////////////////////////////////////
			udPArrBase arrObject = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////////////
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
					row[ m_pModelFormListSumm->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOperation ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objOperationModel->append( ) );
					row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_money		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetMoneyTypeName( add->get( "money" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OnFormListSumm_Convert

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPersAccFormModal::CHPersAccFormModal( ) : CHPersAccForm( ) {
	} // CHPersAccFormModal

	CHPersAccFormModal::CHPersAccFormModal( const CHPersAccFormModal& objCopy ) : CHPersAccForm( ) {
	} // CHPersAccFormModal

	CHPersAccFormModal::~CHPersAccFormModal( ) {
	} // ~CHPersAccFormModal

	CHPersAccFormModal& CHPersAccFormModal::operator =( const CHPersAccFormModal& objCopy ) {
		return *this;
	} // CHPersAccFormModal& operator =

	void CHPersAccFormModal::OnButtonOk( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHPersAccFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void CHPersAccForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHPersAccFormModeless::CHPersAccFormModeless( ) : CHPersAccForm( ) {
	} // CHPersAccFormModeless

	CHPersAccFormModeless::CHPersAccFormModeless( const CHPersAccFormModeless& objCopy ) : CHPersAccForm( ) {
	} // CHPersAccFormModeless

	CHPersAccFormModeless::~CHPersAccFormModeless( ) {
	} // ~CHPersAccFormModeless

	CHPersAccFormModeless& CHPersAccFormModeless::operator =( const CHPersAccFormModeless& objCopy ) {
		return *this;
	} // CHPersAccFormModeless& operator =

	void CHPersAccFormModeless::OnButtonOk( ) {
		g_pApp->GetHandlerManager( )->SetState( "persacc" );
	} // void OnButtonOk

	void CHPersAccFormModeless::OnButtonCancel( ) {
		g_pApp->GetHandlerManager( )->SetState( "persacc" );
	} // void CHPersAccForm

	void CHPersAccFormModeless::Init( ) {
		CHPersAccForm::Init( );
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndFormWrap );
	} // void Init

	void CHPersAccFormModeless::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void CHPersAccFormModeless

} // namespace UdSDK
