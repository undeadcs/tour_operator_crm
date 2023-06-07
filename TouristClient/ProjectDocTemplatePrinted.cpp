
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHDocTemplatePrinted::CHDocTemplatePrinted( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_doctplprinted";
		m_szUpdIndex	= "id";
		m_szName		= "doctplprinted";
		m_fnGetObject	= GetProjectDocTemplatePrinted;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHDocTemplatePrintedForm;
		//
		m_pTour			= NULL;
		m_pUser			= NULL;
		m_bCreated		= false;
		m_lblCreated	= new udWndStatic;
		m_lblCreatedBy	= new udWndStatic;
		m_lblTour		= new udWndStatic;
		m_lblSeries		= new udWndStatic;
		m_lblNumber		= new udWndStatic;
		m_lblName		= new udWndStatic;
		m_lblType		= new udWndStatic;
		m_szDir			= L"";
		//
		m_lblCreated->get( "title" )->ini( wstring( L"" ) );
		m_lblCreated->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lblCreatedBy->get( "title" )->ini( wstring( L"" ) );
		m_lblCreatedBy->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lblTour->get( "title" )->ini( wstring( L"" ) );
		m_lblTour->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lblSeries->get( "title" )->ini( wstring( L"" ) );
		m_lblSeries->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lblNumber->get( "title" )->ini( wstring( L"" ) );
		m_lblNumber->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lblName->get( "title" )->ini( wstring( L"" ) );
		m_lblName->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lblType->get( "title" )->ini( wstring( L"" ) );
		m_lblType->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
	}

	CHDocTemplatePrinted::CHDocTemplatePrinted( const CHDocTemplatePrinted& objCopy ) {
	}

	CHDocTemplatePrinted::~CHDocTemplatePrinted( ) {
		udDELETE( m_pTour );
		udDELETE( m_pUser );
		udDELETE( m_lblCreated );
		udDELETE( m_lblCreatedBy );
		udDELETE( m_lblTour );
		udDELETE( m_lblSeries );
		udDELETE( m_lblNumber );
		udDELETE( m_lblName );
		udDELETE( m_lblType );
	}

	CHDocTemplatePrinted& CHDocTemplatePrinted::operator =( const CHDocTemplatePrinted& objCopy ) {
		return *this;
	}

	udInt CHDocTemplatePrinted::GetListModel( ) {
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
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "series", L"Серия" )->get( "width" )->ini( ( udInt ) 60 );
		wndInsLst->AddAttr( "number", L"Номер" )->get( "width" )->ini( ( udInt ) 100 );
		wndInsLst->AddAttr( "tour_", L"Тур", "tour" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "name", L"Наименование шаблона" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "type_", L"Тип", "type" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "createdby_", L"Распечатано", "createdby" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltSeries, "series", NULL, font, 0 );
		m_wndModeList->AddInput( fltNumber, "number", NULL, font, 0 );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltCreatedBy, "createdby", NULL, "fio", L"выберите пользователя", GetHUserList );
		udArrWString arrString;
		arrString.push_back( L"" );
		arrString.push_back( L"Договор" );
		arrString.push_back( L"Путевка" );
		arrString.push_back( L"Гр.договор" );
		m_wndModeList->AddSelect( fltType, "type", NULL, font, arrString );
		m_wndModeList->AddExtlist( fltTour, "tour", NULL, "name", L"выберите тур", GetHTourList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "createdby" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHDocTemplatePrinted::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 60; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltSeries );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 100; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltNumber );
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
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCreatedBy );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 186; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltType );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += 30 + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		//x += box->w + box->padL + box->padR + m;
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

	udInt CHDocTemplatePrinted::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		m_wndForm->SetIsWatch( true );
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHDocTemplatePrinted::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 180 );
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
		title	= L"Распечатанный документ";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !m_bCreated ) {
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Распечатан";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblCreated->get( "x" )->ini( x );
			m_lblCreated->get( "y" )->ini( y );
			m_lblCreated->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblCreated->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Менеджер";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblCreatedBy->get( "x" )->ini( x );
			m_lblCreatedBy->get( "y" )->ini( y );
			m_lblCreatedBy->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblCreatedBy->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Тур";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblTour->get( "x" )->ini( x );
			m_lblTour->get( "y" )->ini( y );
			m_lblTour->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblTour->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Серия";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblSeries->get( "x" )->ini( x );
			m_lblSeries->get( "y" )->ini( y );
			m_lblSeries->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblSeries->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Номер";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblNumber->get( "x" )->ini( x );
			m_lblNumber->get( "y" )->ini( y );
			m_lblNumber->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblNumber->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Имя шаблона";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblName->get( "x" )->ini( x );
			m_lblName->get( "y" )->ini( y );
			m_lblName->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblName->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Тип шаблона";
			x = 32;
			w = lcol;
			h = 21;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);
			x += w + m;
			m_lblType->get( "x" )->ini( x );
			m_lblType->get( "y" )->ini( y );
			m_lblType->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblType->Create( m_wndForm->GetHWnd( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			m_lnkPrint->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lnkPrint->get( "x" )->ini( x );
			m_lnkPrint->get( "y" )->ini( y );
			m_lnkPrint->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrint->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////
			m_bCreated = true;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHDocTemplatePrinted::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHDocTemplatePrinted::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "createdby_" )->ini( wstring( L"" ) );
		tpl->get( "tour_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHDocTemplatePrinted::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "createdby" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHDocTemplatePrinted::AfterLoadFilterData( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		udInt type = filter->get( "type" )->toInt( );
		filter->get( "type" )->ini( type - 1 );
	} // void AfterLoadFilterData

	void CHDocTemplatePrinted::RequestList( ) {
		udStateHandlerSimple::RequestList( );
		udInt type = 0;
		udPBase obj = NULL;
		map< udInt, wstring > mapType;
		mapType[ udProjectDocTemplate::tplClassic	] = L"";
		mapType[ udProjectDocTemplate::tplContract	] = L"Договор";
		mapType[ udProjectDocTemplate::tplPermit	] = L"Путевка";
		mapType[ udProjectDocTemplate::tplContract2	] = L"Гр.договор";
		map< udInt, wstring >::iterator itType;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			type = obj->get( "type" )->toInt( );
			itType = mapType.find( type );
			if ( itType == mapType.end( ) ) {
				obj->get( "type_" )->ini( wstring( L"" ) );
			} else {
				obj->get( "type_" )->ini( itType->second );
			}
		}
	} // void RequestList

	void CHDocTemplatePrinted::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		/*udPWndFilePick filepick = m_wndForm->GetInfo( ctrlidText )->filepick;
		udUInt num = filepick->GetFile( ).empty( ) ? 0 : 1;
		udBinaryData data;
		data.add( ( char ) 80 );
		data.add( num );
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( num == 1 ) { // отправляем файл
			udFileTransfer objFileTransfer;
			udInt err = 0;
			string log = "";
			objFileTransfer.SetFileName( filepick->GetFile( ) );
			objFileTransfer.SetFilePath( filepick->GetFolder( ) + L"\\" + filepick->GetFile( ) );
			err = objFileTransfer.Send( sock );
			log += udFileTransfer::ErrorExplain( err ) + "\n";
			g_pApp->GetLog( )->Write( log );
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	void CHDocTemplatePrinted::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		char code = 0;
		sock->ReadByte( &code );
		if ( code > 0 ) {
			string log = "\n";
			udPBase obj1 = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			udDELETE( m_pTour );
			udDELETE( m_pUser );
			//////////////////////////////////////////////////////////////////////////////////////
			// тур
			obj1 = GetProjectTour( );
			obj1->read( sock );
			m_pTour = obj1;
			m_pTour->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			// пользователь
			obj1 = GetProjectUser( );
			obj1->read( sock );
			m_pUser = obj1;
			obj1 = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			udUInt cnt = 0;
			wstring tmp_dir = g_pApp->GetUniqueTmpFolder( );
			udInt err = 0;
			udFileTransfer objFileTransfer;
			m_szDir = tmp_dir;
			DirectoryClear( tmp_dir );
			//////////////////////////////////////////////////////////////////////////////////////
			// файл шаблона
			objFileTransfer.SetTmpDir( tmp_dir );
			err = objFileTransfer.Recv( sock );
			log = udFileTransfer::ErrorExplain( err ) + "\n";
			__log->Write( log );
			//////////////////////////////////////////////////////////////////////////////////////
			// файл результата для шаблона
			objFileTransfer.SetTmpDir( tmp_dir );
			err = objFileTransfer.Recv( sock );
			log = udFileTransfer::ErrorExplain( err ) + "\n";
			__log->Write( log );
			//////////////////////////////////////////////////////////////////////////////////////
		} else {
			__log->Write( "файл шаблона отсутствует на сервере\n" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_bCreated ) {
			udInt prev_w = 0, prev_h = 0;
			//////////////////////////////////////////////////////////////////////////////////////
			// дата распечатки
			prev_w = m_lblCreated->get( "width" )->toInt( );
			prev_h = m_lblCreated->get( "height" )->toInt( );
			m_lblCreated->get( "title" )->ini( obj->get( "created" )->toTime( ).toWString( "%d.%m.%Y %H:%M:%S" ) );
			m_lblCreated->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblCreated->RemakeBuffer( prev_w, prev_h, m_lblCreated->get( "width" )->toInt( ), m_lblCreated->get( "height" )->toInt( ) );
			m_lblCreated->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
			// пользователь, который распечатал
			prev_w = m_lblCreatedBy->get( "width" )->toInt( );
			prev_h = m_lblCreatedBy->get( "height" )->toInt( );
			m_lblCreatedBy->get( "title" )->ini( m_pUser->get( "fio" )->toWString( ) );
			m_lblCreatedBy->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblCreatedBy->RemakeBuffer( prev_w, prev_h, m_lblCreatedBy->get( "width" )->toInt( ), m_lblCreatedBy->get( "height" )->toInt( ) );
			m_lblCreatedBy->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
			// тур
			prev_w = m_lblTour->get( "width" )->toInt( );
			prev_h = m_lblTour->get( "height" )->toInt( );
			m_lblTour->get( "title" )->ini( m_pTour->get( "name" )->toWString( ) );
			m_lblTour->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblTour->RemakeBuffer( prev_w, prev_h, m_lblTour->get( "width" )->toInt( ), m_lblTour->get( "height" )->toInt( ) );
			m_lblTour->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
			// серия
			prev_w = m_lblSeries->get( "width" )->toInt( );
			prev_h = m_lblSeries->get( "height" )->toInt( );
			m_lblSeries->get( "title" )->ini( obj->get( "series" )->toWString( ) );
			m_lblSeries->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblSeries->RemakeBuffer( prev_w, prev_h, m_lblSeries->get( "width" )->toInt( ), m_lblSeries->get( "height" )->toInt( ) );
			m_lblSeries->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
			// номер
			prev_w = m_lblNumber->get( "width" )->toInt( );
			prev_h = m_lblNumber->get( "height" )->toInt( );
			m_lblNumber->get( "title" )->ini( obj->get( "number" )->toWString( ) );
			m_lblNumber->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblNumber->RemakeBuffer( prev_w, prev_h, m_lblNumber->get( "width" )->toInt( ), m_lblNumber->get( "height" )->toInt( ) );
			m_lblNumber->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
			// имя шаблона
			prev_w = m_lblName->get( "width" )->toInt( );
			prev_h = m_lblName->get( "height" )->toInt( );
			m_lblName->get( "title" )->ini( obj->get( "name" )->toWString( ) );
			m_lblName->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblName->RemakeBuffer( prev_w, prev_h, m_lblName->get( "width" )->toInt( ), m_lblName->get( "height" )->toInt( ) );
			m_lblName->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
			// тип шаблона
			wstring szTypeName = L"";
			map< udInt, wstring > mapType;
			mapType[ udProjectDocTemplate::tplClassic	] = L"";
			mapType[ udProjectDocTemplate::tplContract	] = L"Договор";
			mapType[ udProjectDocTemplate::tplPermit	] = L"Путевка";
			mapType[ udProjectDocTemplate::tplContract2	] = L"Гр.договор";
			map< udInt, wstring >::iterator itType;
			itType = mapType.find( obj->get( "type" )->toInt( ) );
			if ( itType != mapType.end( ) ) {
				szTypeName = itType->second;
			}
			prev_w = m_lblType->get( "width" )->toInt( );
			prev_h = m_lblType->get( "height" )->toInt( );
			m_lblType->get( "title" )->ini( szTypeName );
			m_lblType->CalculateWidthHeight( m_wndForm->GetHWndParent( ) );
			m_lblType->RemakeBuffer( prev_w, prev_h, m_lblType->get( "width" )->toInt( ), m_lblType->get( "height" )->toInt( ) );
			m_lblType->Move( );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataRecieve

	void CHDocTemplatePrinted::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkPrint ) {
				ExtOpenFile( m_szDir + L"\\out.xml" );
			} else {
				udStateHandlerSimple::LinkClick( lnk );
			}
		} else {
			udStateHandlerSimple::LinkClick( lnk );
		}
	} // void LinkClick

} // namespace UdSDK
