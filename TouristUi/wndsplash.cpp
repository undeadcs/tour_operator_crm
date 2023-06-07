
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndSplash::CWndSplash( HINSTANCE hInstance ) {
		m_hIcon = NULL;
	}

	CWndSplash::CWndSplash( const CWndSplash& objCopy ) {
		m_hIcon = objCopy.m_hIcon;
	}

	CWndSplash::~CWndSplash( ) {
	}

	CWndSplash& CWndSplash::operator =( const CWndSplash& objCopy ) {
		return *this;
	}

	void CWndSplash::SetState( const wstring& szTitle, udFloat percent ) {
		this->get( "load_title" )->ini( szTitle );
		this->get( "percent" )->ini( percent );
		SendMessage( m_hWnd, WM_USER + 1, 0, 0 );
	} // void Set

	void CWndSplash::SetState( const wstring& szTitle ) {
		this->get( "load_title" )->ini( szTitle );
		SendMessage( m_hWnd, WM_USER + 1, 0, 0 );
	} // void SetState

	void CWndSplash::SetState( udFloat percent ) {
		this->get( "percent" )->ini( percent );
		SendMessage( m_hWnd, WM_USER + 1, 0, 0 );
	} // void SetState

	LRESULT CWndSplash::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_GETICON:
				return this->msgGetIcon( wParam );
				break;

			case WM_PAINT:
				return this->msgPaint( );
				break;

			case ( WM_USER + 1 ):
				return this->msgUpdateProgressBar( );
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndSplash::msgGetIcon( WPARAM wParam ) {
		return ( LRESULT ) m_hIcon;
	} // LRESULT msgGetIcon

	LRESULT CWndSplash::msgPaint( ) {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint( m_hWnd, &ps );
		this->Render( hDC, &ps );
		EndPaint( m_hWnd, &ps );
		return 0;
	} // LRESULT msgPain

	void CWndSplash::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
		Graphics	objRender( hDC );
		Color		clrCommon( 255, 255, 255 );
		SolidBrush	brhCommon( clrCommon );
		Pen			penCommon( clrCommon );
		Bitmap		*pSplash	= g_pApp->GetBitmap( "splash.png" );
		udPFont		pFont		= g_pApp->GetFont( L"Tahoma", 10 );
		wstring		szTitle		= this->get( "load_title" )->toWString( );
		udFloat		fPercent	= this->get( "percent" )->toFloat( );

		if ( szTitle.empty( ) ) {
			szTitle = L"Загрузка";
		}

		// заливаем бэкграунд
		objRender.DrawImage( pSplash, 0, 0 );
		// рисуем заголовок
		objRender.DrawString(
			szTitle.data( ), szTitle.size( ), pFont,
			RectF( 308.0f, 314.0f, 172.0f, 21.0f ), NULL, &brhCommon
		);
		// рисуем рамку полосы загрузки
		objRender.DrawRectangle( &penCommon, Rect( 308, 328, 172, 9 ) );
		// рисуем полосу загрузки
		fPercent = floor( 172.0f * ( fPercent / 100.0f ) );
		objRender.FillRectangle( &brhCommon, RectF( 308.0f, 328.0f, fPercent, 9 ) );
	} // void Render

	LRESULT CWndSplash::msgUpdateProgressBar( ) {
		Graphics	objRender( m_hWnd );
		Color		clrCommon( 255, 255, 255 );
		SolidBrush	brhCommon( clrCommon );
		Bitmap		*pSplash	= g_pApp->GetBitmap( "splash.png" );
		udPFont		pFont		= g_pApp->GetFont( L"Tahoma", 10 );
		wstring		szTitle		= this->get( "load_title" )->toWString( );
		udFloat		fPercent	= this->get( "percent" )->toFloat( );

		if ( szTitle.empty( ) ) {
			szTitle = L"Загрузка";
		}

		// заливаем бэкграунд
		objRender.DrawImage(
			pSplash, Rect( 309, 308, 172, 20 ),
			309, 308, 172, 21, UnitPixel
		);
		// рисуем заголовок
		objRender.DrawString(
			szTitle.data( ), szTitle.size( ), pFont,
			RectF( 308.0f, 314.0f, 172.0f, 21.0f ), NULL, &brhCommon
		);
		// рисуем полоску
		fPercent = floor( 172.0f * ( fPercent / 100.0f ) );
		objRender.FillRectangle( &brhCommon, RectF( 308.0f, 328.0f, fPercent, 9 ) );

		return 1;
	} // void UpdateProgressBar

} // namespace UdSDK
