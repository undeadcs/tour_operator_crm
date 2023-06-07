
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHLog::CHLog( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_log";
		m_szUpdIndex	= "id";
		m_szName		= "log";
		m_fnGetObject	= GetProjectLog;
		m_szOrderBy		= "id";
	}

	CHLog::CHLog( const CHLog& objCopy ) {
	}

	CHLog::~CHLog( ) {
	}

	CHLog& CHLog::operator =( const CHLog& objCopy ) {
		return *this;
	}

	udInt CHLog::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST + 1;
		udUInt		size	= 18;
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
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "created_", L"Дата и время", "created" )->get( "width" )->ini( ( udInt ) 150 );
		wndInsLst->AddAttr( "user_", L"Пользователь", "user" )->get( "width" )->ini( ( udInt ) 200 );
		wndInsLst->AddAttr( "action", L"Операция" )->get( "width" )->ini( ( udInt ) 200 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddDate( fltDate1, "date1", NULL, font );
		m_wndModeList->AddDate( fltDate2, "date2", NULL, font );
		m_wndModeList->AddExtlist( fltUser, "user", NULL, "fio", L"выберите пользователя", GetHUserList );
		m_wndModeList->AddInput( fltText, "text", NULL, font, 0 );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "user" )->ini( ( udUInt ) 0 );
		filter->get( "text" )->ini( wstring( L"" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHLog::GetListView( ) {
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
		info = m_wndModeList->GetInfo( fltUser );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 120; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltText );
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

	udInt CHLog::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		return FormHandlerErrorFailed;
	} // udInt GetFormModel

	udInt CHLog::GetFormView( ) {
		return FormHandlerErrorFailed;
	} // udInt GetFormView

	udInt CHLog::ValidateForm( ) {
		return FormHandlerErrorFailed;
	} // udInt ValidateForm

	void CHLog::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "user_" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHLog::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		datetime date;
		date.load( );
		filter->get( "date2" )->ini( date );
		filter->get( "user" )->ini( ( udUInt ) 0 );
		filter->get( "text" )->ini( wstring( L"" ) );
	} // void ClearFilter

	void CHLog::RequestList( ) {
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
