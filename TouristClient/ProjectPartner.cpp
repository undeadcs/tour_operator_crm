
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHPartner::CHPartner( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_partner";
		m_szUpdIndex	= "id";
		m_szName		= "partner";
		m_fnGetObject	= GetProjectPartner;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHPartnerForm;

		m_lnkPrint = new udWndLink;
		m_lnkPrint->get( "title" )->ini( wstring( L"Распечатать" ) );
		m_lnkPrint->get( "style" )->ini( ( udDWord ) ( WS_CHILD ) );
	}

	CHPartner::CHPartner( const CHPartner& objCopy ) {
	}

	CHPartner::~CHPartner( ) {
		udDELETE( m_lnkPrint );
	}

	CHPartner& CHPartner::operator =( const CHPartner& objCopy ) {
		return *this;
	}

	udInt CHPartner::GetListModel( ) {
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
		title = ( modal ? L"Добавить партнера" : L"Новый партнер" );
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
		wndInsLst->AddAttr( "cfio", L"Контактное лицо" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "email", L"E-mail" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "phone", L"Телефон" )->get( "width" )->ini( ( udInt ) 150 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddInput( fltName, "name", NULL, font, 0 );
		m_wndModeList->AddInput( fltCfio, "cfio", NULL, font, 0 );
		m_wndModeList->AddInput( fltEmail, "email", NULL, font, 0 );
		m_wndModeList->AddInput( fltPhone, "phone", NULL, font, 0 );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "cfio" )->ini( wstring( L"" ) );
		filter->get( "email" )->ini( wstring( L"" ) );
		filter->get( "phone" )->ini( wstring( L"" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHPartner::GetListView( ) {
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
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltName );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCfio );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += box->h + box->padT + box->padB + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltEmail );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltPhone );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += 30 + m;
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

	udInt CHPartner::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidDfio, "dfio", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidCfio, "cfio", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidEmail, "email", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidPhone, "phone", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidFax, "fax", NULL, font, 0 );
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHPartner::GetFormView( ) {
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
		title	= L"Партнер";
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
		info = m_wndForm->GetInfo( ctrlidDfio );
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
		info = m_wndForm->GetInfo( ctrlidCfio );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"E-mail";
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
		info = m_wndForm->GetInfo( ctrlidEmail );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Телефон";
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
		info = m_wndForm->GetInfo( ctrlidPhone );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Факс";
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
		info = m_wndForm->GetInfo( ctrlidFax );
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
		info = m_wndForm->GetInfo( ctrlidComment );
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

	udInt CHPartner::ValidateForm( ) {
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
		if ( obj->get( "email" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите e-mail" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "phone" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите телефон" );
			return FormHandlerErrorFailed;
		}
		return FormHandlerErrorOk;
	} // bool ValidateForm

	void CHPartner::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		filter->get( "cfio" )->ini( wstring( L"" ) );
		filter->get( "email" )->ini( wstring( L"" ) );
		filter->get( "phone" )->ini( wstring( L"" ) );
	} // void ClearFilter

	void CHPartner::Show( udPWndModel wnd ) {
		udStateHandlerSimple::Show( wnd );
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) ) {
			if ( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 ) {
				m_lnkPrint->Show( );
			} else {
				m_lnkPrint->Hide( );
			}
		}
	} // void Show

	void CHPartner::LinkClick( udPWndLink lnk ) {
		if ( ( m_iState == INTSTATE_FORM ) && !m_wndForm->IsModal( ) &&
			( m_wndForm->GetObj( )->get( "id" )->toUInt( ) > 0 )
		) {
			if ( lnk == m_lnkPrint ) {
				udPXmlDoc doc = new udXmlDoc;
				string xml;
				wstring folder = g_pApp->GetUniqueTmpFolder( );
				udPMorph decl = NULL;
				udPXmlNode node = NULL;
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
				decl->get( "href" )->ini( wstring( L"partner_out.xsl" ) );
				doc->AddDeclaration( L"xml-stylesheet", decl );
				udDELETE( decl );
				//////////////////////////////////////////////////////////////////////////////////
				obj = GetProjectPartner( );
				obj->apply( m_wndForm->GetObj( ), true, true );
				node = obj->toXml( doc );
				//////////////////////////////////////////////////////////////////////////////////
				node->SetAttribute( L"object_title", L"Партнер" );
				doc->SetRoot( node );
				doc->toString( xml );
				WriteToFile( folder + L"\\out.xml", xml );
				CopyFile( L".\\xsl\\partner_out.xsl", ( folder + wstring( L"\\partner_out.xsl" ) ).data( ), FALSE );
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