
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourFinanceSummCostList( ) {
		udPBase ret = GetProjectFinanceSumm( );
		ret->get( "currency_" )->ini( wstring( L"" ) );
		ret->get( "partner" )->ini( ( udUInt ) 0  );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourFinanceSummCostList

	udPBase GetProjectTourFinanceSummServiceList( ) {
		udPBase ret = GetProjectFinanceSumm( );
		ret->get( "currency_" )->ini( wstring( L"" ) );
		ret->get( "partner" )->ini( ( udUInt ) 0  );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourFinanceSummServiceList

	CFDETourFinance::CFDETourFinance( ) : udFDEObject( ), m_pForm( NULL ) {
	} // CFDETourApp

	CFDETourFinance::CFDETourFinance( const CFDETourFinance& objCopy ) : udFDEObject( ) {
	} // CFDETourApp

	CFDETourFinance::~CFDETourFinance( ) {
	} // ~CFDETourApp

	CFDETourFinance& CFDETourFinance::operator =( const CFDETourFinance& objCopy ) {
		return *this;
	} // CFDETourApp& operator =

	void CFDETourFinance::Load( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDETourFinance::Load, enter\n" );
		if ( m_pForm ) {
			g_pApp->GetLog( )->Write( "CFDETourFinance::Load, ctrlidFinal\n" );
			( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHTourFinanceForm::ctrlidFinal ) )->Load( sock );
			g_pApp->GetLog( )->Write( "CFDETourFinance::Load, ctrlidTour\n" );
			( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHTourFinanceForm::ctrlidTour ) )->Load( sock );
			g_pApp->GetLog( )->Write( "CFDETourFinance::Load, ctrlidCost\n" );
			( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHTourFinanceForm::ctrlidCost ) )->Load( sock );
			g_pApp->GetLog( )->Write( "CFDETourFinance::Load, ctrlidService\n" );
			( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHTourFinanceForm::ctrlidService ) )->Load( sock );
			g_pApp->GetLog( )->Write( "CFDETourFinance::Load, ctrlidTourOperation\n" );
			( ( udPFDEList ) m_pForm->GetData( )->GetExtraData( udHTourFinanceForm::ctrlidTourOperation ) )->Load( sock );
		}
		g_pApp->GetLog( )->Write( "CFDETourFinance::Load, exit\n" );
	} // void Load

	void CFDETourFinance::Clear( ) {
	} // void Clear

	void CFDETourFinance::Send( udPSocket sock ) {
	} // void Send

	void CFDETourFinance::Make( ) {
	} // void Make

	void CFDETourFinance::Make( bool bDelete ) {
	} // void Make

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourFinanceForm::CHTourFinanceForm( ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Финансы тура" ) ),
		m_lblFinal( Glib::locale_to_utf8( "Итоговая цена" ) ),
		m_lblTour( Glib::locale_to_utf8( "Оплата за тур" ) ),
		m_lblCost( Glib::locale_to_utf8( "Себестоимость" ) ),
		m_lblService( Glib::locale_to_utf8( "Оплата услуг" ) ),
		m_pTour( NULL )
	{
		m_szName = "tourfinance_form";

		m_pData->SetName( "tourfinance" );
		m_pData->SetGetObject( GetProjectTour );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		m_lblFinal.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpFinal.set_size_request( 400, 200 );
		m_pModelFormListFinal = new ColModelFormListFinal;
		m_objFinalModel = Gtk::ListStore::create( *m_pModelFormListFinal );
		
		pTree = m_inpFinal.GetTreeView( );
		pTree->set_model( m_objFinalModel );
		pTree->append_column( Glib::locale_to_utf8( "Сумма" ), m_pModelFormListFinal->m_col_summ );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListFinal->m_col_currency );
		//pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListFinal->m_col_type );
		m_inpFinal.SetColumnDefaults( );

		m_btnFinalCalculate = m_inpFinal.AddButton( Glib::locale_to_utf8( "Рассчитать" ) );
		m_btnFinalCalculate->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListFinal_Calculate ) );

		m_btnFinalAdd = m_inpFinal.AddButton( Glib::locale_to_utf8( "Зачислить" ) );
		m_btnFinalAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListFinal_Add ) );

		m_btnFinalBack = m_inpFinal.AddButton( Glib::locale_to_utf8( "Снять" ) );
		m_btnFinalBack->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListFinal_Back ) );

		m_btnFinalConvert = m_inpFinal.AddButton( Glib::locale_to_utf8( "Конвертировать" ) );
		m_btnFinalConvert->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListFinal_Convert ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectPersAccSummList );
		pFdeList->SetCallbackSend( GetProjectFinanceSumm );
		pFdeList->SetMark( ctrlidFinal );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpTour.set_size_request( 400, 200 );
		m_pModelFormListTour = new ColModelFormListTour;
		m_objTourModel = Gtk::ListStore::create( *m_pModelFormListTour );
		
		pTree = m_inpTour.GetTreeView( );
		pTree->set_model( m_objTourModel );
		pTree->append_column( Glib::locale_to_utf8( "Сумма" ), m_pModelFormListTour->m_col_summ );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListTour->m_col_currency );
		//pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListTour->m_col_type );
		m_inpTour.SetColumnDefaults( );

		m_btnTourPayOrder = m_inpTour.AddButton( Glib::locale_to_utf8( "Указание" ) );
		m_btnTourPayOrder->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListTour_PayOrder ) );

		m_btnTourAdd = m_inpTour.AddButton( Glib::locale_to_utf8( "Зачислить" ) );
		m_btnTourAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListTour_Add ) );

		m_btnTourBack = m_inpTour.AddButton( Glib::locale_to_utf8( "Снять" ) );
		m_btnTourBack->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListTour_Back ) );

		m_btnTourConvert = m_inpTour.AddButton( Glib::locale_to_utf8( "Конвертировать" ) );
		m_btnTourConvert->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListTour_Convert ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectPersAccSummList );
		pFdeList->SetCallbackSend( GetProjectFinanceSumm );
		pFdeList->SetMark( ctrlidTour );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpCost.set_size_request( 400, 200 );
		m_pModelFormListCost = new ColModelFormListCost;
		m_objCostModel = Gtk::ListStore::create( *m_pModelFormListCost );
		
		pTree = m_inpCost.GetTreeView( );
		pTree->set_model( m_objCostModel );
		pTree->append_column( Glib::locale_to_utf8( "Сумма" ), m_pModelFormListCost->m_col_summ );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListCost->m_col_currency );
		//pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListCost->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Партнер" ), m_pModelFormListCost->m_col_partner );
		m_inpCost.SetColumnDefaults( );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourFinanceSummCostList );
		pFdeList->SetCallbackSend( GetProjectFinanceSumm );
		pFdeList->SetMark( ctrlidCost );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpService.set_size_request( 400, 200 );
		m_pModelFormListService = new ColModelFormListService;
		m_objServiceModel = Gtk::ListStore::create( *m_pModelFormListService );
		
		pTree = m_inpService.GetTreeView( );
		pTree->set_model( m_objServiceModel );
		pTree->append_column( Glib::locale_to_utf8( "Сумма" ), m_pModelFormListService->m_col_summ );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListService->m_col_currency );
		//pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListService->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Партнер" ), m_pModelFormListService->m_col_partner );
		m_inpService.SetColumnDefaults( );

		m_btnServiceAdd = m_inpService.AddButton( Glib::locale_to_utf8( "Зачислить" ) );
		m_btnServiceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListService_Add ) );

		m_btnServiceBack = m_inpService.AddButton( Glib::locale_to_utf8( "Снять" ) );
		m_btnServiceBack->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListService_Back ) );

		m_btnServiceConvert = m_inpService.AddButton( Glib::locale_to_utf8( "Конвертировать" ) );
		m_btnServiceConvert->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourFinanceForm::OnFormListService_Convert ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourFinanceSummServiceList );
		pFdeList->SetCallbackSend( GetProjectFinanceSumm );
		pFdeList->SetMark( ctrlidService );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_inpOperation.set_size_request( 400, 200 );
		m_pModelFormListOperation = new ColModelFormListOperation;
		m_objOperationModel = Gtk::ListStore::create( *m_pModelFormListOperation );
		
		pTree = m_inpOperation.GetTreeView( );
		pTree->set_model( m_objOperationModel );
		pTree->append_column( Glib::locale_to_utf8( "Дата" ), m_pModelFormListOperation->m_col_created );
		pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListOperation->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Сумма" ), m_pModelFormListOperation->m_col_summ );
		pTree->append_column( Glib::locale_to_utf8( "Валюта" ), m_pModelFormListOperation->m_col_currency );
		//pTree->append_column( Glib::locale_to_utf8( "Тип денег" ), m_pModelFormListOperation->m_col_summ_type );
		m_inpOperation.SetColumnDefaults( );

		pFdeList = new udFDETourPayOperation;
		pFdeList->SetMark( ctrlidTourOperation );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTourFinanceForm

	CHTourFinanceForm::CHTourFinanceForm( const CHTourFinanceForm& objCopy ) : udFormHandler( ) {
	} // CHTourFinanceForm

	CHTourFinanceForm::~CHTourFinanceForm( ) {
	} // ~CHTourFinanceForm

	CHTourFinanceForm& CHTourFinanceForm::operator =( const CHTourFinanceForm& objCopy ) {
		return *this;
	} // CHTourFinanceForm& operator =

	void CHTourFinanceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Итоговая цена
		m_tblInput.attach( m_lblFinal, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpFinal, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		Gtk::HBox* pBoxPay = Gtk::manage( new Gtk::HBox );

		pBoxPay->pack_start( m_inpTour, Gtk::PACK_SHRINK );
		pBoxPay->pack_start( m_inpOperation, Gtk::PACK_SHRINK );

		// Оплата за тур
		//m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		//m_tblInput.attach( m_inpTour, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		m_tblInput.attach( m_lblTour, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( *pBoxPay, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Себестоимость
		m_tblInput.attach( m_lblCost, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCost, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Оплата услуг
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// кнопки
		//m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		//m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHTourFinanceForm::Clear( ) {
		udFormHandler::Clear( );

		m_objFinalModel->clear( );
		m_objTourModel->clear( );
		m_objOperationModel->clear( );
		m_objCostModel->clear( );
		m_objServiceModel->clear( );
	} // void Clear

	void CHTourFinanceForm::Load( ) {
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, enter\n" );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, ctrlidFinal\n" );
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objFinalModel->append( ) );
				row[ m_pModelFormListFinal->m_col_summ ] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListFinal->m_col_currency ] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListFinal->m_col_type ] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListFinal->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, ctrlidTour\n" );
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objTourModel->append( ) );
				row[ m_pModelFormListTour->m_col_summ ] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListTour->m_col_currency ] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListTour->m_col_type ] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListTour->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, ctrlidCost\n" );
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidCost ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objCostModel->append( ) );
				row[ m_pModelFormListCost->m_col_summ ] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListCost->m_col_currency ] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListCost->m_col_type ] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListCost->m_col_partner ] = Glib::locale_to_utf8( add->get( "partner_" )->toString( ) );
				row[ m_pModelFormListCost->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, ctrlidService\n" );
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objServiceModel->append( ) );
				row[ m_pModelFormListService->m_col_summ ] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListService->m_col_currency ] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListService->m_col_type ] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListService->m_col_partner ] = Glib::locale_to_utf8( add->get( "partner_" )->toString( ) );
				row[ m_pModelFormListService->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, ctrlidTourOperation\n" );
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objOperationModel->append( ) );
				row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListOperation->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListOperation->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListOperation->m_col_summ_type	] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "summ_type" )->toInt( ) ) ) );
				row[ m_pModelFormListOperation->m_col_object	] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourFinanceForm::Load, exit\n" );
	} // void Load

	udInt CHTourFinanceForm::Validate( ) {
		return 0;
	} // udInt Validate

	void CHTourFinanceForm::Send( ) {
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourFinanceForm::OnFormListFinal_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummAddFormModal handler = new udHFinanceSummAddFormModal( udProjectFinanceSumm::SummWhite, udProjectFinanceOperation::fctCash );
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
		if ( obj && m_pTour ) {
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

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummAdd );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code > 0 ) {
					bLoad = true;
					udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal );
					m_objFinalModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				}
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
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objFinalModel->append( ) );
					row[ m_pModelFormListFinal->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListFinal->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListFinal->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListFinal->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OnFormListFinal_Add

	void CHTourFinanceForm::OnFormListFinal_Back( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummBackFormModal handler = new udHFinanceSummBackFormModal( udProjectFinanceSumm::SummWhite, udProjectFinanceOperation::fctCash );
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
		if ( obj && m_pTour ) {
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

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummBack );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code > 0 ) {
					bLoad = true;
					udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal );
					m_objFinalModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				}
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
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objFinalModel->append( ) );
					row[ m_pModelFormListFinal->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListFinal->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListFinal->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListFinal->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OnFormListFinal_Back

	void CHTourFinanceForm::OnFormListFinal_Convert( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummConvertFormModal handler = new udHFinanceSummConvertFormModal( udProjectFinanceSumm::SummWhite );
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
		if ( obj && m_pTour ) {
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

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummConvert );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code > 0 ) {
					bLoad = true;
					udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal );
					m_objFinalModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				}
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
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objFinalModel->append( ) );
					row[ m_pModelFormListFinal->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListFinal->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListFinal->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListFinal->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OnFormListFinal_Convert

	void CHTourFinanceForm::OnFormListFinal_Calculate( ) {
		assert( m_pTour );

		udArrBase arrSumm;

		m_pTour->CalculateFinal( arrSumm );

		m_objFinalModel->clear( );
		
		if ( arrSumm.empty( ) ) {
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			mb::err(
				*wndParent, Glib::locale_to_utf8( "Ошибка" ),
				Glib::locale_to_utf8( "Отсутствуют суммы для наполнения, проверьте таблицы цен услуг" )
			);
		} else {
			udPFDEList pList = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidFinal ) );
			udPArrBase arrObject = pList->GetArray( );
			udPBase add = NULL;

			pList->Clear( );

			string log = "\n";
			
			for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
				add = *i;
				add->dump( log );
				arrObject->push_back( add );
				log += "\n";
			}

			g_pApp->GetLog( )->Write( log );

			arrSumm.clear( );

			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objFinalModel->append( ) );
				row[ m_pModelFormListFinal->m_col_summ ] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListFinal->m_col_currency ] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListFinal->m_col_type ] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListFinal->m_col_object ] = add;
			}
		}
	} // void OnFormListFinal_Calculate

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourFinanceForm::OnFormListTour_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummAddFormModal handler = new udHFinanceSummAddFormModal( );
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
		Glib::ustring msg;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj && m_pTour ) {
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

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummAdd );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTour );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					bLoad = true;
					udPFDEList pFdeList = NULL;
					
					pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidTour );
					m_objTourModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );

					pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation );
					m_objOperationModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				} else {
					if ( code == 0 ) {
						msg = Glib::locale_to_utf8( "Не достаточно средств для снятия" );
					} else if ( code == 1 ) {
						msg = Glib::locale_to_utf8( "Лицевой счет не найден" );
					}
					g_pApp->GetLog( )->Write( "CHTourFinanceForm::OnFormListTour_Add, code=" + toString( ( udInt ) code ) + "\n" );
				}
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
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objTourModel->append( ) );
					row[ m_pModelFormListTour->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListTour->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListTour->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListTour->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objOperationModel->append( ) );
					row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListOperation->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListOperation->m_col_summ_type	] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "summ_type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
		if ( !msg.empty( ) ) {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Ошибка при зачислении" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( msg );
			dialog.run( );
		}
	} // void OnFormListTour_Add

	void CHTourFinanceForm::OnFormListTour_Back( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummBackFormModal handler = new udHFinanceSummBackFormModal( );
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
		Glib::ustring msg;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj && m_pTour ) {
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

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummBack );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTour );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					bLoad = true;
					udPFDEList pFdeList = NULL;

					pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidTour );
					m_objTourModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );

					pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation );
					m_objOperationModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				} else {
					if ( code == 0 ) {
						msg = Glib::locale_to_utf8( "Не достаточно средств для возврата" );
					}
					g_pApp->GetLog( )->Write( "CHTourFinanceForm::OnFormListTour_Back, code=" + toString( ( udInt ) code ) + "\n"  );
				}
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
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objTourModel->append( ) );
					row[ m_pModelFormListTour->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListTour->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListTour->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListTour->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objOperationModel->append( ) );
					row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListOperation->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListOperation->m_col_summ_type	] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "summ_type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
		if ( !msg.empty( ) ) {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Ошибка при снятии" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( msg );
			dialog.run( );
		}
	} // void OnFormListTour_Back

	void CHTourFinanceForm::OnFormListTour_Convert( ) {
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
		Glib::ustring msg;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj && m_pTour ) {
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

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummConvert );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTour );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					bLoad = true;
					udPFDEList pFdeList = NULL;
					
					pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidTour );
					m_objTourModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );

					pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation );
					m_objOperationModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				} else {
					if ( code == 0 ) {
						msg = Glib::locale_to_utf8( "Не достаточно средств для конвертации" );
					} else if ( code == 1 ) {
						msg = Glib::locale_to_utf8( "Отсутствует валютная пара для конвертации" );
					} else if ( code == 2 ) {
						msg = Glib::locale_to_utf8( "Не установлен курс валютной пары" );
					}
					g_pApp->GetLog( )->Write( "CHTourFinanceForm::OnFormListTour_Convert, code=" + toString( ( udInt ) code ) + "\n" );
				}
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
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTour ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objTourModel->append( ) );
					row[ m_pModelFormListTour->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListTour->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListTour->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListTour->m_col_object ] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTourOperation ) )->GetArray( );
			if ( arrObject && !arrObject->empty( ) ) {
				udPBase add = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = *i;
					row = *( m_objOperationModel->append( ) );
					row[ m_pModelFormListOperation->m_col_created	] = Glib::locale_to_utf8( add->get( "created" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListOperation->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceOperation::GetOperationTypeName( add->get( "type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
					row[ m_pModelFormListOperation->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
					//row[ m_pModelFormListOperation->m_col_summ_type	] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "summ_type" )->toInt( ) ) ) );
					row[ m_pModelFormListOperation->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
		if ( !msg.empty( ) ) {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Ошибка при конвертации" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( msg );
			dialog.run( );
		}
	} // void OnFormListTour_Convert

	void CHTourFinanceForm::OnFormListTour_PayOrder( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourPayOrderFormModal handler = new udHTourPayOrderFormModal( );
		handler->SetStateValue( ( udInt ) udProjectTourPayOrder::stOpened );
		handler->SetTourValue( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание указания на оплату" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание указания на оплату" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/
		udDELETE( handler );
	} // void OnFormListTour_PayOrder

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourFinanceForm::OnFormListService_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummAddTourServiceFormModal handler = new udHFinanceSummAddTourServiceFormModal( );
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
		Glib::ustring msg;

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
				pObjWrite->get( "partner" )->ini( obj->get( "partner" )->toUInt( ) );

				string log = "\npObjWrite:";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummAdd );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtService );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// считываем данные
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					bLoad = true;
					udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
					m_objServiceModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				} else {
					if ( code == 0 ) {
						msg = Glib::locale_to_utf8( "Не достаточно средств для снятия" );
					} else if ( code == 1 ) {
						msg = Glib::locale_to_utf8( "Лицевой счет не найден" );
					}
					g_pApp->GetLog( )->Write( "code=" + toString( ( udInt ) code ) + "\n" );
				}
				//////////////////////////////////////////////////////////////////////////////////////////////
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( handler );

		if ( bLoad ) {
			ReloadServiceSumm( );
		}
		if ( !msg.empty( ) ) {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Ошибка при зачислении" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( msg );
			dialog.run( );
		}
	} // void OnFormListService_Add

	void CHTourFinanceForm::OnFormListService_Back( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummBackTourServiceFormModal handler = new udHFinanceSummBackTourServiceFormModal( );
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
		Glib::ustring msg;

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
				pObjWrite->get( "partner" )->ini( obj->get( "partner" )->toUInt( ) );

				string log = "\npObjWrite: ";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummBack );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtService );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					bLoad = true;
					udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
					m_objServiceModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				} else {
					if ( code == 0 ) {
						msg = Glib::locale_to_utf8( "Не достаточно средств для возврата" );
					}
					g_pApp->GetLog( )->Write( "code=" + toString( ( udInt ) code ) + "\n" );
				}
				//////////////////////////////////////////////////////////////////////////////////////////////
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( handler );

		if ( bLoad ) {
			ReloadServiceSumm( );
		}
		if ( !msg.empty( ) ) {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Ошибка при снятии" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( msg );
			dialog.run( );
		}
	} // void OnFormListService_Back

	void CHTourFinanceForm::OnFormListService_Convert( ) {
		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHFinanceSummConvertTourServiceFormModal handler = new udHFinanceSummConvertTourServiceFormModal( );
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
		Glib::ustring msg;

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			string log = "\npObjWrite: ";
			obj->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );

			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummConvert( );
				pObjWrite->apply( obj, true, true );
				pObjWrite->get( "partner" )->ini( obj->get( "partner" )->toUInt( ) );

				string log = "\npObjWrite: ";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTourForm::opidSummConvert );
				data.add( m_pTour->GetData( )->GetData( )->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtService );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// считываем данные
				//////////////////////////////////////////////////////////////////////////////////////////////
				// считываем данные
				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 80 ) {
					bLoad = true;
					udPFDEList pFdeList = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
					m_objServiceModel->clear( );
					pFdeList->Clear( );
					pFdeList->Load( sock );
				} else {
					if ( code == 0 ) {
						msg = Glib::locale_to_utf8( "Не достаточно средств для конвертации" );
					} else if ( code == 1 ) {
						msg = Glib::locale_to_utf8( "Отсутствует валютная пара для конвертации" );
					} else if ( code == 2 ) {
						msg = Glib::locale_to_utf8( "Не установлен курс валютной пары" );
					}
					g_pApp->GetLog( )->Write( "code=" + toString( ( udInt ) code ) + "\n" );
				}
				//////////////////////////////////////////////////////////////////////////////////////////////
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}

		udDELETE( handler );

		if ( bLoad ) {
			ReloadServiceSumm( );
		}
		if ( !msg.empty( ) ) {
			Gtk::MessageDialog dialog(
				*wndParent,
				Glib::locale_to_utf8( "Ошибка при конвертации" ),
				false,
				Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_OK
			);
			dialog.set_secondary_text( msg );
			dialog.run( );
		}
	} // void OnFormListService_Convert

	void CHTourFinanceForm::ReloadServiceSumm( ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase arrObject = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
		if ( arrObject && !arrObject->empty( ) ) {
			udPBase add = NULL;
			Gtk::TreeModel::Row row;

			for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
				add = *i;
				row = *( m_objServiceModel->append( ) );
				row[ m_pModelFormListService->m_col_summ		] = Glib::locale_to_utf8( add->get( "summ" )->toString( ) );
				row[ m_pModelFormListService->m_col_currency	] = Glib::locale_to_utf8( add->get( "currency_" )->toString( ) );
				//row[ m_pModelFormListService->m_col_type		] = Glib::locale_to_utf8( ConvertString( udProjectFinanceSumm::GetTypeName( add->get( "type" )->toInt( ) ) ) );
				row[ m_pModelFormListService->m_col_partner		] = Glib::locale_to_utf8( add->get( "partner_" )->toString( ) );
				row[ m_pModelFormListService->m_col_object ] = add;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void ReloadServiceSumm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummAddTourServiceForm::CHFinanceSummAddTourServiceForm( ) :
		CHFinanceSummAddForm( ),
		m_lblPartner( Glib::locale_to_utf8( "Партнер" ) )
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummAddTourServiceForm

	CHFinanceSummAddTourServiceForm::CHFinanceSummAddTourServiceForm( const CHFinanceSummAddTourServiceForm& objCopy ) : CHFinanceSummAddForm( ) {
	} // CHFinanceSummAddTourServiceForm

	CHFinanceSummAddTourServiceForm::~CHFinanceSummAddTourServiceForm( ) {
	} // ~CHFinanceSummAddTourServiceForm

	CHFinanceSummAddTourServiceForm& CHFinanceSummAddTourServiceForm::operator =( const CHFinanceSummAddTourServiceForm& objCopy ) {
		return *this;
	} // CHFinanceSummAddTourServiceForm& operator =

	void CHFinanceSummAddTourServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Партнер
		m_tblInput.attach( m_lblPartner, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPartner, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Способ
		m_tblInput.attach( m_lblHow, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpHow, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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
	} // void Init

	void CHFinanceSummAddTourServiceForm::Clear( ) {
		CHFinanceSummAddForm::Clear( );

		m_inpPartner.SetTitle( "" );
	} // void Clear

	void CHFinanceSummAddTourServiceForm::Load( ) {
	} // void Load

	udInt CHFinanceSummAddTourServiceForm::Validate( ) {
		udInt ret = CHFinanceSummAddForm::Validate( );
		if ( !ret ) {
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPBase obj = NULL;
		
			obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( );
			if ( !obj || !obj->get( "id" )->toUInt( ) ) {
				mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите партнера" ) );
				g_pApp->GetWndWork( )->set_focus( *( m_inpPartner.GetButton( ) ) );
				return 2;
			}
		}

		return ret;
	} // udInt Validate

	void CHFinanceSummAddTourServiceForm::Send( ) {
		CHFinanceSummAddForm::Send( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummAddTourServiceFormModal::CHFinanceSummAddTourServiceFormModal( ) : CHFinanceSummAddTourServiceForm( ) {
	} // CHFinanceSummAddTourServiceFormModal

	CHFinanceSummAddTourServiceFormModal::CHFinanceSummAddTourServiceFormModal( const CHFinanceSummAddTourServiceFormModal& objCopy ) : CHFinanceSummAddTourServiceForm( ) {
	} // CHFinanceSummAddTourServiceFormModal

	CHFinanceSummAddTourServiceFormModal::~CHFinanceSummAddTourServiceFormModal( ) {
	} // ~CHFinanceSummAddTourServiceFormModal

	CHFinanceSummAddTourServiceFormModal& CHFinanceSummAddTourServiceFormModal::operator =( const CHFinanceSummAddTourServiceFormModal& objCopy ) {
		return *this;
	} // CHFinanceSummAddTourServiceFormModal& operator =

	void CHFinanceSummAddTourServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHFinanceSummAddTourServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummBackTourServiceForm::CHFinanceSummBackTourServiceForm( ) :
		CHFinanceSummBackForm( ),
		m_lblPartner( Glib::locale_to_utf8( "Партнер" ) )
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummBackForm

	CHFinanceSummBackTourServiceForm::CHFinanceSummBackTourServiceForm( const CHFinanceSummBackTourServiceForm& objCopy ) : CHFinanceSummBackForm( ) {
	} // CHFinanceSummBackTourServiceForm

	CHFinanceSummBackTourServiceForm::~CHFinanceSummBackTourServiceForm( ) {
	} // ~CHFinanceSummBackTourServiceForm

	CHFinanceSummBackTourServiceForm& CHFinanceSummBackTourServiceForm::operator =( const CHFinanceSummBackTourServiceForm& objCopy ) {
		return *this;
	} // CHFinanceSummBackTourServiceForm& operator =

	void CHFinanceSummBackTourServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Партнер
		m_tblInput.attach( m_lblPartner, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPartner, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Способ
		m_tblInput.attach( m_lblHow, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpHow, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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
	} // void Init

	void CHFinanceSummBackTourServiceForm::Clear( ) {
		CHFinanceSummBackForm::Clear( );
		
		m_inpPartner.SetTitle( "" );
	} // void Clear

	void CHFinanceSummBackTourServiceForm::Load( ) {
	} // void Load

	udInt CHFinanceSummBackTourServiceForm::Validate( ) {
		udInt ret = CHFinanceSummBackForm::Validate( );
		if ( !ret ) {
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPBase obj = NULL;
		
			obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( );
			if ( !obj || !obj->get( "id" )->toUInt( ) ) {
				mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите партнера" ) );
				g_pApp->GetWndWork( )->set_focus( *( m_inpPartner.GetButton( ) ) );
				return 3;
			}
		}

		return ret;
	} // udInt Validate

	void CHFinanceSummBackTourServiceForm::Send( ) {
		CHFinanceSummBackForm::Send( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummBackTourServiceFormModal::CHFinanceSummBackTourServiceFormModal( ) : CHFinanceSummBackTourServiceForm( ) {
	} // CHFinanceSummBackFormModal

	CHFinanceSummBackTourServiceFormModal::CHFinanceSummBackTourServiceFormModal( const CHFinanceSummBackTourServiceFormModal& objCopy ) : CHFinanceSummBackTourServiceForm( ) {
	} // CHFinanceSummBackTourServiceFormModal

	CHFinanceSummBackTourServiceFormModal::~CHFinanceSummBackTourServiceFormModal( ) {
	} // ~CHFinanceSummBackTourServiceFormModal

	CHFinanceSummBackTourServiceFormModal& CHFinanceSummBackTourServiceFormModal::operator =( const CHFinanceSummBackTourServiceFormModal& objCopy ) {
		return *this;
	} // CHFinanceSummBackTourServiceFormModal& operator =

	void CHFinanceSummBackTourServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHFinanceSummBackTourServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummConvertTourServiceForm::CHFinanceSummConvertTourServiceForm( ) :
		CHFinanceSummConvertForm( ),
		m_lblPartner( Glib::locale_to_utf8( "Партнер" ) )
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPFDEObject pFde = new udFDEObject;
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
	} // CHFinanceSummConvertForm

	CHFinanceSummConvertTourServiceForm::CHFinanceSummConvertTourServiceForm( const CHFinanceSummConvertTourServiceForm& objCopy ) : CHFinanceSummConvertForm( ) {
	} // CHFinanceSummConvertTourServiceForm

	CHFinanceSummConvertTourServiceForm::~CHFinanceSummConvertTourServiceForm( ) {
	} // ~CHFinanceSummConvertTourServiceForm

	CHFinanceSummConvertTourServiceForm& CHFinanceSummConvertTourServiceForm::operator =( const CHFinanceSummConvertTourServiceForm& objCopy ) {
		return *this;
	} // CHFinanceSummConvertTourServiceForm& operator =

	void CHFinanceSummConvertTourServiceForm::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Партнер
		m_tblInput.attach( m_lblPartner, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_lblPartner, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта из
		m_tblInput.attach( m_lblCurrency1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта в
		m_tblInput.attach( m_lblCurrency2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип из
		m_tblInput.attach( m_lblType1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип в
		m_tblInput.attach( m_lblType2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpType2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

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
	} // void Init

	void CHFinanceSummConvertTourServiceForm::Clear( ) {
		CHFinanceSummConvertForm::Clear( );

		m_inpPartner.SetTitle( "" );
	} // void Clear

	void CHFinanceSummConvertTourServiceForm::Load( ) {
	} // void Load

	udInt CHFinanceSummConvertTourServiceForm::Validate( ) {
		udInt ret = CHFinanceSummConvertForm::Validate( );
		if ( !ret ) {
			Gtk::Window* wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPBase obj = NULL;
			
			obj = ( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( );
			if ( !obj || !obj->get( "id" )->toUInt( ) ) {
				mb::err( *wndParent, Glib::locale_to_utf8( "Ошибка" ), Glib::locale_to_utf8( "Введите партнера" ) );
				g_pApp->GetWndWork( )->set_focus( *( m_inpPartner.GetButton( ) ) );
				return 2;
			}
		}

		return ret;
	} // udInt Validate

	void CHFinanceSummConvertTourServiceForm::Send( ) {
		CHFinanceSummConvertForm::Send( );
		
		udPBase obj = m_pData->GetData( );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		obj->get( "partner" )->ini(
			( ( udPFDEObject ) m_pData->GetExtraData( ctrlidPartner ) )->GetObject( )->get( "id" )->toUInt( )
		);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHFinanceSummConvertTourServiceFormModal::CHFinanceSummConvertTourServiceFormModal( ) : CHFinanceSummConvertTourServiceForm( ) {
	} // CHFinanceSummConvertTourServiceFormModal

	CHFinanceSummConvertTourServiceFormModal::CHFinanceSummConvertTourServiceFormModal( const CHFinanceSummConvertTourServiceFormModal& objCopy ) : CHFinanceSummConvertTourServiceForm( ) {
	} // CHFinanceSummConvertTourServiceFormModal

	CHFinanceSummConvertTourServiceFormModal::~CHFinanceSummConvertTourServiceFormModal( ) {
	} // ~CHFinanceSummConvertTourServiceFormModal

	CHFinanceSummConvertTourServiceFormModal& CHFinanceSummConvertTourServiceFormModal::operator =( const CHFinanceSummConvertTourServiceFormModal& objCopy ) {
		return *this;
	} // CHFinanceSummConvertFormModal& operator =

	void CHFinanceSummConvertTourServiceFormModal::OnButtonOk( ) {
		if ( Validate( ) != 0 ) {
			return;
		}

		Send( );
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonOk

	void CHFinanceSummConvertTourServiceFormModal::OnButtonCancel( ) {
		( ( Gtk::Window* ) m_wndFormWrap->get_toplevel( ) )->hide( );
	} // void OnButtonCancel

} // namespace UdSDK
