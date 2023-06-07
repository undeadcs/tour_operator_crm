
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void PersAccAfterPickClient( udPBase obj, udPListHandler handler ) {
		udPMorph opt = handler->GetLstOpt( );
		udPWndModeList wndList = ( udPWndModeList ) handler->GetLstOpt( )->get( "wndList" )->toDWord( );
		g_pApp->GetLog( )->Write( handler->GetLstOpt( )->get( "wndList" )->toString( ) + ", 2\n" );
		udInt state = opt->get( "state" )->toInt( );
		if ( state == udHClient::stateFiz ) {
			wndList->GetInfo( CHPersAcc::fltClient )->extlst->extlst->SetAttr( "fio" );
			wndList->GetObjFilter( )->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		} else {
			wndList->GetInfo( CHPersAcc::fltClient )->extlst->extlst->SetAttr( "name" );
			wndList->GetObjFilter( )->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENTUR );
		}
	} // void PersAccAfterPickClient

	CHPersAcc::CHPersAcc( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_persacc";
		m_szUpdIndex	= "id";
		m_szName		= "persacc";
		m_fnGetObject	= GetProjectPersAcc;
		m_szOrderBy		= "id";

		m_wndSumm		= NULL;
		m_wndOp			= NULL;
	}

	CHPersAcc::CHPersAcc( const CHPersAcc& objCopy ) {
		udDELETE( m_wndSumm );
		udDELETE( m_wndOp );
	}

	CHPersAcc::~CHPersAcc( ) {
	}

	CHPersAcc& CHPersAcc::operator =( const CHPersAcc& objCopy ) {
		return *this;
	}

	void CHPersAcc::Clear( ) {
		udStateHandlerSimple::Clear( );
		if ( m_wndSumm ) {
			m_wndSumm->ClearRows( );
		}
		if ( m_wndOp ) {
			m_wndOp->ClearRows( );
		}
	} // void Clear

	udInt CHPersAcc::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= 32001;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		m_pLstOpt->get( "wndList" )->ini( ( udDWord ) m_wndModeList );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !modal ) {
			title = L"На главную";
			url = "main";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "id" )->ini( id++ );
		wndInsLst->AddAttr( "name", L"Наименование" )->get( "width" )->ini( ( udInt ) 400 );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "id" )->ini( id++ );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPFont font = g_pApp->GetFont( );
		m_wndModeList->SetShowFilter( true );
		m_wndModeList->AddExtlist( fltCompany, "company", NULL, "name", L"выберите компанию", GetHCompanyList );
		m_wndModeList->AddExtlist( fltPartner, "partner", NULL, "name", L"выберите партнера", GetHPartnerList );
		m_wndModeList->AddExtlist( fltClient, "client", NULL, "fio", L"выберите клиента", GetHClientList, m_pLstOpt, PersAccAfterPickClient );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "company" )->ini( ( udUInt ) 0 );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHPersAcc::GetListView( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		style	= WS_CHILD | WS_TABSTOP | WS_VISIBLE;
		udInt		x		= 32,
					y		= 10,
					m		= 5,
					w		= 0,
					h		= 0;
		const udLAscWndLink* arrLink = m_wndModeList->GetLinks( );
		udLAscWndLink::const_iterator itLnk;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !modal ) {
			itLnk = arrLink->find( 0 );
			lnk = itLnk->second;
			lnk->get( "style" )->ini( style );
			lnk->get( "x" )->ini( x );
			lnk->get( "y" )->ini( y );
			lnk->CalculateWidthHeight( hWnd );
			x += lnk->get( "width" )->toInt( ) + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		y += lnk->get( "height" )->toInt( ) + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltCompany );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltPartner );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 180; box->h = 28;
		box->padL = 0; box->padT = 0; box->padR = 0; box->padB = 0;
		box->border = 0; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltClient );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		x += box->w + box->padL + box->padR + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 30 + m;
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		m_wndModeList->SetButtonFilterPosition( x, y - m );
		x += m_wndModeList->GetButtonFilter( )->get( "width" )->toInt( ) + m;
		m_wndModeList->SetButtonResetPosition( x, y - m );
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 30 + m;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
		} else {
			x = 32;
		}
		//y += lnk->get( "height" )->toInt( ) + m;
		w = 400;
		h = 400;
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		wndInsLst->get( "style" )->ini( style );
		wndInsLst->get( "x" )->ini( x );
		wndInsLst->get( "y" )->ini( y );
		wndInsLst->get( "width" )->ini( w );
		wndInsLst->get( "height" )->ini( h );
		//////////////////////////////////////////////////////////////////////////////////////////
		x = 32;
		w = 300;
		y += wndInsLst->get( "height" )->toInt( ) + m;
		udPWndPager wndPager = m_wndModeList->GetPager( );
		wndPager->get( "style" )->ini( style );
		wndPager->get( "x" )->ini( x );
		wndPager->get( "y" )->ini( y );
		wndPager->get( "width" )->ini( w );
		wndPager->get( "height" )->ini( 20 );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListView

	udInt CHPersAcc::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		return FormHandlerErrorOk;
	} // udInt GetFormModel

	udInt CHPersAcc::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 400 );
			m_wndForm->get( "height" )->ini( ( udInt ) 120 );
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
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_wndSumm ) { // если уже создавали
		} else { // создаем с нуля
			m_wndSumm = new udWndSumm( this );
			m_wndSumm->get( "x" )->ini( ( udInt ) 50 );
			m_wndSumm->get( "y" )->ini( ( udInt ) 50 );
			m_wndSumm->get( "width" )->ini( ( udInt ) 600 );
			m_wndSumm->get( "height" )->ini( ( udInt ) 600 );
			m_wndSumm->get( "style" )->ini( ( udDWord ) WS_CHILD | WS_VISIBLE );
			m_wndSumm->Create( m_wndForm->GetHWnd( ) );
		}
		//y += m_wndSumm->get( "y" )->toInt( ) + m_wndSumm->get( "height" )->toInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( m_wndOp ) {
		} else {
			m_wndOp = new udWndOperations;
			m_wndOp->get( "x" )->ini( ( udInt ) 610 );
			m_wndOp->get( "y" )->ini( ( udInt ) 50 );
			m_wndOp->get( "width" )->ini( ( udInt ) 600 );
			m_wndOp->get( "height" )->ini( ( udInt ) 600 );
			m_wndOp->get( "style" )->ini( ( udDWord ) WS_CHILD | WS_VISIBLE );
			m_wndOp->Create( m_wndForm->GetHWnd( ) );
		}
		//y += m_wndOp->get( "y" )->toInt( ) + m_wndSumm->get( "height" )->toInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		y += 650;
		//////////////////////////////////////////////////////////////////////////////////////////
		w = 107;
		x = 32 + lcol + m;
		m_wndForm->SetButtonOkPosition( inc( &x, w + m ), y );
		m_wndForm->SetButtonCancelPosition( x, y );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt GetFormView

	udInt CHPersAcc::ValidateForm( ) {
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	void CHPersAcc::OnSelectRow( udLong row ) {
		udPWndInstanceList wndInsLst = m_wndModeList->GetList( );
		if ( wndInsLst ) {
			wndInsLst->SelectRow( row );
			if ( !m_wndModeList->IsModal( ) ) {
				udPWndToolbar tbr = g_pApp->GetModel( )->GetWndWork( )->GetToolbar( );
				tbr->EnableItem( "edit"		);
				//tbr->EnableItem( "delete"	);
			}
		}
	} // void OnSelectRow

	void CHPersAcc::AddExtraAttributes( udPBase tpl ) {
		tpl->get( "name" )->ini( wstring( L"" ) );
	} // void AddExtraAttributes

	void CHPersAcc::ExtraDataSending( udPSocket sock, udPBase obj ) {
	} // void ExtraDataSending

	void CHPersAcc::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
		// считываем суммы
		this->LoadAccSumm( sock );
		// считываем операции
		this->LoadAccOp( sock );
	} // void ExtraDataRecieve

	void CHPersAcc::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "company" )->ini( ( udUInt ) 0 );
		filter->get( "partner" )->ini( ( udUInt ) 0 );
		filter->get( "client" )->ini( ( udUInt ) 0 );
		filter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
	} // void ClearFilter

	void CHPersAcc::OnSummAdd( udPWndSumm wndSumm ) {
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
				data.add( m_szName );
				data.add( ( udInt ) opidSummAdd );
				data.add( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
				pObjWrite->dump( data );
				udDELETE( pObjWrite );
				// отсылаем данные
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// ждем байт подтверждения принятия всех данных
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				sock->ReadByte( &code );
				// считываем данные
				this->LoadAccSumm( sock );
				this->LoadAccOp( sock );
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
	} // void OnSummAdd

	void CHPersAcc::OnSummBack( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummBackForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummBack( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( m_szName );
				data.add( ( udInt ) opidSummBack );
				data.add( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
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
					msg = L"Не достаточно средств для снятия";
				}
				// считываем данные
				this->LoadAccSumm( sock );
				this->LoadAccOp( sock );
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

	void CHPersAcc::OnSummConvert( udPWndSumm wndSumm ) {
		wstring msg = L"";
		udPFormHandler handler = GetHSummConvertForm( );
		udPMorph obj = DialogForm( wndSumm->GetHWnd( ), handler );
		if ( obj ) { // получили объект суммы
			/**
				отправляем сумму
				получаем новый список сумм
				обновляем список сумм
			*/
			udPSocket sock = BeginRequest( );
			if ( sock ) {
				udPBase pObjWrite = GetProjectFinanceSummConvert( );
				pObjWrite->apply( obj, true, true );

				string log = "\n";
				pObjWrite->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				udBinaryData data;
				data.add( m_szName );
				data.add( ( udInt ) opidSummConvert );
				data.add( m_wndForm->GetObj( )->get( "id" )->toUInt( ) );
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
				this->LoadAccSumm( sock );
				this->LoadAccOp( sock );
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

	void CHPersAcc::LoadAccSumm( udPSocket sock ) {
		udArrBase arr;
		udPBase tpl = GetProjectFinanceSumm( );
		tpl->get( "currency_" )->ini( wstring( L"" ) );
		tpl->get( "type_" )->ini( wstring( L"" ) );
		// считываем суммы
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		ReadInstances( sock, arr, GetProjectFinanceSumm, tpl );
		// отправляем подтверждающий байт
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
		// обновляем окно сумм
		assert( m_wndSumm );
		m_wndSumm->ClearRows( );
		m_wndSumm->AddRow( arr );
		ClearArrBase( arr );
	} // void LoadAccSumm

	void CHPersAcc::LoadAccOp( udPSocket sock ) {
		udArrBase arr;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		ReadInstances( sock, arr, GetProjectFinanceOperation );
		// отправляем подтверждающий байт
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
		// распечатаем все операции
		assert( m_wndOp );
		m_wndOp->ClearRows( );
		m_wndOp->SetRows( arr );
		/*string log = "\nCHPersAcc::ExtraDataRecieve, fop:\n";
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );*/
		ClearArrBase( arr );
	} // void LoadAccOp

} // namespace UdSDK
