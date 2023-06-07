
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void OrderServiceAssignTpl( udPBase obj ) {
		obj->get( "#name" )->ini( wstring( L"" ) );
	} // void OrderServiceAssignTpl

	bool OrderSpecLoadEmployer( udPBase main, udPBase& obj, udPWndModeForm form, udPSocket sock ) {
		if ( main->get( "emptype" )->toInt( ) == 1 ) {
			udDELETE( obj );
			obj = new udProjectClientUr;
			form->GetInfo( udHOrder::ctrlidEmployer )->extlst->extlst->SetAttr( "name" );
			return true;
		}
		return false;
	} // bool OrderSpecLoadEmployer

	void OrderAfterPickEmployer( udPBase obj, udPListHandler handler ) {
		udPMorph opt = handler->GetLstOpt( );
		udPWndModeForm wndForm = ( udPWndModeForm ) handler->GetLstOpt( )->get( "wndForm" )->toDWord( );
		udInt state = opt->get( "state" )->toInt( );
		if ( state == udHClient::stateFiz ) {
			wndForm->GetInfo( udHOrder::ctrlidEmployer )->extlst->extlst->SetAttr( "fio" );
		} else {
			wndForm->GetInfo( udHOrder::ctrlidEmployer )->extlst->extlst->SetAttr( "name" );
		}
	} // void OrderAfterPickEmployer

	void OrderAfterPickEmployer2( udPBase obj, udPListHandler handler ) {
		udPMorph opt = handler->GetLstOpt( );
		udPWndModeList wndList = ( udPWndModeList ) handler->GetLstOpt( )->get( "wndList" )->toDWord( );
		udInt state = opt->get( "state" )->toInt( );
		if ( state == udHClient::stateFiz ) {
			wndList->GetInfo( udHOrder::fltEmployer )->extlst->extlst->SetAttr( "fio" );
			wndList->GetObjFilter( )->get( "emptype" )->ini( ( udInt ) udProjectOrder::ORDER_EMPLOYER_CLIENT );
		} else {
			wndList->GetInfo( udHOrder::fltEmployer )->extlst->extlst->SetAttr( "name" );
			wndList->GetObjFilter( )->get( "emptype" )->ini( ( udInt ) udProjectOrder::ORDER_EMPLOYER_CLIENTUR );
		}
	} // void OrderAfterPickEmployer2

	CHOrder::CHOrder( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_order";
		m_szUpdIndex	= "id";
		m_szName		= "order";
		m_fnGetObject	= GetProjectOrder;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHOrderForm;

		this->AddExtraLoadInfo( ctrlidManager, GetProjectUser, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidState, GetProjectSOrder, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidEmployer, GetProjectClient, ( udInt ) extra_load_extlst,
			NULL, NULL, OrderSpecLoadEmployer );
		this->AddExtraLoadInfo( ctrlidDirect, GetProjectDirect, ( udInt ) extra_load_extlst );
		this->AddExtraLoadInfo( ctrlidRoute, GetProjectOrderRoute, ( udInt ) extra_load_frmlst );
		this->AddExtraLoadInfo( ctrlidService, GetProjectOrderService, ( udInt ) extra_load_frmlst, OrderServiceAssignTpl );
		this->AddExtraLoadInfo( ctrlidApp, GetProjectAttach, ( udInt ) extra_load_frmlst );

		m_lnkTour = new udWndLink( ( udUInt ) 18 );
		m_lnkTour->get( "title" )->ini( wstring( L"Создать тур" ) );
		m_lnkTour->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );

		m_lnkPartnerReq = new udWndLink( ( udUInt ) 18 );
		m_lnkPartnerReq->get( "title" )->ini( wstring( L"Создать запрос партнеру" ) );
		m_lnkPartnerReq->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );

		m_lnkPrint = new udWndLink( ( udUInt ) 18 );
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать заявку" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHOrder::CHOrder( const CHOrder& objCopy ) {
	}

	CHOrder::~CHOrder( ) {
		udDELETE( m_lnkTour );
		udDELETE( m_lnkPartnerReq );
		udDELETE( m_lnkPrint );
	}

	CHOrder& CHOrder::operator =( const CHOrder& objCopy ) {
		return *this;
	}

	void CHOrder::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkTour->Show( );
				m_lnkPartnerReq->Show( );
				m_lnkPrint->Show( );
			} else {
				m_lnkTour->Hide( );
				m_lnkPartnerReq->Hide( );
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHOrder::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkTour ) {
				udPFormHandler handler = GetHTourForm( );
				handler->GetFrmOpt( )->get( "order_init" )->ini( ( udDWord ) m_wndForm );
				udPBase obj = GetProjectTour( ), order = m_wndForm->GetObj( );
				obj->get( "status" )->ini( order->get( "status" )->toUInt( ) );
				obj->get( "direct" )->ini( order->get( "direct" )->toUInt( ) );
				obj->get( "employer" )->ini( order->get( "employer" )->toUInt( ) );
				obj->get( "emptype" )->ini( order->get( "emptype" )->toUInt( ) );
				obj->get( "date1" )->ini( order->get( "date1" )->toTime( ) );
				obj->get( "date2" )->ini( order->get( "date2" )->toTime( ) );
				obj->get( "manager2" )->ini( order->get( "manager2" )->toUInt( ) );

				wstring comment = L"Количество человек: " + order->get( "amount" )->toWString( ) + L"\r\n"
					L"Комментарий заявки:\r\n" + order->get( "comment" )->toWString( );
				obj->get( "comment" )->ini( comment );

				udPMorph ret = DialogForm( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), handler, obj );
				delete handler;
				udDELETE( ret );
			} else if ( lnk == m_lnkPartnerReq ) {
				udPFormHandler handler = GetHPartnerReqForm( );
				handler->GetFrmOpt( )->get( "order_init" )->ini( ( udDWord ) m_wndForm );
				udPBase obj = GetProjectPartnerReq( ), order = m_wndForm->GetObj( );

				wstring text = L"Число мест: " + order->get( "amount" )->toWString( ) + L"\r\n"
					L"Желаемая дата отъезда: " + order->get( "date1" )->toTime( ).toWString( "%Y-%m-%d" ) + L"\r\n"
					L"Желаемая дата приезда: " + order->get( "date1" )->toTime( ).toWString( "%Y-%m-%d" ) + L"\r\n";
				udPDwAscBase items = NULL;

				items = m_wndForm->GetInfo( ctrlidRoute )->frmlst->GetItems( );
				if ( !items->empty( ) ) {
					text += L"Маршрут:\r\n";
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						text += i->second->get( "name" )->toWString( );
						text += L"\r\n";
					}
					text += L"\r\n";
				}
				items = m_wndForm->GetInfo( ctrlidService )->frmlst->GetItems( );
				if ( !items->empty( ) ) {
					text += L"Услуги:\r\n";
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						text += i->second->get( "#name" )->toWString( );
						text += L"\r\n";
					}
					text += L"\r\n";
				}

				obj->get( "text" )->ini( text );
				obj->get( "comment" )->ini( order->get( "comment" )->toWString( ) );

				udPMorph ret = DialogForm( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ), handler, obj );
				delete handler;
				udDELETE( ret );
			} else if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
				udPMorph decl = NULL;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
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
				decl->get( "href" )->ini( wstring( L"order_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectOrder( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				doc->SetRoot( node );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHOrder::ctrlidManager )->extlst->extlst;
				node1 = doc->CreateNode( L"manager_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHOrder::ctrlidState )->extlst->extlst;
				node1 = doc->CreateNode( L"state_txt" );
				node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHOrder::ctrlidEmployer )->extlst->extlst;
				node1 = doc->CreateNode( L"employer_txt" );
				if ( extlst->GetAttr( ) == "fio" ) {
					node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]>" );
				} else {
					node1->SetValue( L"<![CDATA[" + extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				}
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				extlst = m_wndForm->GetInfo( udHOrder::ctrlidDirect )->extlst->extlst;
				node1 = doc->CreateNode( L"direct_txt" );
				node1->SetValue( L"<![CDATA[" +  extlst->GetObj( )->get( "name" )->toWString( ) + L"]]>" );
				node->InsertChild( node1 );
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"route_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHOrder::ctrlidRoute )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = i->second->toXml( doc );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node1 = doc->CreateNode( L"service_list" );
				node->InsertChild( node1 );
				items = m_wndForm->GetInfo( udHOrder::ctrlidService )->frmlst->GetItems( );
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					node2 = doc->CreateNode( L"orderservice" );
					node2->SetValue( L"<![CDATA[" +  i->second->get( "#name" )->toWString( ) + L"]]>" );
					//node2 = i->second->toXml( doc );
					node1->InsertChild( node2 );
				}
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Заявка" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\order_out.xsl", ( folder + wstring( L"\\order_out.xsl" ) ).data( ), FALSE );
				ExtOpenFile( folder + L"\\out.xml" );
				//////////////////////////////////////////////////////////////////////////////////
				xml.clear( );
				udDELETE( doc );
				udDELETE( obj );
				//////////////////////////////////////////////////////////////////////////////////
			} else {
				udStateHandlerSimple::LinkClick( lnk );
			}
		} else {
			udStateHandlerSimple::LinkClick( lnk );
		}
	} // void LinkClick

	void CHOrder::AfterLoadFilterData( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		udDWord amount = 0;
		amount = filter->get( "amount1" )->toDWord( );
		filter->get( "amount1" )->ini( amount );
		amount = filter->get( "amount2" )->toDWord( );
		filter->get( "amount2" )->ini( amount );
	} // void AfterLoadFilterData

	udInt CHOrder::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		m_pLstOpt->get( "wndList" )->ini( ( udDWord ) m_wndModeList );
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
		title = ( modal ? L"Добавить заявку" : L"Новая заявка" );
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
		wndInsLst->AddAttr( "manager2_", L"Ответственный", "manager2" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "status_", L"Статус", "status" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "amount", L"Кол-во" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "employer_", L"Заказчик", "employer" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "direct_", L"Направление", "direct" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltManager, "manager", NULL, "fio", L"выберите менеджера", GetHUserList );
		m_wndModeList->AddInput( fltAmount1, "amount1", NULL, font, ES_NUMBER );
		m_wndModeList->AddInput( fltAmount2, "amount2", NULL, font, ES_NUMBER );
		m_wndModeList->AddExtlist( fltEmployer, "employer", NULL, "fio", L"выберите клиента", GetHClientList, m_pLstOpt, OrderAfterPickEmployer2 );
		m_wndModeList->AddExtlist( fltDirect, "direct", NULL, "name", L"выберите направление", GetHDirectList );
		m_wndModeList->AddExtlist( fltRequest, "request", NULL, "fio", L"выберите обращение", GetHRequestList );
		m_wndModeList->AddExtlist( fltTour, "tour", NULL, "name", L"выберите тур", GetHTourList );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		datetime date;
		filter->get( "date1" )->ini( date );
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "amount1" )->ini( ( udDWord ) 0 );
		filter->get( "amount2" )->ini( ( udDWord ) 1000000 );
		filter->get( "employer" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "request" )->ini( ( udUInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHOrder::GetListView( ) {
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
		info = m_wndModeList->GetInfo( fltManager );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 100; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltAmount1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 100; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltAmount2 );
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
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltEmployer );
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
		info = m_wndModeList->GetInfo( fltRequest );
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

	udInt CHOrder::GetFormModel( ) {
		m_pFrmOpt->get( "wndForm" )->ini( ( udDWord ) m_wndForm );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddExtlist(
			ctrlidManager, "manager2",
			NULL, "fio", L"выберите менеджера", GetHUserList
			//NULL, "fio", "pick_manager2", L"выберите менеджера",
			//"ud_user", GetProjectUser
		);
		udArrWString arrSelect;
		arrSelect.push_back( L"Открыта" );
		arrSelect.push_back( L"Закрыта" );
		arrSelect.push_back( L"Требует внимания" );
		m_wndForm->AddSelect( ctrlidStatus, "status", NULL, font, arrSelect );
		m_wndForm->AddExtlist(
			ctrlidState, "state",
			NULL, "name", L"выберите состояние", GetHSOrderList
			//NULL, "name", "pick_state", L"выберите состояние",
			//"ud_sorder", GetProjectSOrder
		);
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		m_wndForm->AddFormlist( ctrlidApp, "app", NULL, g_pApp->GetUniqueTmpFolder( ) )->AddAttr( "name" );
		m_wndForm->AddInput( ctrlidAmount, "amount", NULL, font, ES_NUMBER );
		m_wndForm->AddFormlist( ctrlidRoute, "route", NULL )->AddAttr( "name" );
		m_wndForm->AddExtlist(
			ctrlidEmployer, "employer",
			NULL, "fio", L"выберите клиента", GetHClientList, m_pFrmOpt, OrderAfterPickEmployer
			//NULL, "fio", "pick_employer", L"выберите клиента",
			//"ud_client", GetProjectClient
		);
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddFormlist( ctrlidService, "lserv", NULL )->AddAttr( "#name" );
		m_wndForm->AddExtlist(
			ctrlidDirect, "direct",
			NULL, "name", L"выберите направление", GetHDirectList
			//NULL, "name", "pick_direct", L"выберите направление",
			//"ud_direct", GetProjectDirect
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHOrder::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 1024 );
			m_wndForm->get( "height" )->ini( ( udInt ) 820 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkTour->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkTour->get( "x" )->ini( ( udInt ) 300 );
			m_lnkTour->get( "y" )->ini( ( udInt ) 10 );
			m_lnkTour->Create( m_wndForm->GetHWnd( ) );
			m_lnkTour->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			//////////////////////////////////////////////////////////////////////////////////////
			m_lnkPartnerReq->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPartnerReq->get( "x" )->ini( ( udInt ) ( m_lnkTour->get( "x" )->toInt( ) + m_lnkTour->get( "width" )->toInt( ) + 5 ) );
			m_lnkPartnerReq->get( "y" )->ini( ( udInt ) 10 );
			m_lnkPartnerReq->Create( m_wndForm->GetHWnd( ) );
			m_lnkPartnerReq->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			//////////////////////////////////////////////////////////////////////////////////////
			m_lnkPrint->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrint->get( "x" )->ini( ( udInt ) ( m_lnkPartnerReq->get( "x" )->toInt( ) + m_lnkPartnerReq->get( "width" )->toInt( ) + 5 ) );
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
		title	= L"Заявка";
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
		y		= 114;
		w		= 200;
		h		= 16;
		title	= L"Ответственный менеджер:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidManager );
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
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidStatus );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 169;
		w		= 200;
		h		= 16;
		title	= L"Состояние:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		y += h + 2;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidState );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 221;
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
		x		= 488;
		y		= 337;
		w		= 511;
		h		= 30;
		title	= L"Приложения:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidApp );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 511;
		y		= 230;
		w		= 76;
		h		= 16;
		title	= L"Число мест:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		w = 76;
		y = 224;
		h = 15;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidAmount );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 337;
		w		= 418;
		h		= 30;
		title	= L"Маршрут:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidRoute );
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

		x		= 695;
		y		= 230;
		w		= 64;
		h		= 16;
		title	= L"Заказчик:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		y = 224;
		w = 226;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidEmployer );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 497;
		y		= 270;
		w		= 90;
		h		= 16;
		title	= L"Дата отъезда:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		w = 90;
		y = 266;
		h = 22;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate1 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 688;
		y		= 270;
		w		= 90;
		h		= 16;
		title	= L"Дата приезда:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		w = 90;
		y = 266;
		h = 22;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate2 );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 47;
		y		= 465;
		w		= 953;
		h		= 30;
		title	= L"Примерный список услуг:";
		udRenderer::RenderHeaderRect( gdi, title, 16,
			x, y, w, h, 1, Color::MakeARGB( 255, 211, 211, 211 ),
			Color::MakeARGB( 255, 0, 0, 0 ), Color::MakeARGB( 255, 232, 232, 232 ),
			10, 5, 10, 1 );

		y += h + 2;
		//w = 400;
		h = 64;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidService );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x		= 500;
		y		= 184;
		w		= 87;
		h		= 16;
		title	= L"Направление:";
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel );

		x += w;
		y = 178;
		w = 398;
		h = 28;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = w; box->h = h;
		info = m_wndForm->GetInfo( ctrlidDirect );
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

	void CHOrder::ClickToolbar( string index ) {
		if ( index == "order" ) {
			this->SwitchToForm( );
		} else {
			udStateHandlerSimple::ClickToolbar( index );
		}
	} // void ClickToolbar

	udInt CHOrder::ToolbarAction( string index ) {
		if ( index == "order" ) {
			return 1;
		}
		return 0;
	} // udInt ToolbarAction

	udInt CHOrder::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "manager2" )->toUInt( )  == 0 ) {
			mb::err( hWnd, L"Введите ответственного менеджера" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "amount" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите число мест" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "employer" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите заказчика" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "direct" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите направление" );
			return FormHandlerErrorFailed;
		}

		//udPDwAscBase route = m_frmlstRoute->GetItems( );
		/*if ( !route || route->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите маршрут" );
			return false;
		}*/
		//udPDwAscBase service = m_frmlstService->GetItems( );
		/*if ( !service || service->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите услуги" );
			return false;
		}*/
		//udPDwAscBase attach = m_frmlstAttach->GetItems( );
		/*if ( !attach || attach->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите приложение" );
			return false;
		}*/
		
		udDatetime date;
		date.load( );
		obj->get( "created" )->ini( date );
		obj->get( "manager" )->ini( g_pApp->GetUser( )->get( "id" )->toUInt( ) );

		if ( m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst->GetAttr( ) == "name" ) {
			obj->get( "emptype" )->ini( ( udInt ) 1 );
		} else {
			obj->get( "emptype" )->ini( ( udInt ) 0 );
		}

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHOrder::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			udPFormHandler handler = GetHOrderRouteForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectOrderRoute;
				obj->apply( add );
				add->get( "order" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPFormHandler handler = GetHOrderServiceForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectOrderService;
				obj->apply( add );
				add->get( "order" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				wstring date1 = L"", date2 = L"";
				udDatetime date = obj->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );

				add->get( "#name" )->ini(
					date1 + L"/" + date2 + L", " +
					obj->get( "type_name" )->toWString( ) + L", " +
					obj->get( "city_name" )->toWString( ) + L", \"" +
					obj->get( "comment" )->toWString( ) + L"\""
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
				add->get( "type" )->ini( udProjectAttach::ATTACH_ORDER );
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

	void CHOrder::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			udPBase tmp = new udProjectOrderRoute;
			obj->apply( tmp );
			udPFormHandler handler = GetHOrderRouteForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
				delete obj1;
			}
			delete handler;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPBase tmp = new udProjectOrderService;
			obj->apply( tmp );
			udPFormHandler handler = GetHOrderServiceForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
				wstring date1 = L"", date2 = L"";
				udDatetime date = obj1->get( "date1" )->toTime( );
				date1 = date.toWString( "%Y-%m-%d" );
				date = obj1->get( "date2" )->toTime( );
				date2 = date.toWString( "%Y-%m-%d" );

				obj->get( "#name" )->ini(
					date1 + L"/" + date2 + L", " +
					obj1->get( "type_name" )->toWString( ) + L", " +
					obj1->get( "city_name" )->toWString( ) + L", \"" +
					obj->get( "comment" )->toWString( ) + L"\""
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
					wstring wBase = wnd->GetFolder( ) + L"\\",
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

	bool CHOrder::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidRoute )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить пункт маршрута?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectOrderRoute;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить услугу из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectOrderService;
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

	void CHOrder::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "manager2_" )->ini( wstring( L"" ) );
		tpl->get( "employer_" )->ini( wstring( L"" ) );
		tpl->get( "direct_" )->ini( wstring( L"" ) );
		tpl->get( "status_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHOrder::ExtraDataSending( udPSocket sock, udPBase obj ) {
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
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "request_init" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "request_init" )->toDWord( );
			assert( wndForm );
			udPBase obj = wndForm->GetObj( );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			data.clear( );
			data.add( udProjectObjectLink::objlnkRequest );
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Send( data );
		} else {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		}
	} // void ExtraDataSending

	void CHOrder::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
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
		udPExtList extlst = m_wndForm->GetInfo( ctrlidEmployer )->extlst->extlst;
		if ( obj->get( "emptype" )->toInt( ) == udProjectOrder::ORDER_EMPLOYER_CLIENT ) {
			extlst->SetAttr( "fio" );
		} else {
			extlst->SetAttr( "name" );
		}
		extlst->Render( );
		extlst->Flush( );
	} // void ExtraDataRecieve

	void CHOrder::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		datetime date;
		filter->get( "date1" )->ini( date );
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "manager" )->ini( ( udUInt ) 0 );
		filter->get( "amount1" )->ini( ( udDWord ) 0 );
		filter->get( "amount2" )->ini( ( udDWord ) 1000000 );
		filter->get( "employer" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) 0 );
		filter->get( "direct" )->ini( ( udUInt ) 0 );
		filter->get( "request" )->ini( ( udUInt ) 0 );
		filter->get( "tour" )->ini( ( udUInt ) 0 );
	} // void ClearFilter

	void CHOrder::LoadAfterSetObject( ) {
		if ( m_pFrmOpt->isset( "request_init" ) ) { // была создана модальная форма из обращения
			udPBase obj = m_wndForm->GetObj( );
			udUInt id = 0;
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "request_init" )->toDWord( );
			assert( wndForm );

			id = obj->get( "direct" )->toUInt( );
			if ( id > 0 ) {
				udPBase ret = new udProjectDirect;
				wndForm->GetInfo( udHRequest::ctrlidDirect )->extlst->extlst->GetObj( )->apply( ret );
				m_wndForm->GetInfo( ctrlidDirect )->extlst->extlst->SetObj( ret );
			}

			id = obj->get( "manager2" )->toUInt( );
			if ( id > 0 ) {
				udPBase ret = new udProjectDirect;
				wndForm->GetInfo( udHRequest::ctrlidManager )->extlst->extlst->GetObj( )->apply( ret );
				m_wndForm->GetInfo( ctrlidManager )->extlst->extlst->SetObj( ret );
			}
		}
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

	void CHOrder::SwitchToForm( ) {
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

	void CHOrder::RequestList( ) {
		udStateHandlerSimple::RequestList( );
		datetime date;
		udPBase obj = NULL;
		for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
			obj = *i;
			date = obj->get( "created" )->toTime( );
			obj->get( "created_" )->ini( date.toWString( "%d.%m.%Y" ) );
		}
	} // void RequestList






	CHOrderService::CHOrderService( ) : udFormHandler( ) {
	}

	CHOrderService::~CHOrderService( ) {
	}

	udInt CHOrderService::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddExtlist( ctrlidType, "type", NULL, "name", L"выберите тип", GetHTServiceList );
		m_wndForm->AddExtlist( ctrlidCity, "city", NULL, "name", L"выберите город", GetHCityList );
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHOrderService::GetFormView( ) {
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
		title	= L"Комментарий";
		x		= 32;
		w		= lcol;
		h		= 60;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		y += h + m + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHOrderService::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );

		if ( obj->get( "type" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите тип" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "city" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите город" );
			return FormHandlerErrorFailed;
		}

		ModeFormControlInfo *info = NULL;
		info = m_wndForm->GetInfo( ctrlidType );
		obj->get( "type_name" )->ini( info->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		info = m_wndForm->GetInfo( ctrlidCity );
		obj->get( "city_name" )->ini( info->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );

		return FormHandlerErrorOk;;
	} // udInt ValidateForm

	udInt CHOrderService::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHOrderService::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHOrderService::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

	void CHOrderService::LoadAfterSetObject( ) {
		udPBase obj = m_wndForm->GetObj( );
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "type" )->toUInt( );
		if ( id > 0 ) { // подгрузка типа услуги
			udPBase ret = LoadById( "tservice", id, GetProjectTService );
			if ( ret ) {
				m_wndForm->GetInfo( ctrlidType )->extlst->extlst->SetObj( ret );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "city" )->toUInt( );
		if ( id > 0 ) { // подгрузка города услуги
			udPBase ret = LoadById( "city", id, GetProjectCity );
			if ( ret ) {
				m_wndForm->GetInfo( ctrlidCity )->extlst->extlst->SetObj( ret );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadAfterSetObject

} // namespace UdSDK
