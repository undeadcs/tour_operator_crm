
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHClDoc::CHClDoc( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_cldoc";
		m_szUpdIndex	= "id";
		m_szName		= "cldoc";
		m_fnGetObject	= GetProjectClDoc;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHClDocForm;

		this->AddExtraLoadInfo( ctrlidClient, GetProjectClient, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidType, GetProjectDocType, ( udInt ) extra_load_extlst );

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHClDoc::CHClDoc( const CHClDoc& objCopy ) {
	}

	CHClDoc::~CHClDoc( ) {
		udDELETE( m_lnkPrint );
	}

	CHClDoc& CHClDoc::operator =( const CHClDoc& objCopy ) {
		return *this;
	}

	udInt CHClDoc::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST + 1;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		m_pLstOpt->get( "onlyfiz" );
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
		title = ( modal ? L"Добавить документ" : L"Новый документ" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "client_", L"Клиент", "client" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "type_", L"Тип", "type" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "series", L"Серия" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "number", L"Номер" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddExtlist( fltClient, "client", NULL, "fio", L"выберите клиента", GetHClientList, m_pLstOpt );
		m_wndModeList->AddExtlist( fltType, "type", NULL, "name", L"выберите тип", GetHDocTypeList );
		m_wndModeList->AddInput( fltSeries, "series", NULL, font, 0 );
		m_wndModeList->AddInput( fltNumber, "number", NULL, font, 0 );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHClDoc::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltClient );
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
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltSeries );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltNumber );
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

	udInt CHClDoc::CreateList( HWND hParent, bool modal ) {
		udInt ret = udStateHandlerInfo::CreateList( hParent, modal );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pLstOpt->isset( "client" ) ) {
			udPMorph filter = m_wndModeList->GetObjFilter( );
			udPBase add = new udProjectClient, client = ( udPBase ) m_pLstOpt->get( "client" )->toDWord( );
			filter->get( "client" )->ini( client->get( "id" )->toUInt( ) );
			client->apply( add );
			m_wndModeList->GetInfo( fltClient )->extlst->extlst->SetObj( add );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // udInt CreateList

	udInt CHClDoc::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		if ( m_wndForm->IsModal( ) ) {
		} else {
			m_wndForm->AddExtlist(
				ctrlidClient, "client", NULL, "fio", L"выберите клиента", GetHClientList
				//ctrlidClient, "client",
				//NULL, "fio", "pick_client", L"выберите клиента",
				//"ud_client", GetProjectClient
			);
		}
		m_wndForm->AddExtlist(
			ctrlidType, "type", NULL, "name", L"выберите тип документа", GetHDocTypeList
			//ctrlidType, "type",
			//NULL, "name", "pick_type", L"выберите тип документа",
			//"ud_doctype", GetProjectDocType
		);
		m_wndForm->AddInput( ctrlidSeries, "series", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidNumber, "number", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidIssuedate, "issuedate", NULL, font );
		m_wndForm->AddInput( ctrlidIssuedby, "issuedby", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidBirthplace, "birthplace", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidEnddate, "enddate", NULL, font );
		m_wndForm->AddInput( ctrlidFio, "fio", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidFiolatin, "fiolatin", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHClDoc::GetFormView( ) {
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
		title	= L"Документ клиента";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
		} else {
			title	= L"Клиент";
			x		= 32;
			w		= lcol;
			h		= 28;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);

			x += w + m;
			box = new ModeFormControlBox;
			box->x = x;	box->y = y;	box->w = 200; box->h = 28;
			info = m_wndForm->GetInfo( ctrlidClient );
			info->box = box;
			y += h + m;
		}
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
		title	= L"Серия";
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
		info = m_wndForm->GetInfo( ctrlidSeries );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Номер";
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
		title	= L"Дата выдачи";
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
		info = m_wndForm->GetInfo( ctrlidIssuedate );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата окончания";
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
		info = m_wndForm->GetInfo( ctrlidEnddate );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Кем выдан";
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
		info = m_wndForm->GetInfo( ctrlidIssuedby );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Место рождения";
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
		info = m_wndForm->GetInfo( ctrlidBirthplace );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"ФИО";
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
		info = m_wndForm->GetInfo( ctrlidFio );
		info->box = box;
		y += h + m;
			//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"ФИО латиницей";
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
		info = m_wndForm->GetInfo( ctrlidFiolatin );
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

	udInt CHClDoc::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( !m_wndForm->IsModal( ) && ( obj->get( "client" )->toUInt( ) == 0 ) ) {
			mb::err( hWnd, L"Введите клиента" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "type" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите тип" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "series" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите серию" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "number" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите номер" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "issuedby" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите кем выдан" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "birthplace" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите место рождения" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "fio" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите ФИО" );
			return FormHandlerErrorFailed;
		}
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHClDoc::ProcForm( bool modal ) {
		assert( m_wndForm );
		if ( m_wndForm->IsModal( ) ) {
			m_wndForm->LoadControlData( );
			return ValidateForm( );
		} else {
			return udStateHandlerSimple::ProcForm( modal );
		}
		return FormHandlerErrorOk;
	} // bool ProcForm

	void CHClDoc::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "client_" )->ini( wstring( L"" ) );
		tpl->get( "type_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHClDoc::LoadAfterSetObject( ) {
		udPBase obj = m_wndForm->GetObj( );
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "type" )->toUInt( );
		if ( id > 0 ) {
			udPBase ret = LoadById( "doctype", id, GetProjectDocType );
			m_wndForm->GetInfo( ctrlidType )->extlst->extlst->SetObj( ret );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadAfterSetObject

	void CHClDoc::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "series" )->ini( wstring( L"" ) );
		filter->get( "number" )->ini( wstring( L"" ) );
	} // void ClearFilter

	void CHClDoc::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHClDoc::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
				udPMorph decl = NULL;
				udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
				udPExtList extlst = NULL;
				udPBase obj = NULL;
				//////////////////////////////////////////////////////////////////////////////////
				decl = new udMorph;
				decl->get( "version" )->ini( wstring( L"1.0" ) );
				decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
				doc->AddDeclaration( L"xml", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				decl = new udMorph;
				decl->get( "type" )->ini( wstring( L"text/xsl" ) );
				decl->get( "href" )->ini( wstring( L"cldoc_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectClDoc( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHClDoc::ctrlidType )->extlst->extlst;
				node1 = doc->CreateNode( L"type_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Тур" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\cldoc_out.xsl", ( folder + wstring( L"\\cldoc_out.xsl" ) ).data( ), FALSE );
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
