
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CFDETourApp::CFDETourApp( ) : udFDEObject( ) {
	} // CFDETourApp

	CFDETourApp::CFDETourApp( const CFDETourApp& objCopy ) : udFDEObject( ) {
	} // CFDETourApp

	CFDETourApp::~CFDETourApp( ) {
		//////////////////////////////////////////////////////////////////////////////////////
		if ( m_pObject ) {
			delete ( udPArrBase ) m_pObject->get( "occupancy" )->get( );
			delete ( udPArrBase ) m_pObject->get( "program" )->get( );
			delete ( udPArrBase ) m_pObject->get( "transfer" )->get( );
			delete ( udPArrBase ) m_pObject->get( "cover" )->get( );
			delete ( udPArrBase ) m_pObject->get( "visa" )->get( );
			delete ( udPArrBase ) m_pObject->get( "service" )->get( );
			delete ( udPArrBase ) m_pObject->get( "transfer2" )->get( );
		}
		//////////////////////////////////////////////////////////////////////////////////////
	} // ~CFDETourApp

	CFDETourApp& CFDETourApp::operator =( const CFDETourApp& objCopy ) {
		return *this;
	} // CFDETourApp& operator =

	void CFDETourApp::Load( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDETourApp::Load\n" );
		char code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
		if ( code > 0 ) { // приложение существует
			//////////////////////////////////////////////////////////////////////////////////////
			string log = "\n";
			m_pObject->read( sock );
			m_pObject->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			udPArrBase pArr = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "occupancy" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCOccupancy );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "program" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCProgram );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "transfer" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCTransfer );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "cover" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCCover );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "visa" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCVisa );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "service" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCService );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pObject->get( "transfer2" )->get( );
			udDataAccess::Read( sock, *pArr, GetProjectGCTransfer2 );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		g_pApp->GetLog( )->Write( "CFDETourApp::Load, exit\n" );
	} // void Load

	void CFDETourApp::Clear( ) {
		//////////////////////////////////////////////////////////////////////////////////////
		udPArrBase pArr = NULL;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "occupancy" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "program" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "transfer" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "cover" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "visa" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "service" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pObject->get( "transfer2" )->get( );
		ClearArrBase( pArr );
		delete pArr;
		//////////////////////////////////////////////////////////////////////////////////////
		udDELETE( m_pObject );
	} // void Clear

	void CFDETourApp::Send( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDETourApp::Send\n" );
		if ( m_pObject ) {
			udBinaryData data;
			string log = "CFDETourApp::Send, App1:\n";
			m_pObject->dump( data );
			m_pObject->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			udPArrBase pArr = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, occupancy\n" );
			pArr = ( udPArrBase ) m_pObject->get( "occupancy" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, program\n" );
			pArr = ( udPArrBase ) m_pObject->get( "program" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, transfer\n" );
			pArr = ( udPArrBase ) m_pObject->get( "transfer" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, cover\n" );
			pArr = ( udPArrBase ) m_pObject->get( "cover" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, visa\n" );
			pArr = ( udPArrBase ) m_pObject->get( "visa" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, service\n" );
			pArr = ( udPArrBase ) m_pObject->get( "service" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, transfer2\n" );
			pArr = ( udPArrBase ) m_pObject->get( "transfer2" )->get( );
			udDataAccess::Write( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, sending\n" );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			//////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetLog( )->Write( "CFDETourApp::Send, waiting byte\n" );
			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		g_pApp->GetLog( )->Write( "CFDETourApp::Send, exit\n" );
	} // void Send

	void CFDETourApp::Make( ) {
		if ( m_fnGetObject && !m_pObject ) {
			m_pObject = m_fnGetObject( );
			// создание дочерних объектов
			//////////////////////////////////////////////////////////////////////////////////////
			m_pObject->get( "occupancy" )->ini( ( udPVoid ) new udArrBase );
			m_pObject->get( "program" )->ini( ( udPVoid ) new udArrBase );
			m_pObject->get( "transfer" )->ini( ( udPVoid ) new udArrBase );
			m_pObject->get( "cover" )->ini( ( udPVoid ) new udArrBase );
			m_pObject->get( "visa" )->ini( ( udPVoid ) new udArrBase );
			m_pObject->get( "service" )->ini( ( udPVoid ) new udArrBase );
			m_pObject->get( "transfer2" )->ini( ( udPVoid ) new udArrBase );
			//////////////////////////////////////////////////////////////////////////////////////
		}
	} // void Make

	void CFDETourApp::Make( bool bDelete ) {
		if ( bDelete ) {
			udDELETE( m_pObject );
			//////////////////////////////////////////////////////////////////////////////////////
			delete ( udPArrBase ) m_pObject->get( "occupancy" )->get( );
			delete ( udPArrBase ) m_pObject->get( "program" )->get( );
			delete ( udPArrBase ) m_pObject->get( "transfer" )->get( );
			delete ( udPArrBase ) m_pObject->get( "cover" )->get( );
			delete ( udPArrBase ) m_pObject->get( "visa" )->get( );
			delete ( udPArrBase ) m_pObject->get( "service" )->get( );
			delete ( udPArrBase ) m_pObject->get( "transfer2" )->get( );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		Make( );
	} // void Make

	void CFDETourApp::AddToArray( const string& szName, udPBase obj ) {
		if ( m_pObject && m_pObject->isset( szName ) ) {
			pfnGetObject fnGetObject = GetHandlerByName( szName );
			if ( fnGetObject ) {
				udPBase add = fnGetObject( );
				add->apply( obj, true, true );
				( ( udPArrBase ) m_pObject->get( szName )->get( ) )->push_back( add );
			}
		}
	} // void AddToArray

	void CFDETourApp::LoadArrayFrom( const string& szName, udPArrBase arrObject ) {
		if ( m_pObject && m_pObject->isset( szName ) ) {
			pfnGetObject fnGetObject = GetHandlerByName( szName );
			if ( fnGetObject ) {
				udPArrBase arrCur = ( udPArrBase ) m_pObject->get( szName )->get( );
				udPBase add = NULL;

				for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
					add = fnGetObject( );
					add->apply( *i, true, true );
					arrCur->push_back( add );
				}
			}
		}
	} // void LoadArrayFrom

	pfnGetObject CFDETourApp::GetHandlerByName( const string& szName ) {
		if ( szName == "occupancy" ) {
			return GetProjectGCOccupancy;
		} else if ( szName == "program" ) {
			return GetProjectGCProgram;
		} else if ( szName == "transfer" ) {
			return GetProjectGCTransfer;
		} else if ( szName == "cover" ) {
			return GetProjectGCCover;
		} else if ( szName == "visa" ) {
			return GetProjectGCVisa;
		} else if ( szName == "service" ) {
			return GetProjectGCService;
		} else if ( szName == "transfer2" ) {
			return GetProjectGCTransfer2;
		}
		return NULL;
	} // pfnGetObject GetHandlerByName

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourAppForm::CHTourAppForm( CFDETourApp* pFde ) :
		udFormHandler( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 10 ),
		m_lblHeader( Glib::locale_to_utf8( "Приложение тура" ) ),
		m_lblPrice( Glib::locale_to_utf8( "Сумма за тур" ) ),
		m_lblPay1( Glib::locale_to_utf8( "Оплата 1" ) ),
		m_lblPay2( Glib::locale_to_utf8( "Оплата 2" ) ),
		m_lblDate1( Glib::locale_to_utf8( "Дата оплаты 1" ) ),
		m_lblDate2( Glib::locale_to_utf8( "Дата оплаты 2" ) ),
		m_lblDays1( Glib::locale_to_utf8( "За сколько дней до поездки предоставить документы (не позднее)" ) ),
		m_lblDays2( Glib::locale_to_utf8( "В течение скольки дней после бронирования подтвердить тур" ) ),
		m_lblDays3( Glib::locale_to_utf8( "В течение скольки дней после бронирования оплатить тур (не позднее)" ) ),
		m_lblProduct( Glib::locale_to_utf8( "Туристский продукт" ) ),
		m_lblOccupancy( Glib::locale_to_utf8( "Средство размещения" ) ),
		m_lblProgram( Glib::locale_to_utf8( "Экскурсионная программа" ) ),
		m_lblTransferService( Glib::locale_to_utf8( "Услуги по перевозке туристов" ) ),
		m_lblCover( Glib::locale_to_utf8( "Страхование" ) ),
		m_lblVisa( Glib::locale_to_utf8( "Визовое оформление" ) ),
		m_lblService( Glib::locale_to_utf8( "Другие услуги" ) ),
		m_lblTransfer( Glib::locale_to_utf8( "Трансфер" ) ),
		m_pFde( pFde )
	{
		m_szName = "tourapp_form";

		m_pData->SetName( "tourapp" );
		//m_pData->SetGetObject( GetProjectTourApp );
		m_pData->SetGetObject( GetProjectGroupContractApp1 );

		m_lblHeader.set_alignment( Gtk::ALIGN_START, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		m_lblPrice.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblPay1.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblPay2.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDate1.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDate2.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDays1.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDays2.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblDays3.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblProduct.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblOccupancy.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblProgram.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblTransferService.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblCover.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblVisa.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblService.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		m_lblTransfer.set_alignment( Gtk::ALIGN_END, Gtk::ALIGN_START );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		Gtk::TreeView *pTree = NULL;
		udPFDEList pFdeList = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Средства размещения
		m_inpOccupancy.set_size_request( 400, 200 );
		m_pModelFormListOccupancy = new ColModelFormListOccupancy;
		m_objOccupancyModel = Gtk::ListStore::create( *m_pModelFormListOccupancy );
		
		pTree = m_inpOccupancy.GetTreeView( );
		pTree->set_model( m_objOccupancyModel );
		pTree->append_column( Glib::locale_to_utf8( "Наименование" ), m_pModelFormListOccupancy->m_col_name );
		pTree->append_column( Glib::locale_to_utf8( "Категория" ), m_pModelFormListOccupancy->m_col_category );
		pTree->append_column( Glib::locale_to_utf8( "Количество проживающих" ), m_pModelFormListOccupancy->m_col_amount );
		pTree->append_column( Glib::locale_to_utf8( "Питание" ), m_pModelFormListOccupancy->m_col_feed );
		m_inpOccupancy.SetColumnDefaults( );

		m_btnOccupancyEdit = m_inpOccupancy.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnOccupancyEdit->set_sensitive( false );
		m_btnOccupancyEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListOccupancy_Edit ) );

		m_btnOccupancyDel = m_inpOccupancy.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnOccupancyDel->set_sensitive( false );
		m_btnOccupancyDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListOccupancy_Del ) );

		m_btnOccupancyAdd = m_inpOccupancy.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnOccupancyAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListOccupancy_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListOccupancy_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppOccupancyList );
		pFdeList->SetCallbackSend( GetProjectGCOccupancy );
		pFdeList->SetMark( ctrlidOccupancy );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Экскурсионная программа
		m_inpProgram.set_size_request( 400, 200 );
		m_pModelFormListProgram = new ColModelFormListProgram;
		m_objProgramModel = Gtk::ListStore::create( *m_pModelFormListProgram );
		
		pTree = m_inpProgram.GetTreeView( );
		pTree->set_model( m_objProgramModel );
		pTree->append_column( Glib::locale_to_utf8( "Текст" ), m_pModelFormListProgram->m_col_text );
		m_inpProgram.SetColumnDefaults( );

		m_btnProgramEdit = m_inpProgram.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnProgramEdit->set_sensitive( false );
		m_btnProgramEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListProgram_Edit ) );

		m_btnProgramDel = m_inpProgram.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnProgramDel->set_sensitive( false );
		m_btnProgramDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListProgram_Del ) );

		m_btnProgramAdd = m_inpProgram.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnProgramAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListProgram_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListProgram_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppProgramList );
		pFdeList->SetCallbackSend( GetProjectGCProgram );
		pFdeList->SetMark( ctrlidProgram );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Услуги по перевозке
		m_inpTransferService.set_size_request( 400, 200 );
		m_pModelFormListTransferService = new ColModelFormListTransferService;
		m_objTransferServiceModel = Gtk::ListStore::create( *m_pModelFormListTransferService );
		
		pTree = m_inpTransferService.GetTreeView( );
		pTree->set_model( m_objTransferServiceModel );
		pTree->append_column( Glib::locale_to_utf8( "ФИО туриста" ), m_pModelFormListTransferService->m_col_fio );
		pTree->append_column( Glib::locale_to_utf8( "Перевозчик" ), m_pModelFormListTransferService->m_col_trans );
		pTree->append_column( Glib::locale_to_utf8( "Уровень услуг" ), m_pModelFormListTransferService->m_col_level );
		pTree->append_column( Glib::locale_to_utf8( "Дата убытия" ), m_pModelFormListTransferService->m_col_date1 );
		pTree->append_column( Glib::locale_to_utf8( "Дата прибытия" ), m_pModelFormListTransferService->m_col_date2 );
		m_inpTransferService.SetColumnDefaults( );

		m_btnTransferServiceEdit = m_inpTransferService.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnTransferServiceEdit->set_sensitive( false );
		m_btnTransferServiceEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransferService_Edit ) );

		m_btnTransferServiceDel = m_inpTransferService.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnTransferServiceDel->set_sensitive( false );
		m_btnTransferServiceDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransferService_Del ) );

		m_btnTransferServiceAdd = m_inpTransferService.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnTransferServiceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransferService_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransferService_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppTransferServiceList );
		pFdeList->SetCallbackSend( GetProjectGCTransfer );
		pFdeList->SetMark( ctrlidTransferService );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Страхование
		m_inpCover.set_size_request( 400, 200 );
		m_pModelFormListCover = new ColModelFormListCover;
		m_objCoverModel = Gtk::ListStore::create( *m_pModelFormListCover );
		
		pTree = m_inpCover.GetTreeView( );
		pTree->set_model( m_objCoverModel );
		pTree->append_column( Glib::locale_to_utf8( "Тип страховки" ), m_pModelFormListCover->m_col_type );
		pTree->append_column( Glib::locale_to_utf8( "Страховщик" ), m_pModelFormListCover->m_col_name );
		m_inpCover.SetColumnDefaults( );

		m_btnCoverEdit = m_inpCover.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnCoverEdit->set_sensitive( false );
		m_btnCoverEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListCover_Edit ) );

		m_btnCoverDel = m_inpCover.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnCoverDel->set_sensitive( false );
		m_btnCoverDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListCover_Del ) );

		m_btnCoverAdd = m_inpCover.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnCoverAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListCover_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListCover_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppCoverList );
		pFdeList->SetCallbackSend( GetProjectGCCover );
		pFdeList->SetMark( ctrlidCover );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Визовое оформление
		m_inpVisa.set_size_request( 400, 200 );
		m_pModelFormListVisa = new ColModelFormListVisa;
		m_objVisaModel = Gtk::ListStore::create( *m_pModelFormListVisa );
		
		pTree = m_inpVisa.GetTreeView( );
		pTree->set_model( m_objVisaModel );
		pTree->append_column( Glib::locale_to_utf8( "Тип услуги" ), m_pModelFormListVisa->m_col_type );
		m_inpVisa.SetColumnDefaults( );

		m_btnVisaEdit = m_inpVisa.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnVisaEdit->set_sensitive( false );
		m_btnVisaEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListVisa_Edit ) );

		m_btnVisaDel = m_inpVisa.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnVisaDel->set_sensitive( false );
		m_btnVisaDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListVisa_Del ) );

		m_btnVisaAdd = m_inpVisa.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnVisaAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListVisa_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListVisa_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppVisaList );
		pFdeList->SetCallbackSend( GetProjectGCVisa );
		pFdeList->SetMark( ctrlidVisa );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Другие услуги
		m_inpService.set_size_request( 400, 200 );
		m_pModelFormListService = new ColModelFormListService;
		m_objServiceModel = Gtk::ListStore::create( *m_pModelFormListService );
		
		pTree = m_inpService.GetTreeView( );
		pTree->set_model( m_objServiceModel );
		pTree->append_column( Glib::locale_to_utf8( "Наименование" ), m_pModelFormListService->m_col_name );
		pTree->append_column( Glib::locale_to_utf8( "Характеристики" ), m_pModelFormListService->m_col_char );
		m_inpService.SetColumnDefaults( );

		m_btnServiceEdit = m_inpService.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnServiceEdit->set_sensitive( false );
		m_btnServiceEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListService_Edit ) );

		m_btnServiceDel = m_inpService.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnServiceDel->set_sensitive( false );
		m_btnServiceDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListService_Del ) );

		m_btnServiceAdd = m_inpService.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnServiceAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListService_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListService_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppServiceList );
		pFdeList->SetCallbackSend( GetProjectGCService );
		pFdeList->SetMark( ctrlidService );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Трансфер
		m_inpTransfer.set_size_request( 600, 200 );
		m_pModelFormListTransfer = new ColModelFormListTransfer;
		m_objTransferModel = Gtk::ListStore::create( *m_pModelFormListTransfer );
		
		pTree = m_inpTransfer.GetTreeView( );
		pTree->set_model( m_objTransferModel );
		pTree->append_column( Glib::locale_to_utf8( "Маршрут" ), m_pModelFormListTransfer->m_col_route );
		pTree->append_column( Glib::locale_to_utf8( "Тип" ), m_pModelFormListTransfer->m_col_type );
		m_inpTransfer.SetColumnDefaults( );

		m_btnTransferEdit = m_inpTransfer.AddButton( Glib::locale_to_utf8( "Редактировать" ) );
		m_btnTransferEdit->set_sensitive( false );
		m_btnTransferEdit->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransfer_Edit ) );

		m_btnTransferDel = m_inpTransfer.AddButton( Glib::locale_to_utf8( "Удалить" ) );
		m_btnTransferDel->set_sensitive( false );
		m_btnTransferDel->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransfer_Del ) );

		m_btnTransferAdd = m_inpTransfer.AddButton( Glib::locale_to_utf8( "Добавить" ) );
		m_btnTransferAdd->signal_clicked( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransfer_Add ) );

		pTree->get_selection( )->signal_changed( ).connect( sigc::mem_fun( this, &CHTourAppForm::OnFormListTransfer_Select ) );

		pFdeList = new udFDEList;
		pFdeList->SetCallback( GetProjectTourAppTransferList );
		pFdeList->SetCallbackSend( GetProjectGCTransfer2 );
		pFdeList->SetMark( ctrlidTransfer );
		m_pData->AddExtraData( pFdeList );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	} // CHTourAppForm

	CHTourAppForm::CHTourAppForm( const CHTourAppForm& objCopy ) : udFormHandler( ) {
	} // CHTourAppForm

	CHTourAppForm::~CHTourAppForm( ) {
	} // ~CHTourAppForm

	CHTourAppForm& CHTourAppForm::operator =( const CHTourAppForm& objCopy ) {
		return *this;
	} // CHTourAppForm& operator =

	void CHTourAppForm::Init( ) {
		udInt l = 0, r = 1;
		// заголовок
		//m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма за тур
		m_tblInput.attach( m_lblPrice, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPrice, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Оплата 1
		m_tblInput.attach( m_lblPay1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPay1, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Дата оплаты 1
		m_tblInput.attach( m_lblDate1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate1, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Оплата 2
		m_tblInput.attach( m_lblPay2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpPay2, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Дата оплаты 2
		m_tblInput.attach( m_lblDate2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDate2, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// За сколько дней до поездки предоставить документы (не позднее)
		m_tblInput.attach( m_lblDays1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDays1, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// В течение скольки дней после бронирования подтвердить тур
		m_tblInput.attach( m_lblDays2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDays2, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// В течение скольки дней после бронирования оплатить тур (не позднее)
		m_tblInput.attach( m_lblDays3, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpDays3, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Туристский продукт
		m_tblInput.attach( m_lblProduct, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpProduct, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Средства размещения
		m_tblInput.attach( m_lblOccupancy, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpOccupancy, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Туристская программа
		m_tblInput.attach( m_lblProgram, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpProgram, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Услуги по перевозке
		m_tblInput.attach( m_lblTransferService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpTransferService, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Страхование
		m_tblInput.attach( m_lblCover, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpCover, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Визовое оформление
		m_tblInput.attach( m_lblVisa, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpVisa, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Другие услуги
		m_tblInput.attach( m_lblService, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpService, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// Трансфер
		m_tblInput.attach( m_lblTransfer, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK | Gtk::FILL );
		m_tblInput.attach( m_inpTransfer, 1, 2, l++, r++, Gtk::FILL, Gtk::SHRINK, 2, 1 );

		// кнопки
		/*m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK );*/

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
		//udFormHandler::Init( );
	} // void Init

	void CHTourAppForm::Clear( ) {
		udFormHandler::Clear( );

		m_inpPrice.Reset( );
		m_inpPay1.Reset( );
		m_inpPay2.Reset( );
		m_inpDays1.Reset( );
		m_inpDays2.Reset( );
		m_inpDays3.Reset( );
		m_inpProduct.Reset( );
		m_objOccupancyModel->clear( );
		m_objProgramModel->clear( );
		m_objTransferServiceModel->clear( );
		m_objCoverModel->clear( );
		m_objVisaModel->clear( );
		m_objServiceModel->clear( );
		m_objTransferModel->clear( );
	} // void Clear

	void CHTourAppForm::Load( ) {
		if ( m_pFde ) {
			udPBase obj = m_pFde->GetObject( );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			m_inpPrice.SetValue( Glib::locale_to_utf8( obj->get( "price" )->toString( ) ) );
			m_inpPay1.SetValue( Glib::locale_to_utf8( obj->get( "pay1" )->toString( ) ) );
			m_inpPay2.SetValue( Glib::locale_to_utf8( obj->get( "pay2" )->toString( ) ) );
			m_inpDays1.SetValue( Glib::locale_to_utf8( obj->get( "days1" )->toString( ) ) );
			m_inpDays2.SetValue( Glib::locale_to_utf8( obj->get( "days2" )->toString( ) ) );
			m_inpDays3.SetValue( Glib::locale_to_utf8( obj->get( "days3" )->toString( ) ) );
			m_inpProduct.SetValue( Glib::locale_to_utf8( obj->get( "product" )->toString( ) ) );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			m_inpDate1.SetValue( obj->get( "pay1date" )->toTime( ) );
			m_inpDate2.SetValue( obj->get( "pay2date" )->toTime( ) );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			udPArrBase arrCur = NULL, arrObject = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "occupancy" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOccupancy ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCOccupancy;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objOccupancyModel->append( ) );
					row[ m_pModelFormListOccupancy->m_col_name		] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListOccupancy->m_col_category	] = Glib::locale_to_utf8( add->get( "category" )->toString( ) );
					row[ m_pModelFormListOccupancy->m_col_amount	] = Glib::locale_to_utf8( add->get( "amount" )->toString( ) );
					row[ m_pModelFormListOccupancy->m_col_feed		] = Glib::locale_to_utf8( add->get( "feed" )->toString( ) );
					row[ m_pModelFormListOccupancy->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "program" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidProgram ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCProgram;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objProgramModel->append( ) );
					row[ m_pModelFormListProgram->m_col_text	] = Glib::locale_to_utf8( add->get( "text" )->toString( ) );
					row[ m_pModelFormListProgram->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "transfer" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTransferService ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCTransfer;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objTransferServiceModel->append( ) );
					row[ m_pModelFormListTransferService->m_col_fio	] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
					row[ m_pModelFormListTransferService->m_col_trans	] = Glib::locale_to_utf8( add->get( "trans" )->toString( ) );
					row[ m_pModelFormListTransferService->m_col_level	] = Glib::locale_to_utf8( add->get( "level" )->toString( ) );
					row[ m_pModelFormListTransferService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListTransferService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
					row[ m_pModelFormListTransferService->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "cover" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidCover ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCCover;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objCoverModel->append( ) );
					row[ m_pModelFormListCover->m_col_type		] = Glib::locale_to_utf8( add->get( "type" )->toString( ) );
					row[ m_pModelFormListCover->m_col_name		] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListCover->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "visa" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidVisa ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCVisa;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objVisaModel->append( ) );
					row[ m_pModelFormListVisa->m_col_type	] = Glib::locale_to_utf8( add->get( "type" )->toString( ) );
					row[ m_pModelFormListVisa->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "service" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCService;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objServiceModel->append( ) );
					row[ m_pModelFormListService->m_col_name	] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
					row[ m_pModelFormListService->m_col_char	] = Glib::locale_to_utf8( add->get( "char" )->toString( ) );
					row[ m_pModelFormListService->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( udPArrBase ) m_pFde->GetObject( )->get( "transfer2" )->get( );
			arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTransfer ) )->GetArray( );
			if ( arrObject && arrCur && !arrCur->empty( ) ) {
				udPBase add = NULL, cur = NULL;
				Gtk::TreeModel::Row row;

				for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
					cur = *i;

					add  = new udProjectGCTransfer2;
					add->apply( cur, true, true );
					arrObject->push_back( add );

					row = *( m_objTransferModel->append( ) );
					row[ m_pModelFormListTransfer->m_col_route	] = Glib::locale_to_utf8( add->get( "route" )->toString( ) );
					row[ m_pModelFormListTransfer->m_col_type	] = Glib::locale_to_utf8( add->get( "type" )->toString( ) );
					row[ m_pModelFormListTransfer->m_col_object	] = add;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void Load

	udInt CHTourAppForm::Validate( ) {
		return 0;
	} // udInt Validate

	void CHTourAppForm::Send( ) {
		if ( m_pFde ) {
			//////////////////////////////////////////////////////////////////////////////////////////////////
			udPBase obj = m_pFde->GetObject( );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			Glib::ustring	szPrice,
							szPay1,
							szPay2,
							szDays1,
							szDays2,
							szDays3,
							szProduct;
			//////////////////////////////////////////////////////////////////////////////////////////////////
			szPrice = m_inpPrice.GetValue( );
			szPay1 = m_inpPay1.GetValue( );
			szPay2 = m_inpPay2.GetValue( );
			szDays1 = m_inpDays1.GetValue( );
			szDays2 = m_inpDays2.GetValue( );
			szDays3 = m_inpDays3.GetValue( );
			szProduct = m_inpProduct.GetValue( );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			obj->get( "price" )->ini( ConvertString( szPrice.data( ), CP_UTF8 ) );
			obj->get( "pay1" )->ini( ConvertString( szPay1.data( ), CP_UTF8 ) );
			obj->get( "pay1date" )->ini( m_inpDate1.GetValue( ) );
			obj->get( "pay2" )->ini( ConvertString( szPay2.data( ), CP_UTF8 ) );
			obj->get( "pay2date" )->ini( m_inpDate2.GetValue( ) );
			obj->get( "days1" )->ini( ConvertString( szDays1.data( ), CP_UTF8 ) );
			obj->get( "days2" )->ini( ConvertString( szDays2.data( ), CP_UTF8 ) );
			obj->get( "days3" )->ini( ConvertString( szDays3.data( ), CP_UTF8 ) );
			obj->get( "product" )->ini( ConvertString( szProduct.data( ), CP_UTF8 ) );
			//////////////////////////////////////////////////////////////////////////////////////////////////
			udPArrBase arrCur = NULL, arrObject = NULL;
			udPBase cur = NULL, add = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOccupancy ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "occupancy" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCOccupancy;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidProgram ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "program" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCProgram;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTransferService ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "transfer" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCTransfer;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidCover ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "cover" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCCover;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidVisa ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "visa" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCVisa;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidService ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "service" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCService;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			arrCur = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTransfer ) )->GetArray( );
			arrObject = ( udPArrBase ) obj->get( "transfer2" )->get( );
			ClearArrBase( arrObject ); // очистим, чтобы то, что когда-то скачивалось не отправилось обратно

			for( udArrBase::iterator i = arrCur->begin( ); i != arrCur->end( ); ++i ) {
				cur = *i;
				add = new udProjectGCTransfer2;
				add->apply( cur, true, true );
				arrObject->push_back( add );
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void Send

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListOccupancy_Select( ) {
		Gtk::TreeModel::iterator i = m_inpOccupancy.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnOccupancyEdit->set_sensitive( true );
			m_btnOccupancyDel->set_sensitive( true );
		} else {
			m_btnOccupancyEdit->set_sensitive( false );
			m_btnOccupancyDel->set_sensitive( false );
		}
	} // void OnFormListOccupancy_Select

	void CHTourAppForm::OnFormListOccupancy_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppOccupancyFormModal handler = new udHTourAppOccupancyFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание средства размещения" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание средства размещения" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGCOccupancy;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidOccupancy );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objOccupancyModel->append( ) );

			row[ m_pModelFormListOccupancy->m_col_name		] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
			row[ m_pModelFormListOccupancy->m_col_category	] = Glib::locale_to_utf8( add->get( "category" )->toString( ) );
			row[ m_pModelFormListOccupancy->m_col_amount	] = Glib::locale_to_utf8( add->get( "amount" )->toString( ) );
			row[ m_pModelFormListOccupancy->m_col_feed		] = Glib::locale_to_utf8( add->get( "feed" )->toString( ) );
			row[ m_pModelFormListOccupancy->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListOccupancy_Add

	void CHTourAppForm::OnFormListOccupancy_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpOccupancy.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListOccupancy->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppOccupancyFormModal handler = new udHTourAppOccupancyFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование средства размещения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование средства размещения" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListOccupancy->m_col_name		] = Glib::locale_to_utf8( edit->get( "name" )->toString( ) );
				row[ m_pModelFormListOccupancy->m_col_category	] = Glib::locale_to_utf8( edit->get( "category" )->toString( ) );
				row[ m_pModelFormListOccupancy->m_col_amount	] = Glib::locale_to_utf8( edit->get( "amount" )->toString( ) );
				row[ m_pModelFormListOccupancy->m_col_feed		] = Glib::locale_to_utf8( edit->get( "feed" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListOccupancy_Edit

	void CHTourAppForm::OnFormListOccupancy_Del( ) {
		Gtk::TreeModel::iterator i = m_inpOccupancy.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListOccupancy->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidOccupancy ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListOccupancy_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListOccupancy_Del, not found\n" );
			}

			m_objOccupancyModel->erase( i );
		}
	} // void OnFormListOccupancy_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListProgram_Select( ) {
		Gtk::TreeModel::iterator i = m_inpProgram.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnProgramEdit->set_sensitive( true );
			m_btnProgramDel->set_sensitive( true );
		} else {
			m_btnProgramEdit->set_sensitive( false );
			m_btnProgramDel->set_sensitive( false );
		}
	} // void OnFormListProgram_Select

	void CHTourAppForm::OnFormListProgram_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppProgramFormModal handler = new udHTourAppProgramFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание экскурсионной программы" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание экскурсионной программы" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGCProgram;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidProgram );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objProgramModel->append( ) );

			row[ m_pModelFormListProgram->m_col_text	] = Glib::locale_to_utf8( add->get( "text" )->toString( ) );
			row[ m_pModelFormListProgram->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListProgram_Add

	void CHTourAppForm::OnFormListProgram_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpProgram.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListProgram->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppProgramFormModal handler = new udHTourAppProgramFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование экскурсионной программы" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование экскурсионной программы" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListProgram->m_col_text ] = Glib::locale_to_utf8( edit->get( "text" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListProgram_Edit

	void CHTourAppForm::OnFormListProgram_Del( ) {
		Gtk::TreeModel::iterator i = m_inpProgram.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListProgram->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidProgram ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListProgram_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListProgram_Del, not found\n" );
			}

			m_objProgramModel->erase( i );
		}
	} // void OnFormListProgram_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListTransferService_Select( ) {
		Gtk::TreeModel::iterator i = m_inpTransferService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnTransferServiceEdit->set_sensitive( true );
			m_btnTransferServiceDel->set_sensitive( true );
		} else {
			m_btnTransferServiceEdit->set_sensitive( false );
			m_btnTransferServiceDel->set_sensitive( false );
		}
	} // void OnFormListTransferService_Select

	void CHTourAppForm::OnFormListTransferService_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppTransferServiceFormModal handler = new udHTourAppTransferServiceFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание услуги по перевозке" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание услуги по перевозке" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGCTransfer;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidTransferService );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objTransferServiceModel->append( ) );

			row[ m_pModelFormListTransferService->m_col_fio	] = Glib::locale_to_utf8( add->get( "fio" )->toString( ) );
			row[ m_pModelFormListTransferService->m_col_trans	] = Glib::locale_to_utf8( add->get( "trans" )->toString( ) );
			row[ m_pModelFormListTransferService->m_col_level	] = Glib::locale_to_utf8( add->get( "level" )->toString( ) );
			row[ m_pModelFormListTransferService->m_col_date1	] = Glib::locale_to_utf8( add->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListTransferService->m_col_date2	] = Glib::locale_to_utf8( add->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			row[ m_pModelFormListTransferService->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListTransferService_Add

	void CHTourAppForm::OnFormListTransferService_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpTransferService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListTransferService->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppTransferServiceFormModal handler = new udHTourAppTransferServiceFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование средства размещения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование средства размещения" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListTransferService->m_col_fio	] = Glib::locale_to_utf8( edit->get( "fio" )->toString( ) );
				row[ m_pModelFormListTransferService->m_col_trans	] = Glib::locale_to_utf8( edit->get( "trans" )->toString( ) );
				row[ m_pModelFormListTransferService->m_col_level	] = Glib::locale_to_utf8( edit->get( "level" )->toString( ) );
				row[ m_pModelFormListTransferService->m_col_date1	] = Glib::locale_to_utf8( edit->get( "date1" )->toTime( ).toString( "%d.%m.%Y" ) );
				row[ m_pModelFormListTransferService->m_col_date2	] = Glib::locale_to_utf8( edit->get( "date2" )->toTime( ).toString( "%d.%m.%Y" ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListTransferService_Edit

	void CHTourAppForm::OnFormListTransferService_Del( ) {
		Gtk::TreeModel::iterator i = m_inpTransferService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListTransferService->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTransferService ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListTransferService_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListTransferService_Del, not found\n" );
			}

			m_objTransferServiceModel->erase( i );
		}
	} // void OnFormListTransferService_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListCover_Select( ) {
		Gtk::TreeModel::iterator i = m_inpCover.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnCoverEdit->set_sensitive( true );
			m_btnCoverDel->set_sensitive( true );
		} else {
			m_btnCoverEdit->set_sensitive( false );
			m_btnCoverDel->set_sensitive( false );
		}
	} // void OnFormListCover_Select

	void CHTourAppForm::OnFormListCover_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppCoverFormModal handler = new udHTourAppCoverFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание страхования" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание страхования" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGCCover;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidCover );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objCoverModel->append( ) );

			row[ m_pModelFormListCover->m_col_type		] = Glib::locale_to_utf8( add->get( "type" )->toString( ) );
			row[ m_pModelFormListCover->m_col_name		] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
			row[ m_pModelFormListCover->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListCover_Add

	void CHTourAppForm::OnFormListCover_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpCover.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListCover->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppCoverFormModal handler = new udHTourAppCoverFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование средства размещения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование средства размещения" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListCover->m_col_type	] = Glib::locale_to_utf8( edit->get( "type" )->toString( ) );
				row[ m_pModelFormListCover->m_col_name	] = Glib::locale_to_utf8( edit->get( "name" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListCover_Edit

	void CHTourAppForm::OnFormListCover_Del( ) {
		Gtk::TreeModel::iterator i = m_inpCover.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListCover->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidCover ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListCover_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListCover_Del, not found\n" );
			}

			m_objCoverModel->erase( i );
		}
	} // void OnFormListCover_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListVisa_Select( ) {
		Gtk::TreeModel::iterator i = m_inpVisa.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnVisaEdit->set_sensitive( true );
			m_btnVisaDel->set_sensitive( true );
		} else {
			m_btnVisaEdit->set_sensitive( false );
			m_btnVisaDel->set_sensitive( false );
		}
	} // void OnFormListVisa_Select

	void CHTourAppForm::OnFormListVisa_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppVisaFormModal handler = new udHTourAppVisaFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание визового оформления" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание визового оформления" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGCVisa;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidVisa );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objVisaModel->append( ) );

			row[ m_pModelFormListVisa->m_col_type	] = Glib::locale_to_utf8( add->get( "type" )->toString( ) );
			row[ m_pModelFormListVisa->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListVisa_Add

	void CHTourAppForm::OnFormListVisa_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpVisa.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListVisa->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppVisaFormModal handler = new udHTourAppVisaFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование визового оформления" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование визового оформления" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListVisa->m_col_type ] = Glib::locale_to_utf8( edit->get( "type" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListVisa_Edit

	void CHTourAppForm::OnFormListVisa_Del( ) {
		Gtk::TreeModel::iterator i = m_inpVisa.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListVisa->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidVisa ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListVisa_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListVisa_Del, not found\n" );
			}

			m_objVisaModel->erase( i );
		}
	} // void OnFormListVisa_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListService_Select( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnServiceEdit->set_sensitive( true );
			m_btnServiceDel->set_sensitive( true );
		} else {
			m_btnServiceEdit->set_sensitive( false );
			m_btnServiceDel->set_sensitive( false );
		}
	} // void OnFormListService_Select

	void CHTourAppForm::OnFormListService_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppServiceFormModal handler = new udHTourAppServiceFormModal( );
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
			udPBase add = new udProjectGCService;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidService );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objServiceModel->append( ) );

			row[ m_pModelFormListService->m_col_name	] = Glib::locale_to_utf8( add->get( "name" )->toString( ) );
			row[ m_pModelFormListService->m_col_char	] = Glib::locale_to_utf8( add->get( "char" )->toString( ) );
			row[ m_pModelFormListService->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListService_Add

	void CHTourAppForm::OnFormListService_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpService.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListService->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppServiceFormModal handler = new udHTourAppServiceFormModal( );
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

				row[ m_pModelFormListService->m_col_name	] = Glib::locale_to_utf8( edit->get( "name" )->toString( ) );
				row[ m_pModelFormListService->m_col_char	] = Glib::locale_to_utf8( edit->get( "char" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListService_Edit

	void CHTourAppForm::OnFormListService_Del( ) {
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
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListService_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListService_Del, not found\n" );
			}

			m_objServiceModel->erase( i );
		}
	} // void OnFormListService_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHTourAppForm::OnFormListTransfer_Select( ) {
		Gtk::TreeModel::iterator i = m_inpTransfer.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			m_btnTransferEdit->set_sensitive( true );
			m_btnTransferDel->set_sensitive( true );
		} else {
			m_btnTransferEdit->set_sensitive( false );
			m_btnTransferDel->set_sensitive( false );
		}
	} // void OnFormListTransfer_Select

	void CHTourAppForm::OnFormListTransfer_Add( ) {
		m_pData->MakeObject( );

		Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
		udPHTourAppTransferFormModal handler = new udHTourAppTransferFormModal( );
		handler->Init( );

		ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Создание трансфера" ) );

		/*Gtk::Window wnd;
		wnd.set_modal( true );
		wnd.set_title( Glib::locale_to_utf8( "Создание трансфера" ) );
		wnd.add( *( handler->GetWindow( ) ) );
		wnd.set_transient_for( *wndParent );
		wnd.show_all( );
		Gtk::Main::run( wnd );*/

		udPDAForm pDa = handler->GetData( );
		udPBase obj = pDa->GetData( );
		if ( obj ) {
			udPBase add = new udProjectGCTransfer2;
			udPFDEList pFde = ( udPFDEList ) m_pData->GetExtraData( ctrlidTransfer );
			udPArrBase arrObject = pFde->GetArray( );

			add->apply( obj, true, true );
			add->get( "app" )->ini( m_pData->GetData( )->get( "id" )->toUInt( ) );
			arrObject->push_back( add );

			Gtk::TreeModel::Row row = *( m_objTransferModel->append( ) );

			row[ m_pModelFormListTransfer->m_col_route	] = Glib::locale_to_utf8( add->get( "route" )->toString( ) );
			row[ m_pModelFormListTransfer->m_col_type	] = Glib::locale_to_utf8( add->get( "type" )->toString( ) );
			row[ m_pModelFormListTransfer->m_col_object	] = add;
		}

		udDELETE( handler );
	} // void OnFormListTransfer_Add

	void CHTourAppForm::OnFormListTransfer_Edit( ) {
		Gtk::TreeModel::iterator i = m_inpTransfer.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase edit = row[ m_pModelFormListTransfer->m_col_object ];
			Gtk::Window *wndParent = ( Gtk::Window* ) m_wndFormWrap->get_toplevel( );
			udPHTourAppTransferFormModal handler = new udHTourAppTransferFormModal( );
			handler->Init( );
			handler->LoadFrom( edit );

			ModalWindow( wndParent, handler->GetWindow( ), Glib::locale_to_utf8( "Редактирование средства размещения" ) );

			/*Gtk::Window wnd;
			wnd.set_modal( true );
			wnd.set_title( Glib::locale_to_utf8( "Редактирование средства размещения" ) );
			wnd.add( *( handler->GetWindow( ) ) );
			wnd.set_transient_for( *wndParent );
			wnd.show_all( );
			Gtk::Main::run( wnd );*/

			udPDAForm pDa = handler->GetData( );
			udPBase obj = pDa->GetData( );
			if ( obj ) {
				Gtk::TreeModel::Row row = *i;

				edit->apply( obj, true, true );

				row[ m_pModelFormListTransfer->m_col_route	] = Glib::locale_to_utf8( edit->get( "route" )->toString( ) );
				row[ m_pModelFormListTransfer->m_col_type	] = Glib::locale_to_utf8( edit->get( "type" )->toString( ) );
			}

			udDELETE( handler );
		}
	} // void OnFormListTransfer_Edit

	void CHTourAppForm::OnFormListTransfer_Del( ) {
		Gtk::TreeModel::iterator i = m_inpTransfer.GetTreeView( )->get_selection( )->get_selected( );
		if ( i ) {
			Gtk::TreeModel::Row row = *i;
			udPBase del = row[ m_pModelFormListTransfer->m_col_object ];
			udPArrBase arrObject = ( ( udPFDEList ) m_pData->GetExtraData( ctrlidTransfer ) )->GetArray( );
			udArrBase::iterator toErase = arrObject->end( );

			for( udArrBase::iterator k = arrObject->begin( ); k != arrObject->end( ); ++k ) {
				if ( *k == del ) {
					toErase = k;
					break;
				}
			}

			if ( toErase != arrObject->end( ) ) {
				arrObject->erase( toErase );
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListTransfer_Del, found\n" );
			} else {
				g_pApp->GetLog( )->Write( "CHTourAppForm::OnFormListTransfer_Del, not found\n" );
			}

			m_objTransferModel->erase( i );
		}
	} // void OnFormListTransfer_Del

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace UdSDK
