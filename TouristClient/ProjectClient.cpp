
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void ClientAfterLoadClDoc( udArrBase& arr ) {
		udPBase obj = NULL;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			obj->get( "#name" )->ini(
				obj->get( "series" )->toWString( ) + L" " +
				obj->get( "number" )->toWString( )
			);
		}
	} // void ClientAfterLoadClDoc

	CHClient::CHClient( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_client";
		m_szUpdIndex	= "id";
		m_szName		= "client";
		m_fnGetObject	= GetProjectClient;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHClientForm;
		m_iClState		= stateFiz;

		this->AddExtraLoadInfo( ctrlidDoc, GetProjectClDoc, ( udInt ) extra_load_frmlst, NULL, ClientAfterLoadClDoc );
		this->AddExtraLoadInfo( ctrlidApp, GetProjectAttach, ( udInt ) extra_load_frmlst );

		m_lnkPrintFiz = new udWndLink;
		m_lnkPrintFiz->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrintFiz->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );

		m_lnkPrintUr = new udWndLink;
		m_lnkPrintUr->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrintUr->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHClient::CHClient( const CHClient& objCopy ) {
	}

	CHClient::~CHClient( ) {
		udDELETE( m_lnkPrintFiz );
		udDELETE( m_lnkPrintUr );
	}

	CHClient& CHClient::operator =( const CHClient& objCopy ) {
		return *this;
	}

	void CHClient::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( m_iClState == stateFiz ) {
			if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
				if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
					m_lnkPrintFiz->Show( );
				} else {
					m_lnkPrintFiz->Hide( );
				}
			}
		} else if ( m_iClState == stateUr ) {
			if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
				if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
					m_lnkPrintUr->Show( );
				} else {
					m_lnkPrintUr->Hide( );
				}
			}
		}
	} // void Show

	void CHClient::LinkClick( udPWndLink lnk ) {
		bool bCallDefault = true;
		if ( m_iClState == stateFiz ) {
			if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
				( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
			) {
				if ( lnk == m_lnkPrintFiz ) {
					bCallDefault = false;
					//////////////////////////////////////////////////////////////////////////////
					udPXmlDoc doc = new udXmlDoc;
					string xml;
					udPMorph decl = NULL;
					udPXmlNode node = NULL, node1 = NULL, node2 = NULL;
					wstring folder = g_pApp->GetUniqueTmpFolder( );
					udPBase obj = NULL;
					udPDwAscBase items = NULL;
					//////////////////////////////////////////////////////////////////////////////
					decl = new udMorph;
					decl->get( "version" )->ini( wstring( L"1.0" ) );
					decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
					doc->AddDeclaration( L"xml", decl );
					udDELETE( decl );
					//////////////////////////////////////////////////////////////////////////////
					decl = new udMorph;
					decl->get( "type" )->ini( wstring( L"text/xsl" ) );
					decl->get( "href" )->ini( wstring( L"client_out.xsl" ) );
					doc->AddDeclaration( L"xml-stylesheet", decl );
					udDELETE( decl );
					//////////////////////////////////////////////////////////////////////////////
					obj = GetProjectClient( );
					obj->apply( m_wndForm->GetObj( ), true, true );
					node = obj->toXml( doc );
					//////////////////////////////////////////////////////////////////////////////
					node1 = doc->CreateNode( L"doc_list" );
					node->InsertChild( node1 );
					items = m_wndForm->GetInfo( udHClient::ctrlidDoc )->frmlst->GetItems( );
					for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
						node2 = doc->CreateNode( L"clientdoc" );
						node2->SetValue( L"<![CDATA[" +  i->second->get( "#name" )->toWString( ) + L"]]>" );
						//node2 = i->second->toXml( doc );
						node1->InsertChild( node2 );
					}
					//////////////////////////////////////////////////////////////////////////////
					node->SetAttribute( L"object_title", L"Клиент физ лицо" );
					doc->SetRoot( node );
					doc->toString( xml );
					WriteToFile( folder + L"\\out.xml", xml );
					CopyFile( L".\\xsl\\client_out.xsl", ( folder + wstring( L"\\client_out.xsl" ) ).data( ), FALSE );
					ExtOpenFile( folder + L"\\out.xml" );
					//////////////////////////////////////////////////////////////////////////////
					xml.clear( );
					udDELETE( obj );
					udDELETE( doc );
					//////////////////////////////////////////////////////////////////////////////
				}
			}
		} else if ( m_iClState == stateUr ) {
			if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
				( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
			) {
				if ( lnk == m_lnkPrintUr ) {
					bCallDefault = false;
					//////////////////////////////////////////////////////////////////////////////
					udPXmlDoc doc = new udXmlDoc;
					string xml;
					udPMorph decl = NULL;
					udPXmlNode node = NULL, node1 = NULL;
					wstring folder = g_pApp->GetUniqueTmpFolder( );
					udPBase obj = NULL;
					udPDwAscBase items = NULL;
					//////////////////////////////////////////////////////////////////////////////
					decl = new udMorph;
					decl->get( "version" )->ini( wstring( L"1.0" ) );
					decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
					doc->AddDeclaration( L"xml", decl );
					udDELETE( decl );
					//////////////////////////////////////////////////////////////////////////////
					decl = new udMorph;
					decl->get( "type" )->ini( wstring( L"text/xsl" ) );
					decl->get( "href" )->ini( wstring( L"clientur_out.xsl" ) );
					doc->AddDeclaration( L"xml-stylesheet", decl );
					udDELETE( decl );
					//////////////////////////////////////////////////////////////////////////////
					obj = GetProjectClientUr( );
					obj->apply( m_wndForm->GetObj( ), true, true );
					node = obj->toXml( doc );
					//////////////////////////////////////////////////////////////////////////////
					node->SetAttribute( L"object_title", L"Клиент юр лицо" );
					doc->SetRoot( node );
					doc->toString( xml );
					WriteToFile( folder + L"\\out.xml", xml );
					CopyFile( L".\\xsl\\clientur_out.xsl", ( folder + wstring( L"\\clientur_out.xsl" ) ).data( ), FALSE );
					ExtOpenFile( folder + L"\\out.xml" );
					//////////////////////////////////////////////////////////////////////////////
					xml.clear( );
					udDELETE( obj );
					udDELETE( doc );
					//////////////////////////////////////////////////////////////////////////////
				}
			}
		}
		if ( bCallDefault ) {
			udStateHandlerSimple::LinkClick( lnk );
		}
	} // void LinkClick

	void CHClient::RequestList( ) {
		if ( m_pLstOpt->isset( "pick_tourist" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pLstOpt->get( "pick_tourist" )->toDWord( );
			udPDwAscBase items = wndForm->GetInfo( udHTour::ctrlidTourist )->frmlst->GetItems( );
			if ( !items->empty( ) ) {
				map< udUInt, udUInt > ids;
				string log = "\npick_tourist_ids:\n";
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					ids[ i->second->get( "client" )->toUInt( ) ] = 1;
					log += i->second->get( "client" )->toString( ) + "\n";
				}
				g_pApp->GetLog( )->Write( log );
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					// ListRequest
					udBinaryData data;
					data.add( m_szName );
					data.add( ( udInt ) opidGet2 );
					data.add( m_pPager->GetPage( ) );
					data.add( m_pPager->GetPageSize( ) );
					//data.add( m_szOrder );
					data.add( m_szOrderBy );
					data.add( m_iOrderType );
					// добавляем набор ID клиентов
					data.add( ids.size( ) );
					for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
						data.add( i->first );
					}

					string log = "\nфильтр, ";
					m_wndModeList->GetObjFilter( )->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );
					m_wndModeList->GetObjFilter( )->dump( data );

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
						ReadInstances( sock, m_arrObject, m_fnGetObject, tpl );
						delete tpl;
					}

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				}
			}
		} else if ( m_pLstOpt->isset( "pick_leader" ) ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pLstOpt->get( "pick_leader" )->toDWord( );
			udPDwAscBase items = wndForm->GetInfo( udHTouristList::ctrlidClient )->frmlst->GetItems( );
			if ( !items->empty( ) ) {
				map< udUInt, udUInt > ids;
				string log = "\npick_leader_ids:\n";
				for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
					ids[ i->second->get( "client" )->toUInt( ) ] = 1;
					log += i->second->get( "client" )->toString( ) + "\n";
				}
				g_pApp->GetLog( )->Write( log );
				udPSocket sock = BeginRequest( );
				if ( sock ) {
					// ListRequest
					udBinaryData data;
					data.add( m_szName );
					data.add( ( udInt ) opidGet2 );
					data.add( m_pPager->GetPage( ) );
					data.add( m_pPager->GetPageSize( ) );
					//data.add( m_szOrder );
					data.add( m_szOrderBy );
					data.add( m_iOrderType );
					// добавляем набор ID клиентов
					data.add( ids.size( ) );
					for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
						data.add( i->first );
					}

					string log = "\nфильтр, ";
					m_wndModeList->GetObjFilter( )->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );
					m_wndModeList->GetObjFilter( )->dump( data );

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
						ReadInstances( sock, m_arrObject, m_fnGetObject, tpl );
						delete tpl;
					}

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					sock->Shutdown( );
					sock->Close( );
					delete sock;
				}
			}
		} else {
			udStateHandlerSimple::RequestList( );
			if ( m_iClState == stateFiz ) {
				wstring date, set;
				udPBase obj = NULL;
				//YYYY-MM-DD
				string log;
				for( udArrBase::iterator i = m_arrObject.begin( ); i != m_arrObject.end( ); ++i ) {
					obj = *i;
					set = L"";
					date = obj->get( "born" )->toWString( );
					set += date.substr( 8, 2 );
					set += L".";
					set += date.substr( 5, 2 );
					set += L".";
					set += date.substr( 0, 4 );
					obj->get( "born_" )->ini( set );
				}
			}
		}
	} // udInt RequestList

	udInt CHClient::GetListModel( ) {
		if ( m_iClState == stateUr ) {
			return ListUrModel( );
		}
		return ListFizModel( );
	} // udInt GetListModel

	udInt CHClient::GetListView( ) {
		if ( m_iClState == stateUr ) {
			return ListUrView( );
		}
		return ListFizView( );
	} // udInt GetListView

	udInt CHClient::ListFizModel( ) {
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
		if ( !m_pLstOpt->isset( "onlyfiz" ) ) {
			title = L"Юр. лица";
			url = "clientur";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить клиента" : L"Новый клиент" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "fio", L"ФИО" )->get( "width" )->ini( ( udInt ) 250 );
		wndInsLst->AddAttr( "born_", L"Дата рождения", "born" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltFio, "fio", NULL, font, 0 );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font, TRUE );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font, TRUE );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "fio" )->ini( wstring( L"" ) );
		filter->get( "date1" )->ini( wstring( L"" ) );
		filter->get( "date2" )->ini( wstring( L"" ) );
		/*datetime date;
		filter->get( "date1" )->ini( date );
		date.load( );
		filter->get( "date2" )->ini( date );*/
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt ListFizModel

	udInt CHClient::ListFizView( ) {
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
		//////////////////////////////////////////////////////////////////////////////////////
		if ( !m_pLstOpt->isset( "onlyfiz" ) ) {
			if ( modal ) {
				x = m;
				itLnk = arrLink->find( 0 );
			} else {
				itLnk = arrLink->find( 1 );
			}
			lnk = itLnk->second;
			lnk->get( "style" )->ini( style );
			lnk->get( "x" )->ini( x );
			lnk->get( "y" )->ini( y );
			lnk->CalculateWidthHeight( hWnd );
			x += lnk->get( "width" )->toInt( ) + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pLstOpt->isset( "onlyfiz" ) ) {
			if ( modal ) {
				x = m;
				itLnk = arrLink->find( 0 );
			} else {
				itLnk = arrLink->find( 1 );
			}
		} else {
			if ( modal ) {
				itLnk = arrLink->find( 1 );
			} else {
				itLnk = arrLink->find( 2 );
			}
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
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltFio );
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
		if ( modal ) {
			x = m;
			y += 30 + m;
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
	} // udInt ListFizView

	udInt CHClient::ListUrModel( ) {
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST_UR + 1;
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
		title = L"Физ. лица";
		url = "client";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить клиента" : L"Новый клиент" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "name", L"Наименование" )->get( "width" )->ini( ( udInt ) 250 );
		wndInsLst->AddAttr( "cfio", L"Контактное лицо" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltUrName, "name", NULL, font, 0 );
		m_wndModeList->AddInput( fltUrCfio, "cfio", NULL, font, 0 );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "cfio" )->ini( wstring( L"" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt ListUrModel

	udInt CHClient::ListUrView( ) {
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
			itLnk = arrLink->find( 0 );
		} else {
			itLnk = arrLink->find( 1 );
		}
		lnk = itLnk->second;
		lnk->get( "style" )->ini( style );
		lnk->get( "x" )->ini( x );
		lnk->get( "y" )->ini( y );
		lnk->CalculateWidthHeight( hWnd );
		x += lnk->get( "width" )->toInt( ) + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			itLnk = arrLink->find( 1 );
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
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltUrName );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		if ( modal ) {
			x = m;
			y += box->h + box->padT + box->padB + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltUrCfio );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y += 30 + m;
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
	} // udInt ListUrView

	udInt CHClient::GetFormModel( ) {
		if ( m_iClState == stateUr ) {
			return FormUrModel( );
		}
		return FormFizModel( );
	} // udInt GetFormModel

	udInt CHClient::GetFormView( ) {
		if ( m_iClState == stateUr ) {
			return FormUrView( );
		}
		return FormFizView( );
	} // udInt GetFormView

	udInt CHClient::FormFizModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidFio, "fio", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidFiolatin, "fiolatin", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidBorn, "born", NULL, font, TRUE );
		udArrWString arrSelect;
		arrSelect.push_back( L"Мужской" );
		arrSelect.push_back( L"Женский" );
		m_wndForm->AddSelect( ctrlidSex, "sex", NULL, font, arrSelect );
		m_wndForm->AddFormlist( ctrlidDoc, "ldoc", NULL )->AddAttr( "#name" );
		m_wndForm->AddFormlist( ctrlidApp, "app", NULL, g_pApp->GetUniqueTmpFolder( ) )->AddAttr( "name" );
		m_wndForm->AddText( ctrlidEmail, "lemail", NULL, font );
		m_wndForm->AddText( ctrlidAddr, "laddr", NULL, font );
		m_wndForm->AddText( ctrlidPhone, "lphone", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt FormFizModel

	udInt CHClient::FormFizView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkPrintFiz->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrintFiz->get( "x" )->ini( ( udInt ) 300 );
			m_lnkPrintFiz->get( "y" )->ini( ( udInt ) 10 );
			m_lnkPrintFiz->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrintFiz->SetTargetWnd( m_wndForm->GetHWndParent( ) );
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
		title	= L"Клиент физ. лицо";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
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
		title	= L"Дата рождения";
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
		info = m_wndForm->GetInfo( ctrlidBorn );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Пол";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		udArrWString arrSelect;
		arrSelect.push_back( L"Мужской" );
		arrSelect.push_back( L"Женский" );
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidSex );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Документы";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidDoc );
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
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 400; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidApp );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"E-mail";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidEmail );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Адреса";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidAddr );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Телефоны";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidPhone );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt FormFizView

	udInt CHClient::FormUrModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidUrName, "name", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidUrDfio, "dfio", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidUrCfio, "cfio", NULL, font, 0 );
		m_wndForm->AddText( ctrlidUrEmail, "lemail", NULL, font );
		m_wndForm->AddText( ctrlidUrPhone, "lphone", NULL, font );
		m_wndForm->AddText( ctrlidUrFax, "lfax", NULL, font );
		m_wndForm->AddText( ctrlidUrAddr, "laddr", NULL, font );
		m_wndForm->AddText( ctrlidUrComment, "comment", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt FormUrModel

	udInt CHClient::FormUrView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		} else {
			m_lnkPrintUr->CalculateWidthHeight( m_wndForm->GetHWnd( ) );
			m_lnkPrintUr->get( "x" )->ini( ( udInt ) 300 );
			m_lnkPrintUr->get( "y" )->ini( ( udInt ) 10 );
			m_lnkPrintUr->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrintUr->SetTargetWnd( m_wndForm->GetHWndParent( ) );
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
		title	= L"Клиент юр. лицо";
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
		info = m_wndForm->GetInfo( ctrlidUrName );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Руководитель";
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
		info = m_wndForm->GetInfo( ctrlidUrDfio );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Контактное лицо";
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
		info = m_wndForm->GetInfo( ctrlidUrCfio );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"E-mail";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidUrEmail );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Телефон";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidUrPhone );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Факс";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidUrFax );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Адрес";
		x		= 32;
		w		= lcol;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidUrAddr );
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
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidUrComment );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt FormUrView

	udInt CHClient::ValidateForm( ) {
		if ( m_iClState == stateUr ) {
			return FormUrValidate( );
		}
		return FormFizValidate( );
	} // udInt ValidateForm

	udInt CHClient::FormFizValidate( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "fio" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите ФИО" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "fiolatin" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите ФИО латиницей" );
			return FormHandlerErrorFailed;
		}
		/*if ( obj->get( "lemail" )->toWString( ).empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите e-mail" );
			return false;
		}*/
		/*if ( obj->get( "laddr" )->toWString( ).empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите адрес" );
			return false;
		}*/
		if ( obj->get( "lphone" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите телефон" );
			return FormHandlerErrorFailed;
		}
		//udPDwAscBase attach = m_frmlstAttach->GetItems( );
		/*if ( !attach || attach->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите приложение" );
			return false;
		}*/
		//udPDwAscBase cldoc = m_frmlstClDoc->GetItems( );
		/*if ( !cldoc || cldoc->empty( ) ) {
			mb::err( g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ), L"Введите документы" );
			return false;
		}*/
		return FormHandlerErrorOk;
	} // udInt FormFizValidate

	udInt CHClient::FormUrValidate( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "dfio" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите ФИО руководителя" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "cfio" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите ФИО контактного лица" );
			return FormHandlerErrorFailed;
		}
		return FormHandlerErrorOk;
	} // udInt FormUrValidate

	void CHClient::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidApp )->frmlst ) {
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
				add->get( "type" )->ini( udProjectAttach::ATTACH_CLIENT );
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
		} else if ( wnd == m_wndForm->GetInfo( ctrlidDoc )->frmlst ) {
			udPFormHandler handler = GetHClDocForm( );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = new udProjectClDoc;
				obj->apply( add );
				add->get( "client" )->ini( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				add->get( "#name" )->ini(
					obj->get( "series" )->toWString( ) + L" " +
					obj->get( "number" )->toWString( )
				);
				wnd->AddItem( add );
				delete obj;
			}
			delete handler;
		}
	} // void FormListOnAdd

	void CHClient::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidApp )->frmlst ) {
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
						//obj->get( "path" )->ini( szPath );
						//obj->get( "old_name" )->ini( szOldName );
						obj->get( "tmp_path" )->ini( wFileTmpPath );
					}
				}
			}*/
		} else if ( wnd == m_wndForm->GetInfo( ctrlidDoc )->frmlst ) {
			udPBase tmp = new udProjectClDoc;
			obj->apply( tmp );
			udPFormHandler handler = GetHClDocForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
				obj->get( "#name" )->ini(
					obj1->get( "series" )->toWString( ) + L" " +
					obj1->get( "number" )->toWString( )
				);
				delete obj1;
			}
			delete handler;
		}
	} // void FormListOnEdit

	bool CHClient::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( wnd == m_wndForm->GetInfo( ctrlidApp )->frmlst ) {
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
		} else if ( wnd == m_wndForm->GetInfo( ctrlidDoc )->frmlst ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				if ( MessageBox( hWnd,
					L"Вы действительно хотите удалить документ из списка?", L"Подтвердите удаление",
					MB_YESNO ) == IDNO ) {
					return false;
				}

				if ( obj->get( "id" )->toUInt( ) > 0 ) {
					udPBase del = new udProjectClDoc;
					del->apply( obj, true, true );
					wnd->AddToDel( del );
				}
			}
			return true;
		}
		return false;
	} // bool FormListOnDelete

	void CHClient::OnLinkClick( udPWndLink lnk ) {
		if ( m_iState == INTSTATE_LIST ) {
			string url = lnk->get( "url" )->toString( );
			if ( url == "clientur" ) {
				this->ClearList( );
				
				m_iClState		= stateUr;
				m_szTableName	= "ud_clientur";
				m_szUpdIndex	= "id";
				m_szName		= "clientur";
				m_fnGetObject	= GetProjectClientUr;
				m_szOrderBy		= "id";
				m_pLstOpt->get( "state" )->ini( m_iClState );

				HWND hWnd = m_wndModeList->GetHWndParent( );
				bool modal = m_wndModeList->IsModal( );
				m_wndModeList->Destroy( );
				udDELETE( m_wndModeList );
				this->CreateList( hWnd, modal );
				if ( !modal ) {
					if ( m_wndForm ) {
						m_wndForm->Hide( );
						m_wndForm->Clear( );
						m_wndForm->Destroy( );
						udDELETE( m_wndForm );
					}
					//
					udDELETE( m_lnkPrintFiz );
					udDELETE( m_lnkPrintUr );
					m_lnkPrintFiz = new udWndLink;
					m_lnkPrintFiz->get( "title" )->ini( wstring( L"Распечатать" ) );
					m_lnkPrintFiz->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
					m_lnkPrintUr = new udWndLink;
					m_lnkPrintUr->get( "title" )->ini( wstring( L"Распечатать" ) );
					m_lnkPrintUr->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
					//
					udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
					tbr->EnableItem( "edit",	false );
					tbr->EnableItem( "delete",	false );
					m_arrExtraLoad.clear( );
					this->CreateForm( hWnd, modal );
				}
				this->LoadList( );
				m_wndModeList->Show( );
			} else if ( url == "client" ) {
				this->ClearList( );

				m_iClState		= stateFiz;
				m_szTableName	= "ud_client";
				m_szUpdIndex	= "id";
				m_szName		= "client";
				m_fnGetObject	= GetProjectClient;
				m_szOrderBy		= "id";
				m_pLstOpt->get( "state" )->ini( m_iClState );

				HWND hWnd = m_wndModeList->GetHWndParent( );
				bool modal = m_wndModeList->IsModal( );
				m_wndModeList->Destroy( );
				udDELETE( m_wndModeList );
				this->CreateList( hWnd, modal );
				if ( !modal ) {
					if ( m_wndForm ) {
						m_wndForm->Hide( );
						m_wndForm->Clear( );
						m_wndForm->Destroy( );
						udDELETE( m_wndForm );
					}
					//
					udDELETE( m_lnkPrintFiz );
					udDELETE( m_lnkPrintUr );
					m_lnkPrintFiz = new udWndLink;
					m_lnkPrintFiz->get( "title" )->ini( wstring( L"Распечатать" ) );
					m_lnkPrintFiz->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
					m_lnkPrintUr = new udWndLink;
					m_lnkPrintUr->get( "title" )->ini( wstring( L"Распечатать" ) );
					m_lnkPrintUr->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
					//
					udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
					tbr->EnableItem( "edit",	false );
					tbr->EnableItem( "delete",	false );
					m_arrExtraLoad.clear( );
					this->AddExtraLoadInfo( ctrlidDoc, GetProjectClDoc, ( udInt ) extra_load_frmlst, NULL, ClientAfterLoadClDoc );
					this->AddExtraLoadInfo( ctrlidApp, GetProjectAttach, ( udInt ) extra_load_frmlst );
					this->CreateForm( hWnd, modal );
				}
				this->LoadList( );
				m_wndModeList->Show( );
			} else {
				udStateHandlerSimple::OnLinkClick( lnk );
			}
		} else {
			udStateHandlerSimple::OnLinkClick( lnk );
		}
	} // void OnLinkClick

	void CHClient::ExtraDataSending( udPSocket sock, udPBase obj ) {
		if ( m_iClState == stateUr ) {
			return;
		}
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

	void CHClient::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
		if ( m_iClState == stateUr ) {
			return;
		}
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

	void CHClient::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		if ( m_iClState == stateFiz ) {
			filter->get( "fio" )->ini( wstring( L"" ) );
			filter->get( "date1" )->ini( wstring( L"" ) );
			filter->get( "date2" )->ini( wstring( L"" ) );
			/*datetime date;
			filter->get( "date1" )->ini( date );
			date.load( );
			filter->get( "date2" )->ini( date );*/
		} else {
			filter->get( "name" )->ini( wstring( L"" ) );
			filter->get( "cfio" )->ini( wstring( L"" ) );
		}
	} // void ClearFilter

} // namespace UdSDK
