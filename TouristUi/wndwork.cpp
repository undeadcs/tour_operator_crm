
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndWork::CWndWork( HINSTANCE hInstance ) : udWindow( hInstance ) {
		m_pToolbar	= new udWndToolbar;
		m_pMenu		= new udWndMenu;
		m_pWndModel	= new udWndModel;

		m_iLineHeight	= 32;
		m_iWindowHeight	= 1024;
		m_iWindowWidth	= 1280;
		m_iColumnWidth	= 32;
	}

	CWndWork::CWndWork( const CWndWork& objCopy ) : udWindow( udWindow::GetHInstance( ) ) {
	}

	CWndWork::~CWndWork( ) {
		if ( m_pToolbar ) {
			delete m_pToolbar;
			m_pToolbar = NULL;
		}
		if ( m_pMenu ) {
			delete m_pMenu;
			m_pMenu = NULL;
		}
		if ( m_pWndModel ) {
			delete m_pWndModel;
			m_pWndModel = NULL;
		}
	}

	CWndWork& CWndWork::operator =( const CWndWork& objCopy ) {
		return *this;
	}

	bool CWndWork::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWindow::Create( hParent, hMenu ) ) {
			return false;
		}

		udPMorph	item	= NULL;
		udInt		iWidth	= 0,
					iHeight	= 0,
					iX		= 0,
					iY		= 0;
		udDWord		style	= WS_CHILD | WS_TABSTOP;// | WS_CLIPCHILDREN;
		RECT		rc;
		GetClientRect( m_hWnd, &rc );

		// создаем окно тулбара
		iWidth	= m_iWindowWidth;
		iHeight	= 94;
		m_pToolbar->get( "style" )->ini( style );
		m_pToolbar->get( "x" )->ini( iX );
		m_pToolbar->get( "y" )->ini( iY );
		m_pToolbar->get( "width" )->ini( iWidth );
		m_pToolbar->get( "height" )->ini( iHeight );
		m_pToolbar->Create( m_hWnd );
		iY += iHeight;

		// создаем окно меню
		iWidth	= 235;
		iHeight	= m_iWindowHeight - iHeight;
		m_pMenu->get( "style" )->ini( style );
		m_pMenu->get( "x" )->ini( iX );
		m_pMenu->get( "y" )->ini( iY );
		m_pMenu->get( "width" )->ini( iWidth );
		m_pMenu->get( "height" )->ini( iHeight );
		m_pMenu->Create( m_hWnd );
		iX += iWidth;

		// создаем окно модели
		iWidth	= m_iWindowWidth - iWidth;
		m_pWndModel->get( "style" )->ini( style );
		m_pWndModel->get( "x" )->ini( iX );
		m_pWndModel->get( "y" )->ini( iY );
		m_pWndModel->get( "width" )->ini( iWidth );
		m_pWndModel->get( "height" )->ini( iHeight );
		m_pWndModel->Create( m_hWnd );

		return true;
	} // bool Create

	LRESULT CWndWork::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_GETMINMAXINFO:
				if ( this->msgMaxMinInfo( ( LPMINMAXINFO ) lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_SIZE:
				if ( this->msgSize( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_VSCROLL:
				if ( this->msgVScroll( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_HSCROLL:
				if ( this->msgHScroll( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_ERASEBKGND:
				if ( this->msgEraseBackground( ( HDC ) wParam ) == 1 ) {
					return 1;
				}
				break;

			case WM_MOUSEWHEEL:
				if ( this->msgMouseWheel( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_DESTROY:
				g_pApp->Exit( );
				//PostQuitMessage( 0 );
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndWork::msgEraseBackground( HDC hDC ) {
		/*udGdi gdi( hDC );
		gdi.Clear( Color( 255, 255, 255 ) );*/
		return 1;
	} // LRESULT msgEraseBackground

	void CWndWork::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
		udGdi gdi( hDC );
		gdi.Clear( Color( 255, 255, 255 ) );
	} // LRESULT msgPaint

	LRESULT CWndWork::msgMaxMinInfo( LPMINMAXINFO pInfo ) {
		LRESULT lRet = 1;
		if ( this->isset( "min-width" ) ) {
			pInfo->ptMinTrackSize.x = this->get( "min-width" )->toLong( );
			lRet = 0;
		}
		if ( this->isset( "min-height" ) ) {
			pInfo->ptMinTrackSize.y = this->get( "min-height" )->toLong( );
			lRet = 0;
		}
		if ( this->isset( "max-width" ) ) {
			pInfo->ptMaxTrackSize.x = this->get( "max-width" )->toLong( );
			lRet = 0;
		}
		if ( this->isset( "max-height" ) ) {
			pInfo->ptMaxTrackSize.y = this->get( "max-height" )->toLong( );
			lRet = 0;
		}
		return lRet;
	} // LRESULT msgMaxMinInfo

	LRESULT CWndWork::msgSize( WPARAM wParam, LPARAM lParam ) {
		assert( m_pToolbar );
		assert( m_pMenu );
		udInt	prev_w	= this->get( "width" )->toInt( ),
				prev_h	= this->get( "height" )->toInt( );
		udWindow::msgSize( wParam, lParam );
		this->ResizeScroll( prev_w, prev_h );

		return 0;
	} // LRESULT msgSize

	LRESULT CWndWork::msgHScroll( WPARAM wParam, LPARAM lParam ) {
		if ( lParam == 0 ) {
			udWord	wType	= LOWORD( wParam ),	// тип запроса
					wPos	= HIWORD( wParam ),	// позиция ползунка
					xPos	= 0;
			SCROLLINFO si;
			ZeroMemory( &si, sizeof( SCROLLINFO ) );
			si.cbSize = sizeof( SCROLLINFO );
			si.fMask = SIF_ALL;
			if ( GetScrollInfo( m_hWnd, SB_HORZ, &si ) != 0 ) {
				xPos = si.nPos;
				switch( wType ) {
					case SB_TOP:
						si.nPos = si.nMin;
						break;

					case SB_BOTTOM:
						si.nPos = si.nMax;
						break;

					case SB_LINEUP:
						si.nPos -= 1;
						break;

					case SB_LINEDOWN:
						si.nPos += 1;
						break;

					case SB_PAGEUP:
						si.nPos -= si.nPage;
						break;

					case SB_PAGEDOWN:
						si.nPos += si.nPage;
						break;

					case SB_THUMBTRACK:
						si.nPos = si.nTrackPos;
						break;

					case SB_THUMBPOSITION:
						break;

					case SB_ENDSCROLL:
						break;

					default:
						g_pApp->GetLog( )->Write( "не обработанное сообщение\n" );
						break;
				}
				si.fMask = SIF_POS;
				SetScrollInfo( m_hWnd, SB_HORZ, &si, TRUE );
				GetScrollInfo( m_hWnd, SB_HORZ, &si );
				if ( si.nPos != xPos ) {
					ScrollWindowEx( m_hWnd, m_iColumnWidth * ( xPos - si.nPos ), 0, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE );
					//ScrollWindow( m_hWnd, m_iColumnWidth * ( xPos - si.nPos ), 0, NULL, NULL );
					UpdateWindow( m_hWnd );
				}
			}
		}
		return 0;
	} // LRESULT msgHScroll

	LRESULT CWndWork::msgVScroll( WPARAM wParam, LPARAM lParam ) {
		if ( lParam == 0 ) {
			udWord	wType	= LOWORD( wParam ),	// тип запроса
					wPos	= HIWORD( wParam ),	// позиция ползунка
					yPos	= 0;
			SCROLLINFO si;
			ZeroMemory( &si, sizeof( SCROLLINFO ) );
			si.cbSize = sizeof( SCROLLINFO );
			si.fMask = SIF_ALL;
			if ( GetScrollInfo( m_hWnd, SB_VERT, &si ) != 0 ) {
				yPos = si.nPos;
				switch( wType ) {
					case SB_TOP:
						si.nPos = si.nMin;
						break;

					case SB_BOTTOM:
						si.nPos = si.nMax;
						break;

					case SB_LINEUP:
						si.nPos -= 1;
						break;

					case SB_LINEDOWN:
						si.nPos += 1;
						break;

					case SB_PAGEUP:
						si.nPos -= si.nPage;
						break;

					case SB_PAGEDOWN:
						si.nPos += si.nPage;
						break;

					case SB_THUMBTRACK:
						si.nPos = si.nTrackPos;
						break;

					case SB_THUMBPOSITION:
						break;

					case SB_ENDSCROLL:
						break;

					default:
						g_pApp->GetLog( )->Write( "не обработанное сообщение\n" );
						break;
				}
				si.fMask = SIF_POS;
				SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
				GetScrollInfo( m_hWnd, SB_VERT, &si );
				if ( si.nPos != yPos ) {
					ScrollWindowEx( m_hWnd, 0, m_iLineHeight * ( yPos - si.nPos ), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE );
					UpdateWindow( m_hWnd );
				}
			}
		}
		return 0;
	} // LRESULT msgVScroll

	void CWndWork::ResizeScroll( udInt iPrevW, udInt iPrevH ) {
		WriteUdUiLog( "CWndWork::ResizeScroll enter\n" );
		udInt	w				= this->get( "width" )->toInt( ),
				h				= this->get( "height" )->toInt( ),
				iLinesNum		= 0,
				iColumnNum		= 0,
				iPrevPos		= 0,
				iPrevPageSize	= 0,	// предыдущий размер страницы
				iNewPageSize	= 0;	// новый размер страницы
		// инициализируем инфо структуру
		SCROLLINFO si;
		ZeroMemory( &si, sizeof( SCROLLINFO ) );
		si.cbSize = sizeof( SCROLLINFO );
		// выгребаем текущее состояние
		si.fMask = SIF_ALL;
		if ( GetScrollInfo( m_hWnd, SB_VERT, &si ) == 0 ) { // 1
			WriteUdUiLog( "CWndWork::ResizeScroll, GetScrollInfo (1) failed, code=" + toString( GetLastError( ) ) + "\n" );
		} else {
			// вычисляем значения
			iPrevPos		= si.nPos;
			iPrevPageSize	= si.nPage;
			iNewPageSize	= ( int ) ceil( ( float ) h / ( float ) m_iLineHeight );
			iLinesNum		= m_iWindowHeight / m_iLineHeight;
			// устанавливаем новые значения
			si.fMask	= SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
			si.nMin		= 0;
			si.nMax		= iLinesNum - 1;
			si.nPage	= iNewPageSize;
			WriteUdUiLog( "si.nMax=" + toString( si.nMax ) + ", si.nPage=" + toString( si.nPage ) + "\n" );
			SetScrollInfo( m_hWnd, SB_VERT, &si, FALSE );
			//SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
			// выгребаем результаты
			si.fMask = SIF_ALL;
			if ( GetScrollInfo( m_hWnd, SB_VERT, &si ) == 0 ) { // 2
				WriteUdUiLog( "CWndWork::ResizeScroll, GetScrollInfo (2) failed, code=" + toString( GetLastError( ) ) + "\n" );
			} else {
				// при изменении размерности, необходимо прокрутить окно
				WriteUdUiLog( "SB_VERT, iPrevPos=" + toString( iPrevPos ) + ", si.nPos=" + toString( si.nPos ) + "\n" );
				if ( iPrevPos != si.nPos ) { // ОС меняет позицию ползунка при изменении размерности
					ScrollWindowEx( m_hWnd, 0, m_iLineHeight * ( iPrevPos - si.nPos ), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE );
					UpdateWindow( m_hWnd );
				}
			}
			SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
		}
		// выгребаем текущее состояние
		si.fMask = SIF_ALL;
		if ( GetScrollInfo( m_hWnd, SB_HORZ, &si ) == 0 ) { // 3
			WriteUdUiLog( "CWndWork::ResizeScroll, GetScrollInfo (3) failed, code=" + toString( GetLastError( ) ) + "\n" );
		} else {
			// вычисляем значения
			iPrevPos		= si.nPos;
			iPrevPageSize	= si.nPage;
			iNewPageSize	= ( int ) ceil( ( float ) w / ( float ) m_iColumnWidth );
			iColumnNum		= m_iWindowWidth / m_iColumnWidth;
			// устанавливаем новые значения
			si.fMask	= SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
			si.nMin		= 0;
			si.nMax		= iColumnNum - 1;
			si.nPage	= iNewPageSize;
			WriteUdUiLog( "si.nMax=" + toString( si.nMax ) + ", si.nPage=" + toString( si.nPage ) + "\n" );
			SetScrollInfo( m_hWnd, SB_HORZ, &si, FALSE );
			//SetScrollInfo( m_hWnd, SB_HORZ, &si, TRUE );
			// выгребаем результаты
			si.fMask = SIF_ALL;
			if ( GetScrollInfo( m_hWnd, SB_HORZ, &si ) == 0 ) { // 4
				WriteUdUiLog( "CWndWork::ResizeScroll, GetScrollInfo (4) failed, code=" + toString( GetLastError( ) ) + "\n" );
			} else {
				// при изменении размерности, необходимо прокрутить окно
				WriteUdUiLog( "SB_HORZ, iPrevPos=" + toString( iPrevPos ) + ", si.nPos=" + toString( si.nPos ) + "\n" );
				if ( iPrevPos != si.nPos ) { // ОС меняет позицию ползунка при изменении размерности
					ScrollWindowEx( m_hWnd, m_iColumnWidth * ( iPrevPos - si.nPos ), 0, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE );
					UpdateWindow( m_hWnd );
				}
			}
			SetScrollInfo( m_hWnd, SB_HORZ, &si, TRUE );
		}
		WriteUdUiLog( "CWndWork::ResizeScroll exit\n" );
	} // void ResizeScroll

	LRESULT CWndWork::msgMouseWheel( WPARAM wParam, LPARAM lParam ) {
		udInt	iKeys	= GET_KEYSTATE_WPARAM( wParam ),
				iDelta	= GET_WHEEL_DELTA_WPARAM( wParam ),
				iX		= GET_X_LPARAM( lParam ),
				iY		= GET_Y_LPARAM( lParam ),
				iPosY	= 0;
		SCROLLINFO si;
		ZeroMemory( &si, sizeof( SCROLLINFO ) );
		si.cbSize = sizeof( SCROLLINFO );
		si.fMask = SIF_ALL;
		if ( GetScrollInfo( m_hWnd, SB_VERT, &si ) != 0 ) {
			iPosY = si.nPos;
			if ( iDelta > 0 ) {
				si.nPos -= 1;
			} else {
				si.nPos += 1;
			}
			si.fMask = SIF_POS;
			SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
			GetScrollInfo( m_hWnd, SB_VERT, &si );
			if ( si.nPos != iPosY ) {
				ScrollWindowEx( m_hWnd, 0, m_iLineHeight * ( iPosY - si.nPos ), NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_INVALIDATE );
				//ScrollWindow( m_hWnd, 0, m_iLineHeight * ( iPosY - si.nPos ), NULL, NULL );
				UpdateWindow( m_hWnd );
			}
		}
		return 0;
	} // LRESULT msgMouseWheel

} // namespace UdSDK
