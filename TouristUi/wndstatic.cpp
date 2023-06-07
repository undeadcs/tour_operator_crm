
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndStatic::CWndStatic( ) {
		m_iSize			= 12;
		m_clrText		= Color( 0, 0, 0 );
	}

	CWndStatic::CWndStatic( udUInt size, udDWord color ) {
		m_iSize			= size;
		m_clrText		= Color( color );
	}

	CWndStatic::CWndStatic( udUInt size ) {
		m_iSize			= size;
		m_clrText		= Color( 0, 0, 0 );
	}

	CWndStatic::CWndStatic( udDWord color ) {
		m_iSize			= 12;
		m_clrText		= Color( color );
	}

	CWndStatic::CWndStatic( const CWndStatic& objCopy ) {
	}

	CWndStatic::~CWndStatic( ) {
	}

	CWndStatic& CWndStatic::operator =( const CWndStatic& objCopy ) {
		return *this;
	}

	/*LRESULT CWndStatic::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_ERASEBKGND:
				if ( this->msgEraseBackground( ( HDC ) wParam ) ) {
					return 1;
				}
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc*/

	/*LRESULT CWndStatic::msgEraseBackground( HDC hDC ) {
		udPGdi gdi = new udGdi( hDC );
		gdi->Clear( Color( 255, 255, 255 ) );
		delete gdi;
		return 1;
	} // LRESULT msgEraseBackground*/

	void CWndStatic::Render( ) {
		udPGdi		gdi		= this->GetGdi( );
		wstring		title	= this->get( "title" )->toWString( );
		udFloat		w		= this->get( "width" )->toFloat( ),
					h		= this->get( "height" )->toFloat( );
		int			style	= ( this->isset( "bold" ) ? FontStyleBold : FontStyleRegular );
		RectF		rctLink( 0.0f, 0.0f, w, h );
		SolidBrush	brhText( m_clrText );

		gdi->Clear( Color( 255, 255, 255 ) );
		gdi->DrawString( title.data( ), title.size( ),
			g_pApp->GetFont( L"Tahoma", m_iSize, ( FontStyle ) style ),
			rctLink, NULL, &brhText
		);
		// рисуем рамку для отслеживания ширин окна
		Pen penBorder( Color::MakeARGB( 128, 150, 150, 150 ) );
		penBorder.SetDashStyle( DashStyleDot );
		gdi->DrawRectangle( &penBorder, Rect(
			0, 0, ( int ) w - 1, ( int ) h - 1
		) );
		/*udPGdi orig = new udGdi( hDC );
		udPBitmap imgBuf = new udBitmap( pPs->rcPaint.right, pPs->rcPaint.bottom, orig );
		udPGdi gdi = new udGdi( imgBuf );
		wstring		title	= this->get( "title" )->toWString( );
		udFloat		w		= this->get( "width" )->toFloat( ),
					h		= this->get( "height" )->toFloat( );
		int			style	= ( this->isset( "bold" ) ? FontStyleBold : FontStyleRegular );
		RectF		rctLink( 0.0f, 0.0f, w, h );
		SolidBrush	brhText( m_clrText );

		gdi->Clear( Color( 255, 255, 255 ) );
		gdi->DrawString( title.data( ), title.size( ),
			g_pApp->GetFont( L"Tahoma", m_iSize, ( FontStyle ) style ),
			rctLink, NULL, &brhText
		);
		// рисуем рамку для отслеживания ширин окна
		Pen penBorder( Color::MakeARGB( 128, 150, 150, 150 ) );
		penBorder.SetDashStyle( DashStyleDot );
		gdi->DrawRectangle( &penBorder, Rect(
			0, 0, ( int ) w - 1, ( int ) h - 1
		) );
		CompositingMode old = orig->GetCompositingMode( );
		orig->SetCompositingMode( CompositingModeSourceCopy );
		orig->DrawImage( imgBuf, 0, 0 );
		orig->SetCompositingMode( old );
		delete imgBuf;
		delete gdi;
		delete orig;*/
	} // void Render

	void CWndStatic::CalculateWidthHeight( HWND hParent, calcType calc ) {
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
			if ( calc == WNDSTATIC_CALCULATE_HEIGHT ) {
				this->get( "height" )->ini( height );
			} else if ( calc == WNDSTATIC_CALCULATE_WIDTH ) {
				this->get( "width" )->ini( width );
			} else {
				this->get( "width" )->ini( width );
				this->get( "height" )->ini( height );
			}
			delete gdi;
		}
	} // void CalculateWidthHeight

	bool CWndStatic::Create( HWND hParent, HMENU hMenu ) {
		if ( !this->isset( "width" ) && !this->isset( "height" ) ) {
			this->CalculateWidthHeight( hParent );
		}
		return udWndGraphic::Create( hParent, hMenu );
	} // void Create

} // namespace UdSDK
