
#include "includes.h"

namespace UdSDK {

	static udPWndClass	g_pWc		= NULL;
	static udInt		g_iWcCount	= 0;

	static LRESULT CALLBACK UdWindowMessageHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		static map< HWND, udPWindow > mapWindow;

		udPWindow pWindow = NULL;
		map< HWND, udPWindow >::iterator i = mapWindow.find( hWnd );

		// подключаем скан сообщений
		//UdUiLogMessage( uMsg );
		//

		switch( uMsg ) {
			case WM_CREATE:
			//case WM_NCCREATE:
				if ( i == mapWindow.end( ) ) { // если окно не добавлено
					LPCREATESTRUCT pCreate = ( LPCREATESTRUCT ) lParam;
					if ( pCreate ) {
						udPVoid pParam = pCreate->lpCreateParams;
						if ( pParam ) {
							pWindow = ( udPWindow ) pParam;
							mapWindow[ hWnd ] = pWindow;
						}
					}
				} else { // окно уже было добавлено
					pWindow = i->second;
				}
				break;

			case WM_DESTROY:
				if ( i != mapWindow.end( ) ) { // если окно добавлено
					pWindow = i->second;
					mapWindow.erase( hWnd );	// убираем окно
				}
				break;

			default: // ставим указатель окна
				if ( i != mapWindow.end( ) ) { // если окно добавлено
					pWindow = i->second;
				}
				break;
		}

//assert( pWindow );
//return pWindow->MessageProc( hWnd, uMsg, wParam, lParam );

