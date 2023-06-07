
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndRow::CWndRow( wstring title ) {
		m_wndStatic	= new udWndStatic( ( udUInt ) 10 );
		m_wndEdit	= new udWndLink( ( udUInt ) 10 );
		m_wndDelete	= new udWndLink( ( udUInt ) 10 );

		m_wndStatic->get( "title" )->ini( title );
	}

	CWndRow::CWndRow( const CWndRow& objCopy ) {
	}

	CWndRow::~CWndRow( ) {
		if ( m_wndStatic ) {
			delete m_wndStatic;
			m_wndStatic = NULL;
		}
		if ( m_wndEdit ) {
			delete m_wndEdit;
			m_wndEdit = NULL;
		}
		if ( m_wndDelete ) {
			delete m_wndDelete;
			m_wndDelete = NULL;
		}
	}

	CWndRow& CWndRow::operator =( const CWndRow& objCopy ) {
		return *this;
	}

	LRESULT	CWndRow::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_ERASEBKGND:
				if ( this->msgEraseBackground( ( HDC ) wParam ) != 0 ) {
					return 1;
				}
				break;

			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndRow::msgEraseBackground( HDC hDC ) {
		udGdi gdi( hDC );
		gdi.Clear( Color( 255, 255, 255 ) );
		return 1;
	} // LRESULT msgEraseBackground

	LRESULT CWndRow::msgLinkClick( LPARAM lParam ) {
		udPWndLink link = ( udPWndLink ) lParam;
		if ( link == m_wndEdit ) {
			PostMessage( m_hWndParent, WM_ROW_EDIT, 0, ( LPARAM ) this );
		} else if ( link == m_wndDelete ) {
			PostMessage( m_hWndParent, WM_ROW_DELETE, 0, ( LPARAM ) this );
		}
		return 0;
	} // LRESULT msgLinkClick

	bool CWndRow::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWindow::Create( hParent, hMenu ) ) {
			return false;
		}

		udInt	w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( ),
				x	= 0;
		//udUInt size = 10;

		// создаем окно ссылки удаления
		//m_wndDelete = new udWndLink( size );
		m_wndDelete->get( "title" )->ini( wstring( L"Удалить" ) );
		m_wndDelete->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndDelete->CalculateWidthHeight( m_hWnd );
		x = w - m_wndDelete->get( "width" )->toInt( ) - 1;
		m_wndDelete->get( "x" )->ini( x );
		m_wndDelete->get( "y" )->ini( ( udInt ) 0 );
		m_wndDelete->Create( m_hWnd );

		// создаем окно ссылки редактирования
		//m_wndEdit = new udWndLink( size );
		m_wndEdit->get( "title" )->ini( wstring( L"Редактировать" ) );
		m_wndEdit->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndEdit->CalculateWidthHeight( m_hWnd );
		x -= ( m_wndEdit->get( "width" )->toInt( ) + 1 );
		m_wndEdit->get( "x" )->ini( x );
		m_wndEdit->get( "y" )->ini( ( udInt ) 0 );
		m_wndEdit->Create( m_hWnd );

		// создаем окно надписи
		//m_wndStatic = new udWndStatic( size );
		m_wndStatic->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndStatic->get( "width" )->ini ( x - 1 );
		m_wndStatic->get( "height" )->ini( h );
		m_wndStatic->get( "x" )->ini( ( udInt ) 0 );
		m_wndStatic->get( "y" )->ini( ( udInt ) 0 );
		m_wndStatic->Create( m_hWnd );

		return true;
	} // bool Create

} // namespace UdSDK
