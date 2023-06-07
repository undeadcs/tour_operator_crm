
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndLogin::CWndLogin( HINSTANCE hInstance ) : udWindow( hInstance ) {
		m_hWndLogin		= NULL;
		m_hWndPassword	= NULL;
	}

	CWndLogin::CWndLogin( const CWndLogin& objCopy ) : udWindow( udWindow::GetHInstance( ) ) {
	}

	CWndLogin::~CWndLogin( ) {
	}

	CWndLogin& CWndLogin::operator =( const CWndLogin& objCopy ) {
		return *this;
	}

	bool CWndLogin::Create( HWND hParent, HMENU hMenu ) {
		if ( udWindow::Create( hParent, hMenu ) ) {	// если создали окно добавляем поля ввода
			// устанавливаем отсекающий регион окна
			this->SetWindowPathRegion( );
			// добавляем инпут логина
			m_hWndLogin = CreateWindowEx(
				0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL,
				15, 65, 142, 15, m_hWnd,
				( HMENU ) this->get( "input_login_id" )->toWord( ),
				g_pApp->GetHInstance( ), NULL
			);
			if ( !m_hWndLogin ) {
				return false;
			}
			// добавляем инпут пароля
			m_hWndPassword = CreateWindowEx(
				0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD,
				15, 114, 142, 15, m_hWnd,
				( HMENU ) this->get( "input_login_id" )->toWord( ),
				g_pApp->GetHInstance( ), NULL
			);
			if ( !m_hWndPassword ) {
				return false;
			}
			// устанавливаем шрифт
			HFONT hFont = g_pApp->GetHFont( m_hWnd );
			SendMessage( m_hWndLogin,		WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
			SendMessage( m_hWndPassword,	WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
			// устанавливаем фокус на поле логина
			SetFocus( m_hWndLogin );
			return true;
		}
		return false;
	} // bool Create

	udPMorph CWndLogin::GetData( ) {
		assert( m_hWndLogin );
		assert( m_hWndPassword );
		udPMorph pRet = new udMorph;

		udWChar	szLogin[ 24 ],
				szPassw[ 32 ];
		udInt	iLogin	= GetWindowText( m_hWndLogin, szLogin, 24 ),
				iPassw	= GetWindowText( m_hWndPassword, szPassw, 32 );
		if ( iLogin ) {
			pRet->get( "login" )->ini( wstring( szLogin ) );
		}
		if ( iPassw ) {
			pRet->get( "password" )->ini( wstring( szPassw ) );
		}

		return pRet;
	} // udPMorph GetData

	LRESULT CWndLogin::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_PAINT:
				return this->msgPaint( );
				break;

			case WM_LBUTTONDOWN:
				return this->msgLButtonDown( wParam, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam )  );
				break;

			case WM_DESTROY:
				g_pApp->Exit( );
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndLogin::msgLButtonDown( WPARAM wParam, udInt iX, udInt iY ) {
		if ( ( iY > 148 ) && ( iY < 149 + 29 ) ) { // клик в строке кнопок
			if ( ( iX > 9 ) && ( iX < 10 + 79 ) ) { // клик по кнопке ОК
				g_pApp->Login( );
				return 1;
			} else if ( ( iX > 102 ) && ( iX < 102 + 79 ) ) { // клик по кнопе Отмена
				g_pApp->Exit( );
				return 1;
			}
		}
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndLogin::msgPaint( ) {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint( m_hWnd, &ps );
		this->Render( hDC, &ps );
		EndPaint( m_hWnd, &ps );
		return 0;
	} // LRESULT msgPain

	void CWndLogin::SetWindowPathRegion( ) {
		assert( m_hWnd );
		HDC hDC = GetDC( m_hWnd );
		HRGN hRegion = CreateRoundRectRgn(
			0, 0,
			this->get( "width" )->toInt( ), this->get( "height" )->toInt( ),
			17, 17
		);
		if ( hRegion ) {
			SetWindowRgn( m_hWnd, hRegion, FALSE );
			DeleteRgn( hRegion );
		}
		ReleaseDC( m_hWnd, hDC );
	} // void SetWindowPathRegion

	void CWndLogin::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
		Graphics	objRender( hDC );
		udFloat		fWidth	= this->get( "width" )->toFloat( ),
					fHeight	= this->get( "height" )->toFloat( );
		Color		clrBackground( 239, 238, 238 ),
					clrHeader( 192, 58, 44 ),
					clrLabel( 131, 130, 129 ),	// цвет надписей для инпутов
					clrBckEdit( 255, 255, 255 ),
					clrBtn( 31, 26, 23 );
		udPFont		fntHeader	= g_pApp->GetFont( ),
					fntHeader2	= g_pApp->GetFont( L"Tahoma", 14, FontStyleBold ),
					fntLabel	= g_pApp->GetFont( L"Tahoma", 10 ),
					fntBtn		= g_pApp->GetFont( );
		SolidBrush	brhHeader( clrHeader ),
					brhLabel( clrLabel ),
					brhEdit( clrBckEdit ),
					brhBtn( clrBtn );
		Pen			penBorder( Color( 0, 0, 0 ) ),
					penBorderRed( Color( 255, 0, 0 ) );
		PointF		objPos( 11.0f, 9.0f );
		Bitmap		*imgSprite = g_pApp->GetBitmap( "sprite.png" );

		// заливаем бэкграунд
		objRender.Clear( Color( 238, 238, 238 ) );
		// рисуем замок
		objRender.DrawImage(
			imgSprite, RectF( 175, 10, 100, 178 ),
			990, 255, 100, 178,
			UnitPixel
		);
		// рисуем заголовок
		objRender.DrawString( L"ИСУ БМТ Приморье", -1, fntHeader, objPos, &brhHeader );
		objPos.Y += fntHeader->GetHeight( &objRender );
		objRender.DrawString( L"Вход", -1, fntHeader2, objPos, &brhHeader );
		// рисуем надписи
		objPos.Y = 46.0f;
		objRender.DrawString( L"Логин", -1, fntHeader, objPos, &brhLabel );
		objPos.Y = 95.0f;
		objRender.DrawString( L"Пароль", -1, fntHeader, objPos, &brhLabel );
		// выводим прямоугольники едитов
		Rect rctEdit( 11, 61, 155, 22 );
		objRender.FillRectangle( &brhEdit, rctEdit );
		rctEdit.Y = 110;
		objRender.FillRectangle( &brhEdit, rctEdit );
		// выводим кнопки
		// кнопка ОК
		objRender.DrawImage(
			imgSprite, Rect( 10, 149, 79, 29 ),
			1456, 12, 79, 29, UnitPixel
		);
		objRender.DrawImage(
			imgSprite, Rect( 20, 154, 18, 18 ),
			1465, 57, 18, 18, UnitPixel
		);
		objRender.DrawString( L"ОК", -1, fntBtn, PointF( 10.0f + 30.0f, 149.0f + 8.0f ), &brhBtn );
		/*objRender.DrawRectangle(
			&penBorder, Rect( 10, 149, 79, 29 )
		);
		objRender.DrawRectangle(
			&penBorderRed, Rect( 10 + 30, 149 + 8, 79 - 30, 29 - 8 )
		);*/
		// кнопка Отмена
		objRender.DrawImage(
			imgSprite, Rect( 103, 149, 79, 29 ),
			1456, 12, 79, 29, UnitPixel
		);
		objRender.DrawImage(
			imgSprite, Rect( 111, 154, 21, 18 ),
			1556, 59, 21, 18, UnitPixel
		);
		objRender.DrawString( L"Отмена", -1, fntBtn, PointF( 103.0f + 30.0f, 149.0f + 8.0f ), &brhBtn );
		/*objRender.DrawRectangle(
			&penBorder, Rect( 103, 149, 79, 29 )
		);
		objRender.DrawRectangle(
			&penBorderRed, Rect( 103 + 30, 149 + 8, 79 - 30, 29 - 8 )
		);*/
	} // LRESULT msgPaint

} // namespace UdSDK
