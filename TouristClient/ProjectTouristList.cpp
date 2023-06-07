
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void TouristListAssignTplLink( udPBase obj ) {
		obj->get( "fio" )->ini( wstring( L"" ) );
	} // void TouristListAssignTplLink

	CHTouristList::CHTouristList( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_touristlist";
		m_szUpdIndex	= "id";
		m_szName		= "touristlist";
		m_fnGetObject	= GetProjectTouristList;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHTouristListForm;

		this->AddExtraLoadInfo( ctrlidExit, GetProjectTransferPoint, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidEnter, GetProjectTransferPoint, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidHost, GetProjectHostSide, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidRoute, GetProjectRoute, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidLeader, GetProjectClient, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidClient, GetProjectTouristListLink, ( udInt ) extra_load_frmlst, TouristListAssignTplLink );

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHTouristList::CHTouristList( const CHTouristList& objCopy ) {
	}

	CHTouristList::~CHTouristList( ) {
		udDELETE( m_lnkPrint );
	}

	CHTouristList& CHTouristList::operator =( const CHTouristList& objCopy ) {
		return *this;
	}

	udInt CHTouristList::GetListModel( ) {
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
		title = ( modal ? L"Добавить список" : L"Новый список" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "date1_", L"Дата выезда", "date1" )->get( "width" )->ini( ( udInt ) 200 );
		wndInsLst->AddAttr( "date2_", L"Дата въезда", "date2" )->get( "width" )->ini( ( udInt ) 200 );
		wndInsLst->AddAttr( "leader_", L"Руководитель", "leader" )->get( "width" )->ini( ( udInt ) 300 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltLeader, "leader", NULL, "fio", L"выберите клиента", GetHClientList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( 0, true ) );
		filter->get( "leader" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHTouristList::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltLeader );
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

	udInt CHTouristList::GetFormModel( ) {
		m_pFrmOpt->get( "pick_leader" )->ini( ( udDWord ) m_wndForm );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddExtlist( ctrlidExit, "exit", NULL, "name", L"выберите пункт", GetHTransferPointList );
		m_wndForm->AddExtlist( ctrlidEnter, "enter", NULL, "name", L"выберите пункт", GetHTransferPointList );
		m_wndForm->AddExtlist( ctrlidHost, "host", NULL, "name", L"выберите прин. стор.", GetHHostSideList );
		m_wndForm->AddExtlist( ctrlidRoute, "route", NULL, "name", L"выберите маршрут", GetHRouteList );
		m_wndForm->AddExtlist( ctrlidLeader, "leader", NULL, "fio", L"выберите клиента", GetHClientList, m_pFrmOpt );
		m_wndForm->AddFormlist( ctrlidClient, "_list", NULL )->AddAttr( "fio" );
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTouristList::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 180 );
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
		title	= L"Список туристов";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата выезда";
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
		title	= L"Дата въезда";
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
		title	= L"Пункт выезда";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidExit );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Пункт въезда";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidEnter );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Принимающая сторона";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidHost );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Маршрут";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidRoute );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Руководитель";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidLeader );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Клиенты";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidClient );
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

	udInt CHTouristList::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		//
		if ( obj->get( "leader" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Выберите руководителя группы" );
			return FormHandlerErrorFailed;
		}
		//
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHTouristList::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "leader_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHTouristList::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( 0, true ) );
		filter->get( "leader" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHTouristList::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHTouristList::LinkClick( udPWndLink lnk ) {
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
				decl->get( "href" )->ini( wstring( L"touristlist_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectTouristList( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( ctrlidExit )->extlst->extlst;
				node1 = doc->CreateNode( L"exit_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( ctrlidEnter )->extlst->extlst;
				node1 = doc->CreateNode( L"enter_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( ctrlidHost )->extlst->extlst;
				node1 = doc->CreateNode( L"host_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( ctrlidRoute )->extlst->extlst;
				node1 = doc->CreateNode( L"route_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( ctrlidLeader )->extlst->extlst;
				node1 = doc->CreateNode( L"leader_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"tourist_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( ctrlidClient )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = doc->CreateNode( L"tourist" );
					node2->SetValue( L"<![CDATA[" +  i->second->get( "fio" )->toWString( ) + L"]]>" );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Список туристов" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\touristlist_out.xsl", ( folder + wstring( L"\\touristlist_out.xsl" ) ).data( ), FALSE );
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

	void CHTouristList::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidClient )->frmlst ) {
			udPListHandler handler = GetHClientList( );
			handler->GetLstOpt( )->get( "onlyfiz" )->ini( ( udInt ) 1 );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTouristListLink;
				add->get( "list" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "client" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "fio" )->ini( obj->get( "fio" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		}
	} // void FormListOnAdd

	void CHTouristList::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidClient )->frmlst ) {
			udPListHandler handler = GetHClientList( );
			handler->GetLstOpt( )->get( "onlyfiz" )->ini( ( udInt ) 1 );
			udPMorph obj1 = DialogListPick( hWnd, handler );
			if ( obj1 ) {
				obj->get( "client" )->ini( obj1->get( "id" )->toUInt( ) );
				obj->get( "fio" )->ini( obj1->get( "fio" )->toWString( ) );
				delete obj1;
			}
			delete handler;
		}
	} // void FormListOnEdit

	bool CHTouristList::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidClient )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить клиента из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTouristListLink;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		}
		return false;
	} // bool FormListOnDelete

	void CHTouristList::RequestList( ) {
		udStateHandlerInfo::RequestList( );
		datetime date;
		udPBase obj = NULL;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			date = obj->get( "date1" )->toTime( );
			obj->get( "date1_" )->ini( date.toWString( "%d.%m.%Y" ) );
			date = obj->get( "date2" )->toTime( );
			obj->get( "date2_" )->ini( date.toWString( "%d.%m.%Y" ) );
		}
	} // void RequestList

} // namespace UdSDK
