
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndToolbarBtn::CWndToolbarBtn( udPWndToolbar toolbar, bool enabled ) {
		m_pToolbar	= toolbar;
		m_iBackX	= 0;
		m_iBackY	= 0;
		m_bEnabled	= enabled;
		m_bHovered	= false;
	}

	CWndToolbarBtn::CWndToolbarBtn( const CWndToolbarBtn& objCopy ) {
	}

	CWndToolbarBtn::~CWndToolbarBtn( ) {
		m_pToolbar	= NULL;
		m_iBackX	= 0;
		m_iBackY	= 0;
		m_bEnabled	= true;
		m_bHovered	= false;
	}

	CWndToolbarBtn& CWndToolbarBtn::operator =( const CWndToolbarBtn& objCopy ) {
		return *this;
	}

	LRESULT CWndToolbarBtn::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
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
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndToolbarBtn::msgMouseMove( WPARAM wParam, LPARAM lParam ) {
		if ( !m_bEnabled ) {
			return 0;
		}
		this->Track( TME_HOVER | TME_LEAVE );
		return 0;
	} // LRESULT msgMouseMove

	LRESULT CWndToolbarBtn::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		if ( !m_bEnabled ) {
			return 0;
		}
		::PostMessage( m_pToolbar->GetHWnd( ), WM_TOOLBAR_BUTTON_CLICK, 0, ( LPARAM ) this );
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndToolbarBtn::msgMouseHover( WPARAM wParam, LPARAM lParam ) {
		if ( !m_bEnabled ) {
			return 0;
		}
		if ( !m_bHovered ) {
			m_bHovered = true;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_LEAVE );
		return 0;
	} // LRESULT CWndToolbarBtn

	LRESULT CWndToolbarBtn::msgMouseLeave( ) {
		if ( !m_bEnabled ) {
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

	void CWndToolbarBtn::Render( ) {
		udInt		x				= this->get( "x" )->toInt( ),
					y				= this->get( "y" )->toInt( ),
					w				= this->get( "width" )->toInt( ),
					h				= this->get( "height" )->toInt( );
		udPGdi		gdi				= this->GetGdi( );
		udPBitmap	imgBackground	= m_pToolbar->GetBackground( ),
					imgSprite		= g_pApp->GetBitmap( "sprite.png" );
		wstring		title			= this->get( "title" )->toWString( );
		udPFont		fntLabel		= g_pApp->GetFont( );
		SolidBrush	brhLabel( Color( 0, 0, 0 ) );
		StringFormat	fmtLabel;

		fmtLabel.SetAlignment( StringAlignmentCenter );

		if ( !m_bEnabled ) {
			brhLabel.SetColor( Color( 169, 169, 169 ) );
		}
		// рисуем фон
		CompositingMode old = gdi->GetCompositingMode( );
		gdi->SetCompositingMode( CompositingModeSourceCopy );
		gdi->DrawImage( imgBackground,
			Rect( 0, 0, w, h ),
			x, y, w, h, UnitPixel
		);
		gdi->SetCompositingMode( old );

		if ( m_bEnabled ) {
			gdi->DrawImage( imgSprite, Rect( m_iBackX, m_iBackY, m_rctNormal.Width, m_rctNormal.Height ),
				m_rctNormal.X, m_rctNormal.Y, m_rctNormal.Width, m_rctNormal.Height, UnitPixel );
		} else {
			gdi->DrawImage( imgSprite, Rect( m_iBackX, m_iBackY, m_rctDisabled.Width, m_rctDisabled.Height ),
				m_rctDisabled.X, m_rctDisabled.Y, m_rctDisabled.Width, m_rctDisabled.Height, UnitPixel );
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
			RectF( fX, fY, fW, fH ), &fmtLabel, &brhLabel
		);

		// рисуем рамку
		if ( m_bHovered ) {
			/*Pen	penBorderLT( Color( 222, 222, 222 ), 2 ),
				penBorderRB( Color( 150, 150, 150 ), 2 );
			// рамка слева-сверху
			gdi->DrawLine( &penBorderLT, 0, 1, ( int ) ( w - 1.0f ), 0 );
			gdi->DrawLine( &penBorderLT, 1, 1, 1, ( int ) ( h - 1.0f ) );
			// рамка справа-снизу
			gdi->DrawLine( &penBorderRB, ( int ) ( w - 1.0f ), 1, ( int ) ( w - 1.0f ), ( int ) ( h - 1.0f ) );
			gdi->DrawLine( &penBorderRB, 1, ( int ) ( h - 1.0f ), ( int ) ( w - 1.0f ), ( int ) ( h - 1.0f ) );*/
		}
	} // void Render

	void CWndToolbarBtn::SetEnabled( bool enabled ) {
		if ( m_bEnabled != enabled ) {
			m_bEnabled = enabled;
			m_bHovered = false;
			this->Render( );
			this->Flush( );
		}
	} // void SetEnabled

	void CWndToolbarBtn::Track( udDWord dwFlags ) {
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
