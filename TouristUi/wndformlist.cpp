
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndFormList::CWndFormList( udPFormHandler handler ) {
		m_pHandler		= handler;
		m_wndRows		= new udWndRows;
		m_wndScroller	= new udWndScroller;
		m_arrObject		= new udDwAscBase;
		m_wndAdd		= new udWndLink( ( udUInt ) 10 );
		m_wndFolder		= new udWndLink( ( udUInt ) 10 );
	}

	CWndFormList::CWndFormList( const CWndFormList& objCopy ) {
	}

	CWndFormList::~CWndFormList( ) {
		m_pHandler = NULL;
		udDELETE( m_wndRows );
		udDELETE( m_wndScroller );
		udDELETE( m_wndAdd );
		udDELETE( m_wndFolder );
		ClearArrBase( m_arrToDel );
		
		if ( m_arrObject ) {
			if ( !m_arrObject->empty( ) ) {
				for( udDwAscBase::iterator i = m_arrObject->begin( ); i != m_arrObject->end( ); ++i ) {
					delete i->second;
				}
				m_arrObject->clear( );
			}
			delete m_arrObject;
			m_arrObject = NULL;
		}
	}

	CWndFormList& CWndFormList::operator =( const CWndFormList& objCopy ) {
		return *this;
	}

	void CWndFormList::Clear( ) {
		m_wndRows->ClearRows( );
		ClearArrBase( m_arrToDel );
		if ( m_arrObject ) {
			if ( !m_arrObject->empty( ) ) {
				for( udDwAscBase::iterator i = m_arrObject->begin( ); i != m_arrObject->end( ); ++i ) {
					delete i->second;
				}
				m_arrObject->clear( );
			}
		}
		DirectoryClear( m_szFolder );
	} // void Clear

	void CWndFormList::AddAttr( string name ) {
		m_wndRows->AddAttr( name );
	} // void AddAttr

	udPBase CWndFormList::AddItem( udPBase item ) {
		m_wndRows->AddRow( item );
		if ( m_arrObject->empty( ) ) {
			( *m_arrObject )[ 0 ] = item;
		} else {
			udDwAscBase::iterator i = m_arrObject->end( );
			--i;
			udDWord index = i->first;
			++index;
			( *m_arrObject )[ index ] = item;
		}
		return item;
	} // udPBase AddItem

	LRESULT CWndFormList::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_ERASEBKGND:
				if ( msgEraseBackground( ( HDC ) wParam ) == 1 ) {
					return 1;
				}
				break;

			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndFormList::msgEraseBackground( HDC hDC ) {
		return 1;
	} // LRESULT msgEraseBackground

	void CWndFormList::Render( HDC hDC, LPPAINTSTRUCT pPs ) {
		udGdi gdi( hDC );
		gdi.Clear( Color( 255, 255, 255 ) );
	} // void Render

	bool CWndFormList::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWindow::Create( hParent, hMenu ) ) {
			return false;
		}

		udInt	w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );
		
		m_wndAdd->get( "title" )->ini( wstring( L"Добавить" ) );
		m_wndAdd->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndAdd->CalculateWidthHeight( m_hWnd );
		udInt	x	= w - m_wndAdd->get( "width" )->toInt( ) - 1,
				y	= h - m_wndAdd->get( "height" )->toInt( ) - 1;
		m_wndAdd->get( "x" )->ini( x );
		m_wndAdd->get( "y" )->ini( y );
		m_wndAdd->Create( m_hWnd );

		if ( !m_szFolder.empty( ) ) {
			m_wndFolder->get( "title" )->ini( wstring( L"Открыть папку" ) );
			m_wndFolder->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
			m_wndFolder->CalculateWidthHeight( m_hWnd );
			x = m_wndAdd->get( "x" )->toInt( ) - m_wndFolder->get( "width" )->toInt( ) - 5,
			m_wndFolder->get( "x" )->ini( x );
			m_wndFolder->get( "y" )->ini( y );
			m_wndFolder->Create( m_hWnd );
		}

		m_wndScroller->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE | WS_VSCROLL ) );
		m_wndScroller->get( "width" )->ini( w );
		m_wndScroller->get( "height" )->ini( y );
		m_wndScroller->SetChild( m_wndRows );

		m_wndRows->SetWndList( this );
		m_wndRows->get( "title" )->ini( wstring( L"строки" ) );
		m_wndRows->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndRows->get( "x" )->ini( ( udInt ) 0 );
		m_wndRows->get( "y" )->ini( ( udInt ) 0 );
		m_wndRows->get( "width" )->ini( w - 20 );
		
		if ( !m_wndScroller->Create( m_hWnd ) ) {
			g_pApp->GetLog( )->Write( "не удалось создать окно скроллера\n" );
			return false;
		}

		return true;
	} // void Create

	LRESULT CWndFormList::msgLinkClick( LPARAM lParam ) {
		udPWndLink lnk = ( udPWndLink ) lParam;
		if ( lnk == m_wndAdd ) {
			if ( m_pHandler ) {
				m_pHandler->FormListOnAdd( this );
			}
		} else if ( lnk == m_wndFolder ) {
			if ( !m_szFolder.empty( ) ) {
				ExtOpenFolder( m_szFolder );
			}
		}
		return TRUE;
	} // LRESULT msgLinkClick

	udPBase CWndFormList::GetItem( udDWord row ) {
		udDwAscBase::iterator i = m_arrObject->find( row );
		return ( i == m_arrObject->end( ) ? NULL : i->second );
	} // udPBase GetItem

	void CWndFormList::AddToDel( udPBase obj ) {
		m_arrToDel.push_back( obj );
	} // void AddToDel

	bool CWndFormList::DeleteItem( udDWord row ) {
		if ( m_pHandler ) {
			udDwAscBase::iterator i = m_arrObject->find( row );
			if ( i != m_arrObject->end( ) ) {
				if ( m_pHandler->FormListOnDelete( this, i->second ) ) {
					delete i->second;
					m_arrObject->erase( row );
					return true;
				}
			}
		}
		return false;
	} // bool DeleteItem

	void CWndFormList::EditItem( udDWord row ) {
		if ( m_pHandler ) {
			udDwAscBase::iterator i = m_arrObject->find( row );
			if ( i != m_arrObject->end( ) ) {
				m_pHandler->FormListOnEdit( this, i->second );
				m_wndRows->UpdateRow( row );
			}
		}
	} // void EditItem

} // namespace UdSDK
