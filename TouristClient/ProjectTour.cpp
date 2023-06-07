
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static udPArrBase LoadClients( map< udUInt, udUInt > ids ) {
		udPArrBase ret = NULL;
		udPSocket sock = BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( string( "client" ) );
			data.add( ( udInt ) udHClient::opidGet3 );
			data.add( ( udUInt ) ids.size( ) );
			for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
				data.add( i->first );
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udArrBase arr;
				udType total( ( udUInt ) 0 );
				total.read( sock );
				ReadInstances( sock, arr, GetProjectClient );
				string log = "\n";
				udPBase add = NULL;
				ret = new udArrBase;
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					add = GetProjectClient( );
					add->apply( *i, true, true );
					add->dump( log );
					log += "\n";
					ret->push_back( add );
				}
				g_pApp->GetLog( )->Write( log );
				ClearArrBase( arr );
			}
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////
			sock->Shutdown( );
			sock->Close( );
		}
		udDELETE( sock );
		return ret;
	} // void LoadClients

	void TourAssignTplService( udPBase obj ) {
		obj->get( "#name" )->ini( wstring( L"" ) );
	} // void TourAssignTplService

	void TourAssignTplClient( udPBase obj ) {
		obj->get( "fio" )->ini( wstring( L"" ) );
	} // void TourAssignTplClient

	void TourAssignTplTourLink( udPBase obj ) {
		obj->get( "name" )->ini( wstring( L"" ) );
	} // void TourAssignTplTourLink

	void TourAssignTplRoute( udPBase obj ) {
		obj->get( "name" )->ini( wstring( L"" ) );
	} // void TourAssignTplTourLink

	bool TourSpecLoadEmployer( udPBase main, udPBase& obj, udPWndModeForm form, udPSocket sock ) {
		if ( main->get( "emptype" )->toInt( ) == 1 ) {
			udDELETE( obj );
			obj = new udProjectClientUr;
			form->GetInfo( udHTour::ctrlidEmployer )->extlst->extlst->SetAttr( "name" );
			//obj->read( sock );
			return true;
		}
		return false;
	} // bool TourSpecLoadEmployer

	void TourAfterPickEmployer( udPBase obj, udPListHandler handler ) {
		udPMorph opt = handler->GetLstOpt( );
		udPWndModeForm wndForm = ( udPWndModeForm ) handler->GetLstOpt( )->get( "wndForm" )->toDWord( );
		udInt state = opt->get( "state" )->toInt( );
		if ( state == udHClient::stateFiz ) {
			wndForm->GetInfo( udHTour::ctrlidEmployer )->extlst->extlst->SetAttr( "fio" );
		} else {
			wndForm->GetInfo( udHTour::ctrlidEmployer )->extlst->extlst->SetAttr( "name" );
		}
	} // void TourAfterPickEmployer

	void TourAfterPickEmployer2( udPBase obj, udPListHandler handler ) {
		udPMorph opt = handler->GetLstOpt( );
		udPWndModeList wndList = ( udPWndModeList ) handler->GetLstOpt( )->get( "wndList" )->toDWord( );
		udInt state = opt->get( "state" )->toInt( );
		if ( state == udHClient::stateFiz ) {
			wndList->GetInfo( udHTour::fltEmployer )->extlst->extlst->SetAttr( "fio" );
			wndList->GetObjFilter( )->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		} else {
			wndList->GetInfo( udHTour::fltEmployer )->extlst->extlst->SetAttr( "name" );
			wndList->GetObjFilter( )->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENTUR );
		}
	} // void TourAfterPickEmployer

	CHTour::CHTour( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_tour";
		m_szUpdIndex	= "id";
		m_szName		= "tour";
		m_fnGetObject	= GetProjectTour;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHTourForm;

		this->AddExtraLoadInfo( ctrlidSeller, GetProjectCompany, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidEmployer, GetProjectClient, ( udInt ) extra_load_extlst,
			NULL, NULL, TourSpecLoadEmployer );
		this->AddExtraLoadInfo( ctrlidManager, GetProjectUser, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidDirect, GetProjectDirect, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidState, GetProjectSTour, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidRoute, GetProjectTourRoute, ( udInt ) extra_load_frmlst, TourAssignTplRoute );
		this->AddExtraLoadInfo( ctrlidService, GetProjectTourService, ( udInt ) extra_load_frmlst, TourAssignTplService );
		this->AddExtraLoadInfo( ctrlidTourist, GetProjectTourClient, ( udInt ) extra_load_frmlst, TourAssignTplClient );
		this->AddExtraLoadInfo( ctrlidTour, GetProjectTourTour, ( udInt ) extra_load_frmlst, TourAssignTplTourLink );
		this->AddExtraLoadInfo( ctrlidApp, GetProjectAttach, ( udInt ) extra_load_frmlst );

		m_lnkFinance = new udWndLink;
		m_lnkFinance->get( "title" )->ini( wstring( L"Финансы" ) );
		m_lnkFinance->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );

		m_lnkDocTpl = new udWndLink;
		m_lnkDocTpl->get( "title" )->ini( wstring( L"Распечатка документов" ) );
		m_lnkDocTpl->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );

		m_lnkApp1 = new udWndLink;
		m_lnkApp1->get( "title" )->ini( wstring( L"Доп. информация" ) );
		m_lnkApp1->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатка формы" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );

		m_pApp1 = NULL;
		this->MakeAppData( );
	}

	CHTour::CHTour( const CHTour& objCopy ) {
	}

	CHTour::~CHTour( ) {
		udDELETE( m_lnkFinance );
		udDELETE( m_lnkDocTpl );
		udDELETE( m_lnkApp1 );
		udDELETE( m_lnkPrint );
		this->ClearAppData( );
	}

	CHTour& CHTour::operator =( const CHTour& objCopy ) {
		return *this;
	}

	void CHTour::MakeAppData( ) {
		if ( m_pApp1 ) {
			this->ClearAppData( );
		}
		//////////////////////////////////////////////////////////////////////////
		m_pApp1 = GetProjectGroupContractApp1( );
		//////////////////////////////////////////////////////////////////////////
		udPArrBase pArr = NULL;
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "occupancy" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "program" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "transfer" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "cover" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "visa" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "service" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		m_pApp1->get( "transfer2" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		udPMorph obj = new udMorph;
		m_pApp1->get( "#del" )->ini( ( udPVoid ) obj );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "occupancy" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "program" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "transfer" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "cover" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "visa" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "service" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = new udArrBase;
		obj->get( "transfer2" )->ini( ( udPVoid ) pArr );
		//////////////////////////////////////////////////////////////////////////
	} // void MakeAppData

	void CHTour::ClearAppData( ) {
		//////////////////////////////////////////////////////////////////////////
		udPArrBase pArr = NULL;
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "occupancy" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "program" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "transfer" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "cover" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "visa" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "service" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "transfer2" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		udPMorph obj = ( udPMorph ) m_pApp1->get( "#del" )->get( );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "occupancy" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "program" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "transfer" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "cover" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "visa" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "service" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) obj->get( "transfer2" )->get( );
		ClearArrBase( pArr );
		udDELETE( pArr );
		//////////////////////////////////////////////////////////////////////////
		udDELETE( obj );
		udDELETE( m_pApp1 );
	} // void ClearAppData

	void CHTour::Init( ) {
		udStateHandlerSimple::Init( );
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\tour";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	void CHTour::Clear( ) {
		if ( m_wndForm ) {
			udPDwAscBase attach = m_wndForm->GetInfo( ctrlidApp )->frmlst->GetItems( );
			if ( attach && !attach->empty( ) ) { // сносим временные файлы
				wstring filepath = L"";
				for( udDwAscBase::iterator i = attach->begin( ); i != attach->end( ); ++i ) {
					filepath = i->second->get( "tmp_path" )->toWString( );
					DeleteFile( filepath.data( ) );
				}
			}
		}
	
		udStateHandlerSimple::Clear( );
		this->ClearAppData( );
		this->MakeAppData( );
	} // void Clear

	udInt CHTour::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		m_pLstOpt->get( "wndList" )->ini( ( udDWord ) m_wndModeList );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !modal ) {
			title = L"На главную";
			url = "main";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить тур" : L"Новый тур" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "name", L"Наименование" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "direct_", L"Направление", "direct" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "date1_", L"Отправление", "date1" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "status_", L"Статус", "status" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "employer_", L"Заказчик", "employer" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "manager_", L"Ответственный", "manager2" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltName, "name", NULL, font, 0 );
		m_wndModeList->AddExtlist( fltDirect, "direct", NULL, "name", L"выберите направление", GetHDirectList );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltOrder, "order", NULL, "employer_", L"выберите заявку", GetHOrderList );
		m_wndModeList->AddExtlist( fltTplTour, "tpltour", NULL, "name", L"выберите шаблон", GetHTplTourList );
		udArrWString arrSelect;
		arrSelect.push_back( L"" );
		arrSelect.push_back( L"Открыт" );
		arrSelect.push_back( L"Закрыт" );
		arrSelect.push_back( L"Требует внимания" );
		m_wndModeList->AddSelect( fltStatus, "status", NULL, font, arrSelect );
		m_wndModeList->AddExtlist( fltEmployer, "employer", NULL, "fio", L"выберите клиента", GetHClientList, m_pLstOpt, TourAfterPickEmployer2 );
		m_wndModeList->AddExtlist( fltManager, "manager", NULL, "fio", L"выберите пользователя", GetHUserList );
		datetime date;
		date.load( );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		date.dec( 1, datetime::datetimeMonth );
		filter->get( "date1" )->ini( date );
		date.load( );
		date.inc( 1, datetime::datetimeYear );
		filter->get( "date2" )->ini( date );
		filter->get( "status" )->ini( ( udInt ) 0 );
		filter->get( "employer" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		filter->get( "order" )->ini( ( udUInt ) 0 );
		filter->get( "tpltour" )->ini( ( udUInt ) 0 );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHTour::GetListView( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		style	= WS_CHILD | WS_TABSTOP | WS_VISIBLE;
		udInt		x		= 32,
					y		= 10,
					m		= 5,
					w		= 0,
					h		= 0;
		const udLAscWndLink* arrLink = m_wndModeList->GetLinks( );
		udLAscWndLink::const_iterator itLnk;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !modal ) {
			itLnk = arrLink->find( 0 );
			lnk = itLnk->second;
			lnk->get( "style" )->ini( style );
			lnk->get( "x" )->ini( x );
			lnk->get( "y" )->ini( y );
			lnk->CalculateWidthHeight( hWnd );
			x += lnk->get( "width" )->toInt( ) + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y = m;
			itLnk = arrLink->find( 0 );
		} else {
			itLnk = arrLink->find( 1 );
		}
		lnk = itLnk->second;
		lnk->get( "style" )->ini( style );
		lnk->get( "x" )->ini( x );
		lnk->get( "y" )->ini( y );
		lnk->CalculateWidthHeight( hWnd );
		//////////////////////////////////////////////////////////////////////////////////////////
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += lnk->get( "height" )->toInt( ) + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 100; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltName );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltDirect );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 88; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltDate1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 88; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltDate2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 186; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltStatus );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y += 30 + m;
		} else {
			x += box->w + box->padL + box->padR + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltEmployer );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x += box->w + box->padL + box->padR + m;
		} else {
			x = 32;
			y += 30 + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltOrder );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltTplTour );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltManager );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 30 + m;
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndModeList->SetButtonFilterPosition( x, y - m );
		x += m_wndModeList->GetButtonFilter( )->get( "width" )->toInt( ) + m;
		m_wndModeList->SetButtonResetPosition( x, y - m );
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 30 + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		//y += lnk->get( "height" )->toInt( ) + m;
		w = 400;
		h = 400;
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "style" )->ini( style );
		wndInsLst->get( "x" )->ini( x );
		wndInsLst->get( "y" )->ini( y );
		wndInsLst->get( "width" )->ini( w );
		wndInsLst->get( "height" )->ini( h );
		//////////////////////////////////////////////////////////////////////////////////////////
		x = 32;
		w = 300;
		y += wndInsLst->get( "height" )->toInt( ) + m;
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "style" )->ini( style );
		wndPager->get( "x" )->ini( x );
		wndPager->get( "y" )->ini( y );
		wndPager->get( "width" )->ini( w );
		wndPager->get( "height" )->ini( 20 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListView

	udInt CHTour::GetFormModel( ) {
		m_pFrmOpt->get( "wndForm" )->ini( ( udDWord ) m_wndForm );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddFormlist( ctrlidRoute, "route", NULL )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidService, "lserv", NULL )->AddAttr( "#name" );
		m_wndForm->AddInput( ctrlidCost, "cost", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidPrice, "price", NULL, font, 0 );
		m_wndForm->AddFormlist( ctrlidApp, "app", NULL, g_pApp->GetUniqueTmpFolder( ) )->AddAttr( "name" );
		m_wndForm->AddExtlist(
			ctrlidSeller, "company",
			NULL, "name", L"выберите компанию", GetHCompanyList
			//NULL, "name", "pick_seller", L"выберите компанию",
			//"ud_company", GetProjectCompany
		);
		m_wndForm->AddExtlist(
			ctrlidState, "state",
			NULL, "name", L"выберите состояние", GetHSTourList
			//NULL, "name", "pick_state", L"выберите состояние",
			//"ud_stour", GetProjectSTour
		);
		udArrWString arrSelect;
		arrSelect.push_back( L"Открыт" );
		arrSelect.push_back( L"Закрыт" );
		arrSelect.push_back( L"Требует внимания" );
		m_wndForm->AddSelect( ctrlidStatus, "status", NULL, font, arrSelect );
		m_wndForm->AddExtlist(
			ctrlidEmployer, "employer",
			NULL, "fio", L"выберите клиента", GetHClientList, m_pFrmOpt, TourAfterPickEmployer
			//NULL, "fio", "pick_employer", L"выберите клиента",
			//"ud_client", GetProjectClient
		);
		m_wndForm->AddFormlist( ctrlidTourist, "ltourist", NULL )->AddAttr( "fio" );
		m_wndForm->AddExtlist(
			ctrlidManager, "manager2",
			NULL, "fio", L"выберите менеджера", GetHUserList
			//NULL, "fio", "pick_manager2", L"выберите менеджера",
			//"ud_user", GetProjectUser
		);
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		m_wndForm->AddExtlist(
			ctrlidDirect, "direct",
			NULL, "name", L"выберите направление", GetHDirectList
			//NULL, "name", "pick_direct", L"выберите направление",
			//"ud_direct", GetProjectDirect
		);
		m_wndForm->AddFormlist( ctrlidTour, "ltour", NULL )->AddAttr( "name" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTour::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 1024 );
			m_wndForm->get( "height" )->ini( ( udInt ) 820 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkFinance->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkFinance->get( "x" )->ini( ( udInt ) 300 );
			m_lnkFinance->get( "y" )->ini( ( udInt ) 10 );
			m_lnkFinance->Create( m_wndForm->GetHWnd( ) );
			m_lnkFinance->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			int xDocTpl = 300 + m_lnkFinance->get( "width" )->toInt( ) + 10;
			m_lnkDocTpl->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkDocTpl->get( "x" )->ini( xDocTpl );
			m_lnkDocTpl->get( "y" )->ini( ( udInt ) 10 );
			m_lnkDocTpl->Create( m_wndForm->GetHWnd( ) );
			m_lnkDocTpl->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			int xApp1 = xDocTpl + m_lnkDocTpl->get( "width" )->toInt( ) + 10;
			m_lnkApp1->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkApp1->get( "x" )->ini( xApp1 );
			m_lnkApp1->get( "y" )->ini( ( udInt ) 10 );
			m_lnkApp1->Create( m_wndForm->GetHWnd( ) );
			m_lnkApp1->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			int xPrint = xApp1 + m_lnkApp1->get( "width" )->toInt( ) + 10;
			m_lnkPrint->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrint->get( "x" )->ini( xPrint );
			m_lnkPrint->get( "y" )->ini( ( udInt ) 10 );
			m_lnkPrint->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrint->SetTargetWnd( m_wndForm->GetHWndParent( ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		wstring		title		= L"";
		udPGdi		gdi			= m_wndForm->GetGdi( );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тур";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Наименование";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidName );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 278;
		y		= 162;
		w		= 169;
		h		= 16;
		title	= L"Дата/время начала:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		x = 278;
		w = 169;
		h = 22;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 287;
		y		= 213;
		w		= 150;
		h		= 16;
		title	= L"Дата/время завершения:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		x = 278;
		w = 169;
		h = 22;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 487;
		y		= 553;
		w		= 511;
		h		= 30;
		title	= L"Маршрут:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidRoute );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 553;
		w		= 418;
		h		= 30;
		title	= L"Список услуг:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidService );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 346;
		w		= 200;
		h		= 16;
		title	= L"Себестоимость:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		w = 150;
		h = 15;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidCost );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 406;
		w		= 200;
		h		= 16;
		title	= L"Итоговая цена:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		w = 150;
		h = 15;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidPrice );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 487;
		y		= 456;
		w		= 511;
		h		= 30;
		title	= L"Приложения:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidApp );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 227;
		w		= 200;
		h		= 16;
		title	= L"Компания-продавец:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidSeller );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 286;
		w		= 200;
		h		= 16;
		title	= L"Состояние:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidState );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 269;
		y		= 114;
		w		= 186;
		h		= 16;
		title	= L"Статус:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 200;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidStatus );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 169;
		w		= 200;
		h		= 16;
		title	= L"Заказчик:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidEmployer );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 456;
		w		= 418;
		h		= 30;
		title	= L"Список туристов:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidTourist );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 114;
		w		= 200;
		h		= 16;
		title	= L"Ответственный менеджер:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidManager );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 270;
		y		= 278;
		h		= 16;
		title	= L"Комментарий:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 487;
		y		= 134;
		w		= 511;
		h		= 30;
		title	= L"Информация:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		x		= 500;
		y		= 176;
		w		= 87;
		h		= 16;
		title	= L"Направление:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		y = 170;
		w = 398;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidDirect );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 650;
		w		= 418;
		h		= 30;
		title	= L"Список связанных туров:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidTour );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x = 32;
		y = 766;
		w = 103;
		h = 37;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	void CHTour::ClickToolbar( string index ) {
		if ( index == "tour" ) {
			this->SwitchToForm( );
		} else {
			udStateHandlerSimple::ClickToolbar( index );
		}
	} // void ClickToolbar

	udInt CHTour::ToolbarAction( string index ) {
		if ( index == "tour" ) {
			return 1;
		}
		return 0;
	} // udInt ToolbarAction

	udInt CHTour::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		/*if ( obj->get( "cost" )->toFloat( ) ==  0.0f ) {
			mb::err( hWnd, L"Введите себестоимость" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "price" )->toFloat( ) == 0.0f ) {
			mb::err( hWnd, L"Введите итоговую цену" );
			return FormHandlerErrorFailed;
		}*/
		if ( obj->get( "company" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите продавца" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "employer" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите заказчика" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "manager2" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите ответственного за тур" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "direct" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите направление" );
			return FormHandlerErrorFailed;
		}
		
		//udPDwAscBase route = m_frmlstRoute->GetItems( );
		udPDwAscBase route = m_wndForm->GetInfo( ctrlidRoute )->frmlst->GetItems( );
		if ( !route || route->empty( ) ) {
			mb::err( hWnd, L"Введите маршрут" );
			return FormHandlerErrorFailed;
		}
		//udPDwAscBase service = m_frmlstService->GetItems( );
		udPDwAscBase service = m_wndForm->GetInfo( ctrlidService )->frmlst->GetItems( );
		if ( !service || service->empty( ) ) {
			mb::err( hWnd, L"Введите услуги" );
			return FormHandlerErrorFailed;
		}
		//udPDwAscBase clients = m_frmlstClient->GetItems( );
		udPDwAscBase clients = m_wndForm->GetInfo( ctrlidTourist )->frmlst->GetItems( );
		if ( !clients || clients->empty( ) ) {
			mb::err( hWnd, L"Введите туристов" );
			return FormHandlerErrorFailed;
		}
		//udPDwAscBase tours = m_frmlstTour->GetItems( );
		/*if ( !tours || tours->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите туры" );
			return false;
		}*/
		//udPDwAscBase attach = m_frmlstAttach->GetItems( );
		/*if ( !attach || attach->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите приложение" );
			return false;
		}*/

		udDatetime date;
		date.load( );
		obj->get( "created" )->ini( date );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );

		if ( m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetAttr( ) == "name" ) {
			obj->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENTUR );
		} else {
			obj->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		}

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHTour::ClearFilter( ) {
		datetime date;
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		date.load( );
		date.dec( 1, datetime::datetimeMonth );
		filter->get( "date1" )->ini( date );
		date.load( );
		date.inc( 1, datetime::datetimeYear );
		filter->get( "date2" )->ini( date );
		filter->get( "status" )->ini( ( udInt ) 0 );
		filter->get( "employer" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		filter->get( "order" )->ini( ( udUInt ) 0 );
		filter->get( "tpltour" )->ini( ( udUInt ) 0 );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHTour::AfterLoadFilterData( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		udInt status = filter->get( "status" )->toInt( );
		filter->get( "status" )->ini( status - 1 );
	} // void AfterLoadFilterData

	void CHTour::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			udPListHandler handler = GetHCityList( );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTourRoute;
				add->get( "tour" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "city" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "name" )->ini( obj->get( "name" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPFormHandler handler = GetHTourServiceForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTourService;
				obj->apply( add );
				add->get( "tour" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );

				wstring date1 = L"", date2 = L"", ready = L"";
				udDatetime date = obj->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );
				if ( obj->get( "ready" )->toUInt( ) == 1 ) {
					ready = L"Не готова";
				} else {
					ready = L"Готова";
				}

				add->get( "#name" )->ini(
					ready + L"," + date1 + L"-" + date2 + L", " +
					obj->get( "num" )->toWString( ) + L", " +
					obj->get( "name_" )->toWString( )
				);
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTourist )->frmlst ) {
			udPListHandler handler = GetHClientList( );
			handler->GetLstOpt( )->get( "onlyfiz" )->ini( ( udInt ) 1 );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTourService;
				add->get( "tour" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "client" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "fio" )->ini( obj->get( "fio" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTour )->frmlst ) {
			udPListHandler handler = GetHTourList( );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTourTour;
				add->get( "tour1" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "tour2" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "name" )->ini( obj->get( "name" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidApp )->frmlst ) {
			TCHAR filepath[ 1024 ], filename[ 1024 ];
			memset( filepath, 0, sizeof( filepath ) );
			memset( filename, 0, sizeof( filename ) );
			OPENFILENAME of;
			ZeroMemory( &of, sizeof( OPENFILENAME ) );
			of.lStructSize = sizeof( OPENFILENAME );
			of.hwndOwner = hWnd;
			of.lpstrFilter = L"All\0*.*\0";
			of.lpstrFile = filepath;
			of.nMaxFile = 1024;
			of.lpstrFileTitle = filename;
			of.nMaxFileTitle = 1024;
			of.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

			if ( GetOpenFileName( &of ) == TRUE ) {
				// добавляем элемент
				udPBase add = new udProjectAttach;
				add->get( "obj" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "type" )->ini( udProjectAttach::ATTACH_TOUR );
				add->get( "name" )->ini( wstring( filename ) );
				add->get( "orig" )->ini( wstring( filepath ) );
				
				udPDwAscBase arr = wnd->GetItems( );
				udPBase obj = NULL;
				bool failed = false;
				for( udDwAscBase::iterator i = arr->begin( ); i != arr->end( ); ++i ) {
					obj = i->second;
					if ( obj->get( "name" )->toWString( ) == add->get( "name" )->toWString( ) ) {
						failed = true;
						break;
					}
				}

				if ( failed ) {
					delete add;
					mb::err( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), L"Файл уже присутствует в списке. Смените имя или выберите другой." );
				} else {
					// предварительно пытаемся скопировать файл, если не получится, то ругаемся
					wstring wFileTmpPath = wnd->GetFolder( ) +
						L"\\" + wstring( filename );
					if ( CopyFile( filepath, wFileTmpPath.data( ), FALSE ) == FALSE ) {
						delete add;
						mb::err( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), L"Не удалось скопировать файл. Попробуйте выбрать другой." );
					} else {
						add->get( "tmp_path" )->ini( wFileTmpPath );
						wnd->AddItem( add );
					}
				}
			}
		}
	} // void FormListOnAdd

	void CHTour::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			udPListHandler handler = GetHCityList( );
			udPMorph obj1 = DialogListPick( hWnd, handler );
			if ( obj1 ) {
				obj->get( "city" )->ini( obj1->get( "id" )->toUInt( ) );
				obj->get( "name" )->ini( obj1->get( "name" )->toWString( ) );
				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPBase tmp = new udProjectTourService;
			obj->apply( tmp );
			udPFormHandler handler = GetHTourServiceForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );

				wstring date1 = L"", date2 = L"", ready = L"";
				udDatetime date = obj->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );
				if ( obj->get( "ready" )->toUInt( ) == 1 ) {
					ready = L"Не готова";
				} else {
					ready = L"Готова";
				}

				obj->get( "#name" )->ini(
					ready + L"," + date1 + L"-" + date2 + L", " +
					obj->get( "num" )->toWString( ) + L", " +
					obj->get( "name_" )->toWString( )
				);

				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTourist )->frmlst ) {
			udPListHandler handler = GetHClientList( );
			handler->GetLstOpt( )->get( "onlyfiz" )->ini( ( udInt ) 1 );
			udPMorph obj1 = DialogListPick( hWnd, handler );
			if ( obj1 ) {
				obj->get( "client" )->ini( obj1->get( "id" )->toUInt( ) );
				obj->get( "fio" )->ini( obj1->get( "fio" )->toWString( ) );
				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTour )->frmlst ) {
			udPListHandler handler = GetHTourList( );
			udPMorph obj1 = DialogListPick( hWnd, handler );
			if ( obj1 ) {
				obj->get( "tour2" )->ini( obj1->get( "id" )->toUInt( ) );
				obj->get( "name" )->ini( obj1->get( "name" )->toWString( ) );
				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidApp )->frmlst ) {
			ExtOpenFile( obj->get( "tmp_path" )->toWString( ), false );
			/*TCHAR filepath[ 1024 ], filename[ 1024 ];
			memset( filepath, 0, sizeof( filepath ) );
			memset( filename, 0, sizeof( filename ) );
			OPENFILENAME of;
			ZeroMemory( &of, sizeof( OPENFILENAME ) );
			of.lStructSize = sizeof( OPENFILENAME );
			of.hwndOwner = hWnd;
			of.lpstrFilter = L"All\0*.*\0";
			of.lpstrFile = filepath;
			of.nMaxFile = 1024;
			of.lpstrFileTitle = filename;
			of.nMaxFileTitle = 1024;
			of.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

			if ( GetOpenFileName( &of ) == TRUE ) {
				wstring szOldName = obj->get( "name" )->toWString( ),
					szName( filename ), szPath( filepath ); 
				
				udPDwAscBase arr = wnd->GetItems( );
				udPBase obj1 = NULL;
				bool failed = false;
				for( udDwAscBase::iterator i = arr->begin( ); i != arr->end( ); ++i ) {
					obj1 = i->second;
					if ( ( obj1 != obj ) && ( obj1->get( "name" )->toWString( ) == szName ) ) {
						failed = true;
						break;
					}
				}

				if ( failed ) {
					mb::err( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), L"Файл уже присутствует в списке. Смените имя или выберите другой." );
				} else {
					// предварительно пытаемся скопировать файл, если не получится, то ругаемся
					wstring wBase = g_pApp->GetConfig( )->get( "temporary_dir" )->toWString( ) +
						L"\\",
						wFileTmpPath = wBase + szName,
						wOldFileTmpPath = wBase + szOldName;
					if ( CopyFile( filepath, wFileTmpPath.data( ), FALSE ) == FALSE ) {
						mb::err( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), L"Не удалось скопировать файл. Попробуйте выбрать другой." );
					} else {
						if ( szName != szOldName ) { // если файл заменили другим, то снесем старый
							DeleteFile( wOldFileTmpPath.data( ) );
						}
						obj->get( "name" )->ini( szName );
						obj->get( "path" )->ini( szPath );
						//obj->get( "old_name" )->ini( szOldName );
						obj->get( "tmp_path" )->ini( wFileTmpPath );
					}
				}
			}*/
		}
	} // void FormListOnEdit

	bool CHTour::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить пункт маршрута?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTourRoute;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить услугу из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTourService;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTourist )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить туриста из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTourClient;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTour )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить тур из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTourTour;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidApp )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить файл из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectAttach;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				} else { // удалим временный файл
					DeleteFile( obj->get( "tmp_path" )->toWString( ).data( ) );
				}
			} else {
				DeleteFile( obj->get( "tmp_path" )->toWString( ).data( ) );
			}
			return true;
		}
		return false;
	} // void FormListOnEdit

	void CHTour::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "employer_" )->ini( wstring( L"" ) );
		tpl->get( "direct_" )->ini( wstring( L"" ) );
		tpl->get( "status_" )->ini( wstring( L"" ) );
		//tpl->get( "date1_" )->ini( wstring( L"" ) );
		tpl->get( "manager_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHTour::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndFormList frmlst = m_wndForm->GetInfo( ctrlidApp )->frmlst;
		udPDwAscBase items = frmlst->GetItems( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// отправляем предварительные данные
		udBinaryData data;
		data.add( ( char ) 80 );
		data.add( items->size( ) );
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !items->empty( ) ) {
			udFileTransfer objFileTransfer;
			udInt err = 0;
			string log = "\n";
			udPBase obj1 = NULL;
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				obj1 = i->second;
				obj1->dump( log );
				log += "\n";
				objFileTransfer.SetFileName( obj1->get( "name" )->toWString( ) );
				objFileTransfer.SetFilePath( obj1->get( "tmp_path" )->toWString( ) );
				err = objFileTransfer.Send( sock );
				log += udFileTransfer::ErrorExplain( err ) + "\n";
			}
			g_pApp->GetLog( )->Write( log );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "order_init" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "order_init" )->toDWord( );
			assert( wndForm );
			udPBase obj = wndForm->GetObj( );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			data.clear( );
			data.add( udProjectObjectLink::objlnkOrder );
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Send( data );
		} else if ( m_pFrmOpt->isset( "tpltour_init" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "tpltour_init" )->toDWord( );
			assert( wndForm );
			udPBase obj = wndForm->GetObj( );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			data.clear( );
			data.add( udProjectObjectLink::objlnkTplTour );
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Send( data );
		} else {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		// приложение 1
		udPLog __log = g_pApp->GetLog( );
		string log = "\nApp1:\n";
		data.clear( );
		m_pApp1->dump( data );
		m_pApp1->dump( log );
		__log->Write( log + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPArrBase pArr = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "occupancy" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "program" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "transfer" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "cover" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "visa" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "service" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		pArr = ( udPArrBase ) m_pApp1->get( "transfer2" )->get( );
		WriteInstances( *pArr, data );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pApp1->get( "id" )->toUInt( ) > 0 ) {
			udPMorph obj1 = ( udPMorph ) m_pApp1->get( "#del" )->get( );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "occupancy" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "program" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "transfer" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "cover" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "visa" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "service" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) obj1->get( "transfer2" )->get( );
			WriteInstances( *pArr, data );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		data.clear( );
		char code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	void CHTour::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
		assert( m_wndForm );
		char code = 0;
		sock->ReadByte( &code );
		if ( code > 0 ) {
			udType num( ( udUInt ) 0 );
			if ( num.read( sock ) == 0 ) {
				return;
			}
			udPLog __log = g_pApp->GetLog( );
			string log;
			udUInt cnt = 0, num1 = num.toUInt( );
			wstring tmp_dir = m_wndForm->GetInfo( ctrlidApp )->frmlst->GetFolder( );
			udInt err = 0;
			udFileTransfer objFileTransfer;
			// очищаем временную папку от левых файлов
			DirectoryClear( tmp_dir );
			objFileTransfer.SetTmpDir( tmp_dir );
			while( cnt < num1 ) {
				err = objFileTransfer.Recv( sock );
				log = udFileTransfer::ErrorExplain( err ) + "\n";
				__log->Write( log );
				if ( err != udFileTransfer::FileTransferErrorOk ) {
					break;
				}
				++cnt;
			}
		}
		udPExtList extlst = m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst;
		if ( obj->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
			extlst->SetAttr( "fio" );
		} else {
			extlst->SetAttr( "name" );
		}
		extlst->Render( );
		extlst->Flush( );
		//////////////////////////////////////////////////////////////////////////////////////////
		code = 0;
		sock->ReadByte( &code );
		if ( code > 0 ) { // приложение существует
			g_pApp->GetLog( )->Write( "App1:get\n" );
			m_pApp1->read( sock );
			//////////////////////////////////////////////////////////////////////////////////////
			udPArrBase pArr = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "occupancy" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCOccupancy );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "program" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCProgram );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "transfer" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCTransfer );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "cover" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCCover );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "visa" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCVisa );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "service" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCService );
			//////////////////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) m_pApp1->get( "transfer2" )->get( );
			ReadInstances( sock, *pArr, GetProjectGCTransfer2 );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataRecieve

	void CHTour::LoadAfterSetObject( ) {
		udPBase obj = m_wndForm->GetObj( );
		if ( m_pFrmOpt->isset( "order_init" ) ) {
			udUInt id = 0;
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "order_init" )->toDWord( );
			assert( wndForm );

			id = obj->get( "direct" )->toUInt( );
			if ( id > 0 ) {
				udPBase ret = new udProjectDirect;
				wndForm->GetInfo( udHOrder::ctrlidDirect )->extlst->extlst->GetObj( )->apply( ret );
				m_wndForm->GetInfo( ctrlidDirect )->extlst->extlst->SetObj( ret );
			}

			id = obj->get( "employer" )->toUInt( );
			if ( id > 0 ) {
				udInt emptype = obj->get( "emptype" )->toInt( );
				udPBase ret = NULL;
				if ( emptype == udProjectOrder::ORDER_EMPLOYER_CLIENT ) {
					m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->SetAttr( "fio" );
					ret = new udProjectClient;
				} else if ( emptype == udProjectOrder::ORDER_EMPLOYER_CLIENTUR ) {
					m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->SetAttr( "name" );
					ret = new udProjectClientUr;
				}
				if ( ret ) {
					wndForm->GetInfo( udHOrder::ctrlidEmployer )->extlst->extlst->GetObj( )->apply( ret );
					m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->SetObj( ret );
				}
			}

			id = obj->get( "manager2" )->toUInt( );
			if ( id > 0 ) {
				udPBase ret = new udProjectUser;
				wndForm->GetInfo( udHOrder::ctrlidManager )->extlst->extlst->GetObj( )->apply( ret );
				m_wndForm->GetInfo( ctrlidManager )->extlst->extlst->SetObj( ret );
			}
		} else if ( m_pFrmOpt->isset( "tpltour_init" ) ) {
			udPBase add = NULL, obj1 = NULL;
			udUInt id = 0;
			udPWndFormList frmlst = NULL;
			udPDwAscBase items = NULL;
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "tpltour_init" )->toDWord( );
			assert( wndForm );

			id = obj->get( "direct" )->toUInt( );
			if ( id > 0 ) {
				udPBase ret = new udProjectDirect;
				wndForm->GetInfo( udHTplTour::ctrlidDirect )->extlst->extlst->GetObj( )->apply( ret );
				m_wndForm->GetInfo( ctrlidDirect )->extlst->extlst->SetObj( ret );
			}

			frmlst = m_wndForm->GetInfo( ctrlidRoute )->frmlst;
			items = wndForm->GetInfo( udHTplTour::ctrlidRoute )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				obj1 = i->second;
				add = new udProjectTourRoute;
				add->get( "city" )->ini( obj1->get( "city" )->toUInt( ) );
				add->get( "name" )->ini( obj1->get( "name" )->toWString( ) );
				frmlst->AddItem( add );
			}

			frmlst = m_wndForm->GetInfo( ctrlidService )->frmlst;
			items = wndForm->GetInfo( udHTplTour::ctrlidService )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				obj1 = i->second;
				add = new udProjectTourService;
				add->get( "service" )->ini( obj1->get( "service" )->toUInt( ) );
				add->get( "num" )->ini( obj1->get( "num" )->toUInt( ) );
				add->get( "date1" )->ini( obj1->get( "date1" )->toTime( ) );
				add->get( "date2" )->ini( obj1->get( "date2" )->toTime( ) );
				add->get( "#name" )->ini( L"Не готова, " + obj1->get( "#name" )->toWString( ) );
				frmlst->AddItem( add );
			}

			frmlst = m_wndForm->GetInfo( ctrlidApp )->frmlst;
			wstring folder = frmlst->GetFolder( ), path = L"", name = L"";
			udPWndFormList frmlst1 = wndForm->GetInfo( udHTplTour::ctrlidApp )->frmlst;
			items = frmlst1->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				obj1 = i->second;
				add = new udProjectAttach;
				add->get( "type" )->ini( ( udInt ) udProjectAttach::ATTACH_TOUR );
				name = obj1->get( "name" )->toWString( );
				add->get( "name" )->ini( name );
				path =  folder + L"\\" + name;
				add->get( "tmp_path" )->ini( path );
				frmlst->AddItem( add );
			}
			DirectoryCopyFiles( frmlst1->GetFolder( ), frmlst->GetFolder( ) );
		}
		udPBase manager = m_wndForm->GetInfo( ctrlidManager )->extlst->extlst->GetObj( );
		if ( !manager ) {
			udPBase user = g_pApp->GetUser( );
			if ( user ) {
				manager = new udProjectUser;
				user->apply( manager );
				m_wndForm->GetInfo( ctrlidManager )->extlst->extlst->SetObj( manager );
			}
		}
	} // void LoadAfterSetObject

	void CHTour::SwitchToForm( ) {
		this->Clear( );
		m_iState = INTSTATE_FORM;
		this->Load( );
		udPBase manager = m_wndForm->GetInfo( ctrlidManager )->extlst->extlst->GetObj( );
		if ( !manager ) {
			udPBase user = g_pApp->GetUser( );
			if ( user ) {
				manager = new udProjectUser;
				user->apply( manager );
				m_wndForm->GetInfo( ctrlidManager )->extlst->extlst->SetObj( manager );
			}
		}
		this->Show( g_pApp->GetModel( )->GetWndModel( ) );
	} // void SwitchToForm

	void CHTour::RequestList( ) {
		udStateHandlerSimple::RequestList( );
		datetime date;
		udPBase obj = NULL;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			date = obj->get( "date1" )->toTime( );
			obj->get( "date1_" )->ini( date.toWString( "%d.%m.%Y" ) );
		}
	} // void RequestList

	void CHTour::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkFinance->Show( );
				m_lnkDocTpl->Show( );
				m_lnkPrint->Show( );
			} else {
				m_lnkFinance->Hide( );
				m_lnkDocTpl->Hide( );
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHTour::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkFinance ) {
				udDlgFinance::start( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), m_wndForm->GetObj( ) );
			} else if ( lnk == m_lnkDocTpl ) {
				udPBase			pDocTpl = NULL,		// шаблон документа
								pClient	= NULL,		// клиент
								pEmpl	= NULL,		// заказчик
								pClDoc	= NULL,		// документ заказчика
								pInfo	= NULL,		// общие данные по групповому договору
								pApp1	= NULL,		// приложение 1 группового договора
								pApp2	= NULL;		// приложение 2 группового договора
				udPArrBase		pClients	= NULL;	// туристы из тура
				udPListHandler	hList	= NULL;						// обработчик списка
				udPMorph		tmp		= NULL;						// временный объект
				HWND			hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
				udPLog			__log	= g_pApp->GetLog( );
				string			log		= "";
				//////////////////////////////////////////////////////////////////////////////////
				// выбираем шаблон документа
				hList = GetHDocTemplateList( );
				tmp = DialogListPick( hWnd, hList );
				if ( tmp ) {
					pDocTpl = new udProjectDocTemplate;
					pDocTpl->apply( tmp, true, true );
					
					log = "\nполученный шаблон документа\n";
					pDocTpl->dump( log );
					__log->Write( log + "\n" );
				}
				udDELETE( hList );
				udDELETE( tmp );
				//////////////////////////////////////////////////////////////////////////////////
				// выбираем туриста (если не групповой тур)
				if ( pDocTpl && ( pDocTpl->get( "type" )->toInt( ) != udProjectDocTemplate::tplContract2 ) ) {
					udPDwAscBase items = NULL;
					//////////////////////////////////////////////////////////////////////////////
					// выведем туристов из тура
					log = "\nтуристы из тура\n";
					items = m_wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						i->second->dump( log );
						log += "\n";
					}
					__log->Write( log );
					//////////////////////////////////////////////////////////////////////////////
					hList = GetHClientList( );
					hList->GetLstOpt( )->get( "pick_tourist" )->ini( ( udDWord ) m_wndForm );
					tmp = DialogListPick( hWnd, hList );
					if ( tmp ) {
						pClient = new udProjectClient;
						pClient->apply( tmp, true, true );
					}
					udDELETE( hList );
					udDELETE( tmp );
				}
				// выбираем документ заказчика
				if ( pDocTpl ) {
					if ( m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetAttr( ) == "fio" ) {
						pEmpl = m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetObj( );
						hList = GetHClDocList( );
						hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
						tmp = DialogListPick( hWnd, hList );
						if ( tmp ) {
							if ( tmp->get( "client" )->toUInt( ) == pEmpl->get( "id" )->toUInt( ) ) {
								pClDoc = new udProjectClDoc;
								pClDoc->apply( tmp, true, true );

								log = "\nполученный документ\n";
								pClDoc->dump( log );
								__log->Write( log + "\n" );
							} else {
								log = "\n";
								tmp->dump( log );
								__log->Write( log + "\n" );

								mb::err( hWnd, L"Вы выбрали документ другого клиента" );
								udDELETE( hList );
								udDELETE( tmp );
								hList = GetHClDocList( );
								hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
								while( true ) {
									tmp = DialogListPick( hWnd, hList );
									if ( tmp ) {
										if ( tmp->get( "client" )->toUInt( ) == pEmpl->get( "id" )->toUInt( ) ) {
											pClDoc = new udProjectClDoc;
											pClDoc->apply( tmp, true, true );

											log = "\nполученный документ\n";
											pClDoc->dump( log );
											__log->Write( log + "\n" );
											break;
										} else {
											mb::err( hWnd, L"Вы выбрали документ другого клиента" );
											udDELETE( hList );
											udDELETE( tmp );
											hList = GetHClDocList( );
											hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
										}
									} else {
										break;
									}
								}
							}
						}
					}
					udDELETE( hList );
					udDELETE( tmp );
				}
				// выбираем цену
				wstring amount;
				if ( pDocTpl ) {
					udPFormHandler hPrice = GetHTourPriceForm( );
					udPMorph obj = DialogForm( hWnd, hPrice );
					if ( obj ) {
						amount = obj->get( "name" )->toWString( );
					}
					udDELETE( obj );
					udDELETE( hPrice );
				}
				if ( pDocTpl ) {//&& ( pDocTpl->get( "type" )->toInt( ) == udProjectDocTemplate::tplContract2 ) ) {
					udPFormHandler hForm = NULL;
					udPMorph obj = NULL;
					//////////////////////////////////////////////////////////////////////////////
					udPDwAscBase items = m_wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
					map< udUInt, udUInt > ids;
					udUInt id = 0;
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						id = i->second->get( "client" )->toUInt( );
						ids[ id ] = id;
						g_pApp->GetLog( )->Write( toString( id ) + "\n" );
					}
					pClients = LoadClients( ids );
					//////////////////////////////////////////////////////////////////////////////
				}
				// генерируем результат
				if ( pDocTpl ) {
					// генерируем результат
					udHDocTemplate::ProcContractTemplate( m_wndForm, pClient, pDocTpl, pClDoc, amount, pClients, m_pApp1 );
					if ( pClients ) {
						for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
							udDELETE( *i );
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////////////
				udDELETE( pDocTpl );
				udDELETE( pClient );
				udDELETE( pClDoc );
				udDELETE( hList );
				udDELETE( pInfo );
				//udDELETE( pApp1 );
				udDELETE( pApp2 );
				udDELETE( pClients );
				udDELETE( tmp );
				//////////////////////////////////////////////////////////////////////////////////
			} else if ( lnk == m_lnkApp1 ) {
				udPFormHandler hForm = GetHGroupContractApp1Form( );
				hForm->GetFrmOpt( )->get( "obj" )->ini( ( udPVoid ) m_pApp1 );
				udPBase tmp = GetProjectGroupContractApp1( );
				m_pApp1->apply( tmp );
				udPMorph obj = DialogForm( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), hForm, tmp );
				if ( obj ) {
					obj->apply( m_pApp1 );
					udPArrBase pArr = NULL;
					udPDwAscBase items = NULL;
					udPBase add = NULL;
					udPWndModeForm wndForm = hForm->GetWndForm( );
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "occupancy" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidOccupancy )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCOccupancy( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "program" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidProgram )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCProgram( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "transfer" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidTransfer )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCTransfer( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "cover" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidCover )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCCover( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "visa" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidVisa )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCVisa( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "service" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidService )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCService( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					pArr = ( udPArrBase ) m_pApp1->get( "transfer2" )->get( );
					ClearArrBase( pArr );
					items = wndForm->GetInfo( udHGroupContractApp1::ctrlidTransfer2 )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						add = GetProjectGCTransfer2( );
						add->apply( i->second, true, true );
						pArr->push_back( add );
					}
					//////////////////////////////////////////////////////////////////////////////////
					if ( m_pApp1->get( "id" )->toUInt( ) > 0 ) {
						udPMorph todel = ( udPMorph ) m_pApp1->get( "#del" )->get( );
						udPArrBase itemsdel = NULL;
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "occupancy" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidOccupancy )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCOccupancy( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "program" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidProgram )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCProgram( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "transfer" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidTransfer )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCTransfer( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "cover" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidCover )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCCover( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "visa" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidVisa )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCVisa( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "service" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidService )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCService( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
						pArr = ( udPArrBase ) todel->get( "transfer2" )->get( );
						ClearArrBase( pArr );
						itemsdel = wndForm->GetInfo( udHGroupContractApp1::ctrlidTransfer2 )->frmlst->GetDelItems( );
						for( udArrBase::iterator i = itemsdel->begin( ); i != itemsdel->end( ); ++i ) {
							add = GetProjectGCTransfer2( );
							add->apply( *i, true, true );
							pArr->push_back( add );
						}
						//////////////////////////////////////////////////////////////////////////////
					}
					//////////////////////////////////////////////////////////////////////////////////
				}
				udDELETE( obj );
				udDELETE( hForm );
			} else if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
				udPMorph decl = NULL;
				udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
				udPBase obj = NULL;
				udPExtList extlst = NULL;
				udPDwAscBase items = NULL;
				//////////////////////////////////////////////////////////////////////////////////
				decl = new udMorph;
				decl->get( "version" )->ini( wstring( L"1.0" ) );
				decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
				doc->AddDeclaration( L"xml", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				decl = new udMorph;
				decl->get( "type" )->ini( wstring( L"text/xsl" ) );
				decl->get( "href" )->ini( wstring( L"tour_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectTour( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHTour::ctrlidEmployer )->extlst->extlst;
				node1 = doc->CreateNode( L"employer_txt" );
				if ( extlst->GetAttr( ) == "fio" ) {
					node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]>" );
				} else {
					node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				}
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHTour::ctrlidManager )->extlst->extlst;
				node1 = doc->CreateNode( L"manager_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHTour::ctrlidDirect )->extlst->extlst;
				node1 = doc->CreateNode( L"direct_txt" );
				node1->SetValue( L"<![CDATA[" +  extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHTour::ctrlidState )->extlst->extlst;
				node1 = doc->CreateNode( L"state_txt" );
				node1->SetValue( L"<![CDATA[" +  extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"route_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHTour::ctrlidRoute )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = i->second->toXml( doc );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"tourist_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = i->second->toXml( doc );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"service_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHTour::ctrlidService )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = doc->CreateNode( L"tourservice" );
					node2->SetValue( L"<![CDATA[" +  i->second->get( "#name" )->toWString( ) + L"]]>" );
					//node2 = i->second->toXml( doc );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Тур" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\tour_out.xsl", ( folder + wstring( L"\\tour_out.xsl" ) ).data( ), FALSE );
				ExtOpenFile( folder + L"\\out.xml" );
				//////////////////////////////////////////////////////////////////////////////////
				xml.clear( );
				udDELETE( obj );
				udDELETE( doc );
				//////////////////////////////////////////////////////////////////////////////////
			} else {
				udStateHandlerSimple::LinkClick( lnk );
			}
		} else if ( ( m_iState == INTSTATE_FORM ) && ( lnk == m_lnkApp1 ) ) {
			HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
			udPFormHandler hForm = GetHGroupContractApp1Form( );
			hForm->GetFrmOpt( )->get( "obj" )->ini( ( udPVoid ) m_pApp1 );
			udPBase tmp = GetProjectGroupContractApp1( );
			m_pApp1->apply( tmp );
			udPMorph obj = DialogForm( hWnd, hForm, tmp );
			if ( obj ) {
				obj->apply( m_pApp1 );
				udPArrBase pArr = NULL;
				udPDwAscBase items = NULL;
				udPBase add = NULL;
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "occupancy" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidOccupancy )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCOccupancy( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "program" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidProgram )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCProgram( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "transfer" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidTransfer )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCTransfer( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "cover" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidCover )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCCover( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "visa" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidVisa )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCVisa( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "service" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidService )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCService( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) m_pApp1->get( "transfer2" )->get( );
				ClearArrBase( pArr );
				items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1::ctrlidTransfer2 )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					add = GetProjectGCTransfer2( );
					add->apply( i->second, true, true );
					pArr->push_back( add );
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
			udDELETE( obj );
			udDELETE( hForm );
		} else {
			udStateHandlerSimple::LinkClick( lnk );
		}
	} // void LinkClick
