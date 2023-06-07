
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndScroller::CWndScroller( ) {
		m_wndRows = NULL;
	}

	CWndScroller::CWndScroller( const CWndScroller& objCopy ) {
	}

	CWndScroller::~CWndScroller( ) {
		m_wndRows = NULL;
	}

	CWndScroller& CWndScroller::operator =( const CWndScroller& objCopy ) {
		return *this;
	}

	LRESULT CWndScroller::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_LBUTTONDOWN:
				if ( this->msgLButtonDown( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_ERASEBKGND: {
				udGdi gdi( ( HDC ) wParam );
				gdi.Clear( Color( 255, 255, 255 ) );
				return 1;
			} break;

			case WM_MOUSEWHEEL:
				if ( this->msgMouseWheel( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_SIZE:
				if ( this->msgSize( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_VSCROLL:
				if ( !this->msgVScroll( wParam, lParam ) ) {
					return 0;
				}
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	bool CWndScroller::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWindow::Create( hParent, hMenu ) ) {
			return false;
		}
		if ( m_wndRows && !m_wndRows->Create( m_hWnd ) ) {
			return false;
		}
		return true;
	} // bool Create

	LRESULT CWndScroller::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndScroller::msgMouseWheel( WPARAM wParam, LPARAM lParam ) {
		return 1;
	} // LRESULT

	void CWndScroller::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
	} // void Render

	LRESULT CWndScroller::msgSize( WPARAM wParam, LPARAM lParam ) {
		return udWindow::msgSize( wParam, lParam );
	} // LRESULT msgSize

	LRESULT CWndScroller::msgVScroll( WPARAM wParam, LPARAM lParam ) {
		udWord w = LOWORD( wParam );
		if ( w == SB_LINEDOWN ) {
			ScrollWindow( m_hWnd, 0, -15, NULL, NULL );
		} else if ( w == SB_LINEUP ) {
			ScrollWindow( m_hWnd, 0, 15, NULL, NULL );
		}
		return 0;
	} // LRESULT msgVScroll

} // namespace UdSDK
