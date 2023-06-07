
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHInfoPage::CHInfoPage( ) {
		udPWndLink		item	= NULL;
		udInt			x		= 32,
						y		= 10,
						w		= 100,
						h		= 25,
						margin	= 5,	// отступ снизу после ссылки
						iIndex	= 0;
		udUInt			size	= 18;
		string			index	= "";
		wstring			title	= L"";
		
		index	= "main";
		title	= L"На главную";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		y += h + margin;

		index	= "country";
		title	= L"Страны";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "tservice";
		title	= L"Типы услуги";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "sorder";
		title	= L"Состояния заявки";
		w		= 160;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "rrequest";
		title	= L"Результаты обращения";
		w		= 240;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "stour";
		title	= L"Состояния тура";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "trequest";
		title	= L"Типы обращения";
		w		= 180;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "muservice";
		title	= L"Единицы измерения услуги";
		w		= 270;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "direct";
		title	= L"Направления";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "nameservice";
		title	= L"Названия услуги";
		w		= 180;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "doctype";
		title	= L"Типы документов";
		w		= 180;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "currency";
		title	= L"Валюты";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "city";
		title	= L"Города";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "currencypair";
		title	= L"Валютные пары";
		w		= 180;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "company";
		title	= L"Компании";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "cldoc";
		title	= L"Документы клиента";
		w		= 210;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "partner";
		title	= L"Партнеры";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "partnerreq";
		title	= L"Запросы партнерам";
		w		= 210;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "partneranswr";
		title	= L"Ответы партнеров";
		w		= 210;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "service";
		title	= L"Услуги";
		w		= 150;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "srequest";
		title	= L"Сост. обращения";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x = 32;
		y += h + margin;

		index	= "muservice";
		title	= L"Ед. изм. услуг";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "doctpl";
		title	= L"Шаблоны документов";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;

		x = 32;
		y += h + margin;
		index	= "tourpayorder";
		title	= L"Указания на оплату";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "doctplprinted";
		title	= L"Распечатанные документы";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "transferpoint";
		title	= L"Пункты перехода";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		x = 32;
		y += h + margin;
		index	= "route";
		title	= L"Маршруты";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		index	= "hostside";
		title	= L"Принимающие стороны";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		x = 32;
		y += h + margin;
		index	= "touristlist";
		title	= L"Списки туристов";
		w		= 200;
		item	= new udWndLink( size );
		item->get( "x" )->ini( x );
		item->get( "y" )->ini( y );
		item->get( "width" )->ini( w );
		item->get( "height" )->ini( h );
		item->get( "index" )->ini( index );
		item->get( "title" )->ini( title );
		item->get( "#index" )->ini( iIndex );
		m_arrLink[ iIndex++ ] = item;
		x += w + margin;

		m_wndModel = NULL;
	}

	CHInfoPage::CHInfoPage( const CHInfoPage& objCopy ) {
	}

	CHInfoPage::~CHInfoPage( ) {
		if ( !m_arrLink.empty( ) ) {
			//ClearArrMorph( m_arrLink );
			for( _int_asc_lnk::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				delete i->second;
			}
			m_arrLink.clear( );
		}
		if ( m_wndModel ) {
			delete m_wndModel;
			m_wndModel = NULL;
		}
	}

	CHInfoPage& CHInfoPage::operator =( const CHInfoPage& objCopy ) {
		return *this;
	}

	void CHInfoPage::Show( udPWndModel wnd ) {
		if ( m_wndModel ) {
			m_wndModel->Show( );
		}
	} // void Show

	void CHInfoPage::Init( ) {
	} // void Init

	void CHInfoPage::InitSync( udPDatabaseSync sync ) {
	} // void InitSync

	void CHInfoPage::Clear( ) {
		if ( m_wndModel ) {
			m_wndModel->Hide( );
		}
	} // void Clear

	void CHInfoPage::Load( ) {
		if ( m_wndModel ) {
			return;
		}

		udPWndModel wnd = g_pApp->GetModel( )->GetWndModel( );
		m_wndModel = new udWndGraphic;
		wnd->apply( m_wndModel );
		m_wndModel->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_TABSTOP ) );
		m_wndModel->get( "x" )->ini( ( udInt ) 0 );
		m_wndModel->get( "y" )->ini( ( udInt ) 0 );
		m_wndModel->Create( wnd->GetHWnd( ) );

		udPGdi gdi = m_wndModel->GetGdi( );
		gdi->Clear( Color( 255, 255, 255 ) );
		udInt		w	= m_wndModel->get( "width" )->toInt( ),
					h	= m_wndModel->get( "height" )->toInt( );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		gdi->Clear( Color( 255, 255, 255 ) );
		udRenderer::RenderBackground(
			gdi, imgSprite,
			Rect( 0, 0, 10, h ),
			Rect( 556, 283, 10, 101 ),
			udRenderer::UDSDK_REPEAT_Y
		);
		
		HWND		hWnd		= m_wndModel->GetHWnd( ),
					hWndTarget	= wnd->GetHWnd( );
		udInt		x		= 32,
					y		= 10,
					m		= 5,
					id		= 0;
		udDWord		style	= WS_CHILD | WS_VISIBLE;
		udPWndLink	link	= NULL;

		for( _int_asc_lnk::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
			link = i->second;
			link->get( "style" )->ini( style );
			link->get( "x" )->ini( x );
			link->get( "y" )->ini( y );
			link->CalculateWidthHeight( hWnd );
			link->SetTargetWnd( hWndTarget );
			link->Create( hWnd, ( HMENU ) id++ );
			x += link->get( "width" )->toInt( ) + 5;
			if ( ( id > 0 ) && ( id % 3 == 0 ) ) {
				y += link->get( "height" )->toInt( ) + 5;
				x = 32;
			}
		}

		m_wndModel->ShowChildren( );
	} // void Load

	void CHInfoPage::ClickToolbar( string index ) {
	} // void ClickToolbar

	void CHInfoPage::ClickModel( udInt x, udInt y, WPARAM wParam ) {
	} // void ClickModel

	void CHInfoPage::MouseMove( udInt x, udInt y, WPARAM wParam ) {
	} // void MouseMove

	udInt CHInfoPage::ToolbarAction( string index ) {
		return 0;
	} // udInt ToolbarAction

	void CHInfoPage::LinkClick( udPWndLink link ) {
		string index = link->get( "index" )->toString( );
		if ( index == "main" ) {
			g_pApp->GetModel( )->SwitchToMain( );
		} else {
			udPModelWork model = g_pApp->GetModel( );
			model->GetWndWork( )->GetMenu( )->ClearCurrentItem( );
			model->SetState( index );
		}
	} // void LinkClick

} // namespace UdSDK
