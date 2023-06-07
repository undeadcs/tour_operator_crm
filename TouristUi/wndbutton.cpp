
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndButton::CWndButton( ) {
		m_iSize		= 12;
		m_clrText	= Color( 0, 0, 0 );
		m_bHovered	= false;
		m_clrBack	= Color( 255, 255, 255 );
		m_rctNormal	= NULL;
		m_iPadL		= 0;
		m_iPadT		= 0;
		m_hWndTarget	= NULL;
	}

	CWndButton::CWndButton( udUInt size, const Color& clr, const Color& bck ) {
		m_iSize		= size;
		m_clrText	= clr;
		m_bHovered	= false;
		m_clrBack	= bck;
		m_rctNormal	= NULL;
		m_iPadL		= 0;
		m_iPadT		= 0;
		m_hWndTarget	= NULL;
	}

	CWndButton::~CWndButton( ) {
		if ( m_rctNormal ) {
			delete m_rctNormal;
			m_rctNormal = NULL;
		}
	}

	LRESULT CWndButton::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_MOUSEHOVER:
				return this->msgMouseHover( wParam, lParam );
				break;

			case WM_MOUSELEAVE:
				return this->msgMouseLeave( );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndButton::msgMouseHover( WPARAM wParam, LPARAM lParam ) {
		if ( !m_bHovered ) {
			m_bHovered = true;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_LEAVE );
		return 0;
	} // LRESULT msgMouseHover

	LRESULT CWndButton::msgMouseLeave( ) {
		if ( m_bHovered ) {
			m_bHovered = false;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_HOVER );
		return 0;
	} // LRESULT msgMouseLeave

	LRESULT CWndButton::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		if ( m_hWndTarget ) {
			::PostMessage( m_hWndTarget, WM_BUTTONCLICK, 0, ( LPARAM ) this );
		} else {
			::PostMessage( m_hWndParent, WM_BUTTONCLICK, 0, ( LPARAM ) this );
		}
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndButton::msgMouseMove( WPARAM wParam, LPARAM lParam ) {
		this->Track( TME_HOVER | TME_LEAVE );
		return 0;
	} // LRESULT msgMouseMove

	void CWndButton::Render( ) {
		udPGdi		gdi		= this->GetGdi( );//new udGdi( m_imgBuffer );
		wstring		title	= this->get( "title" )->toWString( );
		udFloat		w		= this->get( "width" )->toFloat( ),
					h		= this->get( "height" )->toFloat( );
		int			style	= FontStyleRegular;
		RectF		rctLink( 0.0f, 0.0f, w, h );
		SolidBrush	brhText( m_clrText );

		if ( m_bHovered ) {
			//style = FontStyleBold;
		}
		rctLink.X += m_iPadL;
		rctLink.Y += m_iPadT;
		rctLink.Width -= m_iPadL;
		rctLink.Height -= m_iPadT;

		gdi->Clear( m_clrBack );
		if ( m_rctNormal ) {
			gdi->DrawImage( g_pApp->GetBitmap( "sprite.png" ),
				Rect( 0, 0, m_rctNormal->Width, m_rctNormal->Height ),
				m_rctNormal->X, m_rctNormal->Y, m_rctNormal->Width, m_rctNormal->Height,
				UnitPixel
			);
		}
		gdi->DrawString( title.data( ), title.size( ),
			g_pApp->GetFont( L"Tahoma", m_iSize, ( FontStyle ) style ),
			rctLink, NULL, &brhText
		);
	} // void Render

	LRESULT CWndButton::msgSetCursor( WPARAM wParam, LPARAM lParam ) {
		return FALSE;
	} // LRESULT msgSetCursor

	void CWndButton::Track( udDWord dwFlags ) {
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
