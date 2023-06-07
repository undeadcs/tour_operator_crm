
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndRows::CWndRows( udPWndFormList wndList, udInt format ) {
		m_iFormat	= format;
		m_dwRowNum	= 0;
		m_pList		= wndList;
	}

	CWndRows::CWndRows( const CWndRows& objCopy ) {
	}

	CWndRows::~CWndRows( ) {
		if ( !m_arrRow.empty( ) ) {
			for( udDwAscWndRow::iterator i = m_arrRow.begin( ); i != m_arrRow.end( ); ++i ) {
				//i->second->Destroy( );
				delete i->second;
			}
			m_arrRow.clear( );
		}
		m_dwRowNum	= 0;
		m_pList		= NULL;
		m_arrAttr.clear( );
	}

	CWndRows& CWndRows::operator =( const CWndRows& objCopy ) {
		return *this;
	}

	void CWndRows::ClearRows( ) {
		if ( !m_arrRow.empty( ) ) {
			for( udDwAscWndRow::iterator i = m_arrRow.begin( ); i != m_arrRow.end( ); ++i ) {
				i->second->Destroy( );
				delete i->second;
			}
			m_arrRow.clear( );
		}
		m_dwRowNum	= 0;
	} // void ClearRows

	void CWndRows::AddAttr( string name ) {
		m_arrAttr.push_back( name );
	} // void AddAttr

	udPWndRow CWndRows::AddRow( udPBase obj ) {
		udInt y = 0;
		udDWord row_index = 0;
		if ( m_arrRow.size( ) > 0 ) {
			udDwAscWndRow::iterator i = m_arrRow.end( );
			--i;
			udPWndRow wndLastRow = i->second;
			row_index = i->first + 1;

			y = wndLastRow->get( "y" )->toInt( ) + wndLastRow->get( "height" )->toInt( ) + 1;
		}
		udPWndRow row = new udWndRow( obj->get( m_arrAttr[ 0 ] )->toWString( ) );
		row->get( "#row" )->ini( row_index );
		row->get( "title" )->ini( wstring( L"ОКНО-СТРОКА" ) );
		row->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		row->get( "x" )->ini( ( udInt ) 0 );
		row->get( "y" )->ini( y );
		row->get( "width" )->ini( this->get( "width" )->toInt( ) );
		row->get( "height" )->ini( ( udInt ) 14 );
		if ( !row->Create( m_hWnd ) ) {
			g_pApp->GetLog( )->Write( "не удалось создать окно строки\n" );
		}
		m_arrRow[ row_index ] = row;
		this->CalculateDimensions( m_hWnd );
		MoveWindow( m_hWnd, 0, 0,
			this->get( "width" )->toInt( ), this->get( "height" )->toInt( ),
			TRUE
		);
		return row;
	} // udPWndRow AddRow

	void CWndRows::DeleteRow( udDWord id ) {
		// удаляем строку
		udDwAscWndRow::iterator i = m_arrRow.find( id );
		if ( i == m_arrRow.end( ) ) {
			return;
		}
		udPWndRow row = i->second;
		row->Destroy( );
		delete row;
		m_arrRow.erase( id );
		--m_dwRowNum;

		// производим перекалькуляцию
		udInt y = 0;
		udDWord cur = 0;
		//udDwAscWndRow arrRow;
		// копируем строки
		for( udDwAscWndRow::iterator i = m_arrRow.begin( ); i != m_arrRow.end( ); ++i ) {
			row = i->second;
			//row->get( "#row" )->ini( cur );
			row->get( "y" )->ini( y );
			//arrRow[ cur++ ] = row;
			y += row->get( "height" )->toInt( );
			row->Move( );
		}
		//m_arrRow.clear( );
		//m_arrRow = arrRow;
		this->CalculateDimensions( m_hWnd );
		this->Move( );
	} // void DeleteRow

	void CWndRows::UpdateRow( udDWord id ) {
		udDwAscWndRow::iterator i = m_arrRow.find( id );
		if ( i != m_arrRow.end( ) ) {
			//InvalidateRect( i->second->GetHWnd( ), NULL, TRUE );
		}
	} // void UpdateRow

	void CWndRows::CalculateDimensions( HWND hWnd ) {
		udInt h = 0;
		for( udDwAscWndRow::iterator i = m_arrRow.begin( ); i != m_arrRow.end( ); ++i ) {
			h += i->second->get( "height" )->toInt( ) + 1;
		}
		this->get( "height" )->ini( h );
	} // void CWndRows

	LRESULT CWndRows::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_ERASEBKGND:
				if ( this->msgEraseBackground( ( HDC ) wParam ) ) {
					return 1;
				}
				break;

			case WM_ROW_EDIT:
				return this->msgRowEdit( wParam, lParam );
				break;

			case WM_ROW_DELETE:
				return this->msgRowDelete( wParam, lParam );
				break;
		}
		return udWindow::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndRows::msgEraseBackground( HDC hDC ) {
		udGdi gdi( hDC );
		gdi.Clear( Color( 255, 255, 255 ) );
		return 1;
	} // LRESULT msgEraseBackground

	LRESULT CWndRows::msgRowEdit( WPARAM wParam, LPARAM lParam ) {
		if ( m_pList ) {
			udPWndRow wndRow = ( udPWndRow ) lParam;
			udDWord row = wndRow->get( "#row" )->toDWord( );
			m_pList->EditItem( row );
			wstring title_new = m_pList->GetItem( row )->get( m_arrAttr[ 0 ] )->toWString( );
			udPWndStatic wndStatic = m_arrRow[ row ]->GetWndStatic( );
			wndStatic->get( "title" )->ini( title_new );
			wndStatic->Render( );
			wndStatic->Flush( );
		}
		return 0;
	} // LRESULT msgRowEdit

	LRESULT CWndRows::msgRowDelete( WPARAM wParam, LPARAM lParam ) {
		if ( m_pList ) {
			udDWord row = ( ( udPWndRow ) lParam )->get( "#row" )->toDWord( );
			if ( m_pList->DeleteItem( row ) ) {
				this->DeleteRow( row );
			}
		}
		return 0;
	} // LRESULT msgRowDelete

	udPWndRow CWndRows::GetRow( udDWord id ) {
		udDwAscWndRow::iterator i = m_arrRow.find( id );
		if ( i != m_arrRow.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPWndRow GetRow

} // namespace UdSDK
