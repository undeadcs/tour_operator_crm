
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void ServiceAfterLoadPeriodList( udArrBase& arr ) {
		udPBase obj = NULL;
		datetime date;
		wstring date1, date2;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			date = obj->get( "date1" )->toTime( );
			date1 = date.toWString( "%Y-%m-%d" );
			date = obj->get( "date2" )->toTime( );
			date2 = date.toWString( "%Y-%m-%d" );
			obj->get( "#name" )->ini( date1 + L" - " + date2 + L" i:" +  obj->get( "index" )->toWString( ) );
		}
	} // void ServiceAfterLoadPeriodList


	CHService::CHService( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_service";
		m_szUpdIndex	= "id";
		m_szName		= "service";
		m_fnGetObject	= GetProjectService;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHServiceForm;

		this->AddExtraLoadInfo( ctrlidName, GetProjectNameService, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidType, GetProjectTService, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidCountry, GetProjectCountry, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidCity, GetProjectCity, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidPartner, GetProjectPartner, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidMunit, GetProjectMUService, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidPeriod, GetProjectServicePeriod, ( udInt ) extra_load_frmlst, NULL, ServiceAfterLoadPeriodList );
		this->AddExtraLoadInfo( ctrlidPricetable, GetProjectPriceTableItem, ( udInt ) extra_load_frmlst );

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHService::CHService( const CHService& objCopy ) {
	}

	CHService::~CHService( ) {
		udDELETE( m_lnkPrint );
	}

	CHService& CHService::operator =( const CHService& objCopy ) {
		return *this;
	}

	udInt CHService::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST + 1;
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
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Справочники";
			url = "info";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			lnk->get( "id" )->ini( id++ );
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить услугу" : L"Новая услуга" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "name_", L"Наименование", "name" )->get( "width" )->ini( ( udInt ) 240 );
		wndInsLst->AddAttr( "type_", L"Тип", "type" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "period_", L"Период", "period" )->get( "width" )->ini( ( udInt ) 60 );
		wndInsLst->AddAttr( "partner_", L"Партнер", "partner" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "price_", L"Цена", "price" )->get( "width" )->ini( ( udInt ) 50 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltName, "name", NULL, font, 0 );
		m_wndModeList->AddExtlist( fltName1, "name1", NULL, "name", L"выберите название", GetHNameServiceList );
		m_wndModeList->AddExtlist( fltType, "type", NULL, "name", L"выберите тип", GetHTServiceList );
		m_wndModeList->AddExtlist( fltPartner, "partner", NULL, "name", L"выберите партнера", GetHPartnerList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "name1" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHService::GetListView( ) {
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
			//////////////////////////////////////////////////////////////////////////////////////
			itLnk = arrLink->find( 1 );
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
			itLnk = arrLink->find( 2 );
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
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
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
		info = m_wndModeList->GetInfo( fltName1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltType );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltPartner );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y += 30 + m;
		} else {
			x += box->w + box->padL + box->padR + m;
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

	udInt CHService::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		m_wndForm->AddExtlist(
			ctrlidName, "name",
			NULL, "name", L"выберите наименование", GetHNameServiceList
			//NULL, "name", "pick_name", L"выберите наименование",
			//"ud_nameservice", GetProjectNameService
		);
		m_wndForm->AddExtlist(
			ctrlidType, "type",
			NULL, "name", L"выберите тип", GetHTServiceList
			//NULL, "name", "pick_type", L"выберите тип",
			//"ud_tservice", GetProjectTService
		);
		m_wndForm->AddExtlist(
			ctrlidCountry, "country",
			NULL, "name", L"выберите страну", GetHCountryList
			//NULL, "name", "pick_country", L"выберите страну",
			//"ud_country", GetProjectCountry
		);
		m_wndForm->AddExtlist(
			ctrlidCity, "city",
			NULL, "name", L"выберите город", GetHCityList
			//NULL, "name", "pick_city", L"выберите город",
			//"ud_city", GetProjectCity
		);
		m_wndForm->AddFormlist( ctrlidPeriod, "lperiod", NULL )->AddAttr( "#name" );
		m_wndForm->AddFormlist( ctrlidPricetable, "lprice", NULL )->AddAttr( "#name" );
		m_wndForm->AddExtlist(
			ctrlidPartner, "partner",
			NULL, "name", L"выберите партнера", GetHPartnerList
			//NULL, "name", "pick_partner", L"выберите партнера",
			//"ud_partner", GetProjectPartner
		);
		m_wndForm->AddExtlist(
			ctrlidMunit, "munit",
			NULL, "name", L"выберите ед. измерения", GetHMUServiceList
			//NULL, "name", "pick_munit", L"выберите ед. измерения",
			//"ud_muservice", GetProjectMUService
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHService::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkPrint->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrint->get( "x" )->ini( ( udInt ) 300 );
			m_lnkPrint->get( "y" )->ini( ( udInt ) 10 );
			m_lnkPrint->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrint->SetTargetWnd( m_wndForm->GetHWndParent( ) );
		}
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
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 ),	// шрифт заголовка
					fntButton	= g_pApp->GetFont( L"Tahoma", 14 );	// шрифт кнопки
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		RectF		rctLayout( ( float ) x, ( float ) y, ( float ) w, ( float ) h ),
					rctBound;
		Pen			penBorder( Color::MakeARGB( 255, 180, 180, 180 ) ),
					penBorderBound( Color::MakeARGB( 64, 255, 180, 180 ) );
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Услуга";
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
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidName );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidType );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Страна";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidCountry );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Город";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidCity );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Периоды";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidPeriod );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Таблица цен";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidPricetable );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Партнер";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidPartner );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Ед. измерения";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidMunit );
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

	udInt CHService::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите название" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "type" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите тип" );
			return FormHandlerErrorFailed;
		}
		/*if ( obj->get( "country" )->toUInt( ) == 0 ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите страну" );
			return false;
		}*/
		/*if ( obj->get( "city" )->toUInt( ) == 0 ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите город" );
			return false;
		}*/
		if ( obj->get( "partner" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите партнера" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "munit" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите единицу измерения" );
			return FormHandlerErrorFailed;
		}

		//udPDwAscBase period = m_frmlstPeriod->GetItems( );
		/*if ( !period || period->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите периодичность" );
			return false;
		}*/
		//udPDwAscBase pricetable = m_frmlstPriceTable->GetItems( );
		/*if ( !pricetable || pricetable->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите цены" );
			return false;
		}*/
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHService::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidPeriod )->frmlst ) {
			udPFormHandler handler = GetHServicePeriodForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase	pObject	= m_wndForm->GetObj( ),
						add		= new udProjectServicePeriod;
				*( add->get( "service" ) ) = *( pObject->get( "id" ) );
				*( add->get( "date1" ) ) = *( obj->get( "date1" ) );
				*( add->get( "date2" ) ) = *( obj->get( "date2" ) );

				udPDwAscBase periods = m_wndForm->GetInfo( ctrlidPeriod )->frmlst->GetItems( );
				if ( !periods || periods->empty( ) ) {
					add->get( "index" )->ini( ( udUInt ) 1 );
				} else {
					udDwAscBase::iterator i = periods->end( );
					--i;
					add->get( "index" )->ini( i->second->get( "index" )->toUInt( ) + 1 );
				}

				wstring date1 = L"", date2 = L"";
				udDatetime date = obj->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );

				add->get( "#name" )->ini(
					date1 + L" - " + date2 + L" i:" + add->get( "index" )->toWString( )
				);
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidPricetable )->frmlst ) {
			udPFormHandler handler = GetHServicePriceForm( );
			handler->GetFrmOpt( )->get( "wndForm" )->ini( ( udDWord ) m_wndForm );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase pObject	= m_wndForm->GetObj( ),
						add		= new udProjectPriceTableItem;
				obj->apply( add );
				*( add->get( "service" ) ) = *( pObject->get( "id" ) );
				udDWord periodIndex = add->get( "period" )->toDWord( );
				udPDwAscBase period = m_wndForm->GetInfo( ctrlidPeriod )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = period->begin( ); i != period->end( ); ++i ) {
					if ( i->second->get( "index" )->toDWord( ) == periodIndex ) {
						add->get( "#name" )->ini(
							i->second->get( "#name" )->toWString( ) + L" n:" +
							add->get( "num" )->toWString( ) + L" s:" + add->get( "amount" )->toWString( )
						);
						break;
					}
				}
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		}
	} // void FormListOnAdd

	void CHService::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidPeriod )->frmlst ) {
			udPBase tmp = new udProjectServicePeriod;
			obj->apply( tmp );
			udPFormHandler handler = GetHServicePeriodForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
				wstring date1 = L"", date2 = L"";
				udDatetime date = obj1->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj1->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );

				obj->get( "#name" )->ini(
					date1 + L" - " + date2 + L" i:" + obj->get( "index" )->toWString( )
				);
				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidPricetable )->frmlst ) {
			udPBase tmp = new udProjectPriceTableItem;
			obj->apply( tmp );
			udPFormHandler handler = GetHServicePriceForm( );
			handler->GetFrmOpt( )->get( "wndForm" )->ini( ( udDWord ) m_wndForm );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
				udDWord periodIndex = obj->get( "period" )->toDWord( );
				udPDwAscBase period = m_wndForm->GetInfo( ctrlidPeriod )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = period->begin( ); i != period->end( ); ++i ) {
					if ( i->second->get( "index" )->toDWord( ) == periodIndex ) {
						obj->get( "#name" )->ini(
							i->second->get( "#name" )->toWString( ) + L" n:" +
							obj->get( "num" )->toWString( ) + L" s:" + obj->get( "amount" )->toWString( )
						);
						break;
					}
				}
				delete obj1;
			}
			delete handler;
		}
	} // void FormListOnEdit

	bool CHService::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidPeriod )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить период из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectServicePeriod;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			udPDwAscBase pricetable = m_wndForm->GetInfo( ctrlidPricetable )->frmlst->GetItems( );
			if ( pricetable && !pricetable->empty( ) ) {
				udDWord period = obj->get( "index" )->toDWord( );
				udPBase obj1 = NULL, obj2 = NULL;
				udDWord rowId = 0;
				udPWndRows wndRows = m_wndForm->GetInfo( ctrlidPricetable )->frmlst->GetWndRows( );
				map< udDWord, udDWord > toDel;
				map< udDWord, udDWord >::iterator j;
				for( udDwAscBase::iterator i = pricetable->begin( ); i != pricetable->end( ); ++i ) {
					rowId = i->first;
					obj1 = i->second;
					if ( period == obj1->get( "period" )->toDWord( ) ) {
						toDel[ rowId ] = rowId;
						if ( obj1->get( "id" )->toUInt( ) > 0 ) {
							obj2 = new udProjectPriceTableItem;
							obj1->apply( obj2 );
							m_wndForm->GetInfo( ctrlidPricetable )->frmlst->AddToDel( obj2 );
						}
					}
				}
				udDwAscBase::iterator i;
				for( j = toDel.begin( ); j != toDel.end( ); ++j ) {
					rowId = j->second;
					i = pricetable->find( rowId );
					if ( i != pricetable->end( ) ) {
						delete i->second;
						pricetable->erase( i );
						wndRows->DeleteRow( rowId );
					}
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidPricetable )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить цену из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectPriceTableItem;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		}
		return false;
	} // void FormListOnEdit

	void CHService::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "name_" )->ini( wstring( L"" ) );
		tpl->get( "type_" )->ini( wstring( L"" ) );
		tpl->get( "partner_" )->ini( wstring( L"" ) );
		tpl->get( "period_" )->ini( wstring( L"" ) );
		tpl->get( "price_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHService::LoadExtraData( udPSocket sock, udPBase obj, udPWndModeForm wndForm ) {
		udStateHandlerSimple::LoadExtraData( sock, obj, wndForm );

		udPWndFormList	frmlstPrice		= wndForm->GetInfo( ctrlidPricetable )->frmlst,
						frmlstPeriod	= wndForm->GetInfo( ctrlidPeriod )->frmlst;
		udArrBase arr;
		udPBase obj1 = NULL;
		udPDwAscBase items = frmlstPrice->GetItems( );
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			obj1 = new udProjectPriceTableItem;
			i->second->apply( obj1 );
			arr.push_back( obj1 );
		}
		frmlstPrice->Clear( );

		items = frmlstPeriod->GetItems( );
		udPBase obj2 = NULL;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj1 = *i;
			for( udDwAscBase::iterator j = items->begin( ); j != items->end( ); ++j ) {
				obj2 = j->second;
				if ( obj1->get( "period" )->toUInt( ) == obj2->get( "index" )->toUInt( ) ) {
					obj1->get( "#name" )->ini(
						obj2->get( "#name" )->toWString( ) + L" n:" +
						obj1->get( "num" )->toWString( ) + L" s:" + obj1->get( "amount" )->toWString( )
					);
				}
			}
			frmlstPrice->AddItem( obj1 );
		}
		arr.clear( );
	} // void LoadExtraData

	void CHService::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "name1" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHService::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHService::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
				udPMorph decl = NULL;
				udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
				udPBase obj = NULL, obj1 = NULL;
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
				decl->get( "href" )->ini( wstring( L"service_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectService( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHService::ctrlidName )->extlst->extlst;
				node1 = doc->CreateNode( L"name_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHService::ctrlidType )->extlst->extlst;
				node1 = doc->CreateNode( L"type_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHService::ctrlidCountry )->extlst->extlst;
				node1 = doc->CreateNode( L"country_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHService::ctrlidCity )->extlst->extlst;
				node1 = doc->CreateNode( L"city_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHService::ctrlidPartner )->extlst->extlst;
				node1 = doc->CreateNode( L"partner_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHService::ctrlidMunit )->extlst->extlst;
				node1 = doc->CreateNode( L"munit_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"period_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHService::ctrlidPeriod )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					obj1 = GetProjectServicePeriod( );
					obj1->apply( i->second, true, true );
					node1->InsertChild( obj1->toXml( doc ) );
					udDELETE( obj1 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"price_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHService::ctrlidPricetable )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					obj1 = GetProjectPriceTableItem( );
					obj1->apply( i->second, true, true );
					node1->InsertChild( obj1->toXml( doc ) );
					udDELETE( obj1 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Услуга" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\service_out.xsl", ( folder + wstring( L"\\service_out.xsl" ) ).data( ), FALSE );
				ExtOpenFile( folder + L"\\out.xml" );
				//////////////////////////////////////////////////////////////////////////////////
				xml.clear( );
				udDELETE( obj );
				udDELETE( doc );
				//////////////////////////////////////////////////////////////////////////////////
			} else {
				udStateHandlerInfo::LinkClick( lnk );
			}
		} else {
			udStateHandlerInfo::LinkClick( lnk );
		}
	} // void LinkClick

	void CHService::GetServicesById( map< udUInt, udUInt > ids, udArrBase& arrResult ) {
		udPSocket sock = BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( m_szName );
			data.add( ( udInt ) opidGet2 );
			data.add( ( udUInt ) 1 );//m_pPager->GetPage( ) );
			data.add( ( udUInt ) 1000 );//m_pPager->GetPageSize( ) );
			//data.add( m_szOrder );
			data.add( m_szOrderBy );
			data.add( m_iOrderType );
			// добавляем набор ID клиентов
			data.add( ids.size( ) );
			for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
				data.add( i->first );
			}

			udPMorph filter = new udMorph;
			filter->get( "name" )->ini( wstring( L"" ) );
			filter->get( "name1" )->ini( ( udUInt ) 0 );
			filter->get( "type" )->ini( ( udUInt ) 0 );
			filter->get( "partner" )->ini( ( udUInt ) 0 );
			filter->dump( data );
			udDELETE( filter );

			/*string log = "\nфильтр, ";
			m_wndModeList->GetObjFilter( )->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_wndModeList->GetObjFilter( )->dump( data );*/

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udType total( ( udUInt ) 0 );
				total.read( sock );
				m_pPager->SetTotal( total.toUInt( ) );
				udPBase tpl = m_fnGetObject( );
				this->AddExtraAttributes( tpl );
				ReadInstances( sock, arrResult, m_fnGetObject, tpl );
				delete tpl;
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
	} // void GetServicesById




	CHServicePeriod::CHServicePeriod( ) : udListHandler( ), udFormHandler( ) {
		m_szOrderBy		= "name";
	}

	CHServicePeriod::~CHServicePeriod( ) {
	}

	void CHServicePeriod::LoadList( ) {
		// тут необходимо обращаться к форме услуги и от туда выгребать периоды
		if ( m_pLstOpt->isset( "wndForm" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pLstOpt->get( "wndForm" )->toDWord( );
			ModeFormControlInfo *info = wndForm->GetInfo( udHService::ctrlidPeriod );
			udPDwAscBase periods = info->frmlst->GetItems( );
			if ( periods && !periods->empty( ) ) {
				udPBase obj = NULL, obj1 = NULL;
				for( udDwAscBase::iterator i = periods->begin( ); i != periods->end( ); ++i ) {
					obj = i->second;
					obj1 = new udProjectServicePeriod;
					obj->apply( obj1 );
					m_arrObject.push_back( obj1 );
				}
			}
		}
		//
		udPWndInstanceList wndInslst = m_wndModeList->GetList( );
		assert( wndInslst );
		wndInslst->GenerateRows( &m_arrObject );
		udPWndPager wndPager = m_wndModeList->GetPager( );
		assert( wndPager );
		wndPager->Generate( m_pPager );
		udInt y = wndInslst->get( "y" )->toInt( ) + wndInslst->get( "height" )->toInt( ) + 5;
		wndPager->get( "y" )->ini( y );
		wndPager->Move( );
		// модальный режим
		if ( m_wndModeList->IsModal( ) ) {
			udInt x = 32, m = 5;
			HWND hWndDialog = m_wndModeList->GetHWndParent( );
			udPWndButton btn = m_wndModeList->GetButtonOk( );
			y += wndPager->get( "height" )->toInt( ) + m;
			btn->get( "x" )->ini( x );
			btn->get( "y" )->ini( y );
			btn->Move( );
			x += btn->get( "width" )->toInt( ) + m;
			btn = m_wndModeList->GetButtonCancel( );
			btn->get( "x" )->ini( x );
			btn->get( "y" )->ini( y );
			btn->Move( );
			// перерисовываем основное окно
			udInt	prev_w	= m_wndModeList->get( "width" )->toInt( ),
					prev_h	= m_wndModeList->get( "height" )->toInt( ),
					w		= wndInslst->get( "width" )->toInt( ) + m * 2,
					h		= btn->get( "y" )->toInt( ) + btn->get( "height" )->toInt( ) + m * 2;
			m_wndModeList->get( "width" )->ini( w );
			m_wndModeList->get( "height" )->ini( h );
			m_wndModeList->RemakeBuffer( prev_w, prev_h, w, h );
			m_wndModeList->Move( );
			// двигаем диалоговое окно
			SetWindowPos( hWndDialog, NULL, 0, 0,
				m_wndModeList->get( "width" )->toInt( ),
				m_wndModeList->get( "height" )->toInt( ),
				SWP_NOMOVE
			);
			CenterOneWindowAboveParent( hWndDialog );
		}
	} // void LoadList

	udInt CHServicePeriod::GetListModel( ) {
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= 32001;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		/*if ( !modal ) {
			title = L"На главную";
			url = "main";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = L"Добавить период";
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );*/
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "#name", L"Наименование" )->get( "width" )->ini( ( udInt ) 400 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHServicePeriod::GetListView( ) {
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
		/*if ( !modal ) {
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
		lnk->CalculateWidthHeight( hWnd );*/
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

	udInt CHServicePeriod::ListOk( ) {
		assert( m_wndModeList );
		if ( m_wndModeList->IsModal( ) ) {
			::PostMessage( m_wndModeList->GetHWndParent( ), WM_COMMAND, MAKEWPARAM( IDOK, 0 ), ( LPARAM ) m_wndModeList );
		}
		return ListHandlerErrorOk;
	} // udInt ListOk

	udInt CHServicePeriod::ListCancel( ) {
		assert( m_wndModeList );
		if ( m_wndModeList->IsModal( ) ) {
			::PostMessage( m_wndModeList->GetHWndParent( ), WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), ( LPARAM ) m_wndModeList );
		}
		return ListHandlerErrorOk;
	} // udInt ListCancel

	void CHServicePeriod::OnLinkClick( udPWndLink lnk ) {
	} // void OnLinkClick

	void CHServicePeriod::OnSelectRow( udLong row ) {
		assert( m_wndModeList );
		m_wndModeList->GetList( )->SelectRow( row );
	} // void OnSelectRow

	void CHServicePeriod::OnHeaderClick( const string& name, const string& attr ) {
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

	void CHServicePeriod::OnPagerSelect( udUInt page ) {
		this->ClearList( );
		m_pPager->SetPage( page );
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnPagerSelect

	void CHServicePeriod::OnButtonClick( udPWndButton btn ) {
	} // void OnButtonClick

	udInt CHServicePeriod::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHServicePeriod::GetFormView( ) {
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
		title	= L"Период действия услуги";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
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
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHServicePeriod::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHServicePeriod::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHServicePeriod::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHServicePeriod::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

} // namespace UdSDK
