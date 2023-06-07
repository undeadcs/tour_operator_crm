
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndListPick::CWndListPick( ) {
		m_pHandler		= NULL;
		m_pPager		= new udPager( 1, 25, 10, 0 );
		m_wndInsLst		= NULL;
		m_btnOk			= NULL;
		m_btnCancel		= NULL;
		m_wndPager		= new udWndPager;
	}

	CWndListPick::CWndListPick( const CWndListPick& objCopy ) {
	}

	CWndListPick::~CWndListPick( ) {
		m_pHandler = NULL;
		udDELETE( m_wndInsLst );
		udDELETE( m_wndPager );
		udDELETE( m_pPager );
		udDELETE( m_btnOk );
		udDELETE( m_btnCancel );
		ClearArrBase( m_arrObject );
		if ( !m_arrLink.empty( ) ) {
			for( udArrLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				delete *i;
			}
			m_arrLink.clear( );
		}
	}

	CWndListPick& CWndListPick::operator =( const CWndListPick& objCopy ) {
		return *this;
	}

	LRESULT CWndListPick::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_BUTTONCLICK:
				return this->msgButtonClick( lParam );
				break;

			case WM_LIST_ROW_SELECT:
				return this->msgListRowSelect( lParam );
				break;

			case WM_LIST_HEADER_CLICK:
				return this->msgListHeaderClick( lParam );
				break;

			case WM_PAGER_SELECT:
				return this->msgPagerSelect( lParam );
				break;

			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndListPick::msgLinkClick( LPARAM lParam ) {
		/*WriteUdUiLog( "CWndListPick::msgLinkClick вход\n" );
		udPWndLink lnk = ( udPWndLink ) lParam;
		if ( m_pHandler && lnk ) {
			WriteUdUiLog( "CWndListPick::msgLinkClick, вызов обработчика нажатия ссылки\n" );
			m_pHandler->lstpckLinkClick( lnk, this );
		}
		WriteUdUiLog( "CWndListPick::msgLinkClick выход\n" );*/
		return TRUE;
	} // LRESULT msgLinkClick

	LRESULT CWndListPick::msgPagerSelect( LPARAM lParam ) {
		/*udUInt page = ( udUInt ) lParam;
		if ( m_pHandler ) {
			ClearArrBase( m_arrObject );
			m_wndPager->Clear( );
			m_wndInsLst->ClearRows( );
			m_pPager->SetPage( page );
			if ( m_pHandler->lstpckLoadData( m_arrObject, m_szIndex, m_pPager ) ) {
				m_wndInsLst->GenerateRows( &m_arrObject );
				udInt y = m_wndInsLst->get( "y" )->toInt( ) + m_wndInsLst->get( "height" )->toInt( ) + 10;
				m_wndPager->Generate( m_pPager );
				m_wndPager->get( "y" )->ini( y );
				m_wndPager->Move( );
				// если делать динамическим размер диалогового окна, то при изменении размера
				// списка сущностей, необходимо изменять размер всего диалогового окна
				this->RemakeWindow( );
			}
		}*/
		return TRUE;
	} // LRESULT

	LRESULT CWndListPick::msgButtonClick( LPARAM lParam ) {
		udPWndButton btn = ( udPWndButton ) lParam;
		if ( btn == m_btnOk ) {
			udLong sel = m_wndInsLst->GetSelected( );
			if ( sel > -1 ) {
				::PostMessage( m_hWndParent, WM_COMMAND, MAKEWPARAM( IDOK, 0 ), ( LPARAM ) this );
			} else {
				mb::err( m_hWnd, L"Выберите элемент из списка" );
			}
		} else if ( btn == m_btnCancel ) {
			::PostMessage( m_hWndParent, WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), ( LPARAM ) this );
		}
		return 1;
	} // LRESULT msgButtonClick

	LRESULT CWndListPick::msgListRowSelect( LPARAM lParam ) {
		m_wndInsLst->SelectRow( ( udLong ) lParam );
		return 1;
	} // LRESULT msgListRowSelect

	LRESULT CWndListPick::msgListHeaderClick( LPARAM lParam ) {
		return 1;
	} // LRESULT msgListHeaderClick

	udPBase CWndListPick::GetSelectedObj( ) {
		udLong sel = m_wndInsLst->GetSelected( );
		if ( sel > -1 ) {
			return m_arrObject[ sel ];
		}
		return NULL;
	} // udPBase GetSelectedObj

	void CWndListPick::Render( ) {
		udPGdi gdi = this->GetGdi( );
		gdi->Clear( Color( 225, 225, 225 ) );
	} // void Render

	bool CWndListPick::Create( HWND hParent, HMENU hMenu ) {
		// высоту высчитываем как сумма высот строк + высота заголовка +
		// высота окна пейджера + высота кнопок + отступы между всеми этими частями
		udInt	x		= 0,
				y		= 0,
				w		= 400,
				h		= 400,
				m		= 10,
				p		= 5,
				//
				list_height		= 0,
				pager_height	= 25,
				button_height	= 37,
				links_height	= 0;
		HMENU	id				= ( HMENU ) CONTROL_ID_LIST;
		// вычисляем высоту окна
		udDWord	style	= WS_CHILD;
		wstring	title	= L"";
		if ( !m_wndInsLst ) {
			x = p;
			y = p;
			title = L"окно списка сущностей";
			m_wndInsLst = new udWndInstanceList;
			if ( m_pHandler ) {
				//m_pHandler->lstpckInitAttr( m_szIndex, m_wndInsLst, this );
			}
			m_wndInsLst->get( "title" )->ini( title );
			m_wndInsLst->get( "style" )->ini( style );
			m_wndInsLst->get( "x" )->ini( x );
			m_wndInsLst->get( "width" )->ini( w );
			// предварительно высчитываем размеры
			m_wndInsLst->CalculateDimensions( w, list_height );	// от списка нужна ширина
			links_height = this->CalcLinksHeight( );
			if ( links_height > 0 ) {
				links_height += m;
				y += links_height;
			}
			m_wndInsLst->get( "y" )->ini( y );
			m_wndInsLst->get( "height" )->ini( list_height );
			w += p + p;
			this->get( "width" )->ini( w );
		}
		// высота нашего окна это сумма отступов, высоты списка, высоты пейджера, высоты кнопок
		h = p + links_height + list_height + m + pager_height + m + button_height + p;
		this->get( "height" )->ini( h );

		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			return false;
		}

		m_wndInsLst->SetTargetWnd( m_hWnd );
		m_wndInsLst->Create( m_hWnd, id++ );
		
		x = p + 1;
		y = p + links_height + list_height + m;
		w = m_wndInsLst->get( "width" )->toInt( );
		m_wndPager->get( "x" )->ini( x );
		m_wndPager->get( "y" )->ini( y );
		m_wndPager->get( "width" )->ini( w );
		m_wndPager->get( "height" )->ini( pager_height );
		m_wndPager->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndPager->Create( m_hWnd, id++ );

		if ( !m_arrLink.empty( ) ) {
			udPWndLink lnk = NULL;
			for( udArrLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				lnk = *i;
				lnk->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
				lnk->Create( m_hWnd, id++ );
			}
		}

		if ( !m_btnOk ) {
			x = p;
			y = p + links_height + list_height + m + pager_height + m;
			w = 103;
			h = button_height;
			title = L"Ок";
			m_btnOk = new udWndButton;
			m_btnOk->get( "title" )->ini( title );
			m_btnOk->get( "style" )->ini( style );
			m_btnOk->get( "x" )->ini( x );
			m_btnOk->get( "y" )->ini( y );
			m_btnOk->get( "width" )->ini( w );
			m_btnOk->get( "height" )->ini( h );
			m_btnOk->SetFontSize( 14 );
			m_btnOk->SetPaddingLeft( 40 );
			m_btnOk->SetPaddingTop( 10 );
			m_btnOk->SetRectNormal( new Rect( 1746, 108, w, h ) );
			m_btnOk->SetTargetWnd( m_hWnd );
			m_btnOk->Create( m_hWnd, id++ );
		}
		if ( !m_btnCancel ) {
			x += w + m;
			w = 103;
			h = button_height;
			title = L"Отмена";
			m_btnCancel = new udWndButton;
			m_btnCancel->get( "title" )->ini( title );
			m_btnCancel->get( "style" )->ini( style );
			m_btnCancel->get( "x" )->ini( x );
			m_btnCancel->get( "y" )->ini( y );
			m_btnCancel->get( "width" )->ini( w );
			m_btnCancel->get( "height" )->ini( h );
			m_btnCancel->SetFontSize( 14 );
			m_btnCancel->SetPaddingLeft( 40 );
			m_btnCancel->SetPaddingTop( 10 );
			m_btnCancel->SetRectNormal( new Rect( 1746, 161, w, h ) );
			m_btnCancel->SetTargetWnd( m_hWnd );
			m_btnCancel->Create( m_hWnd, id++ );
		}
		this->ShowChildren( );
		this->MakeWindow( );

		return true;
	} // void Create

	void CWndListPick::ClearSelection( ) {
	} // void ClearSelection

	void CWndListPick::LoadData( ) {
		/*if ( m_pHandler ) {
			if ( m_pHandler->lstpckLoadData( m_arrObject, m_szIndex, m_pPager ) ) {
				m_wndInsLst->GenerateRows( &m_arrObject );
				udInt y = m_wndInsLst->get( "y" )->toInt( ) + m_wndInsLst->get( "height" )->toInt( ) + 10;
				m_wndPager->Generate( m_pPager );
				m_wndPager->get( "y" )->ini( y );
				m_wndPager->Move( );
				// если делать динамическим размер диалогового окна, то при изменении размера
				// списка сущностей, необходимо изменять размер всего диалогового окна
				this->RemakeWindow( );
			}
		}*/
	} // void LoadData

	void CWndListPick::ClearData( ) {
		m_pPager->SetPage( 1 );
		m_pPager->SetTotal( 0 );
		m_wndInsLst->ClearRows( );
		ClearArrBase( m_arrObject );
	} // void ClearData

	void CWndListPick::ClearLinks( ) {
		if ( !m_arrLink.empty( ) ) {
			udPWndLink lnk = NULL;
			for( udArrLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				lnk = *i;
				lnk->Destroy( );
				delete lnk;
			}
			m_arrLink.clear( );
		}
	} // void CLearLinks

	void CWndListPick::CreateLinks( ) {
		if ( !m_arrLink.empty( ) ) {
			this->CalcLinksHeight( );
			HMENU id = ( HMENU ) ( CONTROL_ID_LIST + 2 );
			udPWndLink lnk = NULL;
			for( udArrLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				lnk = *i;
				lnk->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
				lnk->Create( m_hWnd, id++ );
			}
		}
	} // void CreateLinks

	void CWndListPick::RemakeWindow( ) {
		// высчитываем сколько места займет список
		// вычисляем новые размеры окна
		// пересоздаем буфер
		// устанавливаем их
		// устанавливаем размер и позицию диалогового окна (родительское по умолчанию)
		udInt	prev_w	= this->get( "width" )->toInt( ),
				prev_h	= this->get( "height" )->toInt( ),
				x		= 0,
				y		= 0,
				w		= m_wndInsLst->get( "width" )->toInt( ),
				h		= 0,
				m		= 10,
				p		= 5,
				list_height		= m_wndInsLst->get( "height" )->toInt( ),
				pager_height	= 25,
				button_height	= 37,
				links_height	= this->CalcLinksHeight( );
		// вычисляем размеры списка
		//m_wndInsLst->CalculateDimensions( w, list_height );

		g_pApp->GetLog( )->Write(
			"list_height=" + toString( list_height ) + ", height=" +
			m_wndInsLst->get( "height" )->toString( ) + ", links_height=" +
			toString( links_height ) + "\n"
		);
		// высчитываем новые размеры окна
		if ( links_height > 0 ) {
			links_height += m;
		}
		w += p + p;
		h = p + links_height + list_height + m + pager_height + m + button_height + p;

		if ( ( prev_w != w ) || ( prev_h != h ) ) { // если размеры действительно изменились
			this->RemakeBuffer( prev_w, prev_h, w, h );
			/*wstring title = L"тут будет пейджер";
			udPGdi gdi = this->GetGdi( );
			SolidBrush brush( Color( 220, 220, 220 ) ), lbl( Color( 150, 150, 150 ) );
			StringFormat frmt;
			frmt.SetAlignment( StringAlignmentCenter );
			gdi->FillRectangle( &brush, Rect(
				0, p + list_height + m,
				w, pager_height
			) );
			gdi->DrawString( title.data( ), title.size( ), g_pApp->GetFont( L"Tahoma", 11, FontStyleBold ),
				RectF(
					0, ( float ) p + list_height + m,
					( float ) w, ( float ) pager_height
				), &frmt, &lbl
			);*/

			x = p;
			y = p + links_height + list_height + m + pager_height + m;
			m_btnOk->get( "y" )->ini( y ); m_btnOk->Move( );
			m_btnCancel->get( "y" )->ini( y ); m_btnCancel->Move( );
			this->get( "width" )->ini( w );
			this->get( "height" )->ini( h );
			this->Move( );
			// устанавливаем размер диалогового окна
			SetWindowPos( m_hWndParent, NULL, 0, 0,
				w, h,
				SWP_NOMOVE
				/*SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING |
				SWP_NOZORDER | SWP_NOOWNERZORDER*/
			);
			// центруем диалоговое окно, относительно клиентской части родительского окна
			CenterOneWindowAboveParent( m_hWndParent );
		}
	} // void RemakeWindow

	void CWndListPick::MakeWindow( ) {
		udInt	prev_w	= this->get( "width" )->toInt( ),
				prev_h	= this->get( "height" )->toInt( ),
				x		= 0,
				y		= 0,
				w		= 0,
				h		= 0,
				m		= 10,
				p		= 5,
				list_height		= m_wndInsLst->get( "height" )->toInt( ),
				pager_height	= 25,
				button_height	= 37,
				links_height	= this->CalcLinksHeight( );
		//
		/*wstring title = L"тут будет пейджер";
		udPGdi gdi = this->GetGdi( );
		SolidBrush brush( Color( 220, 220, 220 ) ), lbl( Color( 150, 150, 150 ) );
		StringFormat frmt;
		frmt.SetAlignment( StringAlignmentCenter );
		gdi->FillRectangle( &brush, Rect(
			0, p + list_height + m,
			w, pager_height
		) );
		gdi->DrawString( title.data( ), title.size( ), g_pApp->GetFont( L"Tahoma", 11, FontStyleBold ),
			RectF(
				0, ( float ) p + list_height + m,
				( float ) w, ( float ) pager_height
			), &frmt, &lbl
		);*/
		if ( links_height > 0 ) {
			links_height += m;
		}

		x = p;
		y = p + links_height + list_height + m + pager_height + m;
		m_btnOk->get( "y" )->ini( y ); m_btnOk->Move( );
		m_btnCancel->get( "y" )->ini( y ); m_btnCancel->Move( );
		this->get( "width" )->ini( w );
		this->get( "height" )->ini( h );
		this->Move( );
		// устанавливаем размер диалогового окна
		SetWindowPos( m_hWndParent, NULL, 0, 0,
			w, h,
			SWP_NOMOVE
			/*SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING |
			SWP_NOZORDER | SWP_NOOWNERZORDER*/
		);
		// центруем диалоговое окно, относительно клиентской части родительского окна
		CenterOneWindowAboveParent( m_hWndParent );
	} // void MakeWindow

	void CWndListPick::AddLink( const wstring& title, const string& handler, const string& index ) {
		udPWndLink lnk = new udWndLink;
		lnk->get( "title" )->ini( title );
		lnk->get( "handler" )->ini( handler );
		lnk->get( "index" )->ini( index );
		lnk->CalculateWidthHeight( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ) );
		m_arrLink.push_back( lnk );
	} // void AddLink

	udInt CWndListPick::CalcLinksHeight( ) {
		if ( !m_arrLink.empty( ) ) {
			udPWndLink	lnk			= NULL;
			udInt		w			= m_wndInsLst->get( "width" )->toInt( ),
						ret			= 0,
						iWidthLeft	= w,
						iWidth		= 0,
						x			= 5,
						y			= 5,
						m			= 2,
						p			= 5;
			for( udArrLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				lnk = *i;
				iWidth = lnk->get( "width" )->toInt( );
				lnk->get( "x" )->ini( x );
				lnk->get( "y" )->ini( y );
				if ( iWidthLeft > iWidth ) {
					iWidthLeft -= iWidth;
					x += lnk->get( "width" )->toInt( ) + m;
					if ( ret == 0 ) {
						ret = lnk->get( "height" )->toInt( ) + m;
					}
				} else {
					iWidthLeft = w;
					x = p;
					y += lnk->get( "height" )->toInt( ) + m;
					if ( ret > 0 ) {
						ret += lnk->get( "height" )->toInt( ) + m;
					} else {
						ret = lnk->get( "height" )->toInt( ) + m;
					}
				}
			}
			return ret;
		}
		return 0;
	} // udInt CalcLinksHeight

} // namespace UdSDK