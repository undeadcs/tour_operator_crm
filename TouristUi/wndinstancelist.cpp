
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndInstanceList::CWndInstanceList( ) {
		m_lSelected		= -1;
		m_hWndTarget	= NULL;
	}

	CWndInstanceList::CWndInstanceList( const CWndInstanceList& objCopy ) {
	}

	CWndInstanceList::~CWndInstanceList( ) {
		if ( !m_arrRow.empty( ) ) {
			for( _l_asc_rows::iterator i = m_arrRow.begin( ); i != m_arrRow.end( ); ++i ) {
				for( _l_asc_row::iterator j = i->second.begin( ); j != i->second.end( ); ++j ) {
					delete j->second;
				}
				i->second.clear( );
			}
			m_arrRow.clear( );
		}
		if ( !m_rowHeader.empty( ) ) {
			for( _l_asc_row::iterator i = m_rowHeader.begin( ); i != m_rowHeader.end( ); ++i ) {
				delete i->second;
			}
		}
	}

	CWndInstanceList& CWndInstanceList::operator =( const CWndInstanceList& objCopy ) {
		return *this;
	}

	LRESULT CWndInstanceList::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_BUTTONCLICK:
				return this->msgButtonClick( lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndInstanceList::msgButtonClick( LPARAM lParam ) {
		udPWndButton btn = ( udPWndButton ) lParam;
		HWND hWndTo = ( m_hWndTarget ? m_hWndTarget : m_hWndParent );
		if ( btn->isset( "its_header" ) ) {
			::PostMessage( hWndTo, WM_LIST_HEADER_CLICK, 0, lParam );
		} else {
			udLong row = btn->get( "#row" )->toLong( );
			if ( m_lSelected != row ) {
				::PostMessage( hWndTo, WM_LIST_ROW_SELECT, 0, ( LPARAM ) row );
			}
		}
		return 1;
	} // LRESULT msgButtonClick

	void CWndInstanceList::Render( ) {
		udPGdi gdi = this->GetGdi( );
		gdi->Clear( Color( 200, 200, 200 ) );
	} // void Render

	udPMorph CWndInstanceList::AddAttr( const string& name, const wstring& title, const string& attr ) {
		udLong		num		= m_arrAttr.size( );
		udPMorph	item	= new udMorph;
		item->get( "name" )->ini( name );
		if ( attr.empty( ) ) {
			item->get( "attr" )->ini( name );
		} else {
			item->get( "attr" )->ini( attr );
		}
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( num );
		m_arrAttr[ num ] = item;
		return item;
	} // udPMorph AddAttr

	void CWndInstanceList::GenerateRows( udPArrBase arrObject ) {
		udInt		x		= 1,
					y		= 1,
					w		= 0,
					h		= 0,
					iDefWidth	= 150,
					iDefHeight	= 25,
					iTotalH		= 0,
					iTotalW		= 0,
					iSumWidth	= 0;
		udLong		col			= 0;
		HMENU		id			= ( HMENU ) 1000;
		udDWord		style		= WS_CHILD | WS_VISIBLE;
		string		name		= "",
					log			= "\n";
		wstring		value		= L"";
		udPMorph	item		= NULL;
		_pcell		cell		= NULL;
		Color		clrTxtHeader( 0, 0, 0 ),
					clrBackHeader( 222, 222, 222 ),
					clrBackNormal( 248, 246, 32 ),
					clrBackSelected( 145, 200, 223 );
		// генерируем заголовок
		//log += "заголовок\n";
		for( _l_asc_attr::iterator i = m_arrAttr.begin( ); i != m_arrAttr.end( ); ++i ) {
			col = m_rowHeader.size( );
			item	= i->second;
			name	= item->get( "attr" )->toString( );
			value	= item->get( "title" )->toWString( );
			if ( item->isset( "width" ) ) {
				w = item->get( "width" )->toInt( );
			} else {
				w = iDefWidth;
			}
			h = iDefHeight;

			cell = new udWndButton( 12, clrTxtHeader, clrBackHeader );
			cell->get( "attr" )->ini( name );
			*( cell->get( "name" ) ) = *( item->get( "name" ) );
			cell->get( "title" )->ini( value );
			cell->get( "style" )->ini( style );
			cell->get( "x" )->ini( x );
			cell->get( "y" )->ini( y );
			cell->get( "width" )->ini( w );
			cell->get( "height" )->ini( h );
			cell->get( "its_header" );
			cell->SetPaddingLeft( 5 );
			cell->SetPaddingTop( 5 );
			cell->SetTargetWnd( m_hWnd );
			cell->Create( m_hWnd, id++ );
			m_rowHeader[ col ] = cell;

			//cell->dump( log );
			//log += "\n";

			x += w + 1;
			iSumWidth += w + 1;
		}
		//g_pApp->GetLog( )->Write( log );
		// генерируем тело
		Color	brhBody[ ] = {
						Color( 255, 255, 255 ),
						Color( 248, 246, 232 ),
						Color( 145, 200, 223 )				// бэкграунд выбранной строки
					};
		udPBase		obj	= NULL;
		udPMorph	attr = NULL;
		udLong		row = 0;
		//HMENU		id = ( HMENU ) 1000;
		int line = 1, cur = 0, brush = 0;
		col = 0;
		x = 1;
		y += h + 1;
		if ( iSumWidth > iTotalW ) {
			iTotalW = iSumWidth;
		}
		iTotalH += h + 1;
		//w = 150;
		//h = 25;
		//log = "\nстроки\n";
		for( udArrBase::iterator i = arrObject->begin( ); i != arrObject->end( ); ++i ) {
			iSumWidth = 0;
			line = 1 - line;
			brush = line;
			obj = *i;
			if ( m_lSelected == cur ) {
				brush = 3;
			}
			//obj->dump( log );
			//log += "\n";
			row = m_arrRow.size( );
			for( _l_asc_attr::iterator j = m_arrAttr.begin( ); j != m_arrAttr.end( ); ++j ) {
				col = m_arrRow[ row ].size( );
				attr = j->second;
				name = attr->get( "name" )->toString( );
				value = obj->get( name )->toWString( );
				if ( attr->isset( "width" ) ) {
					w = attr->get( "width" )->toInt( );
				} else {
					w = iDefWidth;
				}
				h = iDefHeight;
				cell = new udWndButton( 12, Color( 0, 0, 0 ), brhBody[ brush ] );
				cell->get( "title" )->ini( value );
				cell->get( "style" )->ini( style );
				cell->get( "x" )->ini( x );
				cell->get( "y" )->ini( y );
				cell->get( "width" )->ini( w );
				cell->get( "height" )->ini( h );
				cell->get( "#row" )->ini( row );
				cell->get( "#col" )->ini( col );
				cell->SetPaddingLeft( 5 );
				cell->SetPaddingTop( 5 );
				cell->SetTargetWnd( m_hWnd );
				cell->Create( m_hWnd, id++ );
				m_arrRow[ row ][ col ] = cell;
				x += w + 1;
				iSumWidth += w + 1;
			}
			x = 1;
			y += h + 1;
			iTotalH += h + 1;
			++cur;
			if ( iSumWidth > iTotalW ) {
				iTotalW = iSumWidth;
			}
		}
		//g_pApp->GetLog( )->Write( log );
		if ( iTotalH != this->get( "height" )->toInt( ) ) {
			++iTotalW;
			++iTotalH;
			this->RemakeBuffer(
				this->get( "width" )->toInt( ),
				this->get( "height" )->toInt( ),
				iTotalW,
				iTotalH 
			);
			this->get( "width" )->ini( iTotalW );
			this->get( "height" )->ini( iTotalH );
			this->Move( );
		}
	} // void GenerateRows

	void CWndInstanceList::ClearRows( ) {
		this->DestroyChildren( );
		for( _l_asc_row::iterator i = m_rowHeader.begin( ); i != m_rowHeader.end( ); ++i ) {
			delete i->second;
		}
		m_rowHeader.clear( );
		for( _l_asc_rows::iterator i = m_arrRow.begin( ); i != m_arrRow.end( ); ++i ) {
			for( _l_asc_row::iterator j = i->second.begin( ); j != i->second.end( ); ++j ) {
				delete j->second;
			}
			i->second.clear( );
		}
		m_arrRow.clear( );
		m_lSelected = -1;
	} // void ClearRows

	void CWndInstanceList::ClearAttr( ) {
		for( _l_asc_attr::iterator i = m_arrAttr.begin( ); i != m_arrAttr.end( ); ++i ) {
			delete i->second;
		}
		m_arrAttr.clear( );
	} // void ClearAttr

	void CWndInstanceList::SelectRow( udLong row ) {
		_l_asc_rows::iterator i = m_arrRow.find( row );
		if ( i != m_arrRow.end( ) ) {
			udPWndButton btn = NULL;
			if ( m_lSelected > -1 ) { // текущую строку делаем обычной
				_l_asc_rows::iterator selected = m_arrRow.find( m_lSelected );
				Color clrNormal = ( ( m_lSelected % 2 ) ? Color( 248, 246, 232 ) : Color( 255, 255, 255 ) );
				for( _l_asc_row::iterator j = selected->second.begin( ); j != selected->second.end( ); ++j ) {
					btn = j->second;
					btn->SetBackColor( clrNormal );
					btn->Render( );
					btn->Flush( );
				}
			}
			// новую строку делаем выбранной
			Color clrSelected( 145, 200, 223 );
			for( _l_asc_row::iterator j = i->second.begin( ); j != i->second.end( ); ++j ) {
				btn = j->second;
				btn->SetBackColor( clrSelected );
				btn->Render( );
				btn->Flush( );
			}
			m_lSelected = row;
		}
	} // void SelectRow

	void CWndInstanceList::CalculateDimensions( udInt& w, udInt& h ) {
		udInt		iDefWidth	= 150,
					iDefHeight	= 25,
					iTotalH		= 0,
					iTotalW		= 0;
		udPMorph	item		= NULL;

		for( _l_asc_attr::iterator i = m_arrAttr.begin( ); i != m_arrAttr.end( ); ++i ) {
			item = i->second;
			if ( item->isset( "width" ) ) {
				w = item->get( "width" )->toInt( );
			} else {
				w = iDefWidth;
			}
			iTotalW += w + 1;
		}
		iTotalH += iDefHeight + 1;
		iTotalH += m_arrRow.size( ) * iDefHeight;

		/*udChar tmp[ 256 ];
		sprintf_s( tmp, 256, "%d, %d\n", iTotalW, iTotalH );
		g_pApp->GetLog( )->Write( tmp );*/

		w = iTotalW;
		h = iTotalH;
	} // void CalculateDimensions

} // namespace UdSDK
