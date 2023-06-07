
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectReportLanding2( ) {
		udPBase ret = GetProjectReportLanding( );
		ret->get( "group_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectReportLanding2

	udPBase GetProjectReportFilling2( ) {
		udPBase ret = GetProjectReportFilling( );
		ret->get( "group_" )->ini( wstring( L"" ) );
		ret->get( "period_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectReportFilling2




	CHReport::CHReport( ) : udStateHandler( ), udListHandler( ) {
		m_iState2		= reportLanding;
		m_szOrderBy		= "id";
		m_iOrderType	= ordDesc;

		m_pPager->SetPageSize( 15 );

		udPWndButton btn = NULL;

		btn = new udWndButton;
		btn->get( "title" )->ini( wstring( L"Фильтровать" ) );
		btn->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		btn->get( "x" )->ini( ( udInt ) 0 );
		btn->get( "y" )->ini( ( udInt ) 0 );
		btn->get( "width" )->ini( ( udInt ) 152 );
		btn->get( "height" )->ini( ( udInt ) 34 );
		btn->SetFontSize( 14 );
		btn->SetPaddingLeft( 10 );
		btn->SetPaddingTop( 10 );
		btn->SetRectNormal( new Rect( 253, 159, 152, 34 ) );
		btn->get( "state" )->ini( reportLanding );
		btn->get( "type" )->ini( btnidFilter );
		m_arrButton.push_back( btn );
		m_mtxButton[ reportLanding ][ btnidFilter ] = btn;

		btn = new udWndButton;
		btn->get( "title" )->ini( wstring( L"Фильтровать" ) );
		btn->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		btn->get( "x" )->ini( ( udInt ) 0 );
		btn->get( "y" )->ini( ( udInt ) 0 );
		btn->get( "width" )->ini( ( udInt ) 152 );
		btn->get( "height" )->ini( ( udInt ) 34 );
		btn->SetFontSize( 14 );
		btn->SetPaddingLeft( 10 );
		btn->SetPaddingTop( 10 );
		btn->SetRectNormal( new Rect( 253, 159, 152, 34 ) );
		btn->get( "state" )->ini( reportFilling );
		btn->get( "type" )->ini( btnidFilter );
		m_arrButton.push_back( btn );
		m_mtxButton[ reportFilling ][ btnidFilter ] = btn;

		btn = new udWndButton;
		btn->get( "title" )->ini( wstring( L"Экспорт" ) );
		btn->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		btn->get( "x" )->ini( ( udInt ) 0 );
		btn->get( "y" )->ini( ( udInt ) 0 );
		btn->get( "width" )->ini( ( udInt ) 152 );
		btn->get( "height" )->ini( ( udInt ) 34 );
		btn->SetFontSize( 14 );
		btn->SetPaddingLeft( 10 );
		btn->SetPaddingTop( 10 );
		btn->SetRectNormal( new Rect( 253, 159, 152, 34 ) );
		btn->get( "state" )->ini( reportLanding );
		btn->get( "type" )->ini( btnidExport );
		m_arrButton.push_back( btn );
		m_mtxButton[ reportLanding ][ btnidExport ] = btn;

		btn = new udWndButton;
		btn->get( "title" )->ini( wstring( L"Экспорт" ) );
		btn->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		btn->get( "x" )->ini( ( udInt ) 0 );
		btn->get( "y" )->ini( ( udInt ) 0 );
		btn->get( "width" )->ini( ( udInt ) 152 );
		btn->get( "height" )->ini( ( udInt ) 34 );
		btn->SetFontSize( 14 );
		btn->SetPaddingLeft( 10 );
		btn->SetPaddingTop( 10 );
		btn->SetRectNormal( new Rect( 253, 159, 152, 34 ) );
		btn->get( "state" )->ini( reportFilling );
		btn->get( "type" )->ini( btnidExport );
		m_arrButton.push_back( btn );
		m_mtxButton[ reportFilling ][ btnidExport ] = btn;
	}

	CHReport::CHReport( const CHReport& objCopy ) {
	}

	CHReport::~CHReport( ) {
		for( vct_btn::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
			delete *i;
		}
		m_mtxButton.clear( );
	}

	void CHReport::Init( ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;

		option.SetTable( "ud_report_group" );
		option.SetClass( GetProjectReportGroup );
		handler->CheckTable( option );

		option.SetTable( "ud_report_landing" );
		option.SetClass( GetProjectReportLanding );
		handler->CheckTable( option );

		option.SetTable( "ud_report_filling" );
		option.SetClass( GetProjectReportFilling );
		handler->CheckTable( option );

		wstring folder = L"export";
		CreateDirectory( folder.data( ), NULL );

		CreateList( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), false );
	} // void Init

	void CHReport::Show( udPWndModel wnd ) {
		if ( m_wndModeList ) {
			m_wndModeList->Show( );
		}
	} // void Show

	void CHReport::Clear( ) {
		this->ClearList( );
	} // void Clear

	void CHReport::Load( ) {
		this->LoadList( );
	} // void Load

	void CHReport::ClickToolbar( string index ) {
	} // void ClickToolbar

	udInt CHReport::ToolbarAction( string index ) {
		return 0;
	} // udInt ToolbarAction

	void CHReport::LinkClick( udPWndLink lnk ) {
	} // void LinkClick

	void CHReport::ButtonClick( udPWndButton btn ) {
	} // void ButtonClick

	void CHReport::LoadList( ) {
		// подгружать список надо из БД
		//*
		udPHandler		handler = g_pApp->GetHandler( );
		udHandlerOption	option;
		udArrError		arrError;
		string			szOrder = "",
						szOrderType = ( m_iOrderType == 1 ) ? "ASC" : "DESC";

		if ( m_iState2 == reportLanding ) {
			option.SetTable( "ud_report_landing" );
			option.SetClass( GetProjectReportLanding2 );

			if ( m_szOrderBy == "group" ) {
				szOrder = "`report_landing_group_` ";
			} else {
				szOrder = "`report_landing_" + m_szOrderBy + "` ";
			}
			if ( !szOrder.empty( ) ) {
				szOrder += szOrderType;
			}
			option.SetOrder( szOrder );
			option.SetWhere( this->GetWhereCondition( ) );
			option.AddLeftJoin(
				"name", "report_landing_group_", "ud_report_group",
				"`ud_report_landing`.`report_landing_group`=`ud_report_group`.`report_group_id`",
				GetProjectReportGroup
			);
		} else {
			option.SetTable( "ud_report_filling" );
			option.SetClass( GetProjectReportFilling2 );

			if ( m_szOrderBy == "group" ) {
				szOrder = "`report_filling_group_` ";
			} else {
				szOrder = "`report_filling_" + m_szOrderBy + "` ";
			}
			if ( !szOrder.empty( ) ) {
				szOrder += szOrderType;
			}
			option.SetOrder( szOrder );
			option.SetWhere( this->GetWhereCondition( ) );
			option.AddLeftJoin(
				"name", "report_filling_group_", "ud_report_group",
				"`ud_report_filling`.`report_filling_group`=`ud_report_group`.`report_group_id`",
				GetProjectReportGroup
			);
			option.SetIgnoreAttr( "period_" );
		}
		option.SetLimit( m_pPager->GetSQLLimit( ) );
		option.SetIgnoreAttr( "group_" );
		handler->GetUdObject( m_arrObject, option, &arrError );
		if ( !arrError.empty( ) ) {
			for( udArrError::iterator i = arrError.begin( ); i != arrError.end( ); ++i ) {
				g_pApp->GetLog( )->Write( ( *i ).GetText( ) + "\n" );
			}
		} else {
			if ( m_iState2 == reportFilling ) {
				udPBase obj = NULL;
				for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
					obj = *i;
					obj->get( "period_" )->ini(
						obj->get( "date1" )->toTime( ).toWString( "%Y-%m-%d" ) + L", " +
						obj->get( "min" )->toWString( ) + L"-" + obj->get( "max" )->toWString( )
					);
				}
			}
			if ( !m_wndModeList ) {
				CreateList( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), false );
			}
			udLongLong cnt = handler->CountUdObject( option );
			m_pPager->SetTotal( ( udUInt ) cnt );
		}
		//*/
		udPWndInstanceList wndInslst = m_wndModeList->GetList( );
		assert( wndInslst );
		wndInslst->GenerateRows( &m_arrObject );
		udPWndPager wndPager = m_wndModeList->GetPager( );
		assert( wndPager );
		wndPager->Generate( m_pPager );
		udInt y = wndInslst->get( "y" )->toInt( ) + wndInslst->get( "height" )->toInt( ) + 5;
		wndPager->get( "y" )->ini( y );
		wndPager->Move( );
	} // void LoadList

	udInt CHReport::GetListModel( ) {
		if ( m_iState2 == reportLanding ) {
			return ListLandingModel( );
		} else if ( m_iState2 == reportFilling ) {
			return ListFillingModel( );
		}
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHReport::GetListView( ) {
		if ( m_iState2 == reportLanding ) {
			return ListLandingView( );
		} else if ( m_iState2 == reportFilling ) {
			return ListFillingView( );
		}
		return ListHandlerErrorOk;
	} // udInt GetListView

	udInt CHReport::ListOk( ) { 
		return ListHandlerErrorOk;
	} // udInt ListOk

	udInt CHReport::ListCancel( ) {
		return ListHandlerErrorOk;
	} // udInt ListCancel

	void CHReport::OnLinkClick( udPWndLink lnk ) {
		bool make_switch = false;
		string url = lnk->get( "url" )->toString( );
		if ( url == "main" ) {
			g_pApp->GetModel( )->SwitchToMain( );
		} else if ( url == "filling" ) {
			this->ClearList( );
			m_iState2	= reportFilling;
			make_switch	=  true;
		} else if ( url == "landing" ) {
			this->ClearList( );
			m_iState2	= reportLanding;
			make_switch	= true;
		}
		if ( make_switch ) {
			HWND hWnd = m_wndModeList->GetHWndParent( );
			bool modal = m_wndModeList->IsModal( );
			m_wndModeList->Destroy( );
			udDELETE( m_wndModeList );
			this->CreateList( hWnd, modal );
			this->LoadList( );
			m_wndModeList->Show( );
		}
	} // void OnLinkClick

	void CHReport::OnSelectRow( udLong row ) {
	} // void OnSelectRow

	void CHReport::OnHeaderClick( const string& name, const string& attr ) {
		assert( m_wndModeList );
		assert( m_wndModeList->GetList( ) );
		assert( m_wndModeList->GetPager( ) );
		this->ClearList( );
		if ( m_szOrderBy == attr ) {
			m_iOrderType = 1 - m_iOrderType;
		} else {
			m_szOrderBy = attr;
			m_iOrderType = ordDesc;
		}
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnHeaderClick

	void CHReport::OnPagerSelect( udUInt page ) {
		this->ClearList( );
		m_pPager->SetPage( page );
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnPagerSelect

	void CHReport::OnButtonClick( udPWndButton btn ) {
		udInt	state	= btn->get( "state" )->toInt( ),
				type	= btn->get( "type" )->toInt( );
		if ( state == reportLanding ) {
			if ( type == btnidFilter ) {
				this->ClearList( );
				m_wndModeList->LoadControlData( );
				this->LoadList( );
				m_wndModeList->Show( );
			} else if ( type == btnidExport ) {
				if ( this->MakeExportLandingFile( ) > 0 ) {
					ExtOpenFile( g_pApp->GetConfig( )->get( "workdir" )->toWString( ) + L"\\export\\landing.html" );
				}
			}
		} else if ( state == reportFilling ) {
			if ( type == btnidFilter ) {
				this->ClearList( );
				m_wndModeList->LoadControlData( );
				this->LoadList( );
				m_wndModeList->Show( );
			} else if ( type == btnidExport ) {
				if ( this->MakeExportFillingFile( ) > 0 ) {
					ExtOpenFile( g_pApp->GetConfig( )->get( "workdir" )->toWString( ) + L"\\export\\filling.html" );
				}
			}
		}
	} // void OnButtonClick

	udInt CHReport::ListLandingModel( ) {
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LAST_CONTROL_ID_LANDING;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
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
		title = L"Ведомость текущего наполнения";
		url = "filling";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "group_", L"Группа", "group" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "name", L"№ п/п" )->get( "width" )->ini( ( udInt ) 50 );
		wndInsLst->AddAttr( "fio", L"ФИО" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "phone", L"Телефон(ы)" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "state", L"Состояние" )->get( "width" )->ini( ( udInt ) 100 );
		wndInsLst->AddAttr( "has_packet", L"Услуга типа \"Пакет\"" )->get( "width" )->ini( ( udInt ) 100 );
		wndInsLst->AddAttr( "has_landing", L"Услуга типа \"Посадка\"" )->get( "width" )->ini( ( udInt ) 100 );
		wndInsLst->AddAttr( "comment", L"Примечание" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( landingDate1, "date1", NULL, font );
		m_wndModeList->AddDate( landingDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( landingGroup, "group", NULL, "name", L"выберите группу", GetHGroupList );
		m_wndModeList->AddExtlist( landingDirect, "direct", NULL, "name", L"выберите направление", GetHDirectList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "group" )->ini( ( udUInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndButton btn = NULL;
		btn = m_wndModeList->GetButtonFilter( );
		btn->get( "title" )->ini( wstring( L"Скачать" ) );
		//btn = m_wndModeList->GetButtonReset( );
		//btn->get( "title" )->ini( wstring( L"Экспорт" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt ListLandingModel

	udInt CHReport::ListLandingView( ) {
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
		itLnk = arrLink->find( 1 );
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
		box->x = x;	box->y = y;	box->w = 88; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( landingDate1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 88; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( landingDate2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( landingGroup );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( landingDirect );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndModeList->SetButtonFilterPosition( x, y - m );
		x += m_wndModeList->GetButtonFilter( )->get( "width" )->toInt( ) + m;
		m_wndModeList->SetButtonResetPosition( x, y - m );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += 30 + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt x1 = m_wndModeList->GetButtonFilter( )->get( "x" )->toInt( );
		udPWndButton btn = NULL;
		btn = m_mtxButton[ reportLanding ][ btnidFilter ];
		btn->get( "x" )->ini( x1 );
		btn->get( "y" )->ini( y );
		//btn->SetTargetWnd( m_wndModeList->GetHWnd( ) );
		btn->Create( m_wndModeList->GetHWnd( ), ( HMENU ) 997 );
		x1 += btn->get( "width" )->toInt( ) + m;
		btn = m_mtxButton[ reportLanding ][ btnidExport ];
		btn->get( "x" )->ini( x1 );
		btn->get( "y" )->ini( y );
		//btn->SetTargetWnd( m_wndModeList->GetHWnd( ) );
		btn->Create( m_wndModeList->GetHWnd( ), ( HMENU ) 996 );
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 30 + m + m;
		//////////////////////////////////////////////////////////////////////////////////////////
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
		w = 400;
		y += wndInsLst->get( "height" )->toInt( ) + m;
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "style" )->ini( style );
		wndPager->get( "x" )->ini( x );
		wndPager->get( "y" )->ini( y );
		wndPager->get( "width" )->ini( w );
		wndPager->get( "height" )->ini( 20 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt ListLandingView

	udInt CHReport::ListFillingModel( ) {
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LAST_CONTROL_ID_FILLING;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
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
		title = L"Посадочная ведомость";
		url = "landing";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "group_", L"Группа", "group" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "period_", L"Период", "period" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "state", L"Состояние" )->get( "width" )->ini( ( udInt ) 200 );
		wndInsLst->AddAttr( "amount", L"Количество туристов" )->get( "width" )->ini( ( udInt ) 200 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fillingDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fillingDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fillingGroup, "group", NULL, "name", L"выберите группу", GetHGroupList );
		m_wndModeList->AddExtlist( fillingDirect, "direct", NULL, "name", L"выберите направление", GetHDirectList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "group" )->ini( ( udUInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndButton btn = NULL;
		btn = m_wndModeList->GetButtonFilter( );
		btn->get( "title" )->ini( wstring( L"Скачать" ) );
		//btn = m_wndModeList->GetButtonReset( );
		//btn->get( "title" )->ini( wstring( L"Экспорт" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt ListFillingModel

	udInt CHReport::ListFillingView( ) {
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
		itLnk = arrLink->find( 1 );
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
		box->x = x;	box->y = y;	box->w = 88; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fillingDate1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 88; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fillingDate2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fillingGroup );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fillingDirect );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndModeList->SetButtonFilterPosition( x, y - m );
		x += m_wndModeList->GetButtonFilter( )->get( "width" )->toInt( ) + m;
		m_wndModeList->SetButtonResetPosition( x, y - m );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += 30 + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt x1 = m_wndModeList->GetButtonFilter( )->get( "x" )->toInt( );
		udPWndButton btn = NULL;
		btn = m_mtxButton[ reportFilling ][ btnidFilter ];
		btn->get( "x" )->ini( x1 );
		btn->get( "y" )->ini( y );
		//btn->SetTargetWnd( m_wndModeList->GetHWnd( ) );
		btn->Create( m_wndModeList->GetHWnd( ), ( HMENU ) 997 );
		x1 += btn->get( "width" )->toInt( ) + m;
		btn = m_mtxButton[ reportFilling ][ btnidExport ];
		btn->get( "x" )->ini( x1 );
		btn->get( "y" )->ini( y );
		//btn->SetTargetWnd( m_wndModeList->GetHWnd( ) );
		btn->Create( m_wndModeList->GetHWnd( ), ( HMENU ) 996 );
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 30 + m + m;
		//////////////////////////////////////////////////////////////////////////////////////////
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
		w = 250;
		y += wndInsLst->get( "height" )->toInt( ) + m;
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "style" )->ini( style );
		wndPager->get( "x" )->ini( x );
		wndPager->get( "y" )->ini( y );
		wndPager->get( "width" )->ini( w );
		wndPager->get( "height" )->ini( 20 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt ListFillingView

	void CHReport::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "group" )->ini( ( udUInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHReport::OnFilter( ) {
		this->ClearList( );
		m_wndModeList->LoadControlData( );
/*
	если не установлена группа, то необходимо добавить колонку
	иначе необходимо добавить надпись с описание группы
*/
		// очищаем таблицу с отчетами
		g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_group`" );
		g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_landing`" );
		g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_filling`" );

		udPSocket sock = BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( string( "report" ) );
			data.add( m_iState2 );
			m_wndModeList->GetObjFilter( )->dump( data );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udPHandler handler = g_pApp->GetHandler( );
				udHandlerOption option;
				udArrBase arr;
				bool show_error = false;
				// группы
				if ( ReadInstances( sock, arr, GetProjectReportGroup ) > 0 ) {
					option.SetTable( "ud_report_group" );
					handler->AddUdObject( arr, option );
				} else {
					show_error = true;
				}
				ClearArrBase( arr );
				// остальные записи
				pfnGetObject fnGetObject = NULL;
				if ( m_iState2 == reportLanding ) {
					fnGetObject = GetProjectReportLanding;
					option.SetTable( "ud_report_landing" );
				} else if ( m_iState2 == reportFilling ) {
					fnGetObject = GetProjectReportFilling;
					option.SetTable( "ud_report_filling" );
				}
				if ( fnGetObject ) {
					if ( ReadInstances( sock, arr, fnGetObject ) > 0 ) { // есть пункты
						handler->AddUdObject( arr, option );
					} else {
						show_error = true;
					}
					ClearArrBase( arr );
				}
				if ( show_error ) {
					g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_group`" );
					g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_landing`" );
					g_pApp->GetDatabase( )->Query( "TRUNCATE `ud_report_filling`" );
				}
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnFilter

	void CHReport::OnFilterReset( ) {
		this->ClearList( );
		this->ClearFilter( );
		m_wndModeList->ClearFilter( );
		m_wndModeList->FillControls( );
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnFilterReset

	string CHReport::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		udPMorph filter = m_wndModeList->GetObjFilter( );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = NULL;
		if ( m_iState2 == reportLanding ) {
			obj = new udProjectReportLanding;
		} else if ( m_iState2 == reportFilling ) {
			obj = new udProjectReportFilling;
		}
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		datetime date;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		date = filter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "date1" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = filter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "date1" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = filter->get( "group" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "group" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = filter->get( "direct" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "direct" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

	static void PrintReportHead( FILE* pFileOut, const wstring& szGroupName = L"" ) {
		wstring tmp = L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
			L"<html xmlns=\"http://www.w3.org/1999/xhtml\">"
			L"<head><title>" + ( szGroupName.empty( ) ?
			L"Данные групп" :
			L"Данные группы &quot;" + szGroupName + L"&quot;"
			) + L"</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>"
			L"<link rel=\"stylesheet\" type=\"text/css\" href=\"main.css\"/>"
			L"</head><body>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportHead

	static void PrintReportGroupInfo( udPBase obj, FILE* pFileOut ) {
		wstring tmp = L"<div class=\"header\">"
			L"<h1>" + obj->get( "name" )->toWString( ) + L"</h1>"
			L"<p><b>" + obj->get( "date1" )->toTime( ).toWString( "%Y-%m-%d" ) + L" &mdash; " +
			obj->get( "date2" )->toTime( ).toWString( "%Y-%m-%d" ) + L"</b></p><p>" +
			obj->get( "min" )->toWString( ) + L" &mdash; " +
			obj->get( "max" )->toWString( ) + L"</p>"
			L"</div>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupInfo

	static void PrintReportGroupTableHeader( FILE* pFileOut ) {
		wstring tmp = L"<table class=\"table\"><tr>"
			L"<th>№ п/п</th>"
			L"<th>ФИО</th>"
			L"<th>Телефон(ы)</th>"
			L"<th>Состояние</th>"
			L"<th>Услуга типа &laquo;Пакет&raquo;</th>"
			L"<th>Услуга типа &laquo;Посадка&raquo;</th>"
			L"<th>Примечание</th>"
			L"</tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupTableHeader

	static void PrintReportGroupTableHeader2( FILE* pFileOut ) {
		wstring tmp = L"<table class=\"table\"><tr>"
			L"<th>Состояние</th>"
			L"<th>Количество туристов</th>"
			L"</tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupTableHeader2

	static void PrintReportGroupTableRow( udInt place, udPBase obj, FILE* pFileOut ) {
		wstring tmp = L"<tr><td style=\"width: 20px;\">" +
			toWString( place ) + L"</td><td>" +
			obj->get( "fio" )->toWString( ) + L"&nbsp;</td><td>" +
			obj->get( "phone" )->toWString( ) + L"&nbsp;</td><td>" +
			obj->get( "state" )->toWString( ) + L"&nbsp;</td><td>" +
			obj->get( "has_packet" )->toWString( ) + L"&nbsp;</td><td>" +
			obj->get( "has_landing" )->toWString( ) + L"&nbsp;</td><td>" +
			obj->get( "comment" )->toWString( ) + L"&nbsp;</td></tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupTableRow

	static void PrintReportGroupTableRow2( udPBase obj, FILE* pFileOut ) {
		wstring tmp = L"<tr><td>" +
			obj->get( "state" )->toWString( ) + L"&nbsp;</td><td>" +
			obj->get( "amount" )->toWString( ) + L"&nbsp;</td></tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportGroupTableRow2

	static void PrintReportFillingPeriodHeader( udPBase obj, FILE* pFileOut ) {
		wstring tmp = L"<tr><th colspan=\"2\">" +
			obj->get( "date1" )->toTime( ).toWString( "%Y-%m-%d" ) + L", " +
			obj->get( "min" )->toWString( ) + L" &mdash; " +
			obj->get( "max" )->toWString( ) + L"&nbsp;</th></tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportFillingPeriodHeader

	static void PrintReportLandingPeriodHeader( udPBase obj, FILE* pFileOut ) {
		wstring tmp = L"<tr><th colspan=\"7\">" +
			obj->get( "date1" )->toTime( ).toWString( "%Y-%m-%d" ) +
			L"&nbsp;</th></tr>";
		string szUtf8 = ConvertString( tmp, CP_UTF8 );
		fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
	} // void PrintReportFillingPeriodHeader

	udInt CHReport::MakeExportLandingFile( ) {
		udInt ret = 0;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udPBase obj = NULL;
		udBaseConfig config;
		udArrBase arr;
		obj = new udProjectReportLanding;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_report_landing" );
		option.SetClass( GetProjectReportLanding );
		option.SetWhere( this->GetWhereCondition( ) );
		option.SetOrder( config.GetAttrIndex( "group" ) + ", " + config.GetAttrIndex( "date1" ) );
		config.Clear( );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			FILE *pFileOut = NULL;
			wstring filename = g_pApp->GetConfig( )->get( "workdir" )->toWString( ) + L"\\export\\landing.html";
			if ( _wfopen_s( &pFileOut, filename.data( ), L"wb" ) == 0 ) {
				udInt	place		= 1;
				udUInt	cnt			= 0;
				bool	add_group	= false;
				wstring	tmp			= L"";
				string	szUtf8		= "";
				udArrBase	arr1;	// для выгребания группы

				obj = new udProjectReportGroup;
				obj->GetConfig( config );
				udDELETE( obj );
				PrintReportHead( pFileOut );
				option.Clear( );
				option.SetTable( "ud_report_group" );
				option.SetClass( GetProjectReportGroup );
				udUInt iGroupId = 0, iPeriodId = 0;

				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					add_group = false;
					if ( iGroupId == 0 ) { // начало
						iGroupId = obj->get( "group" )->toUInt( );
						add_group = true;
					} else if ( iGroupId != obj->get( "group" )->toUInt( ) ) { // смена группы
						iGroupId = obj->get( "group" )->toUInt( );
						add_group = true;
						++cnt;
					}
					if ( add_group ) { // выгребаем группу и выплевываем заголовок для таблицы
						iPeriodId = 0;
						if ( cnt > 0 ) {
							tmp = L"</table>";
							szUtf8 = ConvertString( tmp, CP_UTF8 );
							fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
						}
						option.SetWhere( config.GetAttrIndex( "id" ) + "=" + toString( iGroupId ) );
						handler->GetUdObject( arr1, option );
						if ( !arr1.empty( ) ) {
							PrintReportGroupInfo( arr1[ 0 ], pFileOut );
							ClearArrBase( arr1 );
						}
						PrintReportGroupTableHeader( pFileOut );
						place = 1;
					}
					if ( ( iPeriodId == 0 ) ||
						( iPeriodId != obj->get( "period" )->toUInt( ) )
					) {
						iPeriodId = obj->get( "period" )->toUInt( );
						PrintReportLandingPeriodHeader( obj, pFileOut );
					}
					PrintReportGroupTableRow( place++, obj, pFileOut );
				}

				tmp = L"</table></body></html>";
				szUtf8 = ConvertString( tmp, CP_UTF8 );
				fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
				fclose( pFileOut );
				ret = 1;
			} else {
				g_pApp->GetLog( )->Write( "_wfopen_s failed, error=" + toString( errno ) + "\n" );
			}
			ClearArrBase( arr );
		}
		return ret;
	} // udInt MakeExportLandingFile

	udInt CHReport::MakeExportFillingFile( ) {
		udInt ret = 0;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udPBase obj = NULL;
		udBaseConfig config;
		udArrBase arr;
		obj = new udProjectReportFilling;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_report_filling" );
		option.SetClass( GetProjectReportFilling );
		option.SetWhere( this->GetWhereCondition( ) );
		option.SetOrder( config.GetAttrIndex( "group" ) + ", " + config.GetAttrIndex( "date1" ) );
		config.Clear( );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			FILE *pFileOut = NULL;
			wstring filename = g_pApp->GetConfig( )->get( "workdir" )->toWString( ) + L"\\export\\filling.html";
			if ( _wfopen_s( &pFileOut, filename.data( ), L"wb" ) == 0 ) {
				udUInt	cnt			= 0;
				bool	add_group	= false;
				wstring	tmp			= L"";
				string	szUtf8		= "";
				udArrBase	arr1;	// для выгребания группы

				obj = new udProjectReportGroup;
				obj->GetConfig( config );
				udDELETE( obj );
				PrintReportHead( pFileOut );
				option.Clear( );
				option.SetTable( "ud_report_group" );
				option.SetClass( GetProjectReportGroup );
				udUInt iGroupId = 0, iPeriodId = 0;

				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					add_group = false;
					if ( iGroupId == 0 ) { // начало
						iGroupId = obj->get( "group" )->toUInt( );
						add_group = true;
					} else if ( iGroupId != obj->get( "group" )->toUInt( ) ) { // смена группы
						iGroupId = obj->get( "group" )->toUInt( );
						add_group = true;
						++cnt;
					}
					if ( add_group ) { // выгребаем группу и выплевываем заголовок для таблицы
						iPeriodId = 0;
						if ( cnt > 0 ) {
							tmp = L"</table>";
							szUtf8 = ConvertString( tmp, CP_UTF8 );
							fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
						}
						option.SetWhere( config.GetAttrIndex( "id" ) + "=" + toString( iGroupId ) );
						handler->GetUdObject( arr1, option );
						if ( !arr1.empty( ) ) {
							PrintReportGroupInfo( arr1[ 0 ], pFileOut );
							ClearArrBase( arr1 );
						}
						PrintReportGroupTableHeader2( pFileOut );
					}
					if ( ( iPeriodId == 0 ) ||
						( iPeriodId != obj->get( "period" )->toUInt( ) )
					) {
						iPeriodId = obj->get( "period" )->toUInt( );
						PrintReportFillingPeriodHeader( obj, pFileOut );
					}
					PrintReportGroupTableRow2( obj, pFileOut );
				}

				tmp = L"</table></body></html>";
				szUtf8 = ConvertString( tmp, CP_UTF8 );
				fwrite( szUtf8.data( ), 1, szUtf8.size( ), pFileOut );
				fclose( pFileOut );
				ret = 1;
			} else {
				g_pApp->GetLog( )->Write( "_wfopen_s failed, error=" + toString( errno ) + "\n" );
			}
			ClearArrBase( arr );
		}
		return ret;
	} // udInt MakeExportFillingFile

} // namespace UdSDK