		if ( pWindow ) {
			return pWindow->MessageProc( hWnd, uMsg, wParam, lParam );
		} else { // определяем что за окно такое сотворило
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT UdWindowMessageHandler

	static void UdWindowRegisterClass( HINSTANCE hInstance ) {
		if ( g_pWc ) {
			++g_iWcCount;
		} else {
			assert( hInstance );

			g_iWcCount				= 1;
			g_pWc					= new udWndClass;
			g_pWc->cbSize			= sizeof( udWndClass );
			g_pWc->style			= CS_OWNDC;
			g_pWc->lpfnWndProc		= UdWindowMessageHandler;
			g_pWc->hbrBackground	= ( HBRUSH ) GetStockObject( WHITE_BRUSH );
			g_pWc->hCursor			= LoadCursor( NULL, IDC_ARROW );
			g_pWc->hIcon			= LoadIcon( NULL, IDI_APPLICATION );
			g_pWc->hIconSm			= NULL;
			g_pWc->hInstance		= hInstance;
			g_pWc->lpszClassName	= L"UndeadCS_SDK_Window";
			g_pWc->cbClsExtra		= 0;
			g_pWc->cbWndExtra		= DLGWINDOWEXTRA;
			g_pWc->lpszMenuName		= NULL;

			RegisterClassEx( g_pWc );
		}
	} // void UdWindowRegisterClass

	static void UdWindowUnregisterClass( ) {
		if ( g_iWcCount > 1 ) {
			--g_iWcCount;
		} else if ( g_iWcCount == 1 ) {
			UnregisterClass( g_pWc->lpszClassName, g_pWc->hInstance );
			delete g_pWc;
			g_pWc		= NULL;
			g_iWcCount	= 0;
		}
	} // void UdWindowUnregisterClass

	CUdWindow::CUdWindow( HINSTANCE hInstance ) {
		m_hWnd			= NULL;
		m_hWndParent	= NULL;

		UdWindowRegisterClass( hInstance );
	}

	CUdWindow::CUdWindow( const CUdWindow& objCopy ) {
	}

	CUdWindow::~CUdWindow( ) {
		m_hWnd			= NULL;
		m_hWndParent	= NULL;

		UdWindowUnregisterClass( );
	}

	CUdWindow& CUdWindow::operator =( const CUdWindow& objCopy ) {
		return *this;
	}

	bool CUdWindow::Create( HWND hParent, HMENU hMenu ) {
		assert( g_pWc );
		m_hWndParent = hParent;

		if ( m_hWnd = CreateWindowEx(
			this->get( "exstyle" )->toDWord( ), g_pWc->lpszClassName,
			this->get( "title" )->toWString( ).data( ), this->get( "style" )->toDWord( ),
			this->get( "x" )->toInt( ), this->get( "y" )->toInt( ),
			this->get( "width" )->toInt( ), this->get( "height" )->toInt( ),
			hParent, hMenu, g_pWc->hInstance, this
		) ) {
			return true;
		}

		WriteUdUiLog( "udWindow::Create, CreateWindowEx failed, error code=" + toString( GetLastError( ) ) + "\n" );

		return false;
	} // bool Create

	void CUdWindow::Show( ) {
		if ( m_hWnd ) {
			::ShowWindow( m_hWnd, SW_SHOW );
		}
	} // void Show

	void CUdWindow::Hide( ) {
		if ( m_hWnd ) {
			::ShowWindow( m_hWnd, SW_HIDE );
		}
	} // void Show

	void CUdWindow::Destroy( ) {
		if ( m_hWnd ) {
			::DestroyWindow( m_hWnd );
		}
	} // void Destroy

	void CUdWindow::Update( ) {
		if ( m_hWnd ) {
			::UpdateWindow( m_hWnd );
		}
	} // void Update

	void CUdWindow::Enable( ) {
		if ( m_hWnd ) {
			::EnableWindow( m_hWnd, TRUE );
		}
	} // void Enable

	void CUdWindow::Disable( ) {
		if ( m_hWnd ) {
			::EnableWindow( m_hWnd, FALSE );
		}
	} // void Disable

	void CUdWindow::SetAsForeground( ) {
		if ( m_hWnd ) {
			::SetForegroundWindow( m_hWnd );
		}
	} // void SetAsForeground

	void CUdWindow::Move( ) {
		if ( m_hWnd ) {
			MoveWindow(
				m_hWnd,
				this->get( "x" )->toInt( ), this->get( "y" )->toInt( ),
				this->get( "width" )->toInt( ), this->get( "height" )->toInt( ),
				TRUE
			);
		}
	} // void Move

	HWND CUdWindow::GetHWnd( ) const {
		return m_hWnd;
	} // HWND GetHWnd

	HWND CUdWindow::GetHWndParent( ) const {
		return m_hWndParent;
	} // HWND GetHWndParent

	HINSTANCE CUdWindow::GetHInstance( ) {
		if ( g_pWc ) {
			return g_pWc->hInstance;
		}
		return NULL;
	} // HINSTANCE GetHInstance

	LRESULT	CUdWindow::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_PAINT:
				if ( this->msgPaint( ) == 0 ) {
					return 0;
				}
				break;

			case WM_MOVE:
				if ( this->msgMove( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_SIZE:
				if ( this->msgSize( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_ERASEBKGND:
				if ( this->msgEraseBackground( ( HDC ) wParam ) == 1 ) {
					return 1;
				}
				break;

			case WM_LBUTTONDOWN:
				if ( this->msgLButtonDown( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_MOUSEMOVE:
				if ( this->msgMouseMove( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_SETCURSOR:
				if ( this->msgSetCursor( wParam, lParam ) == TRUE ) {
					return TRUE;
				}
				break;
		}
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CUdWindow::msgMouseMove( WPARAM wParam, LPARAM lParam ) {
		return 1;
	} // LRESULT msgMouseMove

	LRESULT CUdWindow::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		return 1;
	} // LRESULT msgLButtonDown

	LRESULT CUdWindow::msgMove( WPARAM wParam, LPARAM lParam ) {
		this->get( "x" )->ini( ( udInt ) ( short ) LOWORD( lParam ) );
		this->get( "y" )->ini( ( udInt ) ( short ) HIWORD( lParam ) );
		return 0;
	} // LRESULT msgMove

	LRESULT CUdWindow::msgSize( WPARAM wParam, LPARAM lParam ) {
		this->get( "width" )->ini( ( udInt ) LOWORD( lParam ) );
		this->get( "height" )->ini( ( udInt ) HIWORD( lParam ) );
		return 0;
	} // LRESULT msgSize

	LRESULT CUdWindow::msgPaint( ) {
		/*if ( !GetUpdateRect( m_hWnd, NULL, FALSE ) ) {
			return 1;
		}*/
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint( m_hWnd, &ps );
		this->Render( hDC, &ps );
		EndPaint( m_hWnd, &ps );
		return 0;
	} // LRESULT msgPain

	void CUdWindow::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
	} // void Render

	LRESULT CUdWindow::msgEraseBackground( HDC hDC ) {
		return 0;
	} // LRESULT msgEraseBackground

	LRESULT CUdWindow::msgSetCursor( WPARAM wParam, LPARAM lParam ) {
		return FALSE;
	} // LRESULT msgSetCursor

	// енумераторы

	static BOOL CALLBACK udWindowDestroyChildrenProc( HWND hWnd, LPARAM lParam ) {
		::DestroyWindow( hWnd );
		return TRUE;
	}

	static BOOL CALLBACK udWindowShowChildrenProc( HWND hWnd, LPARAM lParam ) {
		::ShowWindow( hWnd, SW_SHOW );
		return TRUE;
	}

	static BOOL CALLBACK udWindowHideChildrenProc( HWND hWnd, LPARAM lParam ) {
		::ShowWindow( hWnd, SW_HIDE );
		return TRUE;
	}

	static BOOL CALLBACK udWindowEnableChildrenProc( HWND hWnd, LPARAM lParam ) {
		::EnableWindow( hWnd, TRUE );
		return TRUE;
	}

	static BOOL CALLBACK udWindowDisableChildrenProc( HWND hWnd, LPARAM lParam ) {
		::EnableWindow( hWnd, FALSE );
		return TRUE;
	}

	static BOOL CALLBACK udWindowUpdateChildrenProc( HWND hWnd, LPARAM lParam ) {
		::UpdateWindow( hWnd );
		return TRUE;
	}

	//

	void CUdWindow::DestroyChildren( ) {
		if ( m_hWnd ) {
			::EnumChildWindows( m_hWnd, udWindowDestroyChildrenProc, 0 );
		}
	} // void DestroyChildren

	void CUdWindow::ShowChildren( ) {
		if ( m_hWnd ) {
			::EnumChildWindows( m_hWnd, udWindowShowChildrenProc, 0 );
		}
	} // void ShowChildren

	void CUdWindow::HideChildren( ) {
		if ( m_hWnd ) {
			::EnumChildWindows( m_hWnd, udWindowHideChildrenProc, 0 );
		}
	} // void HideChildren

	void CUdWindow::EnableChildren( ) {
		if ( m_hWnd ) {
			::EnumChildWindows( m_hWnd, udWindowEnableChildrenProc, 0 );
		}
	} // void EnableChildren

	void CUdWindow::DisableChildren( ) {
		if ( m_hWnd ) {
			::EnumChildWindows( m_hWnd, udWindowDisableChildrenProc, 0 );
		}
	} // void EnableChildren

	void CUdWindow::UpdateChildren( ) {
		if ( m_hWnd ) {
			::EnumChildWindows( m_hWnd, udWindowUpdateChildrenProc, 0 );
		}
	} // void EnableChildren

} // namespace UdSDK