
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndMenuBtn::CWndMenuBtn( ) {
		m_pMenu			= NULL;
		m_bHovered		= false;
		m_bSelected		= false;
		//m_imgBuffer		= NULL;
	}

	CWndMenuBtn::CWndMenuBtn( udPWndMenu menu, const Rect& rctNormal, const Rect& rctSelected, const Rect& rctHovered, bool selected ) {
		m_pMenu			= menu;
		m_rctNormal		= rctNormal;
		m_rctSelected	= rctSelected;
		m_rctHovered	= rctHovered;
		m_bHovered		= false;
		m_bSelected		= selected;
		//m_imgBuffer		= NULL;
	}

	CWndMenuBtn::CWndMenuBtn( const CWndMenuBtn& objCopy ) {
	}

	CWndMenuBtn::~CWndMenuBtn( ) {
		m_pMenu = NULL;

		/*if ( m_imgBuffer ) {
			delete m_imgBuffer;
			m_imgBuffer = NULL;
		}*/
	}

	CWndMenuBtn& CWndMenuBtn::operator =( const CWndMenuBtn& objCopy ) {
		return *this;
	}

	LRESULT CWndMenuBtn::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_MOUSEMOVE:
				if ( this->msgMouseMove( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_MOUSEHOVER:
				if ( this->msgMouseHover( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_MOUSELEAVE:
				if ( this->msgMouseLeave( ) == 0 ) {
					return 0;
				}
				break;

			case WM_LBUTTONDOWN:
				if ( this->msgLButtonDown( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			/*case WM_ERASEBKGND:
				if ( this->msgEraseBackground( ( HDC ) wParam ) == 1 ) {
					return 1;
				}
				break;

			case WM_SETCURSOR:
				if ( this->msgSetCursor( wParam, lParam ) == TRUE ) {
					return TRUE;
				}
				break;*/
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	/*LRESULT CWndMenuBtn::msgSetCursor( WPARAM wParam, LPARAM lParam ) {
		return FALSE;
		if ( m_bSelected ) {
			return FALSE;
		}
		return TRUE;
	} // LRESULT msgSetCursor*/

	LRESULT CWndMenuBtn::msgMouseMove( WPARAM wParam, LPARAM lParam ) {
		if ( m_bSelected ) {
			return 0;
		}
		this->Track( TME_HOVER | TME_LEAVE );
		return 0;
	} // LRESULT msgMouseMove

	LRESULT CWndMenuBtn::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		if ( m_bSelected ) {
			return 0;
		}
		::PostMessage( m_pMenu->GetHWnd( ), WM_MENU_BUTTON_CLICK, 0, ( LPARAM ) this );
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndMenuBtn::msgMouseHover( WPARAM wParam, LPARAM lParam ) {
		if ( m_bSelected ) {
			return 0;
		}
		if ( !m_bHovered ) {
			m_bHovered = true;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_LEAVE );
		return 0;
	} // LRESULT CWndMenuBtn

	LRESULT CWndMenuBtn::msgMouseLeave( ) {
		if ( m_bSelected ) {
			return 0;
		}
		if ( m_bHovered ) {
			m_bHovered = false;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_HOVER );
		return 0;
	} // LRESULT msgMouseLeave

	/*LRESULT CWndMenuBtn::msgEraseBackground( HDC hDC ) {
		this->Flush( hDC );
		return 1;
	} // LRESULT msgEraseBackground*/

	/*void CWndMenuBtn::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
		//this->Flush( hDC );
	} // void Render*/

	/*void CWndMenuBtn::Flush( HDC hDC ) {
		udInt	x		= this->get( "x" )->toInt( ),
				y		= this->get( "y" )->toInt( ),
				w		= this->get( "width" )->toInt( ),
				h		= this->get( "height" )->toInt( );
		udPGdi	orig	= ( hDC ? new udGdi( hDC ) : new udGdi( m_hWnd ) );

		CompositingMode old = orig->GetCompositingMode( );
		orig->SetCompositingMode( CompositingModeSourceCopy );
		orig->DrawImage( m_imgBuffer, Rect( 0, 0, w, h ), 0, 0, w, h, UnitPixel );
		orig->SetCompositingMode( old );

		delete orig;
	} // void Flush*/

	/*bool CWndMenuBtn::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWindow::Create( hParent, hMenu ) ) {
			return false;
		}
		if ( m_imgBuffer ) {
			delete m_imgBuffer;
			m_imgBuffer = NULL;
		}

		udInt	w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );

		m_imgBuffer = new udBitmap( w, h );
		this->Render( );

		return true;
	} // bool Create*/

	void CWndMenuBtn::Render( ) {
		udInt		x				= this->get( "x" )->toInt( ),
					y				= this->get( "y" )->toInt( ),
					w				= this->get( "width" )->toInt( ),
					h				= this->get( "height" )->toInt( );
		//udPGdi		gdi				= new udGdi( m_imgBuffer );
		udPGdi		gdi				= this->GetGdi( );
		udPBitmap	imgBackground	= m_pMenu->GetBackground( ),
					imgSprite		= g_pApp->GetBitmap( "sprite.png" );
		wstring		title			= this->get( "title" )->toWString( );
		udPFont		fntLabel		= g_pApp->GetFont( L"Tahoma", 18 );
		SolidBrush	brhLabel( Color( 0, 0, 0 ) );

		if ( this->isset( "color" ) ) {
			brhLabel.SetColor( this->get( "color" )->toDWord( ) );
		}
		// рисуем фон
		CompositingMode old = gdi->GetCompositingMode( );
		gdi->SetCompositingMode( CompositingModeSourceCopy );
		gdi->DrawImage( imgBackground,
			Rect( 0, 0, w, h ),
			x, y, w, h, UnitPixel
		);
		gdi->SetCompositingMode( old );

		if ( m_bSelected ) {
			gdi->DrawImage( imgSprite, Rect( 0, 0, m_rctSelected.Width, m_rctSelected.Height ),
				m_rctSelected.X, m_rctSelected.Y, m_rctSelected.Width, m_rctSelected.Height, UnitPixel );
		} else {
			if ( m_bHovered ) {
				gdi->DrawImage( imgSprite, Rect( 0, 0, m_rctHovered.Width, m_rctHovered.Height ),
					m_rctHovered.X, m_rctHovered.Y, m_rctHovered.Width, m_rctHovered.Height, UnitPixel );
			} else {
				gdi->DrawImage( imgSprite, Rect( 0, 0, m_rctNormal.Width, m_rctNormal.Height ),
					m_rctNormal.X, m_rctNormal.Y, m_rctNormal.Width, m_rctNormal.Height, UnitPixel );
			}
		}
		// рисуем текст
		float	fX	= 0.0f,
				fY	= 0.0f,
				fW	= ( float ) w,
				fH	= ( float ) h;
		if ( this->isset( "padding-left" ) ) {
			fX += this->get( "padding-left" )->toFloat( );
			fW -= fX;
		}
		if ( this->isset( "padding-top" ) ) {
			fY += this->get( "padding-top" )->toFloat( );
			fH -= fY;
		}
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( fX, fY, fW, fH ), NULL, &brhLabel
		);
		/*Pen penTxtBorder( Color::MakeARGB( 128, 200, 90, 90 ) );
		penTxtBorder.SetDashStyle( DashStyleDot );
		gdi->DrawRectangle( &penTxtBorder, RectF( fX, fY, fW, fH ) );*/

		// рисуем рамку
		/*Pen penBorder( Color::MakeARGB( 128, 150, 150, 150 ) );
		penBorder.SetDashStyle( DashStyleDot );
		gdi->DrawRectangle( &penBorder, Rect( 0, 0, ( int ) w - 1, ( int ) h - 1 ) );*/

		//delete gdi;
	} // void Render

	void CWndMenuBtn::SetSelected( bool selected ) {
		if ( m_bSelected != selected ) {
			m_bSelected = selected;
			m_bHovered = false;
			this->Render( );
			this->Flush( );
		}
	} // void SetSelected

	void CWndMenuBtn::Track( udDWord dwFlags ) {
		TRACKMOUSEEVENT te;
		te.cbSize		= sizeof( TRACKMOUSEEVENT );
		te.hwndTrack	= m_hWnd;
		te.dwHoverTime	= 6;
		te.dwFlags		= 0;

		if ( dwFlags & TME_HOVER ) {
			te.dwFlags |= TME_HOVER;
		}
		if ( dwFlags & TME_LEAVE ) {
			te.dwFlags |= TME_LEAVE;
		}

		TrackMouseEvent( &te );
	} // void Track

} // namespace UdSDK