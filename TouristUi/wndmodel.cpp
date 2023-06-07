
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndModel::CWndModel( ) {
		m_pHandler		= NULL;
	}

	CWndModel::CWndModel( const CWndModel& objCopy ) {
	}

	CWndModel::~CWndModel( ) {
		m_pHandler = NULL;
	}

	CWndModel& CWndModel::operator =( const CWndModel& objCopy ) {
		return *this;
	}

	LRESULT CWndModel::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_MOUSEMOVE:
				if ( this->msgMouseMove( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_LBUTTONDOWN:
				if ( this->msgLButtonDown( wParam, lParam ) == 0 ) {
					return 0;
				}
				break;

			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;

			case WM_BUTTONCLICK:
				return this->msgButtonClick( lParam );
				break;

			case WM_LIST_ROW_SELECT:
				return this->msgListRowSelect( lParam );
				break;

			case WM_LIST_HEADER_CLICK:
				return this->msgListHeaderClick( lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndModel::msgListHeaderClick( LPARAM lParam ) {
		/*if ( m_pHandler ) {
			udPWndButton btn = ( udPWndButton ) lParam;
			string	name	= btn->get( "name" )->toString( ),
					attr	= btn->get( "attr" )->toString( );
			m_pHandler->InsLstClickHeader( name, attr );
		}*/
		return 1;
	} // LRESULT msgListHeaderClick

	LRESULT CWndModel::msgListRowSelect( LPARAM lParam ) {
		/*if ( m_pHandler ) {
			m_pHandler->SelectInsLstRow( ( udLong ) lParam );
		}*/
		return 1;
	} // LRESULT msgListRowSelect

	LRESULT CWndModel::msgLinkClick( LPARAM lParam ) {
		if ( m_pHandler ) {
			m_pHandler->LinkClick( ( udPWndLink ) lParam );
		}
		return 1;
	} // LRESULT msgLinkClick

	LRESULT CWndModel::msgButtonClick( LPARAM lParam ) {
		if ( m_pHandler ) {
			m_pHandler->ButtonClick( ( udPWndButton ) lParam );
		}
		return TRUE;
	} // LRESULT msgButtonClick

	LRESULT CWndModel::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		//m_pHandler->ClickModel( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ), wParam );
		return 0;
	} // LRESULT msgLButtonDown

	LRESULT CWndModel::msgMouseMove( WPARAM wParam, LPARAM lParam ) {
		//m_pHandler->MouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ), wParam );
		return 0;
	} // LRESULT msgMouseMove

	void CWndModel::Render( ) {
		udInt		iWndWidth	= this->get( "width" )->toInt( ),
					iWndHeight	= this->get( "height" )->toInt( );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPGdi		gdi			= this->GetGdi( );
		//
		gdi->Clear( Color( 255, 255, 255 ) );
		udRenderer::RenderBackground(
			gdi, imgSprite,
			Rect( 0, 0, 10, iWndHeight ),
			Rect( 556, 283, 10, 101 ),
			udRenderer::UDSDK_REPEAT_Y
		);
	} // void Render

} // namespace UdSDK