/*
	void CHTour::DocTemplateData( udPBase pDocTpl ) {
		assert( pDocTpl );
		udInt type = pDocTpl->get( "type" )->toInt( );
		switch( type ) {
			case udProjectDocTemplate::tplClassic:												break;
			case udProjectDocTemplate::tplContract:		this->DocTemplateContract( pDocTpl );	break;
			case udProjectDocTemplate::tplPermit:		this->DocTemplatePermit( pDocTpl );		break;
			case udProjectDocTemplate::tplContract2:	this->DocTemplateContract2( pDocTpl );	break;
			default:
				break;
		}
	} // void DocTemplateData

	void CHTour::DocTemplateContract( udPBase pDocTpl ) {
		assert( pDocTpl );
		//////////////////////////////////////////////////////////////////////////////
		udPLog			__log		= g_pApp->GetLog( );
		string			log			= "";
		udPDwAscBase	items		= NULL;
		udPListHandler	hList		= NULL;
		udPMorph		tmp			= NULL;
		HWND			hWnd		= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		udPBase			pClient		= NULL,
						pEmpl		= NULL,
						pClDoc		= NULL;
		//////////////////////////////////////////////////////////////////////////////
		// выведем туристов из тура
		log = "\nтуристы из тура\n";
		items = m_wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			i->second->dump( log );
			log += "\n";
		}
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////
		// выбор туриста
		hList = GetHClientList( );
		hList->GetLstOpt( )->get( "pick_tourist" )->ini( ( udDWord ) m_wndForm );
		tmp = DialogListPick( hWnd, hList );
		if ( tmp ) {
			log = "\nполученный tmp клиента\n";
			tmp->dump( log );
			__log->Write( log + "\n" );

			pClient = new udProjectClient;
			pClient->apply( tmp, true, true );

			log = "\nполученный клиент\n";
			pClient->dump( log );
			__log->Write( log + "\n" );
		}
		udDELETE( hList );
		udDELETE( tmp );
		// выбираем документ заказчика
		if ( m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetAttr( ) == "fio" ) {
			pEmpl = m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetObj( );
			hList = GetHClDocList( );
			hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
			tmp = DialogListPick( hWnd, hList );
			if ( tmp ) {
				if ( tmp->get( "client" )->toUInt( ) == pEmpl->get( "id" )->toUInt( ) ) {
					pClDoc = new udProjectClDoc;
					pClDoc->apply( tmp, true, true );

					log = "\nполученный документ\n";
					pClDoc->dump( log );
					__log->Write( log + "\n" );
				} else {
					log = "\n";
					tmp->dump( log );
					__log->Write( log + "\n" );

					mb::err( hWnd, L"Вы выбрали документ другого клиента" );
					udDELETE( hList );
					udDELETE( tmp );
					hList = GetHClDocList( );
					hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
					while( true ) {
						tmp = DialogListPick( hWnd, hList );
						if ( tmp ) {
							if ( tmp->get( "client" )->toUInt( ) == pEmpl->get( "id" )->toUInt( ) ) {
								pClDoc = new udProjectClDoc;
								pClDoc->apply( tmp, true, true );

								log = "\nполученный документ\n";
								pClDoc->dump( log );
								__log->Write( log + "\n" );
								break;
							} else {
								mb::err( hWnd, L"Вы выбрали документ другого клиента" );
								udDELETE( hList );
								udDELETE( tmp );
								hList = GetHClDocList( );
								hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
							}
						} else {
							break;
						}
					}
				}
			}
		}
		udDELETE( hList );
		udDELETE( tmp );
		// выбираем цену
		wstring amount;
		udPFormHandler hPrice = GetHTourPriceForm( );
		udPMorph obj = DialogForm( hWnd, hPrice );
		if ( obj ) {
			amount = obj->get( "name" )->toWString( );
		}
		udDELETE( obj );
		udDELETE( hPrice );
		udHDocTemplate::ProcContractTemplate( m_wndForm, pClient, pDocTpl, pClDoc, amount );
		//////////////////////////////////////////////////////////////////////////////
		udDELETE( pClient );
		udDELETE( pClDoc );
		udDELETE( hList );
		udDELETE( tmp );
	} // void DocTemplateContract

	void CHTour::DocTemplatePermit( udPBase pDocTpl ) {
		DocTemplateContract( pDocTpl );
	} // void DocTemplatePermit

	void CHTour::DocTemplateContract2( udPBase pDocTpl ) {
		assert( pDocTpl );
		//////////////////////////////////////////////////////////////////////////////
		udPLog			__log		= g_pApp->GetLog( );
		string			log			= "";
		udPDwAscBase	items		= NULL;
		udPListHandler	hList		= NULL;
		udPMorph		tmp			= NULL;
		HWND			hWnd		= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		udPBase			pClient		= NULL,
						pEmpl		= NULL,
						pClDoc		= NULL,
						//
						pInfo		= NULL,
						pApp1		= NULL,
						pApp2		= NULL;
		udPArrBase		pClients	= NULL;
		//////////////////////////////////////////////////////////////////////////////
		// выведем туристов из тура
		log = "\nтуристы из тура\n";
		items = m_wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			i->second->dump( log );
			log += "\n";
		}
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////
		// документ заказчика
		if ( m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetAttr( ) == "fio" ) {
			pEmpl = m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetObj( );
			hList = GetHClDocList( );
			hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
			tmp = DialogListPick( hWnd, hList );
			if ( tmp ) {
				if ( tmp->get( "client" )->toUInt( ) == pEmpl->get( "id" )->toUInt( ) ) {
					pClDoc = new udProjectClDoc;
					pClDoc->apply( tmp, true, true );

					log = "\nполученный документ\n";
					pClDoc->dump( log );
					__log->Write( log + "\n" );
				} else {
					log = "\n";
					tmp->dump( log );
					__log->Write( log + "\n" );

					mb::err( hWnd, L"Вы выбрали документ другого клиента" );
					udDELETE( hList );
					udDELETE( tmp );
					hList = GetHClDocList( );
					hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
					while( true ) {
						tmp = DialogListPick( hWnd, hList );
						if ( tmp ) {
							if ( tmp->get( "client" )->toUInt( ) == pEmpl->get( "id" )->toUInt( ) ) {
								pClDoc = new udProjectClDoc;
								pClDoc->apply( tmp, true, true );

								log = "\nполученный документ\n";
								pClDoc->dump( log );
								__log->Write( log + "\n" );
								break;
							} else {
								mb::err( hWnd, L"Вы выбрали документ другого клиента" );
								udDELETE( hList );
								udDELETE( tmp );
								hList = GetHClDocList( );
								hList->GetLstOpt( )->get( "client" )->ini( ( udDWord ) pEmpl );
							}
						} else {
							break;
						}
					}
				}
			}
		}
		udDELETE( hList );
		udDELETE( tmp );
		//////////////////////////////////////////////////////////////////////////////
		// цена
		wstring amount;
		udPFormHandler hPrice = GetHTourPriceForm( );
		udPMorph obj = DialogForm( hWnd, hPrice );
		if ( obj ) {
			amount = obj->get( "name" )->toWString( );
		}
		udDELETE( obj );
		udDELETE( hPrice );
		//////////////////////////////////////////////////////////////////////////////
		udPFormHandler hForm = NULL;
		//////////////////////////////////////////////////////////////////////////////
		// туристы
		items = m_wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
		map< udUInt, udUInt > ids;
		udUInt id = 0;
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			id = i->second->get( "client" )->toUInt( );
			ids[ id ] = id;
			g_pApp->GetLog( )->Write( toString( id ) + "\n" );
		}
		pClients = LoadClients( ids );
		//////////////////////////////////////////////////////////////////////////////
		// информация
		hForm = GetHGroupContractFForm( );
		obj = DialogForm( hWnd, hForm );
		if ( obj ) {
			pInfo = GetProjectGroupContractF( );
			obj->apply( pInfo );
			log = "\ngroup_contract_info:\n";
			obj->dump( log );
			__log->Write( log + "\n" );
		}
		udDELETE( obj );
		udDELETE( hForm );
		//////////////////////////////////////////////////////////////////////////////
		// приложение 1
		hForm = GetHGroupContractApp1FForm( );
		hForm->GetFrmOpt( )->get( "tour_clients" )->ini( ( udDWord ) pClients );
		obj = DialogForm( hWnd, hForm );
		if ( obj ) {
			pApp1 = GetProjectGroupContractApp1F( );
			obj->apply( pApp1 );
			log = "\ngroup_contract_app1:\n";
			obj->dump( log );
			log += "\n\n";
			udPArrBase pArr = NULL;
			udPBase add = NULL;
			udPDwAscBase items = NULL;
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidTouristInfo )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "info" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidOccupancy )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "occupancy" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidProgram )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "program" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidTransfer )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "transfer" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidCover )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "cover" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidVisa )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "visa" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidService )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "service" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = new udArrBase;
			items = hForm->GetWndForm( )->GetInfo( udHGroupContractApp1F::ctrlidTransfer2 )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				add = new udBase;
				i->second->apply( add );
				pArr->push_back( add );
			}
			pApp1->get( "transfer2" )->ini( ( udPVoid ) pArr );
			//////////////////////////////////////////////////////////////////////////
		}
		udDELETE( obj );
		udDELETE( hForm );
		//////////////////////////////////////////////////////////////////////////////
		// приложение 2
		hForm = GetHGroupContractApp2FForm( );
		obj = DialogForm( hWnd, hForm );
		if ( obj ) {
			pApp2 = GetProjectGroupContractApp2F( );
			obj->apply( pApp2 );
			log = "\ngroup_contract_app2:\n";
			obj->dump( log );
			__log->Write( log + "\n" );
		}
		udDELETE( obj );
		udDELETE( hForm );
		//////////////////////////////////////////////////////////////////////////////
		// распечатываем документ
		udHDocTemplate::ProcContractTemplate( m_wndForm, pClient, pDocTpl, pClDoc, amount, pClients, pInfo, pApp1, pApp2 );
		if ( pApp1 ) { // удаляем дополнительные атрибуты
			udPArrBase pArr = NULL;
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "info" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "occupancy" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "program" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "transfer" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "cover" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "visa" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "service" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
			pArr = ( udPArrBase ) pApp1->get( "transfer2" )->get( );
			ClearArrBase( pArr );
			udDELETE( pArr );
			//////////////////////////////////////////////////////////////////////////
		}
		if ( pClients ) {
			ClearArrBase( pClients );
			udDELETE( pClients );
		}
		//////////////////////////////////////////////////////////////////////////////
		udDELETE( pClient );
		udDELETE( pClDoc );
		udDELETE( hList );
		udDELETE( pInfo );
		udDELETE( pApp1 );
		udDELETE( pApp2 );
		udDELETE( pClients );
		udDELETE( tmp );
		//////////////////////////////////////////////////////////////////////////////
	} // void DocTemplateContract2
*/




	CHTourService::CHTourService( ) : udFormHandler( ) {
	}

	CHTourService::~CHTourService( ) {
	}

	udInt CHTourService::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddExtlist( ctrlidService, "service", NULL, "name_", L"выберите услугу", GetHServiceList );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddInput( ctrlidNum, "num", NULL, font, ES_NUMBER );
		udArrWString arrSelect;
		arrSelect.push_back( L"Не готова" );
		arrSelect.push_back( L"Готова" );
		m_wndForm->AddSelect( ctrlidReady, "ready", NULL, font, arrSelect );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTourService::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		wstring		title		= L"";
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		udPGdi		gdi			= m_wndForm->GetGdi( );
		assert( gdi );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Информация об услуге";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Услуга";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidService );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата \"c\"";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата \"по\"";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Кол-во";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidNum );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Готова?";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		udArrWString arrSelect;
		arrSelect.push_back( L"Не готова" );
		arrSelect.push_back( L"Готова" );
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidReady );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHTourService::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );

		if ( obj->get( "service" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите услугу" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "num" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите количество" );
			return FormHandlerErrorFailed;
		}

		ModeFormControlInfo *info = m_wndForm->GetInfo( ctrlidService );
		udPBase service = info->extlst->extlst->GetObj( );
		obj->get( "name_" )->ini( service->get( "name_" )->toWString( ) );

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHTourService::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHTourService::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHTourService::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

	void CHTourService::LoadAfterSetObject( ) {
		udPBase obj = m_wndForm->GetObj( );
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "service" )->toUInt( );
		if ( id > 0 ) {
			udPBase	ret		= LoadById( "service", id, GetProjectService ),
					name	= LoadById( "nameservice", ret->get( "name" )->toUInt( ), GetProjectNameService );
			ret->get( "name_" )->ini( name->get( "name" )->toWString( ) );
			delete name;
			m_wndForm->GetInfo( ctrlidService )->extlst->extlst->SetObj( ret );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadAfterSetObject




	CHTourPrice::CHTourPrice( ) {
	}

	CHTourPrice::~CHTourPrice( ) {
	}

	udInt CHTourPrice::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidAmount, "name", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTourPrice::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 200 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		wstring		title		= L"";
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		udPGdi		gdi			= m_wndForm->GetGdi( );
		assert( gdi );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Сумма за тур";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Сумма";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidAmount );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHTourPrice::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHTourPrice::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHTourPrice::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHTourPrice::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



