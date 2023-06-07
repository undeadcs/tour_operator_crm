
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHRequest::CHRequest( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_request";
		m_szUpdIndex	= "id";
		m_szName		= "request";
		m_fnGetObject	= GetProjectRequest;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHRequestForm;

		this->AddExtraLoadInfo( ctrlidState, GetProjectSRequest, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidType, GetProjectTRequest, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidDirect, GetProjectDirect, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidManager, GetProjectUser, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidResult, GetProjectRRequest, ( udInt ) extra_load_extlst );

		m_lnkOrder = new udWndLink( ( udUInt ) 18 );
		m_lnkOrder->get( "title" )->ini( wstring( L"Создать заявку" ) );
		m_lnkOrder->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );

		m_lnkPrint = new udWndLink( ( udUInt ) 18 );
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHRequest::CHRequest( const CHRequest& objCopy ) {
	}

	CHRequest::~CHRequest( ) {
		udDELETE( m_lnkOrder );
		udDELETE( m_lnkPrint );
	}

	CHRequest& CHRequest::operator =( const CHRequest& objCopy ) {
		return *this;
	}

	void CHRequest::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkOrder->Show( );
				m_lnkPrint->Show( );
			} else {
				m_lnkOrder->Hide( );
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHRequest::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkOrder ) {
				udPFormHandler handler = GetHOrderForm( );
				handler->GetFrmOpt( )->get( "request_init" )->ini( ( udDWord ) m_wndForm );
				udPBase obj = GetProjectOrder( ), request = m_wndForm->GetObj( );
				obj->get( "status" )->ini( request->get( "status" )->toUInt( ) );
				obj->get( "direct" )->ini( request->get( "direct" )->toUInt( ) );
				obj->get( "manager2" )->ini( request->get( "manager2" )->toUInt( ) );

				wstring comment = L"ФИО: " + request->get( "fio" )->toWString( ) + L"\r\n"
					L"Контактные данные: " + request->get( "contact" )->toWString( ) + L"\r\n"
					L"Комментарий обращения: " + request->get( "comment" )->toWString( );
				obj->get( "comment" )->ini( comment );

				udPMorph ret = DialogForm( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), handler, obj );
				delete handler;
				udDELETE( ret );
			} else if ( lnk == m_lnkPrint ) {
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
				decl->get( "href" )->ini( wstring( L"request_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectRequest( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHRequest::ctrlidManager )->extlst->extlst;
				node1 = doc->CreateNode( L"manager_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHRequest::ctrlidDirect )->extlst->extlst;
				node1 = doc->CreateNode( L"direct_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHRequest::ctrlidState )->extlst->extlst;
				node1 = doc->CreateNode( L"state_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHRequest::ctrlidType )->extlst->extlst;
				node1 = doc->CreateNode( L"type_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHRequest::ctrlidResult )->extlst->extlst;
				node1 = doc->CreateNode( L"result_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Обращение" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\request_out.xsl", ( folder + wstring( L"\\request_out.xsl" ) ).data( ), FALSE );
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

	udInt CHRequest::GetListModel( ) {
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
		title = ( modal ? L"Добавить обращения" : L"Новое обращение" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "created_", L"Дата и время", "created" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "type_", L"Тип", "type" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "fio", L"Абонент" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "direct_", L"Направление", "direct" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "manager2_", L"Ответственный", "manager2" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltType, "type", NULL, "name", L"выберите тип", GetHTRequestList );
		m_wndModeList->AddInput( fltFio, "fio", NULL, font, 0 );
		m_wndModeList->AddExtlist( fltDirect, "direct", NULL, "name", L"выберите направление", GetHDirectList );
		m_wndModeList->AddExtlist( fltManager, "manager", NULL, "fio", L"выберите пользователя", GetHUserList );
		m_wndModeList->AddExtlist( fltOrder, "order", NULL, "employer_", L"выберите заявку", GetHOrderList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "order" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHRequest::GetListView( ) {
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
		info = m_wndModeList->GetInfo( fltType );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltFio );
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
		info = m_wndModeList->GetInfo( fltManager );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += 30 + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltOrder );
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

	udInt CHRequest::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddExtlist(
			ctrlidType, "type",
			NULL, "name", L"выберите тип", GetHTRequestList
			//NULL, "name", "pick_type", L"выберите тип",
			//"ud_trequest", GetProjectTRequest
		);
		udArrWString arrSelect;
		arrSelect.push_back( L"Открыто" );
		arrSelect.push_back( L"Закрыто" );
		arrSelect.push_back( L"Требует внимания" );
		m_wndForm->AddSelect( ctrlidStatus, "status", NULL, font, arrSelect );
		m_wndForm->AddExtlist(
			ctrlidState, "state",
			NULL, "name", L"выберите состояние", GetHSRequestList
			//NULL, "name", "pick_state", L"выберите состояние",
			//"ud_srequest", GetProjectSRequest
		);
		m_wndForm->AddInput( ctrlidFio, "fio", NULL, font, 0 );
		m_wndForm->AddText( ctrlidContact, "contact", NULL, font );
		m_wndForm->AddExtlist(
			ctrlidDirect, "direct",
			NULL, "name", L"выберите направление", GetHDirectList
			//NULL, "name", "pick_direct", L"выберите направление",
			//"ud_direct", GetProjectDirect
		);
		m_wndForm->AddExtlist(
			ctrlidManager, "manager2",
			NULL, "fio", L"выберите менеджера", GetHUserList
			//NULL, "fio", "pick_manager", L"выберите менеджера",
			//"ud_user", GetProjectUser
		);
		m_wndForm->AddExtlist(
			ctrlidResult, "result",
			NULL, "name", L"выберите результат", GetHRRequestList
			//NULL, "name", "pick_result", L"выберите результат",
			//"ud_rrequest", GetProjectRRequest
		);
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		m_wndForm->AddDate( ctrlidRemind, "remind", NULL, font );
		m_wndForm->AddText( ctrlidRemindc, "remindc", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHRequest::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 120 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkOrder->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkOrder->get( "x" )->ini( ( udInt ) 300 );
			m_lnkOrder->get( "y" )->ini( ( udInt ) 10 );
			m_lnkOrder->Create( m_wndForm->GetHWnd( ) );
			m_lnkOrder->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			//////////////////////////////////////////////////////////////////////////////////////
			m_lnkPrint->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrint->get( "x" )->ini( ( udInt ) m_lnkOrder->get( "x" )->toInt( ) + m_lnkOrder->get( "width" )->toInt( ) + 5 );
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
		title	= L"Обращение";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 169;
		w		= 200;
		h		= 16;
		title	= L"Тип обращения:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidType );
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
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidStatus );
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
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidState );
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

		x		= 492;
		y		= 184;
		w		= 95;
		h		= 16;
		title	= L"ФИО абонента:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		y = 178;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 390; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidFio );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Контактные данные";
		x		= 488;
		y		= 252;
		w		= 400;
		h		= 16;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		w = 400;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidContact );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 500;
		y		= 222;
		w		= 87;
		h		= 16;
		title	= L"Направление:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		y = 216;
		w = 400;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidDirect );
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
		x		= 47;
		y		= 227;
		w		= 200;
		h		= 16;
		title	= L"Результат обращения:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidResult );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 345;
		h		= 16;
		title	= L"Комментарий:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 278;
		y		= 162;
		w		= 169;
		h		= 16;
		title	= L"Дата/время напоминания:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		w = 169;
		h = 22;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidRemind );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 278;
		y		= 219;
		w		= 169;
		h		= 16;
		title	= L"Сообщение:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidRemindc );
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

	void CHRequest::ClickToolbar( string index ) {
		if ( index == "request" ) {
			this->SwitchToForm( );
		} else {
			udStateHandlerSimple::ClickToolbar( index );
		}
	} // void ClickToolbar

	udInt CHRequest::ToolbarAction( string index ) {
		if ( index == "request" ) {
			return 1;
		}
		return 0;
	} // udInt ToolbarAction

	udInt CHRequest::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "type" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите тип" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "state" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите состояние" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "manager2" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите ответственного менеджера" );
			return FormHandlerErrorFailed;
		}

		udDatetime date;
		date.load( );
		obj->get( "created" )->ini( date );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );

		return FormHandlerErrorOk;
	} // bool ValidateObj

	void CHRequest::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "type_" )->ini( wstring( L"" ) );
		tpl->get( "direct_" )->ini( wstring( L"" ) );
		tpl->get( "manager2_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHRequest::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "type" )->ini( ( udUInt ) 0 );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "order" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHRequest::LoadAfterSetObject( ) {
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

	void CHRequest::SwitchToForm( ) {
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

	void CHRequest::RequestList( ) {
		udStateHandlerSimple::RequestList( );
		datetime date;
		udPBase obj = NULL;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			date = obj->get( "created" )->toTime( );
			obj->get( "created_" )->ini( date.toWString( "%d.%m.%Y" ) );
		}
	} // void RequestList

} // namespace UdSDK
