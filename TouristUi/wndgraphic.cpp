
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndGraphic::CWndGraphic( ) {
		m_gdiBuffer	= NULL;
		m_imgBuffer	= NULL;
	}

	CWndGraphic::~CWndGraphic( ) {
		if ( m_imgBuffer ) {
			delete m_imgBuffer;
			m_imgBuffer = NULL;
		}
		if ( m_gdiBuffer ) {
			delete m_gdiBuffer;
			m_gdiBuffer = NULL;
		}
	}

	void CWndGraphic::RemakeBuffer( udInt prev_w, udInt prev_h, udInt now_w, udInt now_h ) {
		if ( ( prev_w != now_w ) || ( prev_h != now_h ) ) {
			udGdi		gdi( m_hWnd );
			udPBitmap	imgBuffer	= new udBitmap( now_w, now_h, &gdi );
			udPGdi		gdiBuffer	= new udGdi( imgBuffer );

			delete m_imgBuffer;
			delete m_gdiBuffer;

			m_imgBuffer = imgBuffer;
			m_gdiBuffer = gdiBuffer;

			this->Render( );
		}
	} // void RemakeBuffer

	/*LRESULT CWndGraphic::msgSize( WPARAM wParam, LPARAM lParam ) {
		udInt	prev_w	= this->get( "width" )->toInt( ),
				prev_h	= this->get( "height" )->toInt( );
		udWindow::msgSize( wParam, lParam );
		udInt	now_w	= this->get( "width" )->toInt( ),
				now_h	= this->get( "height" )->toInt( );
		if ( ( prev_w != now_w ) || ( prev_h != now_h ) ) {
			udGdi		gdi( m_hWnd );
			udPBitmap	imgBuffer	= new udBitmap( now_w, now_h, &gdi );
			udPGdi		gdiBuffer	= new udGdi( imgBuffer );

			m_imgBuffer = imgBuffer;
			m_gdiBuffer = gdiBuffer;
		}
		return 0;
	} // LRESULT msgSize*/

	bool CWndGraphic::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWindow::Create( hParent, hMenu ) ) {
			return false;
		}
		if ( m_imgBuffer ) {
			delete m_imgBuffer;
			m_imgBuffer = NULL;
		}
		if ( m_gdiBuffer ) {
			delete m_gdiBuffer;
			m_gdiBuffer = NULL;
		}

		udInt	w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );
		udGdi	gdi( m_hWnd );

		m_imgBuffer	= new udBitmap( w, h, &gdi );
		m_gdiBuffer	= new udGdi( m_imgBuffer );
		this->Render( );

		return true;
	} // bool Create

	void CWndGraphic::Flush( HDC hDC ) {
		udInt	x	= this->get( "x" )->toInt( ),
				y	= this->get( "y" )->toInt( ),
				w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );
		udPGdi	gdi	= ( hDC ? new udGdi( hDC ) : new udGdi( m_hWnd ) );

		CompositingMode old = gdi->GetCompositingMode( );
		gdi->SetCompositingMode( CompositingModeSourceCopy );
		gdi->DrawImage( m_imgBuffer, Rect( 0, 0, w, h ), 0, 0, w, h, UnitPixel );
		gdi->SetCompositingMode( old );

		delete gdi;
	} // void CWndGraphic

	void CWndGraphic::Render( ) {
	} // void Render

	void CWndGraphic::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
		if ( !hDC ) {
			string log = "\n";
			this->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
		}
		this->Flush( hDC );
	} // void Render

	LRESULT CWndGraphic::msgEraseBackground( HDC hDC ) {
		/*if ( !hDC ) {
			string log = "\n";
			this->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
		}
		this->Flush( hDC );*/
		return 1;
	} // LRESULT msgEraseBackground

	LRESULT CWndGraphic::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_NCPAINT:
				return 0;
				//g_pApp->GetLog( )->Write( "CWndGraphic::MessageProc, WM_NCPAINT\n" );
				break;

			case WM_SYNCPAINT:
				g_pApp->GetLog( )->Write( "CWndGraphic::MessageProc, WM_SYNCPAINT\n" );
				break;

			case WM_ERASEBKGND:
				//g_pApp->GetLog( )->Write( "CWndGraphic::MessageProc, WM_ERASEBKGND\n" );
				break;

			case WM_PAINT:
				//g_pApp->GetLog( )->Write( "CWndGraphic::MessageProc, WM_PAINT\n" );
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

} // namespace UdSDK