/*
	udPBase GetProjectGroupContractF( ) {
		udPBase ret = new udBase;
		ret->get( "days1" )->ini( wstring( L"" ) );
		ret->get( "days2" )->ini( wstring( L"" ) );
		ret->get( "days3" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroupContractF

	CHGroupContractF::CHGroupContractF( ) {
	}

	CHGroupContractF::~CHGroupContractF( ) {
	}

	udInt CHGroupContractF::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidDays1, "days1", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidDays2, "days2", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidDays3, "days3", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGroupContractF::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 500 );
			m_wndForm->get( "height" )->ini( ( udInt ) 400 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		wstring		title		= L"";
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		udPGdi		gdi			= m_wndForm->GetGdi( );
		assert( gdi );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Общие данные";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"За сколько дней до поездки предоставить документы (не позднее)";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 19;
		//x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDays1 );
		info->box = box;
		y += h + m + 5;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"В течение скольки дней после бронирования подтвердить тур";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 19;
		//x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDays2 );
		info->box = box;
		y += h + m + 5;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"В течение скольки дней после бронирования оплатить тур (не позднее)";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 19;
		//x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDays3 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHGroupContractF::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGroupContractF::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGroupContractF::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGroupContractF::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



	udPBase GetProjectGroupContractApp2F( ) {
		udPBase ret = new udBase;
		ret->get( "name" )->ini( wstring( L"" ) );
		ret->get( "addr" )->ini( wstring( L"" ) );
		ret->get( "number" )->ini( wstring( L"" ) );
		ret->get( "fintype" )->ini( wstring( L"" ) );
		ret->get( "garant" )->ini( wstring( L"" ) );
		ret->get( "orgname" )->ini( wstring( L"" ) );
		ret->get( "garantaddr" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGCTouristInfo

	CHGroupContractApp2F::CHGroupContractApp2F( ) {
	}

	CHGroupContractApp2F::~CHGroupContractApp2F( ) {
	}

	udInt CHGroupContractApp2F::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidAddr, "addr", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidNumber, "number", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidFinType, "fintype", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidGarant, "garant", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidOrgName, "orgname", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidGarantAddr, "garantaddr", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGroupContractApp2F::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 600 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		wstring		title		= L"";
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		udPGdi		gdi			= m_wndForm->GetGdi( );
		assert( gdi );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Приложение 2";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Название туроператора";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidName );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Адрес";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidAddr );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Реестровый номер";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidNumber );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Вид и размер финансового обеспечения";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidFinType );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата и срок действия договора страхования ответственности туроператора или банковской гарантии";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidGarant );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Наименование организации, предоставившей финансовое обеспечение";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidOrgName );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Адрес гаранта";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidGarantAddr );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHGroupContractApp2F::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGroupContractApp2F::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGroupContractApp2F::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGroupContractApp2F::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm
*/
} // namespace UdSDK
