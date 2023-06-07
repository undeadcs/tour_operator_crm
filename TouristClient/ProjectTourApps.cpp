
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHGroupContractApp1::CHGroupContractApp1( ) {
	}

	CHGroupContractApp1::~CHGroupContractApp1( ) {
	}

	udInt CHGroupContractApp1::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidPay1, "pay1", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidPay2, "pay2", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidPay1Date, "pay1date", NULL, font );
		m_wndForm->AddDate( ctrlidPay2Date, "pay2date", NULL, font );
		m_wndForm->AddInput( ctrlidDays1, "days1", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidDays2, "days2", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidDays3, "days3", NULL, font, 0 );
		/*m_wndForm->AddInput( ctrlidCountry, "country", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		m_wndForm->AddInput( ctrlidPrice1, "price1", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidPrice2, "price2", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidAdvance, "advance", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidAdvDate, "advdate", NULL, font );
		m_wndForm->AddInput( ctrlidSurCharge, "surcharge", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidSurDate, "surdate", NULL, font, 0 );*/
		m_wndForm->AddInput( ctrlidProduct, "product", NULL, font, 0 );
		//m_wndForm->AddFormlist( ctrlidTouristInfo, "info", NULL )->AddAttr( "fio" );
		m_wndForm->AddFormlist( ctrlidOccupancy, "occupancy", NULL )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidProgram, "program", NULL )->AddAttr( "text" );
		m_wndForm->AddFormlist( ctrlidTransfer, "transfer", NULL )->AddAttr( "fio" );
		m_wndForm->AddFormlist( ctrlidCover, "cover", NULL )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidVisa, "visa", NULL )->AddAttr( "type" );
		m_wndForm->AddFormlist( ctrlidService, "service", NULL )->AddAttr( "name" );
		m_wndForm->AddFormlist( ctrlidTransfer2, "transfer2", NULL )->AddAttr( "route" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGroupContractApp1::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 710 );
			m_wndForm->RemakeBuffer( prev_w, prev_h, m_wndForm->get( "width" )->toInt( ), m_wndForm->get( "height" )->toInt( ) );
			m_wndForm->Move( );
		}
		wstring		title		= L"";
		udInt		x			= 0,
					y			= 0,
					w			= 0,
					h			= 0,
					m			= 5,
					lcol		= 150;
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
		title	= L"Приложение 1";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Оплата 1";
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
		info = m_wndForm->GetInfo( ctrlidPay1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата оплаты 1";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidPay1Date );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Оплата 2";
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
		info = m_wndForm->GetInfo( ctrlidPay2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата оплаты 2";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidPay2Date );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"За сколько дней до поездки предоставить документы (не позднее)";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		x += lcol + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDays1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"В течение скольки дней после бронирования подтвердить тур";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		x += lcol + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDays2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"В течение скольки дней после бронирования оплатить тур (не позднее)";
		x		= 32;
		w		= 400;
		h		= 17;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		x += lcol + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDays3 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		/*title	= L"Страна";
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
		info = m_wndForm->GetInfo( ctrlidCountry );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Cрок поездки с";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Cрок поездки по";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Цена в у.е.";
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
		info = m_wndForm->GetInfo( ctrlidPrice1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Цена в рублях";
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
		info = m_wndForm->GetInfo( ctrlidPrice2 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Аванс";
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
		info = m_wndForm->GetInfo( ctrlidAdvance );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата аванса";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidAdvDate );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Доплата";
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
		info = m_wndForm->GetInfo( ctrlidSurCharge );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата доплаты";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidSurDate );
		info->box = box;
		y += h + m;*/
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Туристский продукт";
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
		info = m_wndForm->GetInfo( ctrlidProduct );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		/*title	= L"Сведения о туристе";
		x		= 32;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidTouristInfo );
		info->box = box;
		y += h + m;*/
		y += 10;
		g_pApp->GetLog( )->Write( "y=" + toString( y ) + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Средство размещения";
		x		= 32;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidOccupancy );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Экскурсионная программа";
		x		= 32;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidProgram );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Услуги по перевозке туристов";
		x		= 32;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidTransfer );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		int xcol = 360, y2 = 292;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Страхование";
		x		= xcol;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y2, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y2 += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y2; box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidCover );
		info->box = box;
		y2 += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Визовое оформление";
		x		= xcol;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y2, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y2 += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y2; box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidVisa );
		info->box = box;
		y2 += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Другие услуги";
		x		= xcol;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y2, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y2 += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y2; box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidService );
		info->box = box;
		y2 += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Трансфер";
		x		= xcol;
		w		= 400;
		h		= 66;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y2, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		y2 += 20;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y2; box->w = 300; box->h = 64;
		info = m_wndForm->GetInfo( ctrlidTransfer2 );
		info->box = box;
		y2 += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 20 + 66 + m;
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	void CHGroupContractApp1::LoadAfterSetObject( ) {
		if ( m_pFrmOpt->isset( "obj" ) ) {
			udPBase			obj		= ( udPBase ) m_pFrmOpt->get( "obj" )->get( ),
							obj1	= NULL;
			udPArrBase		pArr	= NULL;
			udPWndFormList	frmlst	= NULL;
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidOccupancy )->frmlst;
			pArr = ( udPArrBase ) obj->get( "occupancy" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCOccupancy( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidProgram )->frmlst;
			pArr = ( udPArrBase ) obj->get( "program" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCProgram( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidTransfer )->frmlst;
			pArr = ( udPArrBase ) obj->get( "transfer" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCTransfer( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidCover )->frmlst;
			pArr = ( udPArrBase ) obj->get( "cover" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCCover( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidVisa )->frmlst;
			pArr = ( udPArrBase ) obj->get( "visa" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCVisa( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidService )->frmlst;
			pArr = ( udPArrBase ) obj->get( "service" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCService( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
			frmlst = m_wndForm->GetInfo( ctrlidTransfer2 )->frmlst;
			pArr = ( udPArrBase ) obj->get( "transfer2" )->get( );
			for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
				obj1 = GetProjectGCTransfer2( );
				( *i )->apply( obj1 );
				frmlst->AddItem( obj1 );
			}
			//////////////////////////////////////////////////////////////////////
		}
	} // void LoadAfterSetObject

	udInt CHGroupContractApp1::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGroupContractApp1::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGroupContractApp1::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGroupContractApp1::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

	void CHGroupContractApp1::FormListOnAdd( udPWndFormList wnd ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		/*if ( wnd == m_wndForm->GetInfo( ctrlidTouristInfo )->frmlst ) {
			udPFormHandler handler = GetHGCTouristInfoForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCTouristInfo( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else*/ if ( wnd == m_wndForm->GetInfo( ctrlidOccupancy )->frmlst ) {
			udPFormHandler handler = GetHGCOccupancyForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCOccupancy( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidProgram )->frmlst ) {
			udPFormHandler handler = GetHGCProgramForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCProgram( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTransfer )->frmlst ) {
			udPFormHandler handler = GetHGCTransferForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCTransfer( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidCover )->frmlst ) {
			udPFormHandler handler = GetHGCCoverForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCCover( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidVisa )->frmlst ) {
			udPFormHandler handler = GetHGCVisaForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCVisa( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPFormHandler handler = GetHGCServiceForm( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCService( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTransfer2 )->frmlst ) {
			udPFormHandler handler = GetHGCTransfer2Form( );
			assert( handler );
			udPMorph obj = DialogForm( hWnd, handler );
			if ( obj ) {
				udPBase add = GetProjectGCTransfer2( );
				obj->apply( add );
				wnd->AddItem( add );
			}
			udDELETE( obj );
			udDELETE( handler );
		}
	} // void FormListOnAdd

	void CHGroupContractApp1::FormListOnEdit( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		/*if ( wnd == m_wndForm->GetInfo( ctrlidTouristInfo )->frmlst ) {
			udPBase tmp = GetProjectGCTouristInfo( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCTouristInfoForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else*/ if ( wnd == m_wndForm->GetInfo( ctrlidOccupancy )->frmlst ) {
			udPBase tmp = GetProjectGCOccupancy( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCOccupancyForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidProgram )->frmlst ) {
			udPBase tmp = GetProjectGCProgram( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCProgramForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTransfer )->frmlst ) {
			udPBase tmp = GetProjectGCTransfer( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCTransferForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidCover )->frmlst ) {
			udPBase tmp = GetProjectGCCover( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCCoverForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidVisa )->frmlst ) {
			udPBase tmp = GetProjectGCVisa( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCVisaForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			udPBase tmp = GetProjectGCService( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCServiceForm( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTransfer2 )->frmlst ) {
			udPBase tmp = GetProjectGCTransfer2( );
			obj->apply( tmp );
			udPFormHandler handler = GetHGCTransfer2Form( );
			udPMorph obj1 = DialogForm( hWnd, handler, tmp );
			if ( obj1 ) {
				obj1->apply( obj );
			}
			udDELETE( obj1 );
			udDELETE( handler );
		}
	} // void FormListOnEdit

	bool CHGroupContractApp1::FormListOnDelete( udPWndFormList wnd, udPBase obj ) {
		HWND hWnd = m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		if ( MessageBox( hWnd,
			L"Вы действительно хотите удалить элемент из списка?", L"Подтвердите удаление",
			MB_YESNO ) == IDNO ) {
			return false;
		}
		if ( wnd == m_wndForm->GetInfo( ctrlidOccupancy )->frmlst ) {
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				udPBase del = GetProjectGCOccupancy( );
				del->apply( obj, true, true );
				wnd->AddToDel( del );
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidProgram )->frmlst ) {
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				udPBase del = GetProjectGCProgram( );
				del->apply( obj, true, true );
				wnd->AddToDel( del );
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidTransfer )->frmlst ) {
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				udPBase del = GetProjectGCTransfer( );
				del->apply( obj, true, true );
				wnd->AddToDel( del );
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidCover )->frmlst ) {
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				udPBase del = GetProjectGCCover( );
				del->apply( obj, true, true );
				wnd->AddToDel( del );
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidVisa )->frmlst ) {
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				udPBase del = GetProjectGCVisa( );
				del->apply( obj, true, true );
				wnd->AddToDel( del );
			}
			return true;
		} else if ( wnd == m_wndForm->GetInfo( ctrlidService )->frmlst ) {
			if ( obj->get( "id" )->toUInt( ) > 0 ) {
				udPBase del = GetProjectGCService( );
				del->apply( obj, true, true );
				wnd->AddToDel( del );
			}
			return true;
		}
		return false;
	} // void FormListOnDelete



	CHGCTouristInfo::CHGCTouristInfo( ) {
	}

	CHGCTouristInfo::~CHGCTouristInfo( ) {
	}

	udInt CHGCTouristInfo::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidFio, "fio", NULL, font, 0 );
		udArrWString arrSelect;
		arrSelect.push_back( L"Мужской" );
		arrSelect.push_back( L"Женский" );
		m_wndForm->AddSelect( ctrlidSex, "sex", NULL, font, arrSelect );
		m_wndForm->AddDate( ctrlidBorn, "born", NULL, font, TRUE );
		m_wndForm->AddText( ctrlidPassport, "passport", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCTouristInfo::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Сведение о туристе";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"ФИО";
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
		info = m_wndForm->GetInfo( ctrlidFio );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата рождения";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidBorn );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Пол";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		udArrWString arrSelect;
		arrSelect.push_back( L"Мужской" );
		arrSelect.push_back( L"Женский" );
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidSex );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Паспортные данные";
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
		info = m_wndForm->GetInfo( ctrlidPassport );
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

	udInt CHGCTouristInfo::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCTouristInfo::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCTouristInfo::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCTouristInfo::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm




	CHGCOccupancy::CHGCOccupancy( ) {
	}

	CHGCOccupancy::~CHGCOccupancy( ) {
	}

	udInt CHGCOccupancy::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidCategory, "category", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidAmount, "amount", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidFeed, "feed", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCOccupancy::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Средство размещения";
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
		title	= L"Категория";
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
		info = m_wndForm->GetInfo( ctrlidCategory );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Количество проживающих";
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
		title	= L"Питание";
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
		info = m_wndForm->GetInfo( ctrlidFeed );
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

	udInt CHGCOccupancy::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCOccupancy::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCOccupancy::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCOccupancy::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm




	CHGCProgram::CHGCProgram( ) {
	}

	CHGCProgram::~CHGCProgram( ) {
	}

	udInt CHGCProgram::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidText, "text", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCProgram::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Экскурсионная программа";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Текст";
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
		info = m_wndForm->GetInfo( ctrlidText );
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

	udInt CHGCProgram::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCProgram::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCProgram::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCProgram::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



	CHGCTransfer::CHGCTransfer( ) {
	}

	CHGCTransfer::~CHGCTransfer( ) {
	}

	udInt CHGCTransfer::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidFio, "fio", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidTrans, "trans", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidLevel, "level", NULL, font, 0 );
		m_wndForm->AddDate( ctrlidDate1, "date1", NULL, font );
		m_wndForm->AddDate( ctrlidDate2, "date2", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCTransfer::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Услуги по перевозке туристов";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"ФИО туриста";
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
		info = m_wndForm->GetInfo( ctrlidFio );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Перевозчик";
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
		info = m_wndForm->GetInfo( ctrlidTrans );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Уровень сервиса";
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
		info = m_wndForm->GetInfo( ctrlidLevel );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата убытия";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate1 );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Дата прибытия";
		x		= 32;
		w		= lcol;
		h		= 22;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 22;
		box->padL = 2; box->padT = 2; box->padR = 2; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndForm->GetInfo( ctrlidDate2 );
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

	udInt CHGCTransfer::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCTransfer::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCTransfer::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCTransfer::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



	CHGCCover::CHGCCover( ) {
	}

	CHGCCover::~CHGCCover( ) {
	}

	udInt CHGCCover::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidType, "type", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCCover::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Страхование";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип страховки";
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
		info = m_wndForm->GetInfo( ctrlidType );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Страховщик";
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

	udInt CHGCCover::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCCover::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCCover::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCCover::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



	CHGCVisa::CHGCVisa( ) {
	}

	CHGCVisa::~CHGCVisa( ) {
	}

	udInt CHGCVisa::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidType, "type", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCVisa::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Визовое оформление";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип услуги";
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
		info = m_wndForm->GetInfo( ctrlidType );
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

	udInt CHGCVisa::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCVisa::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCVisa::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCVisa::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm



	CHGCService::CHGCService( ) {
	}

	CHGCService::~CHGCService( ) {
	}

	udInt CHGCService::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidChar, "char", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCService::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Другие услуги";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Наименование услуги";
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
		title	= L"Характеристики";
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
		info = m_wndForm->GetInfo( ctrlidChar );
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

	udInt CHGCService::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCService::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCService::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCService::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm


	CHGCTransfer2::CHGCTransfer2( ) {
	}

	CHGCTransfer2::~CHGCTransfer2( ) {
	}

	udInt CHGCTransfer2::GetFormModel( ) {
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidRoute, "route", NULL, font, 0 );
		m_wndForm->AddInput( ctrlidType, "type", NULL, font, 0 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHGCTransfer2::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 800 );
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
		title	= L"Трансфер";
		x		= 32;
		y		= 10;
		w		= 500;
		h		= 21;
		gdi->DrawString( title.data( ), title.size( ), fntHeader,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhHeader
		);
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Маршрут";
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
		info = m_wndForm->GetInfo( ctrlidRoute );
		info->box = box;
		y += h + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Тип трансфера";
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
		info = m_wndForm->GetInfo( ctrlidType );
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

	udInt CHGCTransfer2::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		m_wndForm->LoadControlData( );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHGCTransfer2::FormOk( ) {
		return ProcForm( );
	} // udInt FormOk

	udInt CHGCTransfer2::FormCancel( ) {
		return FormHandlerErrorOk;
	} // udInt FormCancel

	udInt CHGCTransfer2::ProcForm( bool modal ) {
		assert( m_wndForm );
		m_wndForm->LoadControlData( );
		return ValidateForm( );
	} // udInt ProcForm

} // namespace UdSDK
