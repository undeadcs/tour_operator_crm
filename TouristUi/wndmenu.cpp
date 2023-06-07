
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndMenu::CWndMenu( ) {
		m_iSelected	= -1;
		m_pModel	= NULL;
	}

	CWndMenu::CWndMenu( const CWndMenu& objCopy ) {
	}

	CWndMenu::~CWndMenu( ) {
		m_pModel = NULL;
		if ( !m_arrButton.empty( ) ) {
			for( _int_asc_btn::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
				delete i->second;
			}
			m_arrButton.clear( );
		}
		m_mapButton.clear( );
	}

	CWndMenu& CWndMenu::operator =( const CWndMenu& objCopy ) {
		return *this;
	}

	udPWndMenuBtn CWndMenu::AddItem( string index, wstring title, bool selected, bool create ) {
		_asc_btn::iterator i = m_mapButton.find( index );
		if ( i != m_mapButton.end( ) ) {
			return i->second;
		}
		
		udInt	iIndex	= ( udInt ) m_arrButton.size( );
		udInt	x		= 21,
				y		= 21,
				w		= 191,
				h		= 44;
		udDWord	style	= WS_CHILD | WS_TABSTOP;
		// позиционируем кнопку относительно других
		if ( iIndex > 0 ) {
			udPWndMenuBtn btn = m_arrButton[ iIndex - 1 ];
			y = btn->get( "y" )->toInt( ) + btn->get( "height" )->toInt( ) + btn->get( "margin-bottom" )->toInt( );
		}
		//
		udPWndMenuBtn item = new udWndMenuBtn( this,
			Rect( 22, 154, w, h ),
			Rect( 458, 154, w, h ),
			Rect( 1151, 313, w, h ),
			selected
		);
		item->get( "#index" )->ini( iIndex );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "style" )->ini( style );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "margin-bottom" )->ini( ( udInt ) 8 );
		item->get( "padding-left" )->ini( ( udInt ) 13 );
		item->get( "padding-top" )->ini( ( udInt ) 11 );

		if ( create ) {
			item->Create( m_hWnd, ( HMENU ) iIndex );
		}
		if ( selected ) {
			m_iSelected = iIndex;
		}

		m_arrButton[ iIndex ] = item;
		m_mapButton[ index ] = item;

		return item;
	} // udPWndMenuBtn AddItem

	bool CWndMenu::SelectItem( string index ) {
		_asc_btn::iterator i = m_mapButton.find( index );
		if ( i != m_mapButton.end( ) ) {
			if ( m_iSelected > -1 ) {
				( m_arrButton[ m_iSelected ] )->SetSelected( false );
			}
			udPWndMenuBtn btn = i->second;
			btn->SetSelected( true );
			m_iSelected = btn->get( "#index" )->toInt( );
		}
		return false;
	} // void SelectItem

	void CWndMenu::ClearCurrentItem( ) {
		if ( m_iSelected > -1 ) {
			( m_arrButton[ m_iSelected ] )->SetSelected( false );
			m_iSelected = -1;
		}
	} // void ClearCurrentItem

	LRESULT CWndMenu::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_MENU_BUTTON_CLICK:
				return this->msgBtnClick( ( udPWndMenuBtn ) lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndMenu::msgBtnClick( udPWndMenuBtn btn ) {
		if ( !m_pModel->ClickMenu( btn->get( "index" )->toString( ) ) ) {
			return 0;
		}
		if ( m_iSelected > -1 ) {
			( m_arrButton[ m_iSelected ] )->SetSelected( false );
		}
		btn->SetSelected( true );
		m_iSelected = btn->get( "#index" )->toInt( );
		return 0;
	} // LRESULT msgBtnClick

	void CWndMenu::Render( ) {
		udInt	x	= this->get( "x" )->toInt( ),
				y	= this->get( "y" )->toInt( ),
				w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );
		udPGdi	gdi	= this->GetGdi( );//new udGdi( m_imgBuffer );
		udInt	iBackgroundX		= 292,
				iBackgroundY		= 283,
				iBackgroundWidth	= 239,
				iBackgroundHeight	= 100;
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );

		CompositingMode old = gdi->GetCompositingMode( );
		gdi->SetCompositingMode( CompositingModeSourceCopy );
		udRenderer::RenderBackground(
			gdi, g_pApp->GetBitmap( "sprite.png" ),
			Rect( 0, 0, w, h ),
			Rect( iBackgroundX, iBackgroundY, iBackgroundWidth, iBackgroundHeight ),
			udRenderer::UDSDK_REPEAT_Y
		);
		gdi->SetCompositingMode( old );
		gdi->DrawImage(
			imgSprite, Rect( 19, 804, 195, 33 ),
			1395, 160, 195, 33, UnitPixel
		);
	} // void Render

	udPBitmap CWndMenu::GetBackground( ) const {
		return this->GetBitmap( );
	} // udPBitmap GetBackground

} // namespace UdSDK
