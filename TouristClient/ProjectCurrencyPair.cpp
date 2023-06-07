
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHCurrencyPair::CHCurrencyPair( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_currencypair";
		m_szUpdIndex	= "id";
		m_szName		= "currencypair";
		m_fnGetObject	= GetProjectCurrencyPair;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHCurrencyPairForm;

		this->AddExtraLoadInfo( ctrlidC1, GetProjectCurrency, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidC2, GetProjectCurrency, ( udInt ) extra_load_extlst );

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHCurrencyPair::CHCurrencyPair( const CHCurrencyPair& objCopy ) {
	}

	CHCurrencyPair::~CHCurrencyPair( ) {
		udDELETE( m_lnkPrint );
	}

	CHCurrencyPair& CHCurrencyPair::operator =( const CHCurrencyPair& objCopy ) {
		return *this;
	}

	udInt CHCurrencyPair::GetListModel( ) {
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
		title = ( modal ? L"Добавить валютную пару" : L"Новая валютная пара" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "pair", L"Валюта 1/Валюта 2" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "course", L"Курс ЦБ" )->get( "width" )->ini( ( udInt ) 100 );
		wndInsLst->AddAttr( "set_", L"Дата установки", "set" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "internal", L"Внутренний курс" )->get( "width" )->ini( ( udInt ) 120 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddInput( fltCourse1, "course1", NULL, font, 0 );
		m_wndModeList->AddInput( fltCourse2, "course2", NULL, font, 0 );
		m_wndModeList->AddInput( fltCb1, "cb1", NULL, font, 0 );
		m_wndModeList->AddInput( fltCb2, "cb2", NULL, font, 0 );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "course1" )->ini( ( udFloat ) 0.0f );
		filter->get( "course2" )->ini( ( udFloat ) 0.0f );
		filter->get( "cb1" )->ini( ( udFloat ) 0.0f );
		filter->get( "cb2" )->ini( ( udFloat ) 0.0f );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHCurrencyPair::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCourse1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCourse2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCb1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCb2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
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

	udInt CHCurrencyPair::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddExtlist(
			ctrlidC1, "currency1",
			NULL, "name", L"выберите валюту", GetHCurrencyList
			//NULL, "name", "pick_currency", L"выберите валюту",
			//"ud_currency", GetProjectCurrency
		);
		m_wndForm->AddExtlist(
			ctrlidC2, "currency2",
			NULL,  "name", L"выберите валюту", GetHCurrencyList
			//NULL, "name", "pick_currency", L"выберите валюту",
			//"ud_currency", GetProjectCurrency
		);
		m_wndForm->AddInput( ctrlidCb, "course", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidSet, "set", NULL, font );
		m_wndForm->AddInput( ctrlidInternal, "internal", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHCurrencyPair::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 120 );
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
		gdi->Clear( Color( 255, 255, 255 ) );
		udRenderer::RenderBackground(
			gdi, imgSprite,
			Rect( 0, 0, 10, m_wndForm->get( "height" )->toInt( ) ),
			Rect( 556, 283, 10, 101 ),
			udRenderer::UDSDK_REPEAT_Y
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Валютная пара";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Первая валюта";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidC1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Вторая валюта";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidC2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Курс ЦБ";
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
		info = m_wndForm->GetInfo( ctrlidCb );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата установки";
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
		info = m_wndForm->GetInfo( ctrlidSet );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Внутренний курс";
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
		info = m_wndForm->GetInfo( ctrlidInternal );
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

	udInt CHCurrencyPair::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "currency1" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите первую валюту" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "currency2" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите вторую валюту" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "course" )->toFloat( ) == 0.0f ) {
			mb::err( hWnd, L"Введите курс ЦБ" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "internal" )->toFloat( ) == 0.0f ) {
			mb::err( hWnd, L"Введите внутренний курс" );
			return FormHandlerErrorFailed;
		}

		*( obj->get( "course" ) )	= obj->get( "course" )->toFloat( );
		*( obj->get( "internal" ) )	= obj->get( "internal" )->toFloat( );

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHCurrencyPair::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "pair" )->ini( wstring( L"" ) );
		//tpl->get( "set_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHCurrencyPair::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "course1" )->ini( ( udFloat ) 0.0f );
		filter->get( "course2" )->ini( ( udFloat ) 0.0f );
		filter->get( "cb1" )->ini( ( udFloat ) 0.0f );
		filter->get( "cb2" )->ini( ( udFloat ) 0.0f );
	} // void ClearFilter

	void CHCurrencyPair::AfterLoadFilterData( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		udFloat val = 0.0f;
		val = filter->get( "course1" )->toFloat( );
		filter->get( "course1" )->ini( val );
		val = filter->get( "course2" )->toFloat( );
		filter->get( "course2" )->ini( val );
		val = filter->get( "cb1" )->toFloat( );
		filter->get( "cb1" )->ini( val );
		val = filter->get( "cb2" )->toFloat( );
		filter->get( "cb2" )->ini( val );
	} // void AfterLoadFilterData

	void CHCurrencyPair::RequestList( ) {
		udStateHandlerSimple::RequestList( );
		datetime date;
		udPBase obj = NULL;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			date = obj->get( "set" )->toTime( );
			obj->get( "set_" )->ini( date.toWString( "%d.%m.%Y" ) );
		}
	} // void RequestList

	void CHCurrencyPair::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHCurrencyPair::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
				udPMorph decl = NULL;
				udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
				udPBase obj = NULL;
				udPExtList extlst = NULL;
				//////////////////////////////////////////////////////////////////////////////////
				decl = new udMorph;
				decl->get( "version" )->ini( wstring( L"1.0" ) );
				decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
				doc->AddDeclaration( L"xml", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				decl = new udMorph;
				decl->get( "type" )->ini( wstring( L"text/xsl" ) );
				decl->get( "href" )->ini( wstring( L"currencypair_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectCurrencyPair( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHCurrencyPair::ctrlidC1 )->extlst->extlst;
				node1 = doc->CreateNode( L"currency1_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHCurrencyPair::ctrlidC2 )->extlst->extlst;
				node1 = doc->CreateNode( L"currency2_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Валютная пара" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\currencypair_out.xsl", ( folder + wstring( L"\\currencypair_out.xsl" ) ).data( ), FALSE );
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

} // namespace UdSDK
