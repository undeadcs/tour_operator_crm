
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndToolbar::CWndToolbar( ) {
		m_pModel = NULL;
	}

	CWndToolbar::CWndToolbar( const CWndToolbar& objCopy ) {
	}

	CWndToolbar::~CWndToolbar( ) {
		m_pModel = NULL;
		if ( !m_arrButton.empty( ) ) {
			for( _int_asc_btn::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
				delete i->second;
			}
			m_arrButton.clear( );
			m_mapButton.clear( );
		}
	}

	CWndToolbar& CWndToolbar::operator =( const CWndToolbar& objCopy ) {
		return *this;
	}

	void CWndToolbar::AddItem( udPWndToolbarBtn btn, string index ) {
		_asc_btn::iterator i = m_mapButton.find( index );
		if ( i == m_mapButton.end( ) ) {
			m_mapButton[ index ] = btn;
			m_arrButton[ m_arrButton.size( ) ] = btn;
		}
	} // void AddItem

	udPWndToolbarBtn CWndToolbar::AddItem( string index, wstring title, bool enabled, bool create ) {
		_asc_btn::iterator i = m_mapButton.find( index );
		if ( i != m_mapButton.end( ) ) {
			return i->second;
		}

		udInt	x	= 32,
				y	= 12,
				w	= 80,
				h	= 70,
				m	= 0;
		udInt	iIndex	= m_arrButton.size( );
		if ( iIndex > 0 ) {
			udPWndToolbarBtn btn = m_arrButton[ iIndex - 1 ];
			m = btn->get( "margin-right" )->toInt( );
			x = btn->get( "x" )->toInt( ) + btn->get( "width" )->toInt( ) + m;
		}

		udPWndToolbarBtn item = new udWndToolbarBtn( this, enabled );
		item->get( "#index"			)->ini( iIndex );
		item->get( "index"			)->ini( index );
		item->get( "title"			)->ini( title );
		item->get( "style"			)->ini( ( udDWord ) ( WS_CHILD | WS_TABSTOP ) );
		item->get( "x"				)->ini( x );
		item->get( "y"				)->ini( y );
		item->get( "width"			)->ini( w );
		item->get( "height"			)->ini( w );
		item->get( "margin-right"	)->ini( m );
		if ( create ) {
			item->Create( m_hWnd, ( HMENU ) iIndex );
		}
		m_mapButton[ index ] = item;
		m_arrButton[ m_arrButton.size( ) ] = item;
		return item;
	} // udPMorph AddItem

	void CWndToolbar::EnableItem( string index, bool enabled ) {
		_asc_btn::iterator i = m_mapButton.find( index );
		if ( i != m_mapButton.end( ) ) {
			i->second->SetEnabled( enabled );
		}
	} // void EnableItem

	LRESULT CWndToolbar::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_TOOLBAR_BUTTON_CLICK:
				return this->msgBtnClick( ( udPWndToolbarBtn ) lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndToolbar::msgBtnClick( udPWndToolbarBtn btn ) {
		m_pModel->ClickToolbar( btn->get( "index" )->toString( ) );
		return 0;
	} // LRESULT msgBtnClick

	void CWndToolbar::Render( ) {
		udInt	x	= this->get( "x" )->toInt( ),
				y	= this->get( "y" )->toInt( ),
				w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );
		udPGdi	gdi	= this->GetGdi( );

		udInt	iBackgroundX		= 786,
				iBackgroundY		= 286,
				iBackgroundWidth	= 171,
				iBackgroundHeight	= 94;

		CompositingMode old = gdi->GetCompositingMode( );
		gdi->SetCompositingMode( CompositingModeSourceCopy );
		udRenderer::RenderBackground(
			gdi, g_pApp->GetBitmap( "sprite.png" ),
			Rect( 0, 0, w, h ),
			Rect( iBackgroundX, iBackgroundY, iBackgroundWidth, iBackgroundHeight ),
			udRenderer::UDSDK_REPEAT_X
		);
		gdi->SetCompositingMode( old );
	} // void Render

	udPBitmap CWndToolbar::GetBackground( ) const {
		return this->GetBitmap( );
	} // udPBitmap

} // namespace UdSDK
