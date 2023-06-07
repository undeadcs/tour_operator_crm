
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void TplTourAssignTplRoute( udPBase tpl ) {
		tpl->get( "name" )->ini( wstring( L"" ) );
	} // void TplTourAssignTplRoute

	void TplTourAssignTplService( udPBase tpl ) {
		tpl->get( "#name" )->ini( wstring( L"" ) );
	} // void TplTourAssignTplService

	CHTplTour::CHTplTour( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_tpltour";
		m_szUpdIndex	= "id";
		m_szName		= "tpltour";
		m_fnGetObject	= GetProjectTplTour;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHTplTourForm;

		this->AddExtraLoadInfo( ctrlidDirect, GetProjectDirect, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidRoute, GetProjectTplTourRoute, ( udInt ) extra_load_frmlst, TplTourAssignTplRoute );
		this->AddExtraLoadInfo( ctrlidService, GetProjectTplTourService, ( udInt ) extra_load_frmlst, TplTourAssignTplService );
		this->AddExtraLoadInfo( ctrlidApp, GetProjectAttach, ( udInt ) extra_load_frmlst );

		m_lnkTour = new udWndLink( ( udUInt ) 18 );
		m_lnkTour->get( "title" )->ini( wstring( L"Создать тур" ) );
		m_lnkTour->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );

		m_lnkPrint = new udWndLink( ( udUInt ) 18 );
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHTplTour::CHTplTour( const CHTplTour& objCopy ) {
	}

	CHTplTour::~CHTplTour( ) {
		udDELETE( m_lnkTour );
		udDELETE( m_lnkPrint );
	}

	CHTplTour& CHTplTour::operator =( const CHTplTour& objCopy ) {
		return *this;
	}

	void CHTplTour::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkTour->Show( );
				m_lnkPrint->Show( );
			} else {
				m_lnkTour->Hide( );
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHTplTour::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkTour ) {
				udPFormHandler handler = GetHTourForm( );
				handler->GetFrmOpt( )->get( "tpltour_init" )->ini( ( udDWord ) m_wndForm );
				udPBase obj = GetProjectTour( ), tpltour = m_wndForm->GetObj( );
				//obj->get( "name" )->ini( tpltour->get( "name" )->toWString( ) );
				obj->get( "cost" )->ini( tpltour->get( "cost" )->toFloat( ) );
				obj->get( "price" )->ini( tpltour->get( "price" )->toFloat( ) );
				obj->get( "direct" )->ini( tpltour->get( "direct" )->toUInt( ) );

				udPMorph ret = DialogForm( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), handler, obj );
				delete handler;
				udDELETE( ret );
			} else if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
				udPMorph decl = NULL;
				udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
				udPExtList extlst = NULL;
				udPBase obj = NULL;
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
				decl->get( "href" )->ini( wstring( L"tpltour_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectTplTour( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHTplTour::ctrlidDirect )->extlst->extlst;
				node1 = doc->CreateNode( L"direct_txt" );
				node1->SetValue( L"<![CDATA[" +  extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"route_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHTplTour::ctrlidRoute )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = i->second->toXml( doc );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"service_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHTplTour::ctrlidService )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = doc->CreateNode( L"tpltourservice" );
					node2->SetValue( L"<![CDATA[" +  i->second->get( "#name" )->toWString( ) + L"]]>" );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Шаблон тура" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\tpltour_out.xsl", ( folder + wstring( L"\\tpltour_out.xsl" ) ).data( ), FALSE );
				ExtOpenFile( folder + L"\\out.xml" );
				//////////////////////////////////////////////////////////////////////////////////
				xml.clear( );
				udDELETE( obj );
				udDELETE( doc );
				//////////////////////////////////////////////////////////////////////////////////
			} else {
				udStateHandlerSimple::LinkClick( lnk );
			}
		} else {
			udStateHandlerSimple::LinkClick( lnk );
		}
	} // void LinkClick

	void CHTplTour::Init( ) {
		udStateHandlerSimple::Init( );

		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\tpltour";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	void CHTplTour::Clear( ) {
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
	} // void Clear

	udInt CHTplTour::GetListModel( ) {
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
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить шаблон тура" : L"Новый шаблон тура" );
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
		wndInsLst->AddAttr( "cost", L"Себестоимость" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "price", L"Итоговая цена" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltName, "name", NULL, font, 0 );
		m_wndModeList->AddExtlist( fltDirect, "direct", NULL, "name", L"выберите направление", GetHDirectList );
		m_wndModeList->AddExtlist( fltTour, "tour", NULL, "name", L"выберите тур", GetHTourList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHTplTour::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltTour );
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

	udInt CHTplTour::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddFormlist( ctrlidRoute, "route", NULL )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidService, "service", NULL )->AddAttr( "#name" );
		m_wndForm->AddInput( ctrlidCost, "cost", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidPrice, "price", NULL, font, 0 );
		m_wndForm->AddFormlist( ctrlidApp, "app", NULL, g_pApp->GetUniqueTmpFolder( ) )->AddAttr( "name" );
		m_wndForm->AddExtlist(
			ctrlidDirect, "direct",
			NULL, "name", L"выберите направление", GetHDirectList
			//NULL, "name", "pick_direct", L"выберите направление",
			//"ud_direct", GetProjectDirect
		);
		m_wndForm->AddInput( ctrlidDaynum, "daynum", NULL, font, ES_NUMBER );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTplTour::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkTour->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkTour->get( "x" )->ini( ( udInt ) 300 );
			m_lnkTour->get( "y" )->ini( ( udInt ) 10 );
			m_lnkTour->Create( m_wndForm->GetHWnd( ) );
			m_lnkTour->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			//////////////////////////////////////////////////////////////////////////////////////
			m_lnkPrint->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrint->get( "x" )->ini( ( udInt ) m_lnkTour->get( "x" )->toInt( ) + m_lnkTour->get( "width" )->toInt( ) + 5 );
			m_lnkPrint->get( "y" )->ini( ( udInt ) 10 );
			m_lnkPrint->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrint->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			//////////////////////////////////////////////////////////////////////////////////////
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
		title	= L"Шаблон тура";
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
		title	= L"Маршрут";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		h = 64;
		w = 400;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidRoute );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Услуги";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		h = 64;
		w = 400;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidService );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Стоимость";
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
		info = m_wndForm->GetInfo( ctrlidCost );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Цена";
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
		info = m_wndForm->GetInfo( ctrlidPrice );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Приложения";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		h = 64;
		w = 400;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidApp );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Направление";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidDirect );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Продолжительность";
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
		info = m_wndForm->GetInfo( ctrlidDaynum );
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

	udInt CHTplTour::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "cost" )->toFloat( ) == 0.0f ) {
			mb::err( hWnd, L"Введите стоимость" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "price" )->toFloat( ) == 0.0f ) {
			mb::err( hWnd, L"Введите цену" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "direct" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите направление" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "daynum" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите продолжительность" );
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
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHTplTour::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			udPListHandler handler = GetHCityList( );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTplTourRoute;
				add->get( "tpltour" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "city" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "name" )->ini( obj->get( "name" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPFormHandler handler = GetHTplTourServiceForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTplTourService;
				obj->apply( add );
				add->get( "tpltour" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );

				wstring date1 = L"", date2 = L"";
				udDatetime date = obj->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );

				add->get( "#name" )->ini(
					date1 + L"-" + date2 + L", " +
					obj->get( "num" )->toWString( ) + L", " +
					obj->get( "name_" )->toWString( )
				);
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
			/*udPListHandler handler = GetHServiceList( );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectTplTourService;
				add->get( "tpltour" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "service" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "#name" )->ini( obj->get( "name_" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;*/
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
				//add->get( "obj" )->ini( m_pObject->get( "id" )->toUInt( ) );
				add->get( "obj" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "type" )->ini( udProjectAttach::ATTACH_TPLTOUR );
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

	void CHTplTour::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
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
			udPBase tmp = new udProjectTplTourService;
			obj->apply( tmp );
			udPFormHandler handler = GetHTplTourServiceForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );

				wstring date1 = L"", date2 = L"";
				udDatetime date = obj->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );

				obj->get( "#name" )->ini(
					date1 + L"-" + date2 + L", " +
					obj->get( "num" )->toWString( ) + L", " +
					obj->get( "name_" )->toWString( )
				);

				delete obj1;
			}
			delete handler;
			/*udPListHandler handler = GetHServiceList( );
			udPMorph obj1 = DialogListPick( hWnd, handler );
			if ( obj1 ) {
				obj->get( "service" )->ini( obj1->get( "id" )->toUInt( ) );
				obj->get( "#name" )->ini( obj1->get( "name_" )->toWString( ) );
				delete obj1;
			}
			delete handler;*/
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

	bool CHTplTour::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить пункт маршрута?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTplTourRoute;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить пункт маршрута?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectTplTourService;
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

	void CHTplTour::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "direct_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHTplTour::ExtraDataSending( udPSocket sock, udPBase obj ) {
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
	} // void ExtraDataSending

	void CHTplTour::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
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
	} // void ExtraDataRecieve

	void CHTplTour::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
	} // void ClearFilter







	CHTplTourService::CHTplTourService( ) : udFormHandler( ) {
	}

	CHTplTourService::~CHTplTourService( ) {
	}

	udInt CHTplTourService::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddExtlist( ctrlidService, "service", NULL, "name_", L"выберите услугу", GetHServiceList );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddInput( ctrlidNum, "num", NULL, font, ES_NUMBER );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHTplTourService::GetFormView( ) {
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
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHTplTourService::ValidateForm( ) {
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

	udInt CHTplTourService::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHTplTourService::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHTplTourService::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

	void CHTplTourService::LoadAfterSetObject( ) {
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

} // namespace UdSDK
