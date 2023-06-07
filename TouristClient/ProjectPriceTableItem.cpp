
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHServicePrice::CHServicePrice( ) : udFormHandler( ) {
	}

	CHServicePrice::~CHServicePrice( ) {
	}

	udInt CHServicePrice::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddExtlist( ctrlidPeriod, "period", NULL, "#name", L"выберите период", GetHServicePeriodList, m_pFrmOpt );
		m_wndForm->AddInput( ctrlidNum, "num", NULL, font, ES_NUMBER );
		m_wndForm->AddInput( ctrlidAmount, "amount", NULL, font, 0 );
		m_wndForm->AddExtlist( ctrlidCurrency, "currency", NULL, "name", L"выберите валюту", GetHCurrencyList );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHServicePrice::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		wstring		title		= L"";
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 108;
		udPGdi		gdi			= m_wndForm->GetGdi( );
		assert( gdi );
		udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
		udPFont		fntLabel	= g_pApp->GetFont( ),	// шрифт надписи
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 );	// шрифт заголовка
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Цена услуги";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Период";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
		info = m_wndForm->GetInfo( ctrlidPeriod );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Кол-во человек";
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
		info = m_wndForm->GetInfo( ctrlidNum );
		info->box = box;
		y += h + m;
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
		info = m_wndForm->GetInfo( ctrlidAmount );
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
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHServicePrice::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		
		ModeFormControlInfo *info = m_wndForm->GetInfo( ctrlidPeriod );
		udPBase period = info->extlst->extlst->GetObj( );
		if ( !period ) {
			mb::err( hWnd, L"Введите период" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "num" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите количество людей" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "amount" )->toFloat( ) == 0.0f ) {
			mb::err( hWnd, L"Введите сумму" );
			return FormHandlerErrorFailed;
		}
		if ( obj->get( "currency" )->toUInt( ) == 0 ) {
			mb::err( hWnd, L"Введите валюту" );
			return FormHandlerErrorFailed;
		}

		obj->get( "period" )->ini( period->get( "index" )->toUInt( ) );

		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHServicePrice::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHServicePrice::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHServicePrice::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

	void CHServicePrice::LoadAfterSetObject( ) {
		udPBase obj = m_wndForm->GetObj( );
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "period" )->toUInt( );
		if ( id > 0 ) {
			udPWndModeForm wndForm = ( udPWndModeForm ) m_pFrmOpt->get( "wndForm" )->toDWord( );
			ModeFormControlInfo *info = wndForm->GetInfo( udHService::ctrlidPeriod );
			udPDwAscBase periods = info->frmlst->GetItems( );
			if ( periods && !periods->empty( ) ) {
				for( udDwAscBase::iterator i = periods->begin( ); i != periods->end( ); ++i ) {
					if ( i->second->get( "index" )->toDWord( ) == id ) {
						udPBase period = new udProjectServicePeriod;
						i->second->apply( period );
						m_wndForm->GetInfo( ctrlidPeriod )->extlst->extlst->SetObj( period );
						break;
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = obj->get( "currency" )->toUInt( );
		if ( id > 0 ) {
			udPBase ret = LoadById( "currency", id, GetProjectCurrency );
			if ( ret ) {
				m_wndForm->GetInfo( ctrlidCurrency )->extlst->extlst->SetObj( ret );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadAfterSetObject

} // namespace UdSDK
