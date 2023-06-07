
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndLink::CWndLink( ) {
		m_bHovered		= false;
		m_iSize			= 12;
		m_clrText		= Color( Color::MakeARGB( 255, 0, 147, 221 ) );
		m_hWndTarget	= NULL;
	}

	CWndLink::CWndLink( udUInt size, udDWord color ) {
		m_bHovered		= false;
		m_iSize			= size;
		m_clrText		= Color( color );
		m_hWndTarget	= NULL;
	}

	CWndLink::CWndLink( udUInt size ) {
		m_bHovered		= false;
		m_iSize			= size;
		m_clrText		= Color( Color::MakeARGB( 255, 0, 147, 221 ) );
		m_hWndTarget	= NULL;
	}

	CWndLink::CWndLink( udDWord color ) {
		m_bHovered		= false;
		m_iSize			= 12;
		m_clrText		= Color( color );
		m_hWndTarget	= NULL;
	}

	CWndLink::CWndLink( const CWndLink& objCopy ) {
	}

	CWndLink::~CWndLink( ) {
	}

	CWndLink& CWndLink::operator =( const CWndLink& objCopy ) {
		return *this;
	}

	LRESULT CWndLink::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
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

	LRESULT CWndLink::msgMouseHover( WPARAM wParam, LPARAM lParam ) {
		if ( !m_bHovered ) {
			m_bHovered = true;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_LEAVE );
		return 0;
	} // LRESULT msgMouseHover

	LRESULT CWndLink::msgMouseLeave( ) {
		if ( m_bHovered ) {
			m_bHovered = false;
			this->Render( );
			this->Flush( );
		}
		this->Track( TME_HOVER );
		return 0;
	} // LRESULT msgMouseLeave

	LRESULT CWndLink::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		if ( m_hWndTarget ) {
			::PostMessage( m_hWndTarget, WM_LINKCLICK, 0, ( LPARAM ) this );
		} else {
			::PostMessage( m_hWndParent, WM_LINKCLICK, 0, ( LPARAM ) this );
		}
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndLink::msgMouseMove( WPARAM wParam, LPARAM lParam ) {
		this->Track( TME_HOVER | TME_LEAVE );
		return 0;
	} // LRESULT msgMouseMove

	void CWndLink::Render( ) {
		udPGdi		gdi		= this->GetGdi( );//new udGdi( m_imgBuffer );
		wstring		title	= this->get( "title" )->toWString( );
		udFloat		w		= this->get( "width" )->toFloat( ),
					h		= this->get( "height" )->toFloat( );
		int			style	= ( this->isset( "bold" ) ? FontStyleBold : FontStyleRegular );
		RectF		rctLink( 0.0f, 0.0f, w, h );
		SolidBrush	brhText( m_clrText );

		if ( !m_bHovered ) {
			style |= FontStyleUnderline;
		}

		gdi->Clear( Color::MakeARGB( 255, 255, 255, 255 ) );
		gdi->DrawString( title.data( ), title.size( ),
			g_pApp->GetFont( L"Tahoma", m_iSize, ( FontStyle ) style ),
			rctLink, NULL, &brhText
		);
		// рисуем рамку для отслеживания ширин окна
		/*Pen penBorder( Color::MakeARGB( 128, 150, 150, 150 ) );
		penBorder.SetDashStyle( DashStyleDot );
		gdi->DrawRectangle( &penBorder, Rect(
			0, 0, ( int ) w - 1, ( int ) h - 1
		) );*/
	} // void Render

	void CWndLink::CalculateWidthHeight( HWND hParent, calcType calc ) {
		if ( hParent ) {
			udInt	width	= 0,
					height	= 0;
			wstring	title	= this->get( "title" )->toWString( );
			udPFont	fntText	= g_pApp->GetFont( L"Tahoma", m_iSize );
			udPGdi	gdi		= new udGdi( hParent );
			RectF rctLayout( 0.0f, 0.0f, 1000.0f, 1000.0f ), rctBound( 0.0f, 0.0f, 1000.0f, 1000.0f );
			gdi->MeasureString( title.data( ), title.size( ), fntText, rctLayout, NULL, &rctBound );
			width	= ( int ) ceil( rctBound.Width );
			height	= ( int ) ceil( rctBound.Height );
			if ( calc == WNDLINK_CALCULATE_HEIGHT ) {
				this->get( "height" )->ini( height );
			} else if ( calc == WNDLINK_CALCULATE_WIDTH ) {
				this->get( "width" )->ini( width );
			} else {
				this->get( "width" )->ini( width );
				this->get( "height" )->ini( height );
			}
			delete gdi;
		}
	} // void CalculateWidthHeight

	bool CWndLink::Create( HWND hParent, HMENU hMenu ) {
		if ( !this->isset( "width" ) && !this->isset( "height" ) ) {
			this->CalculateWidthHeight( hParent );
		} else if ( !this->isset( "width" ) ) {
			this->CalculateWidthHeight( hParent, WNDLINK_CALCULATE_WIDTH );
		} else if ( !this->isset( "height" ) ) {
			this->CalculateWidthHeight( hParent, WNDLINK_CALCULATE_HEIGHT );
		}
		return udWndGraphic::Create( hParent, hMenu );
	} // void Create

	LRESULT CWndLink::msgSetCursor( WPARAM wParam, LPARAM lParam ) {
		::SetCursor( g_pApp->GetCursor( "hand" ) );
		return TRUE;
	} // LRESULT msgSetCursor

	void CWndLink::Track( udDWord dwFlags ) {
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
