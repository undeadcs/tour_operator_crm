
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CStateHandlerInfo::CStateHandlerInfo( ) : udStateHandlerSimple( ) {
	}

	CStateHandlerInfo::CStateHandlerInfo( const CStateHandlerInfo& objCopy ) {
	}

	CStateHandlerInfo::~CStateHandlerInfo( ) {
	}

	CStateHandlerInfo& CStateHandlerInfo::operator =( const CStateHandlerInfo& objCopy ) {
		return *this;
	}

	void CStateHandlerInfo::OnLinkClick( udPWndLink lnk ) {
		if ( m_iState == INTSTATE_LIST ) {
			if ( m_wndModeList->IsModal( ) ) {
				udStateHandlerSimple::OnLinkClick( lnk );
			} else {
				string url = lnk->get( "url" )->toString( );
				if ( url == "info" ) {
					g_pApp->GetModel( )->GetWndWork( )->GetMenu( )->SelectItem( "#info" );
					g_pApp->GetModel( )->SetState( "#info" );
				} else {
					udStateHandlerSimple::OnLinkClick( lnk );
				}
			}
		} else {
			udStateHandlerSimple::OnLinkClick( lnk );
		}
	} // void OnLinkClick





	CStateHandlerSimpleOneName::CStateHandlerSimpleOneName( ) : udStateHandlerInfo( ) {
		m_lnkPrint	= new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать форму" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
	}

	CStateHandlerSimpleOneName::CStateHandlerSimpleOneName( const CStateHandlerSimpleOneName& objCopy ) {
	}

	CStateHandlerSimpleOneName::~CStateHandlerSimpleOneName( ) {
		udDELETE( m_lnkPrint );
	}

	CStateHandlerSimpleOneName& CStateHandlerSimpleOneName::operator =( const CStateHandlerSimpleOneName& objCopy ) {
		return *this;
	}

	udInt CStateHandlerSimpleOneName::GetListModel( ) {
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= ctrlidName + 1;
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
		title = ( modal ? m_szAddModal : m_szAddModeless );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "name", L"Наименование" )->get( "width" )->ini( ( udInt ) 400 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( ctrlidName, "name", NULL, g_pApp->GetFont( ), 0 );
		m_wndModeList->GetObjFilter( )->get( "name" )->ini( wstring( L"" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CStateHandlerSimpleOneName::GetListView( ) {
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
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
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
		if ( !modal ) {
			x += lnk->get( "width" )->toInt( ) + m;
			m_wndModeList->SetButtonFilterPosition( x, y );
			x += m_wndModeList->GetButtonReset( )->get( "width" )->toInt( ) + m;
			m_wndModeList->SetButtonResetPosition( x, y );
		}
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
		info = m_wndModeList->GetInfo( ctrlidName );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		y += box->h + box->padT + box->padB + m;
		if ( modal ) {
			m_wndModeList->SetButtonFilterPosition( x, y );
			x += m_wndModeList->GetButtonFilter( )->get( "width" )->toInt( ) + m;
			m_wndModeList->SetButtonResetPosition( x, y );
			x = m;
			y += m_wndModeList->GetButtonFilter( )->get( "height" )->toInt( ) + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//y += box->h + box->padT + box->padB + m;
		//////////////////////////////////////////////////////////////////////////////////////////
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

	udInt CStateHandlerSimpleOneName::GetFormModel( ) {
		m_wndForm->AddInput( ctrlidName, "name", NULL, g_pApp->GetFont( ), 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // void GetFormModel

	udInt CStateHandlerSimpleOneName::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 120 );
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
		title	= m_szFormTitle;
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
		} else {
			//////////////////////////////////////////////////////////////////////////////////////
			x += w + m;
			m_lnkPrint->get( "x" )->ini( x );
			m_lnkPrint->get( "y" )->ini( y );
			m_lnkPrint->Create( m_wndForm->GetHWnd( ) );
			m_lnkPrint->SetTargetWnd( m_wndForm->GetHWndParent( ) );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////////////
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
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CStateHandlerSimpleOneName::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		return FormHandlerErrorOk;
	} // udInt ValidateObj

	void CStateHandlerSimpleOneName::ClearFilter( ) {
		m_wndModeList->GetObjFilter( )->get( "name" )->ini( wstring( L"" ) );
	} // void ClearFilter

	void CStateHandlerSimpleOneName::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CStateHandlerSimpleOneName::LinkClick( udPWndLink lnk ) {
		if ( m_iState == INTSTATE_FORM ) {
			if ( m_wndForm->IsModal( ) ) {
			} else {
				if ( lnk == m_lnkPrint ) {
					udPXmlDoc doc = new udXmlDoc;
					string xml;
					udPMorph decl = NULL;
					udPXmlNode node = NULL;
					wstring folder = g_pApp->GetUniqueTmpFolder( );
					//////////////////////////////////////////////////////////////////////////////
					decl = new udMorph;
					decl->get( "version" )->ini( wstring( L"1.0" ) );
					decl->get( "encoding" )->ini( wstring( L"UTF-8" ) );
					doc->AddDeclaration( L"xml", decl );
					udDELETE( decl );
					//////////////////////////////////////////////////////////////////////////////
					decl = new udMorph;
					decl->get( "type" )->ini( wstring( L"text/xsl" ) );
					decl->get( "href" )->ini( wstring( L"form_out.xsl" ) );
					doc->AddDeclaration( L"xml-stylesheet", decl );
					udDELETE( decl );
					//////////////////////////////////////////////////////////////////////////////
					node = m_wndForm->GetObj( )->toXml( doc );
					node->SetAttribute( L"object_title", m_szFormTitle );
					doc->SetRoot( node );
					doc->toString( xml );
					WriteToFile( folder + L"\\out.xml", xml );
					CopyFile( L".\\xsl\\form_out.xsl", ( folder + wstring( L"\\form_out.xsl" ) ).data( ), FALSE );
					ExtOpenFile( folder + L"\\out.xml" );
					//////////////////////////////////////////////////////////////////////////////
					xml.clear( );
					udDELETE( doc );
					//////////////////////////////////////////////////////////////////////////////
				}
			}
		}
	} // void LinkClick

} // namespace UdSDK
