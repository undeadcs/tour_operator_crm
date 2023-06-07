
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHTourPayOrder::CHTourPayOrder( ) {
		m_szTableName	= "ud_tourpayorder";
		m_szUpdIndex	= "id";
		m_szName		= "tourpayorder";
		m_fnGetObject	= GetProjectTourPayOrder;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHTourPayOrderForm;

		m_wndSumm		= NULL;

		m_lnkEmployer	= new udWndLink;
		m_lnkCompany	= new udWndLink;
		m_lnkPersAcc	= new udWndLink;

		m_lnkEmployer->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lnkCompany->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lnkPersAcc->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lnkEmployer->get( "title" )->ini( wstring( L"заказчик" ) );
		m_lnkCompany->get( "title" )->ini( wstring( L"компания" ) );
		m_lnkPersAcc->get( "title" )->ini( wstring( L"лицевой счет" ) );

		m_pTour		= NULL;
		m_pEmployer	= NULL;
		m_pCompany	= NULL;
		m_pPersAcc	= NULL;
	}

	CHTourPayOrder::CHTourPayOrder( const CHTourPayOrder& objCopy ) {
	}

	CHTourPayOrder::~CHTourPayOrder( ) {
		udDELETE( m_lnkEmployer );
		udDELETE( m_lnkCompany );
		udDELETE( m_lnkPersAcc );
		udDELETE( m_pTour );
		udDELETE( m_pEmployer );
		udDELETE( m_pCompany );
		udDELETE( m_pPersAcc );
	}

	CHTourPayOrder& CHTourPayOrder::operator =( const CHTourPayOrder& objCopy ) {
		return *this;
	}

	udInt CHTourPayOrder::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !modal ) {
			title = L"На главную";
			url = "main";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			lnk->get( "id" )->ini( id++ );
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
		/*title = ( modal ? L"Добавить город" : L"Новый город" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );*/
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "created_", L"Дата", "created" )->get( "width" )->ini( ( udInt ) 200 );
		wndInsLst->AddAttr( "tour_", L"Тур", "tour" )->get( "width" )->ini( ( udInt ) 200 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltTour, "tour", NULL, "name", L"выберите тур", GetHTourList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		datetime date;
		date.load( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( date );
		filter->get( "country" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHTourPayOrder::GetListView( ) {
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
		}
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
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltTour );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y += box->h + box->padT + box->padB + m;
		} else {
			x += box->w + box->padL + box->padR + m;
		}
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

	udInt CHTourPayOrder::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		if ( !m_pFrmOpt->isset( "from_tour" ) ) {
			//////////////////////////////////////////////////////////////////////////////////////
			udArrWString arrString;
			arrString.push_back( L"Открыто" );
			arrString.push_back( L"Закрыто" );
			m_wndForm->AddSelect( ctrlidState, "state", NULL, font, arrString );
			arrString.clear( );
			//m_wndForm->AddStatic( ctrlidLabelEmployer, NULL, L"заказчик", 12 );
			//m_wndForm->AddStatic( ctrlidLabelCompany, NULL, L"компания", 12 );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTourPayOrder::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 620 );
			m_wndForm->get( "height" )->ini( ( udInt ) 620 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
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
		title	= L"Указание на оплату";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !m_pFrmOpt->isset( "from_tour" ) ) {
			//////////////////////////////////////////////////////////////////////////////////////
			if ( !modal ) {
				//////////////////////////////////////////////////////////////////////////////////
				title	= L"Заказчик";
				x		= 32;
				w		= lcol;
				h		= 24;
				gdi->DrawString( title.data( ), title.size( ), fntLabel,
					RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
				);

				x += w + m;

				m_lnkEmployer->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
				m_lnkEmployer->get( "x" )->ini( ( udInt ) x );
				m_lnkEmployer->get( "y" )->ini( ( udInt ) y );
				m_lnkEmployer->Create( m_wndForm->GetHWnd( ) );
				m_lnkEmployer->SetTargetWnd( m_wndForm->GetHWndParent( ) );

				y += h + m;
				//////////////////////////////////////////////////////////////////////////////////
				title	= L"Компания";
				x		= 32;
				w		= lcol;
				h		= 24;
				gdi->DrawString( title.data( ), title.size( ), fntLabel,
					RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
				);

				x += w + m;

				m_lnkCompany->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
				m_lnkCompany->get( "x" )->ini( ( udInt ) x );
				m_lnkCompany->get( "y" )->ini( ( udInt ) y );
				m_lnkCompany->Create( m_wndForm->GetHWnd( ) );
				m_lnkCompany->SetTargetWnd( m_wndForm->GetHWndParent( ) );

				y += h + m;
				//////////////////////////////////////////////////////////////////////////////////
				title	= L"Лицевой счет";
				x		= 32;
				w		= lcol;
				h		= 24;
				gdi->DrawString( title.data( ), title.size( ), fntLabel,
					RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
				);

				x += w + m;

				m_lnkPersAcc->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
				m_lnkPersAcc->get( "x" )->ini( ( udInt ) x );
				m_lnkPersAcc->get( "y" )->ini( ( udInt ) y );
				m_lnkPersAcc->Create( m_wndForm->GetHWnd( ) );
				m_lnkPersAcc->SetTargetWnd( m_wndForm->GetHWndParent( ) );

				y += h + m;
				//////////////////////////////////////////////////////////////////////////////////
			}
			//////////////////////////////////////////////////////////////////////////////////////
			title	= L"Состояние";
			x		= 32;
			w		= lcol;
			h		= 24;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);

			x += w + m;

			box = new ModeFormControlBox;
			box->x = x;	box->y = y;	box->w = 200; box->h = 22;
			box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
			box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
			info = m_wndForm->GetInfo( ctrlidState );
			info->box = box;
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_wndSumm ) {
		} else {
			m_wndSumm = new udWndSumm( this );
			m_wndSumm->get( "x" )->ini( ( udInt ) x );
			m_wndSumm->get( "y" )->ini( ( udInt ) y );
			m_wndSumm->get( "width" )->ini( ( udInt ) 400 );
			m_wndSumm->get( "height" )->ini( ( udInt ) 400 );
			m_wndSumm->get( "style" )->ini( ( udDWord ) WS_CHILD | WS_VISIBLE );
			m_wndSumm->Create( m_wndForm->GetHWnd( ) );
		}
		y += 510;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHTourPayOrder::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );

		if ( m_pFrmOpt->isset( "from_tour" ) ) {
			udPBase tour = ( udPBase ) m_pFrmOpt->get( "from_tour" )->toDWord( );
			if ( tour ) {
				obj->get( "tour" )->ini( tour->get( "id" )->toUInt( ) );
			}
		}

		datetime date;
		date.load( );
		obj->get( "created" )->ini( date );
		
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHTourPayOrder::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "tour_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHTourPayOrder::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		datetime date;
		date.load( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( date );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHTourPayOrder::ExtraDataSending( udPSocket sock, udPBase obj ) {
		const udDwAscBase& rows = m_wndSumm->GetRows( );
		udBinaryData data;
		char code = 0;
		udPBase to = NULL;
		data.add( ( char ) m_pFrmOpt->isset( "from_tour" ) );
		data.add( rows.size( ) );
		for( udDwAscBase::const_iterator i = rows.begin( ); i != rows.end( ); ++i ) {
			to = GetProjectFinanceSumm( );
			to->apply( i->second, true, true );
			to->dump( data );
			udDELETE( to );
		}
		// отправляем данные
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		// ждем подтверждение приема
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
	} // void ExtraDataSending

	void CHTourPayOrder::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
		g_pApp->GetLog( )->Write( "CHTourPayOrder::ExtraDataRecieve, enter\n" );
		udArrBase arr;
		udPBase tpl = GetProjectFinanceSumm( );
		tpl->get( "currency_" )->ini( wstring( L"" ) );
		tpl->get( "type_" )->ini( wstring( L"" ) );
		// считываем суммы
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
		// отправляем подтверждающий байт
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
		// обновляем окно сумм
		assert( m_wndSumm );
		m_wndSumm->ClearRows( );
		m_wndSumm->AddRow( arr );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		udDELETE( m_pTour );
		udDELETE( m_pEmployer );
		udDELETE( m_pCompany );
		udDELETE( m_pPersAcc );
		//////////////////////////////////////////////////////////////////////////////////////////
		// тур
		string log = "\n";
		g_pApp->GetLog( )->Write( "считываем тур\n" );
		m_pTour = GetProjectTour( );
		m_pTour->read( sock );
		m_pTour->dump( log );
		log += "\n";
		//////////////////////////////////////////////////////////////////////////////////////////
		// заказчик
		if ( m_pTour->get( "emptype" )->toInt( ) ==  udProjectTour::TOUR_EMPLOYER_CLIENT ) {
			m_pEmployer = GetProjectClient( );
		} else {
			m_pEmployer = GetProjectClientUr( );
		}
		g_pApp->GetLog( )->Write( "считываем заказчика\n" );
		m_pEmployer->read( sock );
		m_pEmployer->dump( log );
		log += "\n";
		//////////////////////////////////////////////////////////////////////////////////////////
		// компания
		g_pApp->GetLog( )->Write( "считываем компанию\n" );
		m_pCompany = GetProjectCompany( );
		m_pCompany->read( sock );
		m_pCompany->dump( log );
		log += "\n";
		//////////////////////////////////////////////////////////////////////////////////////////
		// лицевой счет
		m_pPersAcc = GetProjectPersAcc( );
		g_pApp->GetLog( )->Write( "считываем лицевой счет\n" );
		m_pPersAcc->read( sock );
		m_pPersAcc->dump( log );
		log += "\n";
		//////////////////////////////////////////////////////////////////////////////////////////
		// отправляем подтверждающий байт
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pTour->get( "emptype" )->toInt( ) ==  udProjectTour::TOUR_EMPLOYER_CLIENT ) {
			m_lnkEmployer->get( "title" )->ini( m_pEmployer->get( "fio" )->toWString( ) );
			m_lnkPersAcc->get( "title" )->ini( m_pEmployer->get( "fio" )->toWString( ) + L" - " + m_pCompany->get( "name" )->toWString( ) );
		} else {
			m_lnkEmployer->get( "title" )->ini( m_pEmployer->get( "name" )->toWString( ) );
			m_lnkPersAcc->get( "title" )->ini( m_pEmployer->get( "name" )->toWString( ) + L" - " + m_pCompany->get( "name" )->toWString( ) );
		}
		m_lnkCompany->get( "title" )->ini( m_pCompany->get( "name" )->toWString( ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt prev_w = 0, prev_h = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		prev_w = m_lnkEmployer->get( "width" )->toInt( );
		prev_h = m_lnkEmployer->get( "height" )->toInt( );
		m_lnkEmployer->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
		m_lnkEmployer->RemakeBuffer( prev_w, prev_h, m_lnkEmployer->get( "width" )->toInt( ), m_lnkEmployer->get( "height" )->toInt( ) );
		m_lnkEmployer->Move( );
		m_lnkEmployer->Render( );
		m_lnkEmployer->Flush( );
		//////////////////////////////////////////////////////////////////////////////////////////
		prev_w = m_lnkCompany->get( "width" )->toInt( );
		prev_h = m_lnkCompany->get( "height" )->toInt( );
		m_lnkCompany->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
		m_lnkCompany->RemakeBuffer( prev_w, prev_h, m_lnkCompany->get( "width" )->toInt( ), m_lnkCompany->get( "height" )->toInt( ) );
		m_lnkCompany->Move( );
		m_lnkCompany->Render( );
		m_lnkCompany->Flush( );
		//////////////////////////////////////////////////////////////////////////////////////////
		prev_w = m_lnkPersAcc->get( "width" )->toInt( );
		prev_h = m_lnkPersAcc->get( "height" )->toInt( );
		m_lnkPersAcc->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
		m_lnkPersAcc->RemakeBuffer( prev_w, prev_h, m_lnkPersAcc->get( "width" )->toInt( ), m_lnkPersAcc->get( "height" )->toInt( ) );
		m_lnkPersAcc->Move( );
		m_lnkPersAcc->Render( );
		m_lnkPersAcc->Flush( );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHTourPayOrder::ExtraDataRecieve, exit\n" );
	} // void ExtraDataRecieve

	void CHTourPayOrder::RequestList( ) {
		udStateHandlerSimple::RequestList( );
		datetime date;
		udPBase obj = NULL;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			date = obj->get( "created" )->toTime( );
			obj->get( "created_" )->ini( date.toWString( "%d.%m.%Y" ) );
		}
	} // void RequestList

	/*void CHTourPayOrder::OnSelectRow( udLong row ) {
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		if ( wndInsLst ) {
			wndInsLst->SelectRow( row );
			if ( !m_wndModeList->IsModal( ) ) {
				udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
				tbr->EnableItem( "edit"		);
				tbr->EnableItem( "delete"	);
			}
		}
	} // void OnSelectRow*/

	void CHTourPayOrder::AfterConstruct( udPWndSumm wndSumm ) {
		wndSumm->GetLnkAdd( )->get( "title" )->ini( wstring( L"добавить" ) );
		wndSumm->GetLnkBack( )->get( "title" )->ini( wstring( L"снять" ) );
	} // void AfterConstruct

	void CHTourPayOrder::OnSummAdd( udPWndSumm wndSumm ) {
		udPFormHandler handler = GetHSummAddForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udArrBase arr;
			const udDwAscBase& rows = wndSumm->GetRows( );
			udUInt iRowNum = rows.size( );
			if ( iRowNum == 0 ) { // сумм нет. добавим
				udPBase add = new udProjectFinanceSumm;
				obj->apply( add );
				arr.push_back( add );
			} else {
				udPBase obj1 = NULL;
				bool found = false;
				udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
				udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
				udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
				for( udDwAscBase::const_iterator i = rows.begin( ); i != rows.end( ); ++i ) {
					obj1 = i->second;
					c1 = obj1->get( "currency" )->toUInt( );
					t1 = obj1->get( "type" )->toInt( );
					if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
						s1 = obj1->get( "summ" )->toFloat( );
						obj1->get( "summ" )->ini( s1 + s2 );
						found = true;
						break;
					}
					obj1 = new udProjectFinanceSumm;
					i->second->apply( obj1 );
					arr.push_back( obj1 );
				}
				if ( !found ) { // если найдена, то обновляем
					udPBase add = new udProjectFinanceSumm;
					obj->apply( add );
					arr.push_back( add );
				}
			}
			wndSumm->ClearRows( );
			wndSumm->AddRow( arr );
			delete obj;
		}
		delete handler;
	} // void OnSummAdd

	void CHTourPayOrder::OnSummBack( udPWndSumm wndSumm ) {
		udPFormHandler handler = GetHSummBackForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udArrBase arr;
			const udDwAscBase& rows = wndSumm->GetRows( );
			udUInt iRowNum = rows.size( );
			if ( iRowNum == 0 ) {
			} else {
				udPBase pFound = NULL, obj1 = NULL;
				udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
				udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
				udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
				for( udDwAscBase::const_iterator i = rows.begin( ); i != rows.end( ); ++i ) {
					obj1 = i->second;
					c1 = obj1->get( "currency" )->toUInt( );
					t1 = obj1->get( "type" )->toInt( );
					obj1 = new udProjectFinanceSumm;
					i->second->apply( obj1 );
					arr.push_back( obj1 );
					if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
						pFound = obj1;
						break;
					}
				}
				obj = NULL;
				if ( pFound ) {
					s1 = pFound->get( "summ" )->toFloat( );
					if ( s1 < s2 ) {
					} else {
						pFound->get( "summ" )->ini( s1 - s2 );
					}
				}
			}
			wndSumm->ClearRows( );
			wndSumm->AddRow( arr );
			delete obj;
		}
		delete handler;
	} // void OnSummBack

	void CHTourPayOrder::OnSummConvert( udPWndSumm wndSumm ) {
		udPFormHandler handler = GetHSummConvertForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			delete obj;
		}
		delete handler;
	} // void OnSummConvert

	void CHTourPayOrder::AfterCreate( udPWndSumm wndSumm ) {
		if ( !m_pFrmOpt->isset( "from_tour" ) ) {
			wndSumm->GetLnkAdd( )->Hide( );
			wndSumm->GetLnkBack( )->Hide( );
		}
		wndSumm->GetLnkConvert( )->Hide( );
	} // void AfterCreate

	/*udInt CHTourPayOrder::ProcForm( bool modal ) {
		if ( m_pFrmOpt->isset( "from_tour" ) ) {
			//////////////////////////////////////////////////////////////////////////////////////////
			// подгрузка данных формы
			assert( m_wndForm );
			m_wndForm->LoadControlData( );
			//////////////////////////////////////////////////////////////////////////////////////////
			// проверка данных формы
			udInt err = ValidateForm( );
			if ( err != FormHandlerErrorOk ) {
				return err;
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			return FormHandlerErrorOk;
		}
		return udStateHandlerSimple::ProcForm( modal );
	} // udInt ProcForm*/

	void CHTourPayOrder::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkEmployer ) {
				string szHandlerName = "";
				if ( m_pTour->get( "emptype" )->toInt( ) ==  udProjectTour::TOUR_EMPLOYER_CLIENT ) {
					szHandlerName = "client";
				} else {
					szHandlerName = "clientur";
				}
				g_pApp->GetModel( )->SetState( szHandlerName );
				udPStateHandlerSimple shandler = ( udPStateHandlerSimple ) g_pApp->GetModel( )->GetHandler( szHandlerName );
				udPAppConfig appConfig = g_pApp->GetConfig( );
				udLong		sel	= m_wndModeList->GetList( )->GetSelected( );
				udPWndModel	wnd	= g_pApp->GetModel( )->GetWndModel( );
				pfnGetObject	fnGetObject = shandler->GetObjectCallback( );
				udPBase		tmp	= m_pEmployer,
							obj	= fnGetObject( );
				bool		watch_mode = false; // режим просмотра, редактирование невозможно
				
				tmp->apply( obj );
				shandler->Clear( );
				shandler->SetInternalState( udStateHandlerSimple::INTSTATE_FORM );
				shandler->Load( );
				
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					udBinaryData data;
					data.add( shandler->GetHandlerName( ) );
					data.add( ( udInt ) opidEdt );
					shandler->PackForEdt( data, obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					char code = 0;
					sock->ReadByte( &code );
					if ( code > 0 ) {
						if ( code == 2 ) {
							watch_mode = true;
						}
						shandler->ReadForEdit( sock, obj );
						shandler->LoadExtraData( sock, obj, shandler->GetWndForm( ) );
						shandler->ExtraDataRecieve( sock, obj );
					}
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				} else {
					g_pApp->GetLog( )->Write( "не удалось подсоединиться" );
				}

				shandler->GetWndForm( )->SetIsWatch( watch_mode );
				shandler->GetWndForm( )->SetObj( obj );
				shandler->Show( wnd );
			} else if ( lnk == m_lnkCompany ) {
				string szHandlerName = "company";
				g_pApp->GetModel( )->SetState( szHandlerName );
				udPStateHandlerSimple shandler = ( udPStateHandlerSimple ) g_pApp->GetModel( )->GetHandler( szHandlerName );
				udPAppConfig appConfig = g_pApp->GetConfig( );
				udLong		sel	= m_wndModeList->GetList( )->GetSelected( );
				udPWndModel	wnd	= g_pApp->GetModel( )->GetWndModel( );
				pfnGetObject	fnGetObject = shandler->GetObjectCallback( );
				udPBase		tmp	= m_pCompany,
							obj	= fnGetObject( );
				bool		watch_mode = false; // режим просмотра, редактирование невозможно
				
				tmp->apply( obj );
				shandler->Clear( );
				shandler->SetInternalState( udStateHandlerSimple::INTSTATE_FORM );
				shandler->Load( );
				
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					udBinaryData data;
					data.add( shandler->GetHandlerName( ) );
					data.add( ( udInt ) opidEdt );
					shandler->PackForEdt( data, obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					char code = 0;
					sock->ReadByte( &code );
					if ( code > 0 ) {
						if ( code == 2 ) {
							watch_mode = true;
						}
						shandler->ReadForEdit( sock, obj );
						shandler->LoadExtraData( sock, obj, shandler->GetWndForm( ) );
						shandler->ExtraDataRecieve( sock, obj );
					}
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				} else {
					g_pApp->GetLog( )->Write( "не удалось подсоединиться" );
				}

				shandler->GetWndForm( )->SetIsWatch( watch_mode );
				shandler->GetWndForm( )->SetObj( obj );
				shandler->Show( wnd );
			} else if ( lnk == m_lnkPersAcc ) {
				string szHandlerName = "persacc";
				g_pApp->GetModel( )->SetState( szHandlerName );
				udPStateHandlerSimple shandler = ( udPStateHandlerSimple ) g_pApp->GetModel( )->GetHandler( szHandlerName );
				udPAppConfig appConfig = g_pApp->GetConfig( );
				udLong		sel	= m_wndModeList->GetList( )->GetSelected( );
				udPWndModel	wnd	= g_pApp->GetModel( )->GetWndModel( );
				pfnGetObject	fnGetObject = shandler->GetObjectCallback( );
				udPBase		tmp	= m_pPersAcc,
							obj	= fnGetObject( );
				bool		watch_mode = false; // режим просмотра, редактирование невозможно
				
				tmp->apply( obj );
				shandler->Clear( );
				shandler->SetInternalState( udStateHandlerSimple::INTSTATE_FORM );
				shandler->Load( );
				
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					udBinaryData data;
					data.add( shandler->GetHandlerName( ) );
					data.add( ( udInt ) opidEdt );
					shandler->PackForEdt( data, obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					char code = 0;
					sock->ReadByte( &code );
					if ( code > 0 ) {
						if ( code == 2 ) {
							watch_mode = true;
						}
						shandler->ReadForEdit( sock, obj );
						shandler->LoadExtraData( sock, obj, shandler->GetWndForm( ) );
						shandler->ExtraDataRecieve( sock, obj );
					}
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				} else {
					g_pApp->GetLog( )->Write( "не удалось подсоединиться" );
				}

				shandler->GetWndForm( )->SetIsWatch( watch_mode );
				shandler->GetWndForm( )->SetObj( obj );
				shandler->Show( wnd );
			}
		} else {
			udStateHandlerSimple::LinkClick( lnk );
		}
	} // void LinkClick

	void CHTourPayOrder::SwitchToForm( ) {
		this->Clear( );
		m_iState = INTSTATE_FORM;
		this->Load( );
		// тут устанавливаем тексты для ссылок
		this->Show( g_pApp->GetModel( )->GetWndModel( ) );
	} // void SwitchToForm

} // namespace UdSDK
