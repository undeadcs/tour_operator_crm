
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHRoute::CHRoute( ) : udStateHandlerSimpleOneName( ) {
		m_szTableName	= "ud_route";
		m_szUpdIndex	= "id";
		m_szName		= "route";
		m_fnGetObject	= GetProjectRoute;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHRouteForm;
		m_szAddModeless	= L"Новый маршрут";
		m_szAddModal	= L"Добавить маршрут";
		m_szFormTitle	= L"Маршрут";
	}

	CHRoute::CHRoute( const CHRoute& objCopy ) {
	}

	CHRoute::~CHRoute( ) {
	}

	CHRoute& CHRoute::operator =( const CHRoute& objCopy ) {
		return *this;
	}




	CHOrderRoute::CHOrderRoute( ) : udFormHandler( ) {
	}

	CHOrderRoute::~CHOrderRoute( ) {
	}

	udInt CHOrderRoute::GetFormModel( ) {
		m_wndForm->AddInput( ctrlidName, "name", NULL, g_pApp->GetFont( ), 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHOrderRoute::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 120 );
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
		title	= L"Пункт маршрута";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Наименование";
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
		info = m_wndForm->GetInfo( ctrlidName );
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

	udInt CHOrderRoute::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::err( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHOrderRoute::FormOk( ) {
		udInt err = ProcForm( );
		if ( err == FormHandlerErrorOk ) {
			//this->SwitchToList( );
		}
		return err;
	} // udInt FormOk

	udInt CHOrderRoute::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHOrderRoute::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

} // namespace UdSDK
