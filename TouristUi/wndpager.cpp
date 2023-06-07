
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndPager::CWndPager( udDWord dwBackgroundColor ) {
		m_dwBackgroundColor = dwBackgroundColor;
		m_wndPrevFrame	= new udWndButton;
		m_wndNextFrame	= new udWndButton;
		m_wndPrevPage	= new udWndButton;
		m_wndNextPage	= new udWndButton;

		udInt x = 0, y = 0, w = 22, h = 22, m = 5;
		udDWord style = WS_CHILD;

		m_wndPrevFrame->get( "x" )->ini( x );
		m_wndPrevFrame->get( "y" )->ini( y );
		m_wndPrevFrame->get( "width" )->ini( w );
		m_wndPrevFrame->get( "height" )->ini( h );
		m_wndPrevFrame->get( "style" )->ini( style );
		x += w + m;

		m_wndPrevPage->get( "x" )->ini( x );
		m_wndPrevPage->get( "y" )->ini( y );
		m_wndPrevPage->get( "width" )->ini( w );
		m_wndPrevPage->get( "height" )->ini( h );
		m_wndPrevPage->get( "style" )->ini( style );
		x += w + m;

		m_wndNextPage->get( "x" )->ini( x );
		m_wndNextPage->get( "y" )->ini( y );
		m_wndNextPage->get( "width" )->ini( w );
		m_wndNextPage->get( "height" )->ini( h );
		m_wndNextPage->get( "style" )->ini( style );
		x += w + m;

		m_wndNextFrame->get( "x" )->ini( x );
		m_wndNextFrame->get( "y" )->ini( y );
		m_wndNextFrame->get( "width" )->ini( w );
		m_wndNextFrame->get( "height" )->ini( h );
		m_wndNextFrame->get( "style" )->ini( style );
		x += w + m;
	}

	CWndPager::~CWndPager( ) {
		udDELETE( m_wndPrevFrame );
		udDELETE( m_wndNextFrame );
		udDELETE( m_wndPrevPage );
		udDELETE( m_wndNextPage );
		ClearArrMorph( m_arrWndLink );
	}

	bool CWndPager::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			return false;
		}

		m_wndPrevFrame->SetRectNormal( new Rect( 698, 15, 22, 22 ) );
		m_wndPrevPage->SetRectNormal( new Rect( 725, 15, 22, 22 ) );
		m_wndNextPage->SetRectNormal( new Rect( 765, 15, 22, 22 ) );
		m_wndNextFrame->SetRectNormal( new Rect( 814, 15, 22, 22 ) );
		m_wndPrevFrame->Create( m_hWnd );
		m_wndPrevPage->Create( m_hWnd );
		m_wndNextPage->Create( m_hWnd );
		m_wndNextFrame->Create( m_hWnd );

		this->PositionControls( );

		return true;
	} // bool Create

	LRESULT CWndPager::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;

			case WM_BUTTONCLICK:
				return this->msgButtonClick( lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndPager::msgLinkClick( LPARAM lParam ) {
		udPWndLink lnk = ( udPWndLink ) lParam;
		if ( lnk ) {
			::PostMessage( m_hWndParent, WM_PAGER_SELECT, 0, ( LPARAM ) lnk->get( "page" )->toUInt( ) );
		}
		return 1;
	} // LRESULT msgLinkClick

	LRESULT CWndPager::msgButtonClick( LPARAM lParam ) {
		udPWndButton btn = ( udPWndButton ) lParam;
		if ( btn ) {
			::PostMessage( m_hWndParent, WM_PAGER_SELECT, 0, ( LPARAM ) btn->get( "page" )->toUInt( ) );
		}
		return TRUE;
	} // LRESULT msgButtonClick

	void CWndPager::Render( ) {
		udPGdi gdi = this->GetGdi( );
		gdi->Clear( Color( m_dwBackgroundColor ) );
	} // void Render

	void CWndPager::Clear( ) {
		udPWindow wnd = NULL;
		for( udArrMorph::iterator i = m_arrWndLink.begin( ); i != m_arrWndLink.end( ); ++i ) {
			wnd = ( udPWindow ) *i;
			wnd->Destroy( );
		}
		ClearArrMorph( m_arrWndLink );
		m_wndPrevFrame->Hide( );
		m_wndNextFrame->Hide( );
		m_wndPrevPage->Hide( );
		m_wndNextPage->Hide( );
	} // void Clear

	void CWndPager::Generate( udPPager pager ) {
		//g_pApp->GetLog( )->Write( "CWndPager::Generate\n" );
		udUInt	page		= pager->GetPage( ),
				pagesize	= pager->GetPageSize( ),
				total		= pager->GetTotal( ),
				pagelen		= pager->GetPageLength( );

		/*g_pApp->GetLog( )->Write(
			"page=" + toString( page ) + ", pagesize=" + toString( pagesize ) +
			", total=" + toString( total ) + ", pagelen=" + toString( pagelen ) + "\n"
		);*/

		if ( page && ( total > 0 ) && ( pagesize > 0 ) && ( pagelen > 0 ) ) {
			if ( page < page_min ) {
				page = page_min;
			}
			if ( pagesize < pagesize_min ) {
				pagesize = pagesize_min;
			}
			if ( pagelen < pagelen_min ) {
				pagelen = pagelen_min;
			}
			udUInt	max_page	= ( udUInt ) ceil( ( float ) total / ( float ) pagesize ),
					cur_frame	= ( udUInt ) ceil( ( float ) page / ( float ) pagelen ),
					start		= ( cur_frame - 1 ) * pagelen,
					end			= cur_frame * pagelen + 1;
			/*g_pApp->GetLog( )->Write(
				"max_page=" + toString( max_page ) + ", cur_frame=" + toString( cur_frame ) +
				", start=" + toString( start ) + ", end=" + toString( end ) + "\n"
			);*/
			// предыдущий фрэйм
			if ( start ) {
				m_wndPrevFrame->get( "page" )->ini( start );
				//m_wndPrevFrame->Show( );
			} else {
				m_wndPrevFrame->get( "page" )->ini( ( udUInt ) 0 );
				//m_wndPrevFrame->Hide( );
			}
			// следующий фрэйм
			if ( ( end > pagelen ) && ( end < ( max_page + 1 ) ) ) {
				m_wndNextFrame->get( "page" )->ini( end );
				//m_wndNextFrame->Show( );
			} else {
				m_wndNextFrame->get( "page" )->ini( ( udUInt ) 0 );
				//m_wndNextFrame->Hide( );
			}
			// предыдущая страница
			if ( page > 1 ) {
				m_wndPrevPage->get( "page" )->ini( page - 1 );
				//m_wndPrevPage->Show( );
			} else {
				m_wndPrevPage->get( "page" )->ini( ( udUInt ) 0 );
				//m_wndPrevPage->Hide( );
			}
			// следующая страница
			if ( page < max_page ) {
				m_wndNextPage->get( "page" )->ini( page + 1 );
				//m_wndNextPage->Show( );
			} else {
				m_wndNextPage->get( "page" )->ini( ( udUInt ) 0 );
				//m_wndNextPage->Hide( );
			}
			// страницы
			++start;
			this->Clear( );
			udPMorph item = NULL;
			for( udUInt i = start; ( i < end ) && ( i <= max_page ); ++i ) {
				if ( i == page ) {
					item = new udWndStatic;
					item->get( "current" );
				} else {
					item = new udWndLink;
				}
				item->get( "title" )->ini( toWString( i ) );
				item->get( "page" )->ini( i );
				m_arrWndLink.push_back( item );
			}
			this->PositionControls( );
		}
	} // void Generate

	void CWndPager::PositionControls( ) {
		//g_pApp->GetLog( )->Write( "CWndPager::PositionControls\n" );
		//string log;
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt x = 0, y = 0, w = 22, h = 22, m = 5;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndPrevFrame->get( "x" )->ini( x );
		m_wndPrevFrame->get( "y" )->ini( y );
		if ( m_wndPrevFrame->get( "page" )->toUInt( ) == 0 ) {
			m_wndPrevFrame->Hide( );
		} else {
			m_wndPrevFrame->Move( );
			m_wndPrevFrame->Show( );
		}
		
		/*log = "\nm_wndPrevFrame, ";
		m_wndPrevFrame->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );*/

		x += w + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndPrevPage->get( "x" )->ini( x );
		m_wndPrevPage->get( "y" )->ini( y );
		if ( m_wndPrevPage->get( "page" )->toUInt( ) == 0 ) {
			m_wndPrevPage->Hide( );
		} else {
			m_wndPrevPage->Move( );
			m_wndPrevPage->Show( );
		}

		/*log = "\nm_wndPrevPage, ";
		m_wndPrevPage->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );*/

		x += w + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWindow wnd = NULL;
		udPWndStatic stc = NULL;
		udPWndLink lnk = NULL;
		for( udArrMorph::iterator i = m_arrWndLink.begin( ); i != m_arrWndLink.end( ); ++i ) {
			wnd = ( udPWindow ) *i;
			wnd->get( "x" )->ini( x );
			wnd->get( "y" )->ini( y );
			wnd->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
			if ( wnd->isset( "current" ) ) {
				stc = ( udPWndStatic ) wnd;
				stc->CalculateWidthHeight( m_hWnd );
			} else {
				lnk = ( udPWndLink ) wnd;
				lnk->CalculateWidthHeight( m_hWnd );
			}
			wnd->Create( m_hWnd );
			x += wnd->get( "width" )->toInt( ) + m;

			/*log = "\nlnk, ";
			lnk->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );*/
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndNextPage->get( "x" )->ini( x );
		m_wndNextPage->get( "y" )->ini( y );
		if ( m_wndNextPage->get( "page" )->toUInt( ) == 0 ) {
			m_wndNextPage->Hide( );
		} else {
			m_wndNextPage->Move( );
			m_wndNextPage->Show( );
		}

		/*log = "\nm_wndNextPage, ";
		m_wndNextPage->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );*/
		x += w + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndNextFrame->get( "x" )->ini( x );
		m_wndNextFrame->get( "y" )->ini( y );
		if ( m_wndNextFrame->get( "page" )->toUInt( ) == 0 ) {
			m_wndNextFrame->Hide( );
		} else {
			m_wndNextFrame->Move( );
			m_wndNextFrame->Show( );
		}

		/*log = "\nm_wndNextFrame, ";
		m_wndNextFrame->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );*/
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void PositionControls

} // namespace UdSDK
