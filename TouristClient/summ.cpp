
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndSumm::CWndSumm( udPWndSummHandler handler ) {
		m_pHandler		= handler;
		m_lnkAdd		= new udWndLink;
		m_lnkBack		= new udWndLink;
		m_lnkConvert	= new udWndLink;
		m_bGrouping		= false;

		m_lnkAdd->get( "title" )->ini( wstring( L"зачислить" ) );
		m_lnkAdd->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lnkAdd->get( "y" )->ini( ( udInt ) 0 );
		m_lnkBack->get( "title" )->ini( wstring( L"снять" ) );
		m_lnkBack->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lnkBack->get( "y" )->ini( ( udInt ) 0 );
		m_lnkConvert->get( "title" )->ini( wstring( L"конвертировать" ) );
		m_lnkConvert->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lnkConvert->get( "y" )->ini( ( udInt ) 0 );

		if ( m_pHandler ) {
			m_pHandler->AfterConstruct( this );
		}
	}

	CWndSumm::~CWndSumm( ) {
		udDELETE( m_lnkAdd );
		udDELETE( m_lnkBack );
		udDELETE( m_lnkConvert );
		m_pHandler = NULL;
		for( dwa_wrows::iterator i = m_mapRows.begin( ); i != m_mapRows.end( ); ++i ) {
			delete i->second;
		}
		ClearDwAscBase( m_ascRows );
		for( uwa_headers::iterator i = m_ascGroups.begin( ); i != m_ascGroups.end( ); ++i ) {
			delete i->second;
		}
		m_ascGroups.clear( );
	}

	static wstring toWString_2zero_precision( udFloat v ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, L"%.2f", v );
		return wstring( tmp );
	} // wstring toWString_2zero_precision

	udInt CWndSumm::AddRow( udArrBase& arr ) {
		// отступ между строками в 1 пиксел
		udInt cnt = 0, x = 0, y = 20, w = 0, h = 0, m = 1;
		udPWndStatic add = NULL; // вновь добавляемое окно строки
		udPBase obj = NULL; // текущий объект
		udDWord index = 0; // текущий индекс объекта
		udUInt size = 12;
		///////////////////////////////////////////////////////////////////////////////////////
		if ( m_bGrouping ) {
			///////////////////////////////////////////////////////////////////////////////////
			if ( m_mapRows.empty( ) ) {
				udPWndStatic wndHeader = NULL;
				uwa_headers::iterator itHeader;
				udUInt id = 0;
				///////////////////////////////////////////////////////////////////////////////
				string log = "\nsumm: суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj = *i;
					obj->dump( log );
					log += "\n";
					id = obj->get( m_szGroupAttr )->toUInt( );
					itHeader = m_ascGroups.find( id );
					if ( itHeader == m_ascGroups.end( ) ) {
						wndHeader = new udWndStatic( ( udUInt ) 14 );
						wndHeader->get( "title" )->ini( obj->get( "partner_" )->toWString( ) );
						m_ascGroups[ id ] = wndHeader;
					}
				}
				g_pApp->GetLog( )->Write( log );
				///////////////////////////////////////////////////////////////////////////////
				udPWndStatic add1 = NULL;
				wstring title = L"";
				for( uwa_headers::iterator i = m_ascGroups.begin( ); i != m_ascGroups.end( ); ++i ) {
					add = i->second;
					add->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
					add->get( "x" )->ini( x );
					add->get( "y" )->ini( y );
					add->CalculateWidthHeight( m_hWnd );
					add->Create( m_hWnd );
					y += add->get( "height" )->toInt( ) + m;
					
					for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
						obj = *j;
						if ( obj->get( "partner" )->toUInt( ) == i->first ) {
							// создаем окно
							add1 = new udWndStatic( size );
							add1->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
							add1->get( "x" )->ini( x );
							add1->get( "y" )->ini( y );
							title = toWString_2zero_precision( obj->get( "summ" )->toFloat( ) ) + L", " +
								obj->get( "currency_" )->toWString( ) + L", " +
								obj->get( "type_" )->toWString( );
							add1->get( "title" )->ini( title );
							add1->CalculateWidthHeight( m_hWnd );
							add1->Create( m_hWnd );
							// сохраняем
							m_mapRows[ index ] = add1;
							m_ascRows[ index ] = obj;
							// следующие значения
							y += add->get( "height" )->toInt( ) + m;
							++index;
						}
					}
				}
				///////////////////////////////////////////////////////////////////////////////
			} else {
			}
			///////////////////////////////////////////////////////////////////////////////////
		} else {
			///////////////////////////////////////////////////////////////////////////////////
			if ( !m_mapRows.empty( ) ) { // если есть строки, то добавляем в конец
				dwa_wrows::iterator i = --( m_mapRows.end( ) );
				y = i->second->get( "y" )->toInt( ) + i->second->get( "height" )->toInt( ) + m;
				index = i->first;
			}
			///////////////////////////////////////////////////////////////////////////////////
			wstring title = L"";
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				// создаем окно
				add = new udWndStatic( size );
				add->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
				add->get( "x" )->ini( x );
				add->get( "y" )->ini( y );
				title = toWString_2zero_precision( obj->get( "summ" )->toFloat( ) ) + L", " +
					obj->get( "currency_" )->toWString( ) + L", " +
					obj->get( "type_" )->toWString( );
				add->get( "title" )->ini( title );
				add->CalculateWidthHeight( m_hWnd );
				add->Create( m_hWnd );
				// сохраняем
				m_mapRows[ index ] = add;
				m_ascRows[ index ] = obj;
				// следующие значения
				y += add->get( "height" )->toInt( ) + m;
				++index;
			}
			///////////////////////////////////////////////////////////////////////////////////
		}
		///////////////////////////////////////////////////////////////////////////////////////
		arr.clear( );
		return cnt;
	} // udInt AddRow

	udInt CWndSumm::ShowRow( udDWord index ) {
		dwa_wrows::iterator i = m_mapRows.find( index );
		if ( i != m_mapRows.end( ) ) {
			i->second->Hide( );
			return 1;
		}
		return 0;
	} // udInt ShowRow

	void CWndSumm::ShowRows( ) {
		for( dwa_wrows::iterator i = m_mapRows.begin( ); i != m_mapRows.end( ); ++i ) {
			i->second->Show( );
		}
	} // void ShowRows

	void CWndSumm::HideRows( ) {
		for( dwa_wrows::iterator i = m_mapRows.begin( ); i != m_mapRows.end( ); ++i ) {
			i->second->Hide( );
		}
	} // void HideRows

	void CWndSumm::ClearRows( ) {
		if ( !m_mapRows.empty( ) ) {
			for( dwa_wrows::iterator i = m_mapRows.begin( ); i != m_mapRows.end( ); ++i ) {
				i->second->Destroy( );
				delete i->second;
			}
			m_mapRows.clear( );
			ClearDwAscBase( m_ascRows );
			for( uwa_headers::iterator i = m_ascGroups.begin( ); i != m_ascGroups.end( ); ++i ) {
				i->second->Destroy( );
				delete i->second;
			}
			m_ascGroups.clear( );
		}
	} // void ClearRows

	void CWndSumm::Render( ) {
		udPGdi	gdi = this->GetGdi( );

		gdi->Clear( Color( 255, 255, 255 ) );
	} // void Render

	bool CWndSumm::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			return false;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udInt x = 0, m = 5;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_lnkAdd->get( "x" )->ini( x );
		m_lnkAdd->CalculateWidthHeight( m_hWnd );
		m_lnkAdd->Create( m_hWnd );
		x += m_lnkAdd->get( "width" )->toInt( ) + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_lnkBack->get( "x" )->ini( x );
		m_lnkBack->CalculateWidthHeight( m_hWnd );
		m_lnkBack->Create( m_hWnd );
		x += m_lnkBack->get( "width" )->toInt( ) + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		m_lnkConvert->get( "x" )->ini( x );
		m_lnkConvert->CalculateWidthHeight( m_hWnd );
		m_lnkConvert->Create( m_hWnd );
		x += m_lnkConvert->get( "width" )->toInt( ) + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pHandler ) {
			m_pHandler->AfterCreate( this );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool Create

	LRESULT CWndSumm::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndSumm::msgLinkClick( LPARAM lParam ) {
		if ( m_pHandler ) { // если есть обработчик
			udPWndLink lnk = ( udPWndLink ) lParam;
			if ( lnk == m_lnkAdd ) {
				m_pHandler->OnSummAdd( this );
			} else if ( lnk == m_lnkBack ) {
				m_pHandler->OnSummBack( this );
			} else if ( lnk == m_lnkConvert ) {
				m_pHandler->OnSummConvert( this );
			}
		}
		return TRUE;
	} // LRESULT msgLinkClick

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHSummAdd::CHSummAdd( ) {
	}

	CHSummAdd::~CHSummAdd( ) {
	}

	udInt CHSummAdd::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->SetGetObjectCallback( GetProjectFinanceSummAdd );
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			m_wndForm->AddExtlist( ctrlidPartner, "partner", NULL, "name", L"выберите партнера", GetHPartnerList );
		}
		m_wndForm->AddInput( ctrlidSumm, "summ", NULL, font, 0 );
		m_wndForm->AddExtlist( ctrlidCurrency, "currency", NULL, "name", L"выберите валюту", GetHCurrencyList );
		udArrWString arrString;
		arrString.push_back( L"Белые" );
		arrString.push_back( L"Черные" );
		m_wndForm->AddSelect( ctrlidType, "type", NULL, font, arrString );
		arrString.clear( );
		arrString.push_back( L"Наличные" );
		arrString.push_back( L"Карточка" );
		arrString.push_back( L"Безналичные" );
		m_wndForm->AddSelect( ctrlidHow, "how", NULL, font, arrString );
		arrString.clear( );
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHSummAdd::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 500 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		wstring		title		= L"";
		udPGdi		gdi			= m_wndForm->GetGdi( );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Зачислить";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			title	= L"Партнер";
			x		= 32;
			w		= lcol;
			h		= 28;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);

			x += w + m;
			box = new ModeFormControlBox;
			box->x = x;	box->y = y;	box->w = 200; box->h = 28;
			info = m_wndForm->GetInfo( ctrlidPartner );
			info->box = box;
			y += h + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Сумма";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidSumm );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Валюта";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidCurrency );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип";
		x		= 32;
		w		= lcol;
		h		= 24;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;

		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidType );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Способ";
		x		= 32;
		w		= lcol;
		h		= 24;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;

		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidHow );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Комментарий";
		x		= 32;
		w		= lcol;
		h		= 64;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHSummAdd::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		///////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			if ( obj->get( "partner" )->toUInt( ) == 0 ) {
				mb::inf( hWnd, L"Введите партнера" );
				return FormHandlerErrorFailed;
			}
		}
		if ( obj->get( "summ" )->toFloat( ) <= 0.0f ) {
			mb::inf( hWnd, L"Введите сумму" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "currency" )->toUInt( ) == 0 ) {
			mb::inf( hWnd, L"Введите валюту" );
			return FormHandlerErrorFailed;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		//obj->get( "type" )->ini( obj->get( "type" )->toInt( ) - 1 );
		///////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHSummAdd::FormOk( ) {
		return FormHandlerErrorOk;
	} // udInt FormOk

	udInt CHSummAdd::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHSummAdd::ProcForm( bool modal ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// подгрузка данных формы
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// проверка данных формы
		udInt err = ValidateForm( );
		if ( err != FormHandlerErrorOk ) {
			return err;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_wndForm->GetObj( );
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			obj->get( "partner_" )->ini( m_wndForm->GetInfo( ctrlidPartner )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		}
		obj->get( "currency_" )->ini( m_wndForm->GetInfo( ctrlidCurrency )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( obj->get( "type" )->toInt( ) ) );
		//obj->get( "how_" )->ini( udProjectFinanceOperation::GetMoneyTypeName( obj->get( "how" )->toInt( ) ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ProcForm

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHSummBack::CHSummBack( ) {
	}

	CHSummBack::~CHSummBack( ) {
	}

	udInt CHSummBack::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->SetGetObjectCallback( GetProjectFinanceSummBack );
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			m_wndForm->AddExtlist( ctrlidPartner, "partner", NULL, "name", L"выберите партнера", GetHPartnerList );
		}
		m_wndForm->AddInput( ctrlidSumm, "summ", NULL, font, 0 );
		m_wndForm->AddExtlist( ctrlidCurrency, "currency", NULL, "name", L"выберите валюту", GetHCurrencyList );
		udArrWString arrString;
		arrString.push_back( L"Белые" );
		arrString.push_back( L"Черные" );
		m_wndForm->AddSelect( ctrlidType, "type", NULL, font, arrString );
		arrString.clear( );
		arrString.push_back( L"Наличные" );
		arrString.push_back( L"Карточка" );
		arrString.push_back( L"Безналичные" );
		m_wndForm->AddSelect( ctrlidHow, "how", NULL, font, arrString );
		arrString.clear( );
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHSummBack::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 500 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		wstring		title		= L"";
		udPGdi		gdi			= m_wndForm->GetGdi( );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Снять";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			title	= L"Партнер";
			x		= 32;
			w		= lcol;
			h		= 28;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);

			x += w + m;
			box = new ModeFormControlBox;
			box->x = x;	box->y = y;	box->w = 200; box->h = 28;
			info = m_wndForm->GetInfo( ctrlidPartner );
			info->box = box;
			y += h + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Сумма";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidSumm );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Валюта";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidCurrency );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип";
		x		= 32;
		w		= lcol;
		h		= 24;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;

		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidType );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Способ";
		x		= 32;
		w		= lcol;
		h		= 24;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;

		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidHow );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Комментарий";
		x		= 32;
		w		= lcol;
		h		= 64;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHSummBack::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		///////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			if ( obj->get( "partner" )->toUInt( ) == 0 ) {
				mb::inf( hWnd, L"Введите партнера" );
				return FormHandlerErrorFailed;
			}
		}
		if ( obj->get( "summ" )->toFloat( ) <= 0.0f ) {
			mb::inf( hWnd, L"Введите сумму" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "currency" )->toUInt( ) == 0 ) {
			mb::inf( hWnd, L"Введите валюту" );
			return FormHandlerErrorFailed;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		//obj->get( "type" )->ini( obj->get( "type" )->toInt( ) - 1 );
		///////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHSummBack::FormOk( ) {
		return FormHandlerErrorOk;
	} // udInt FormOk

	udInt CHSummBack::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHSummBack::ProcForm( bool modal ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// подгрузка данных формы
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// проверка данных формы
		udInt err = ValidateForm( );
		if ( err != FormHandlerErrorOk ) {
			return err;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = m_wndForm->GetObj( );
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			obj->get( "partner_" )->ini( m_wndForm->GetInfo( ctrlidPartner )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		}
		obj->get( "currency_" )->ini( m_wndForm->GetInfo( ctrlidCurrency )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( obj->get( "type" )->toInt( ) ) );
		//obj->get( "how_" )->ini( udProjectFinanceOperation::GetMoneyTypeName( obj->get( "how" )->toInt( ) ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ProcForm

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHSummConvert::CHSummConvert( ) {
	}

	CHSummConvert::~CHSummConvert( ) {
	}

	udInt CHSummConvert::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->SetGetObjectCallback( GetProjectFinanceSummConvert );
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			m_wndForm->AddExtlist( ctrlidPartner, "partner", NULL, "name", L"выберите партнера", GetHPartnerList );
		}
		m_wndForm->AddInput( ctrlidSumm, "summ", NULL, font, 0 );
		m_wndForm->AddExtlist( ctrlidCurrency1, "currency1", NULL, "name", L"выберите валюту", GetHCurrencyList );
		m_wndForm->AddExtlist( ctrlidCurrency2, "currency2", NULL, "name", L"выберите валюту", GetHCurrencyList );
		udArrWString arrString;
		arrString.push_back( L"Белые" );
		arrString.push_back( L"Черные" );
		m_wndForm->AddSelect( ctrlidType1, "type1", NULL, font, arrString );
		arrString.clear( );
		arrString.push_back( L"- нет -" );
		arrString.push_back( L"Белые" );
		arrString.push_back( L"Черные" );
		m_wndForm->AddSelect( ctrlidType2, "type2", NULL, font, arrString );
		m_wndForm->AddText( ctrlidComment, "comment", NULL, font );
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHSummConvert::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 500 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		wstring		title		= L"";
		udPGdi		gdi			= m_wndForm->GetGdi( );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Конвертировать";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			title	= L"Партнер";
			x		= 32;
			w		= lcol;
			h		= 28;
			gdi->DrawString( title.data( ), title.size( ), fntLabel,
				RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
			);

			x += w + m;
			box = new ModeFormControlBox;
			box->x = x;	box->y = y;	box->w = 200; box->h = 28;
			info = m_wndForm->GetInfo( ctrlidPartner );
			info->box = box;
			y += h + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Сумма";
		x		= 32;
		w		= lcol;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidSumm );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Валюта \"из\"";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidCurrency1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Валюта \"в\"";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidCurrency2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип \"из\"";
		x		= 32;
		w		= lcol;
		h		= 24;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;

		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidType1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип \"в\"";
		x		= 32;
		w		= lcol;
		h		= 24;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;

		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidType2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Комментарий";
		x		= 32;
		w		= lcol;
		h		= 64;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 60;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidComment );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHSummConvert::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		///////////////////////////////////////////////////////////////////////////////////////////
		if ( m_pFrmOpt->isset( "use_partner" ) ) {
			if ( obj->get( "partner" )->toUInt( ) == 0 ) {
				mb::inf( hWnd, L"Введите партнера" );
				return FormHandlerErrorFailed;
			}
		}
		if ( obj->get( "summ" )->toFloat( ) <= 0.0f ) {
			mb::inf( hWnd, L"Введите сумму" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "currency1" )->toUInt( ) == 0 ) {
			mb::inf( hWnd, L"Введите валюту \"из\"" );
			return FormHandlerErrorFailed;
		}
		if ( ( obj->get( "currency2" )->toUInt( ) == 0 ) &&
			( obj->get( "type2" )->toInt( ) == 1 )
		) {
			mb::inf( hWnd, L"Введите валюту \"в\" или тип \"в\"" );
			return FormHandlerErrorFailed;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		// проверяем валюта - валюта, тип - тип
		udUInt	c1	= obj->get( "currency1" )->toUInt( ),
				c2	= obj->get( "currency2" )->toUInt( );
		udInt	t1	= obj->get( "type1" )->toInt( ),
				t2	= obj->get( "type2" )->toInt( ) - 1;
		if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
			mb::inf( hWnd, L"Разными должны быть валюты или типы" );
			return FormHandlerErrorFailed;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		udInt type2 = obj->get( "type2" )->toInt( );
		obj->get( "type2" )->ini( type2 - 1 );
		///////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHSummConvert::FormOk( ) {
		return FormHandlerErrorOk;
	} // udInt FormOk

	udInt CHSummConvert::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHSummConvert::ProcForm( bool modal ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// подгрузка данных формы
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// проверка данных формы
		udInt err = ValidateForm( );
		if ( err != FormHandlerErrorOk ) {
			return err;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//udPBase obj = m_wndForm->GetObj( );
		//if ( m_pFrmOpt->isset( "use_partner" ) ) {
		//	obj->get( "partner_" )->ini( m_wndForm->GetInfo( ctrlidPartner )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		//}
		//obj->get( "currency1_" )->ini( m_wndForm->GetInfo( ctrlidCurrency1 )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		//obj->get( "currency2_" )->ini( m_wndForm->GetInfo( ctrlidCurrency2 )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) );
		//obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( obj->get( "type" )->toInt( ) ) );
		//obj->get( "how_" )->ini( udProjectFinanceOperation::GetMoneyTypeName( obj->get( "how" )->toInt( ) ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ProcForm

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHWndSummTourPrice::CHWndSummTourPrice( udPBase pTour) {
		m_pTour = pTour;
	}

	CHWndSummTourPrice::~CHWndSummTourPrice( ) {
	}

	void CHWndSummTourPrice::AfterConstruct( udPWndSumm wndSumm ) {
	} // void AfterConstruct

	void CHWndSummTourPrice::OnSummAdd( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummAddForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummAdd( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummAdd );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// считываем код результата
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для снятия";
				} else if ( code == 1 ) {
					msg = L"Лицевой счет не найден";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummAdd

	void CHWndSummTourPrice::OnSummBack( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummBackForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummBack( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummBack );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для возврата";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummBack

	void CHWndSummTourPrice::OnSummConvert( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummConvertForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummConvert( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummConvert );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для конвертации";
				} else if ( code == 1 ) {
					msg = L"Отсутствует валютная пара для конвертации";
				} else if ( code == 2 ) {
					msg = L"Не установлен курс валютной пары";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummConvert

	void CHWndSummTourPrice::AfterCreate( udPWndSumm wndSumm ) {
	} // void AfterCreate

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHWndSummTour::CHWndSummTour( udPBase pTour ) {
		m_pTour = pTour;
	}

	CHWndSummTour::~CHWndSummTour( ) {
	}

	void CHWndSummTour::AfterConstruct( udPWndSumm wndSumm ) {
		wndSumm->GetLnkAdd( )->get( "title" )->ini( wstring( L"оплатить" ) );
		wndSumm->GetLnkBack( )->get( "title" )->ini( wstring( L"возврат" ) );
	} // void AfterConstruct

	void CHWndSummTour::OnSummAdd( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummAddForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummAdd( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummAdd );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTour );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// считываем код результата
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для снятия";
				} else if ( code == 1 ) {
					msg = L"Лицевой счет не найден";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummAdd

	void CHWndSummTour::OnSummBack( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummBackForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummBack( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummBack );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTour );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для возврата";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummBack

	void CHWndSummTour::OnSummConvert( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummConvertForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummConvert( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummConvert );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtTour );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для конвертации";
				} else if ( code == 1 ) {
					msg = L"Отсутствует валютная пара для конвертации";
				} else if ( code == 2 ) {
					msg = L"Не установлен курс валютной пары";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummConvert

	void CHWndSummTour::AfterCreate( udPWndSumm wndSumm ) {
	} // void AfterCreate

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHWndSummTourCost::CHWndSummTourCost( udPBase pTour ) {
		m_pTour = pTour;
		m_lblWarning	= new udWndStatic( ( udUInt ) 12, Color::MakeARGB( 255, 180, 5, 5 ) );

		m_lblWarning->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lblWarning->get( "x" )->ini( ( udInt ) 0 );
		m_lblWarning->get( "y" )->ini( ( udInt ) 0 );
		m_lblWarning->get( "title" )->ini( wstring( L"себестоимость расчитана приблизительно" ) );
	}

	CHWndSummTourCost::~CHWndSummTourCost( ) {
		udDELETE( m_lblWarning );
	}

	void CHWndSummTourCost::AfterConstruct( udPWndSumm wndSumm ) {
		wndSumm->SetGrouping( true );
		wndSumm->SetGroupAttr( "partner" );
		wndSumm->SetGroupTitleAttr( "partner_" );
	} // void AfterConstruct

	void CHWndSummTourCost::OnSummAdd( udPWndSumm wndSumm ) {
	} // void OnSummAdd

	void CHWndSummTourCost::OnSummBack( udPWndSumm wndSumm ) {
	} // void OnSummBack

	void CHWndSummTourCost::OnSummConvert( udPWndSumm wndSumm ) {
	} // void OnSummConvert

	void CHWndSummTourCost::AfterCreate( udPWndSumm wndSumm ) {
		wndSumm->GetLnkAdd( )->Hide( );
		wndSumm->GetLnkBack( )->Hide( );
		wndSumm->GetLnkConvert( )->Hide( );

		m_lblWarning->CalculateWidthHeight( wndSumm->GetHWnd( ) );
		m_lblWarning->Create( wndSumm->GetHWnd( ) );
	} // void AfterCreate

	///////////////////////////////////////////////////////////////////////////////////////////////

	CHWndSummService::CHWndSummService( udPBase pTour ) {
		m_pTour = pTour;
	}

	CHWndSummService::~CHWndSummService( ) {
	}

	void CHWndSummService::AfterConstruct( udPWndSumm wndSumm ) {
		wndSumm->GetLnkAdd( )->get( "title" )->ini( wstring( L"оплатить" ) );
		wndSumm->GetLnkBack( )->get( "title" )->ini( wstring( L"возврат" ) );
		wndSumm->SetGrouping( true );
		wndSumm->SetGroupAttr( "partner" );
		wndSumm->SetGroupTitleAttr( "partner_" );
	} // void AfterConstruct

	void CHWndSummService::OnSummAdd( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummAddForm( );
		handler->GetFrmOpt( )->get( "use_partner" )->ini( ( udUInt ) 1 );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummAdd( );
				pObjWrite->apply( obj, true, true );
				pObjWrite->get( "partner" )->ini( obj->get( "partner" )->toUInt( ) );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummAdd );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtService );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// считываем код результата
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для снятия";
				} else if ( code == 1 ) {
					msg = L"Лицевой счет не найден";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				tpl->get( "partner" )->ini( ( udUInt ) 0 );
				tpl->get( "partner_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummAdd

	void CHWndSummService::OnSummBack( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummBackForm( );
		handler->GetFrmOpt( )->get( "use_partner" )->ini( ( udUInt ) 1 );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummBack( );
				pObjWrite->apply( obj, true, true );
				pObjWrite->get( "partner" )->ini( obj->get( "partner" )->toUInt( ) );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummBack );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtService );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для возврата";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				tpl->get( "partner" )->ini( ( udUInt ) 0 );
				tpl->get( "partner_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummBack

	void CHWndSummService::OnSummConvert( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummConvertForm( );
		handler->GetFrmOpt( )->get( "use_partner" )->ini( ( udUInt ) 1 );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummConvert( );
				pObjWrite->apply( obj, true, true );
				pObjWrite->get( "partner" )->ini( obj->get( "partner" )->toUInt( ) );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( string( "tour" ) );
				data.add( ( udInt ) udHTour::opidSummConvert );
				data.add( m_pTour->get( "id" )->toUInt( ) );
				data.add( ( udInt ) udProjectFinanceSumm::SummAtService );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// код результата операции
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				if ( code == 0 ) {
					msg = L"Не достаточно средств для конвертации";
				} else if ( code == 1 ) {
					msg = L"Отсутствует валютная пара для конвертации";
				} else if ( code == 2 ) {
					msg = L"Не установлен курс валютной пары";
				}
				// считываем данные
				udArrBase arr;
				udPBase tpl = GetProjectFinanceSumm( );
				tpl->get( "currency_" )->ini( wstring( L"" ) );
				tpl->get( "type_" )->ini( wstring( L"" ) );
				tpl->get( "partner" )->ini( ( udUInt ) 0 );
				tpl->get( "partner_" )->ini( wstring( L"" ) );
				ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
				udDELETE( tpl );
				// отправляем подтверждающий байт
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );
				//
				log = "\nполученные суммы\n";
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					( *i )->dump( log );
					log += "\n";
				}
				wndSumm->ClearRows( );
				wndSumm->AddRow( arr );
				// закрываем соединение
				sock->Shutdown( );
				sock->Close( );
				delete sock;
			} else {
				g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
			}
			delete obj;
		}
		delete handler;
		if ( !msg.empty( ) ) {
			mb::err( wndSumm->GetHWnd( ), msg.data( ) );
		}
	} // void OnSummConvert

	void CHWndSummService::AfterCreate( udPWndSumm wndSumm ) {
	} // void AfterCreate

	///////////////////////////////////////////////////////////////////////////////////////////////

	CDlgFinance::CDlgFinance( udPBase pTour ) {
		m_pTour			= pTour;
		//
		m_lblTourPrice	= new udWndStatic( ( udUInt ) 18 );
		m_lblSummTour	= new udWndStatic( ( udUInt ) 18 );
		m_lblTourCost	= new udWndStatic( ( udUInt ) 18 );
		m_lblService	= new udWndStatic( ( udUInt ) 18 );
		//
		m_hTourPrice	= new udHWndSummTourPrice( pTour );
		m_hSummTour		= new udHWndSummTour( pTour );
		m_hTourCost		= new udHWndSummTourCost( pTour );
		m_hService		= new udHWndSummService( pTour );
		//
		m_wndTourPrice	= new udWndSumm( m_hTourPrice );
		m_wndSummTour	= new udWndSumm( m_hSummTour );
		m_wndTourCost	= new udWndSumm( m_hTourCost );
		m_wndService	= new udWndSumm( m_hService );
		//
		m_lblTourPrice->get( "title" )->ini( wstring( L"Итоговая цена" ) );
		m_lblTourPrice->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lblSummTour->get( "title" )->ini( wstring( L"Оплата тура" ) );
		m_lblSummTour->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lblTourCost->get( "title" )->ini( wstring( L"Себестоимость" ) );
		m_lblTourCost->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_lblService->get( "title" )->ini( wstring( L"Оплата услуг" ) );
		m_lblService->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_wndTourPrice->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndSummTour->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndTourCost->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		m_wndService->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		//
		m_wndLnkPayOrder = new CWndLinkTourPayOrder( pTour );
		m_wndLnkPayOrder->get( "title" )->ini( wstring( L"окно ссылка на указание" ) );
		m_wndLnkPayOrder->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
	}

	CDlgFinance::~CDlgFinance( ) {
		// надписи
		udDELETE( m_lblTourPrice );
		udDELETE( m_lblSummTour );
		udDELETE( m_lblTourCost );
		udDELETE( m_lblService );
		// обработчики
		udDELETE( m_hTourPrice );
		udDELETE( m_hSummTour );
		udDELETE( m_hTourCost );
		udDELETE( m_hService );
		// окна сумм
		udDELETE( m_wndTourPrice );
		udDELETE( m_wndSummTour );
		udDELETE( m_wndTourCost );
		udDELETE( m_wndService );
		//
		udDELETE( m_wndLnkPayOrder );
	}

	void CDlgFinance::Render( ) {
		udPGdi	gdi = this->GetGdi( );

		gdi->Clear( Color( 255, 255, 255 ) );
	} // void Render

	bool CDlgFinance::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			return false;
		}
		udInt x = 10, y = 10, w = 235, h = 200, m = 10;
		///////////////////////////////////////////////////////////////////////////////////////////
		// ссылка на указание по оплате
		m_wndLnkPayOrder->get( "x" )->ini( ( udInt ) 255 );
		m_wndLnkPayOrder->get( "y" )->ini( ( udInt ) 10 );
		m_wndLnkPayOrder->get( "width" )->ini( ( udInt ) 300 );
		m_wndLnkPayOrder->get( "height" )->ini( ( udInt ) 20 );
		// итоговая цена
		x = 10;
		y = 30;
		m_lblTourPrice->get( "x" )->ini( x );
		m_lblTourPrice->get( "y" )->ini( y );
		m_lblTourPrice->CalculateWidthHeight( m_hWnd );
		y += m_lblTourPrice->get( "height" )->toInt( ) + 2;
		m_wndTourPrice->get( "x" )->ini( x );
		m_wndTourPrice->get( "y" )->ini( y );
		m_wndTourPrice->get( "width" )->ini( w );
		m_wndTourPrice->get( "height" )->ini( h );
		// фактически заплачено
		x = 255;
		y = 30;
		m_lblSummTour->get( "x" )->ini( x );
		m_lblSummTour->get( "y" )->ini( y );
		m_lblSummTour->CalculateWidthHeight( m_hWnd );
		y += m_lblSummTour->get( "height" )->toInt( ) + 2;
		m_wndSummTour->get( "x" )->ini( x );
		m_wndSummTour->get( "y" )->ini( y );
		m_wndSummTour->get( "width" )->ini( w );
		m_wndSummTour->get( "height" )->ini( h );
		y += h + m;
		// себестоимость
		x = 10;
		m_lblTourCost->get( "x" )->ini( x );
		m_lblTourCost->get( "y" )->ini( y );
		m_lblTourCost->CalculateWidthHeight( m_hWnd );
		y += m_lblTourCost->get( "height" )->toInt( ) + 2;
		m_wndTourCost->get( "x" )->ini( x );
		m_wndTourCost->get( "y" )->ini( y );
		m_wndTourCost->get( "width" )->ini( w );
		m_wndTourCost->get( "height" )->ini( h );
		// оплата услуг
		x = 255;
		y = 276;
		m_lblService->get( "x" )->ini( x );
		m_lblService->get( "y" )->ini( y );
		m_lblService->CalculateWidthHeight( m_hWnd );
		y += m_lblService->get( "height" )->toInt( ) + 2;
		m_wndService->get( "x" )->ini( x );
		m_wndService->get( "y" )->ini( y );
		m_wndService->get( "width" )->ini( w );
		m_wndService->get( "height" )->ini( h );
		///////////////////////////////////////////////////////////////////////////////////////////
		// создаем
		m_wndLnkPayOrder->Create( m_hWnd );
		m_lblTourPrice->Create( m_hWnd );
		m_lblSummTour->Create( m_hWnd );
		m_lblTourCost->Create( m_hWnd );
		m_lblService->Create( m_hWnd );
		m_wndTourPrice->Create( m_hWnd );
		m_wndSummTour->Create( m_hWnd );
		m_wndTourCost->Create( m_hWnd );
		m_wndService->Create( m_hWnd );
		///////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool Create

	void CDlgFinance::LoadData( ) {
		udPSocket sock = BeginRequest( );
		if ( sock ) {
			char code = 0;
			string log = "";
			udBinaryData data;
			data.add( string( "tour" ) );
			data.add( ( udInt ) udHTour::opidSummLoad );
			data.add( m_pTour->get( "id" )->toUInt( ) );
			// отсылаем данные
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			// ждем байт подтверждения принятия всех данных
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			// считываем данные
			udArrBase arr;
			udPBase tpl = GetProjectFinanceSumm( );
			tpl->get( "currency_" )->ini( wstring( L"" ) );
			tpl->get( "type_" )->ini( wstring( L"" ) );
			//////////////////////////////////////////////////////////////////////////////////////
			// итоговая цена
			ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			m_wndTourPrice->ClearRows( );
			m_wndTourPrice->AddRow( arr );
			arr.clear( );
			//////////////////////////////////////////////////////////////////////////////////////
			// оплата тура
			ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			m_wndSummTour->ClearRows( );
			m_wndSummTour->AddRow( arr );
			arr.clear( );
			//////////////////////////////////////////////////////////////////////////////////////
			// себестоимость
			tpl->get( "partner" )->ini( ( udUInt ) 0 ); // партнер по каждой сумме
			tpl->get( "partner_" )->ini( wstring( L"" ) );
			ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			m_wndTourCost->ClearRows( );
			m_wndTourCost->AddRow( arr );
			arr.clear( );
			//////////////////////////////////////////////////////////////////////////////////////
			// оплата услуг
			ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			m_wndService->ClearRows( );
			m_wndService->AddRow( arr );
			arr.clear( );
			//////////////////////////////////////////////////////////////////////////////////////
			udDELETE( tpl );
			// закрываем соединение
			sock->Shutdown( );
			sock->Close( );
			delete sock;
		} else {
			g_pApp->GetLog( )->Write( "не удалось соединиться\n" );
		}
	} // void LoadData

	///////////////////////////////////////////////////////////////////////////////////////////////

	static BOOL CALLBACK dlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_INITDIALOG: {
				udPDlgFinance dlg = ( udPDlgFinance ) lParam;
				dlg->Create( hWnd );
				dlg->LoadData( );
				SetWindowPos( hWnd, NULL, 0, 0,
					dlg->get( "width" )->toInt( ),
					dlg->get( "height" )->toInt( ),
					SWP_NOMOVE
				);
				CenterOneWindowAboveParent( hWnd );
				return TRUE;
			} break;

			case WM_COMMAND: {
				switch( LOWORD( wParam ) ) {
					case IDOK: {
						EndDialog( hWnd, 0 );
						return TRUE;
					} break;
					case IDCANCEL:
						EndDialog( hWnd, 0 );
						return TRUE;
						break;
				}
			} break;

			default:
				break;
		}

		return FALSE;
	} // BOOL dlgProc

	void CDlgFinance::start( HWND hWndParent, udPBase pTour ) {
		udPDlgFinance dlg = new udDlgFinance( pTour );
		dlg->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
		dlg->get( "x" )->ini( ( udInt ) 0 );
		dlg->get( "y" )->ini( ( udInt ) 0 );
		dlg->get( "width" )->ini( ( udInt ) 600 );
		dlg->get( "height" )->ini( ( udInt ) 600 );
		MakeDialog( hWndParent, ( DLGPROC ) dlgProc, ( LPARAM ) dlg );
		udDELETE( dlg );
	} // void start

	///////////////////////////////////////////////////////////////////////////////////////////////

	CWndLinkTourPayOrder::CWndLinkTourPayOrder( udPBase pTour ) {
		m_pTour = pTour;
		m_lnkPayOrder = new udWndLink( ( udUInt ) 12 );

		m_lnkPayOrder->get( "title" )->ini( wstring( L"Указание к оплате" ) );
		m_lnkPayOrder->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
	}

	CWndLinkTourPayOrder::~CWndLinkTourPayOrder( ) {
		udDELETE( m_lnkPayOrder );
	}

	void CWndLinkTourPayOrder::Render( ) {
		udPGdi gdi = this->GetGdi( );

		gdi->Clear( Color( 255, 255, 255 ) );
	} // void Render

	bool CWndLinkTourPayOrder::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			return false;
		}
		m_lnkPayOrder->get( "x" )->ini( ( udInt ) 0 );
		m_lnkPayOrder->get( "y" )->ini( ( udInt ) 0 );
		m_lnkPayOrder->CalculateWidthHeight( m_hWnd );
		m_lnkPayOrder->Create( m_hWnd );
		return true;
	} // bool Create

	LRESULT CWndLinkTourPayOrder::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndLinkTourPayOrder::msgLinkClick( LPARAM lParam ) {
		udPWndLink lnk = ( udPWndLink ) lParam;
		if ( lnk == m_lnkPayOrder ) {
			udPFormHandler handler = GetHTourPayOrderForm( );
			if ( handler ) {
				handler->GetFrmOpt( )->get( "from_tour" )->ini( ( udDWord ) m_pTour );
				udPMorph obj = DialogForm( m_hWndParent, handler );
				if ( obj ) { // отсылаем указание на сервак
					/*udPWndSumm wndSumm = ( ( udPHTourPayOrder ) handler )->GetWndSumm( );
					const udDwAscBase& rows = wndSumm->GetRows( );
					string log = "\nrows:\n";
					for( udDwAscBase::const_iterator i = rows.begin( ); i != rows.end( ); ++i ) {
						i->second->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log );*/
				}
				udDELETE( obj );
			}
			udDELETE( handler );
		}
		return TRUE;
	} // LRESULT msgLinkClick

	///////////////////////////////////////////////////////////////////////////////////////////////

} // namespace UdSDK
