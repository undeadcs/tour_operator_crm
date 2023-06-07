
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CExtList::CExtList( ) {
		m_fnGetListHandler	= NULL;
		m_pObject			= NULL;
		m_hWndDlgParent		= NULL;
		m_pOptHandler		= NULL;
		m_fnAfterPick		= NULL;
		m_bReadOnly			= false;
	}

	CExtList::CExtList( const CExtList& objCopy ) {
	}

	CExtList::~CExtList( ) {
		udDELETE( m_pObject );
	}

	CExtList& CExtList::operator =( const CExtList& objCopy ) {
		return *this;
	}

	LRESULT CExtList::msgSetCursor( WPARAM wParam, LPARAM lParam ) {
		if ( m_bReadOnly ) {
			::SetCursor( g_pApp->GetCursor( "arrow" ) );
		} else {
			::SetCursor( g_pApp->GetCursor( "hand" ) );
		}
		return TRUE;
	} // LRESULT msgSetCursor

	LRESULT CExtList::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		if ( m_fnGetListHandler && !m_bReadOnly ) {
			HWND hWnd = ( m_hWndDlgParent ? m_hWndDlgParent : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ) );
			udPListHandler handler = m_fnGetListHandler( );
			if ( m_pOptHandler ) {
				m_pOptHandler->apply( handler->GetLstOpt( ) );
			}
			udPMorph obj = DialogListPick( hWnd, handler );
			if ( obj ) {
				udDELETE( m_pObject );
				m_pObject = new udBase;
				obj->apply( m_pObject );
				delete obj;
				if ( m_fnAfterPick ) {
					m_fnAfterPick( m_pObject, handler );
				}
				this->Render( );
				this->Flush( );
			}
			delete handler;
		}
		return 0;
	} // LRESULT msgLButtonDown

	void CExtList::Render( ) {
		udPGdi	gdi	= this->GetGdi( );
		udPFont fntLabel	= g_pApp->GetFont( );
		wstring	title		= L"";
		udInt	w			= this->get( "width" )->toInt( ),
				h			= this->get( "height" )->toInt( ),
				padLeft		= 7,
				padTop		= 7,
				iImgW		= 27,	// ширина картинки с плюсом
				iImgH		= 29;	// высота картинки с плюсом
		RectF	rctLayout(
			( float ) padLeft, ( float ) padTop,
			( float ) ( w - iImgW - padLeft ), ( float ) ( h - padTop )
		);
		gdi->Clear( Color( 255, 255, 255 ) );
		if ( m_pObject ) {
			if ( m_pObject->isset( m_szAttr ) ) {
				SolidBrush	brhLabel( Color( 0, 0, 0 ) );
				title = m_pObject->get( m_szAttr )->toWString( );
				// пишем надпись
				gdi->DrawString( title.data( ), title.size( ), fntLabel,
					rctLayout,
					NULL, &brhLabel
				);
			}
		} else {
			SolidBrush	brhLabel( Color( 200, 200, 200 ) );
			title = this->get( "title" )->toWString( );
			// пишем надпись
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				rctLayout, NULL, &brhLabel
			);
		}
		gdi->DrawImage( g_pApp->GetBitmap( "sprite.png" ),
			Rect( ( w - iImgW - 1 ), 0, iImgW, iImgH ),
			599, 12, iImgW, iImgH, UnitPixel
		);
		Pen penBorder( Color( 201, 202, 204 ), 2 );
		gdi->DrawLine( &penBorder, 0, 1, ( w - iImgW ), 0 );
		gdi->DrawLine( &penBorder, 0, h - 1, ( w - iImgW ), h - 1 );
		gdi->DrawLine( &penBorder, 1, 1, 1, h - 1 );
	} // void Render

	void CExtList::SetObj( udPBase obj ) {
		udDELETE( m_pObject );
		m_pObject = obj;
		this->Render( );
		this->Flush( );
	} // void SetObj

	void CExtList::ClearSelection( ) {
		udDELETE( m_pObject );
		this->Render( );
		this->Flush( );
	} // void ClearSelection

} // namespace UdSDK
