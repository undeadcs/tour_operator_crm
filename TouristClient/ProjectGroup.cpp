
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void GroupAssignTpl_Tour( udPBase tpl ) {
		tpl->get( "name" )->ini( wstring( L"" ) );
		tpl->get( "date1" )->ini( datetime( ) );
		tpl->get( "date2" )->ini( datetime( ) );
		tpl->get( "num" )->ini( ( udUInt ) 0 );
	} // void GroupAssignTpl_Tour

	static void GroupAssignTpl_Common( udPBase tpl ) {
		tpl->get( "name" )->ini( wstring( L"" ) );
	} // void GroupAssignTpl_Common

	CHGroup::CHGroup( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_group";
		m_szUpdIndex	= "id";
		m_szName		= "group";
		m_fnGetObject	= GetProjectGroup;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHGroupForm;

		this->AddExtraLoadInfo( ctrlidDirect, GetProjectDirect, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidTour, GetProjectGroupTour, ( udInt ) extra_load_frmlst, GroupAssignTpl_Tour );
		this->AddExtraLoadInfo( ctrlidPeriod, GetProjectGroupPeriod, ( udInt ) extra_load_frmlst, GroupAssignTpl_Common );
		this->AddExtraLoadInfo( ctrlidApp, GetProjectAttach, ( udInt ) extra_load_frmlst );
		this->AddExtraLoadInfo( ctrlidTpltour, GetProjectGroupTplTour, ( udInt ) extra_load_frmlst, GroupAssignTpl_Common );

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHGroup::CHGroup( const CHGroup& objCopy ) {
		udDELETE( m_lnkPrint );
	}

	CHGroup::~CHGroup( ) {
	}

	CHGroup& CHGroup::operator =( const CHGroup& objCopy ) {
		return *this;
	}

	void CHGroup::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHGroup::LinkClick( udPWndLink lnk ) {
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
				decl->get( "href" )->ini( wstring( L"group_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectGroup( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHGroup::ctrlidDirect )->extlst->extlst;
				node1 = doc->CreateNode( L"direct_txt" );
				node1->SetValue( L"<![CDATA[" +  extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"tour_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHGroup::ctrlidTour )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node1->InsertChild( i->second->toXml( doc ) );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"period_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHGroup::ctrlidPeriod )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node1->InsertChild( i->second->toXml( doc ) );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"tpltour_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHGroup::ctrlidTpltour )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node1->InsertChild( i->second->toXml( doc ) );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Группа" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\group_out.xsl", ( folder + wstring( L"\\group_out.xsl" ) ).data( ), FALSE );
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

	udInt CHGroup::GetListModel( ) {
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
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить группу" : L"Новая группа" );
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
		wndInsLst->AddAttr( "placenum_", L"Места", "placenum" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "lperiod_", L"Периодичность", "lperiod" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "tpltour_", L"Шаблон тура", "tpltour" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltName, "name", NULL, font, 0 );
		//m_wndModeList->AddExtlist( fltTplTour, "tpltour", NULL, "name", L"выберите шаблон", GetHTplTourList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		//filter->get( "tpltour" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHGroup::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltName );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		/*box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltTplTour );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;*/
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

	udInt CHGroup::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddFormlist( ctrlidTour, "ltour", NULL )->AddAttr( "name" );
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		m_wndForm->AddFormlist( ctrlidApp, "app", NULL, g_pApp->GetUniqueTmpFolder( ) )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidPeriod, "lperiod", NULL )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidTpltour, "ltpltour", NULL )->AddAttr( "name" );
		m_wndForm->AddExtlist(
			ctrlidDirect, "direct",
			NULL, "name", L"выберите направление", GetHDirectList
			//NULL, "name", "pick_direct", L"выберите направление",
			//"ud_direct", GetProjectDirect
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGroup::GetFormView( ) {
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
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Группа";
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
		title	= L"Туры";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		h = 64;
		w = 600;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 600; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidTour );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Комментарий";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		w = 212;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Приложение";
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
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidApp );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Периодичность";
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
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidPeriod );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Шаблоны туров";
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
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidTpltour );
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
		w = 212;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidDirect );
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

	void CHGroup::AddExtraAttributes( udPBase tpl ) {
	} // void AddExtraAttributes

	udInt CHGroup::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "direct" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите направление" );
			return FormHandlerErrorFailed;
		}
		//udPDwAscBase tours = m_frmlstTour->GetItems( );
		/*if ( !tours || tours->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите туры" );
			return false;
		}*/
		udPDwAscBase period = m_wndForm->GetInfo( ctrlidPeriod )->frmlst->GetItems( );
		if ( !period || period->empty( ) ) {
			mb::err( hWnd, L"Введите периодичность" );
			return FormHandlerErrorFailed;
		}
		//udPDwAscBase attach = m_frmlstAttach->GetItems( );
		/*if ( !attach || attach->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите приложение" );
			return false;
		}*/
		//udPDwAscBase tpltour = m_frmlstTplTour->GetItems( );
		/*if ( !tpltour || tpltour->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите шаблоны туров" );
			return false;
		}*/
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHGroup::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidTpltour )->frmlst ) {
			udPListHandler handler = GetHTplTourList( );
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectGroupTplTour;
				add->get( "group" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "tpltour" )->ini( obj->get( "id" )->toUInt( ) );
				add->get( "name" )->ini( obj->get( "name" )->toWString( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTour )->frmlst ) {
			udPFormHandler handler = GetHGroupTourForm( );
			handler->GetFrmOpt( )->get( "wndForm" )->ini( ( udDWord ) m_wndForm );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectGroupTour;
				obj->apply( add );

				add->get( "name" )->ini(
					add->get( "name" )->toWString( ) + L" (" + 
					add->get( "date1" )->toWString( ) + L" - " +
					add->get( "date2" )->toWString( ) + L")"
				);

				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidPeriod )->frmlst ) {
			udPFormHandler handler = GetHGroupPeriodForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectGroupPeriod;
				obj->apply( add );

				udPDwAscBase periods = m_wndForm->GetInfo( ctrlidPeriod )->frmlst->GetItems( );
				if ( !periods || periods->empty( ) ) {
					add->get( "index" )->ini( ( udUInt ) 1 );
				} else {
					udDwAscBase::iterator i = periods->end( );
					--i;
					add->get( "index" )->ini( i->second->get( "index" )->toUInt( ) + 1 );
				}

				add->get( "name" )->ini(
					add->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L", " +
					add->get( "min" )->toWString( ) + L" - " + add->get( "max" )->toWString( ) +
					L" i:" + add->get( "index" )->toWString( )
				);

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
				add->get( "type" )->ini( udProjectAttach::ATTACH_GROUP );
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

	void CHGroup::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidTpltour )->frmlst ) {
			udPListHandler handler = GetHTplTourList( );
			udPMorph obj1 = DialogListPick( hWnd, handler );
			if ( obj1 ) {
				obj->get( "tpltour" )->ini( obj1->get( "id" )->toUInt( ) );
				obj->get( "name" )->ini( obj1->get( "name" )->toWString( ) );
				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTour )->frmlst ) {
			udPBase tmp = new udProjectGroupTour;
			obj->apply( tmp );
			udPFormHandler handler = GetHGroupTourForm( );
			handler->GetFrmOpt( )->get( "wndForm" )->ini( ( udDWord ) m_wndForm );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );

				obj->get( "name" )->ini(
					obj->get( "name" )->toWString( ) + L" (" + 
					obj->get( "date1" )->toWString( ) + L" - " +
					obj->get( "date2" )->toWString( ) + L")"
				);

				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidPeriod )->frmlst ) {
			udPBase tmp = new udProjectGroupPeriod;
			obj->apply( tmp );
			udPFormHandler handler = GetHGroupPeriodForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );

				obj->get( "name" )->ini(
					obj->get( "date1" )->toTime( ).toWString( "%d.%m.%Y" ) + L", " +
					obj->get( "min" )->toWString( ) + L" - " + obj->get( "max" )->toWString( ) +
					L" i:" + obj->get( "index" )->toWString( )
				);

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

	bool CHGroup::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidTpltour )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить шаблон из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectGroupTplTour;
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
					udPBase del = new udProjectGroupTour;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidPeriod )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить период из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectGroupPeriod;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
				udPWndFormList frmlstTour = m_wndForm->GetInfo( ctrlidTour )->frmlst;
				udPDwAscBase items = frmlstTour->GetItems( );
				if ( items && !items->empty( ) ) {
					udDWord period = obj->get( "index" )->toDWord( );
					udPBase obj1 = NULL, del = NULL;
					udDWord rowId = 0;
					udPWndRows wndRows = frmlstTour->GetWndRows( );
					map< udDWord, udDWord > toDel;
					map< udDWord, udDWord >::iterator j;
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						rowId = i->first;
						obj1 = i->second;
						if ( period == obj1->get( "period" )->toDWord( ) ) {
							toDel[ rowId ] = rowId;
							if ( obj1->get( "id" )->toUInt( ) > 0 ) {
								del = new udProjectGroupTour;
								del->apply( obj1, true, true );
								frmlstTour->AddToDel( del );
							}
						}
					}
					udDwAscBase::iterator i;
					for( j = toDel.begin( ); j != toDel.end( ); ++j ) {
						rowId = j->second;
						i = items->find( rowId );
						if ( i != items->end( ) ) {
							delete i->second;
							items->erase( i );
							wndRows->DeleteRow( rowId );
						}
					}
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

	void CHGroup::ExtraDataSending( udPSocket sock, udPBase obj ) {
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

	void CHGroup::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
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

	void CHGroup::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		//filter->get( "tpltour" )->ini( ( udUInt ) 0 );
	} // void ClearFilter




	CHGroupPeriod::CHGroupPeriod( ) : udListHandler( ), udFormHandler( ) {
	}

	CHGroupPeriod::~CHGroupPeriod( ) {
	}

	void CHGroupPeriod::LoadList( ) {
		// тут необходимо обращаться к форме услуги и от туда выгребать периоды
		if ( m_pLstOpt->isset( "wndForm" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pLstOpt->get( "wndForm" )->toDWord( );
			ModeFormControlInfo *info = wndForm->GetInfo( udHGroup::ctrlidPeriod );
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

	udInt CHGroupPeriod::GetListModel( ) {
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
		wndInsLst->AddAttr( "name", L"Наименование" )->get( "width" )->ini( ( udInt ) 400 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHGroupPeriod::GetListView( ) {
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
	} // udInt GetListView

	udInt CHGroupPeriod::ListOk( ) {
		assert( m_wndModeList );
		if ( m_wndModeList->IsModal( ) ) {
			::PostMessage( m_wndModeList->GetHWndParent( ), WM_COMMAND, MAKEWPARAM( IDOK, 0 ), ( LPARAM ) m_wndModeList );
		}
		return ListHandlerErrorOk;
	} // udInt ListOk

	udInt CHGroupPeriod::ListCancel( ) {
		assert( m_wndModeList );
		if ( m_wndModeList->IsModal( ) ) {
			::PostMessage( m_wndModeList->GetHWndParent( ), WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), ( LPARAM ) m_wndModeList );
		}
		return ListHandlerErrorOk;
	} // udInt ListCancel

	void CHGroupPeriod::OnLinkClick( udPWndLink lnk ) {
		string url = lnk->get( "url" )->toString( );
		if ( url == "#form" ) {
			udPFormHandler handler = GetHGroupPeriodForm( );
			udPMorph obj = DialogForm( m_wndModeList->GetHWndParent( ), handler );
			delete handler;
			udDELETE( obj );
			//
			this->ClearList( );
			this->LoadList( );
			m_wndModeList->Show( );
		}
	} // void OnLinkClick

	void CHGroupPeriod::OnSelectRow( udLong row ) {
		assert( m_wndModeList );
		m_wndModeList->GetList( )->SelectRow( row );
	} // void OnSelectRow

	void CHGroupPeriod::OnHeaderClick( const string& name, const string& attr ) {
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

	void CHGroupPeriod::OnPagerSelect( udUInt page ) {
		this->ClearList( );
		m_pPager->SetPage( page );
		this->LoadList( );
		m_wndModeList->Show( );
	} // void OnPagerSelect

	void CHGroupPeriod::OnButtonClick( udPWndButton btn ) {
	} // void OnButtonClick

	udInt CHGroupPeriod::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddInput( ctrlidMin, "min", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidMax, "max", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGroupPeriod::GetFormView( ) {
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
		title	= L"Период";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата";
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
		title	= L"Места минимум";
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
		info = m_wndForm->GetInfo( ctrlidMin );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Места максимум";
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
		info = m_wndForm->GetInfo( ctrlidMax );
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

	udInt CHGroupPeriod::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGroupPeriod::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGroupPeriod::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGroupPeriod::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



	CHGroupTour::CHGroupTour( ) : udFormHandler( ) {
	}

	CHGroupTour::~CHGroupTour( ) {
	}

	udInt CHGroupTour::GetFormModel( ) {
		m_wndForm->AddExtlist( ctrlidTour, "tour", NULL, "name", L"выберите тур", GetHTourList );
		m_wndForm->AddExtlist( ctrlidPeriod, "period", NULL, "name", L"выберите период", GetHGroupPeriodList, m_pFrmOpt );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGroupTour::GetFormView( ) {
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
		title	= L"Тур группы";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тур";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidTour );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Период";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidPeriod );
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

	udInt CHGroupTour::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );

		if ( obj->get( "tour" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите тур" );
			return FormHandlerErrorFailed;
		}
		ModeFormControlInfo *info = m_wndForm->GetInfo( ctrlidPeriod );
		udPBase period = info->extlst->extlst->GetObj( );
		if ( !period ) {
			mb::err( hWnd, L"Введите период" );
			return FormHandlerErrorFailed;
		}
		obj->get( "period" )->ini( period->get( "index" )->toUInt( ) );

		info = m_wndForm->GetInfo( ctrlidTour );
		udPBase tour = info->extlst->extlst->GetObj( );
		obj->get( "name" )->ini( tour->get( "name" )->toWString( ) );
		obj->get( "date1" )->ini( tour->get( "date1" )->toTime( ) );
		obj->get( "date2" )->ini( tour->get( "date2" )->toTime( ) );

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGroupTour::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGroupTour::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGroupTour::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

	void CHGroupTour::LoadAfterSetObject( ) {
		udPBase obj = m_wndForm->GetObj( );
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "tour" )->toUInt( );
		if ( id > 0 ) {
			udPBase ret = LoadById( "tour", id, GetProjectTour );
			if ( ret ) {
				m_wndForm->GetInfo( ctrlidTour )->extlst->extlst->SetObj( ret );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "period" )->toUInt( );
		if ( id > 0 ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "wndForm" )->toDWord( );
			ModeFormControlInfo *info = wndForm->GetInfo( udHGroup::ctrlidPeriod );
			udPDwAscBase periods = info->frmlst->GetItems( );
			if ( periods && !periods->empty( ) ) {
				for( udDwAscBase::iterator i = periods->begin( ); i != periods->end( ); ++i ) {
					if ( i->second->get( "index" )->toDWord( ) == id ) {
						udPBase period = new udProjectServicePeriod;
						i->second->apply( period );
						m_wndForm->GetInfo( ctrlidPeriod )->extlst->extlst->SetObj( period );
						break;
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadAfterSetObject

} // namespace UdSDK
