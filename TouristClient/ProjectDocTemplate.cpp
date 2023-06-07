
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHDocTemplate::CHDocTemplate( ) : udStateHandlerInfo( ) {
		m_szTableName	= "ud_doctpl";
		m_szUpdIndex	= "id";
		m_szName		= "doctpl";
		m_fnGetObject	= GetProjectDocTemplate;
		m_szOrderBy		= "id";
		m_fnGetFormHandler	= GetHDocTemplateForm;
	}

	CHDocTemplate::~CHDocTemplate( ) {
	}

	udInt CHDocTemplate::GetFormModel( ) {
		m_wndForm->SetGetObjectCallback( m_fnGetObject );
		udPFont font = g_pApp->GetFont( );
		m_wndForm->AddInput( ctrlidName, "name", NULL, font, 0 );
		udArrWString arrString;
		arrString.push_back( L"Договор" );
		arrString.push_back( L"Путевка" );
		arrString.push_back( L"Гр.договор" );
		m_wndForm->AddSelect( ctrlidType, "type", NULL, font, arrString );
		m_wndForm->AddFilePick( ctrlidText, "text", NULL, L"выберите файл", g_pApp->GetUniqueTmpFolder( ) );
		//m_wndForm->AddText( ctrlidText, "text", NULL, font );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // void GetFormModel

	udInt CHDocTemplate::GetFormView( ) {
		bool		modal		= m_wndForm->IsModal( );
		if ( modal ) {
			udInt	prev_w = m_wndForm->get( "width" )->toInt( ),
					prev_h = m_wndForm->get( "height" )->toInt( );
			m_wndForm->get( "width" )->ini( ( udInt ) 800 );
			m_wndForm->get( "height" )->ini( ( udInt ) 600 );
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
					fntHeader	= g_pApp->GetFont( L"Tahoma", 18 ),	// шрифт заголовка
					fntButton	= g_pApp->GetFont( L"Tahoma", 14 );	// шрифт кнопки
		SolidBrush	brhLabel( Color( 0, 0, 0 ) ),		// кисть надписи
					brhHeader( Color( 0, 0, 0 ) );		// кисть заголовка
		RectF		rctLayout( ( float ) x, ( float ) y, ( float ) w, ( float ) h ),
					rctBound;
		Pen			penBorder( Color::MakeARGB( 255, 180, 180, 180 ) ),
					penBorderBound( Color::MakeARGB( 64, 255, 180, 180 ) );
		ModeFormControlBox *box = NULL;
		ModeFormControlInfo *info = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		title	= L"Шаблон документа";
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
		title	= L"Файл шаблона";
		x		= 32;
		w		= lcol;
		h		= 28;
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ), NULL, &brhLabel
		);

		x += w + m;
		box = new ModeFormControlBox;
		box->x = x;	box->y = y;	box->w = 200; box->h = 28;
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

	udInt CHDocTemplate::ValidateForm( ) {
		HWND hWnd = ( m_wndForm->IsModal( ) ? m_wndForm->GetHWndParent( ) : g_pApp->GetModel( )->GetWndModel( )->GetHWnd( ) );
		udPBase obj = m_wndForm->GetObj( );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( obj->get( "name" )->toWString( ).empty( ) ) {
			mb::inf( hWnd, L"Введите наименование" );
			return FormHandlerErrorFailed;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		string log = "\n";
		obj->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FormHandlerErrorOk;
	} // udInt ValidateForm

	udInt CHDocTemplate::GetListModel( ) {
		HWND		hWnd	= g_pApp->GetModel( )->GetWndWork( )->GetHWnd( );
		bool		modal	= m_wndModeList->IsModal( );
		udPWndLink	lnk		= NULL;
		string		url		= "";
		udDWord		id		= LIST_CONTROL_ID_LAST + 1;
		udUInt		size	= modal ? 12 : 18;
		wstring		title	= L"";
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !modal ) {
			title = L"На главную";
			url = "main";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			m_wndModeList->AddLink( lnk );
			//////////////////////////////////////////////////////////////////////////////////////
			title = L"Справочники";
			url = "info";
			lnk = new udWndLink( size );
			lnk->get( "title" )->ini( title );
			lnk->get( "url" )->ini( url );
			lnk->get( "id" )->ini( id++ );
			m_wndModeList->AddLink( lnk );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		title = ( modal ? L"Добавить шаблон" : L"Новый шаблон" );
		url = "#form";
		lnk = new udWndLink( size );
		lnk->get( "title" )->ini( title );
		lnk->get( "url" )->ini( url );
		lnk->get( "id" )->ini( id++ );
		m_wndModeList->AddLink( lnk );
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
		m_wndModeList->AddInput( fltName, "name", NULL, font, 0 );
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ListHandlerErrorOk;
	} // udInt GetListModel

	udInt CHDocTemplate::GetListView( ) {
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
			//////////////////////////////////////////////////////////////////////////////////////
			itLnk = arrLink->find( 1 );
			lnk = itLnk->second;
			lnk->get( "style" )->ini( style );
			lnk->get( "x" )->ini( x );
			lnk->get( "y" )->ini( y );
			lnk->CalculateWidthHeight( hWnd );
			x += lnk->get( "width" )->toInt( ) + m;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y = m;
			itLnk = arrLink->find( 0 );
		} else {
			itLnk = arrLink->find( 2 );
		}
		lnk = itLnk->second;
		lnk->get( "style" )->ini( style );
		lnk->get( "x" )->ini( x );
		lnk->get( "y" )->ini( y );
		lnk->CalculateWidthHeight( hWnd );
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
		box->x = x;	box->y = y;	box->w = 150; box->h = 15;
		box->padL = 3; box->padT = 2; box->padR = 3; box->padB = 2;
		box->border = 1; box->border_color = Color::MakeARGB( 255, 180, 180, 180 );
		info = m_wndModeList->GetInfo( fltName );
		info->box = box;
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( modal ) {
			x = m;
			y += 30 + m;
		} else {
			x += box->w + box->padL + box->padR + m;
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

	void CHDocTemplate::ClearFilter( ) {
		udPMorph filter = m_wndModeList->GetObjFilter( );
		filter->get( "name" )->ini( wstring( L"" ) );
	} // void ClearFilter

	void CHDocTemplate::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPWndFilePick filepick = m_wndForm->GetInfo( ctrlidText )->filepick;
		udUInt num = filepick->GetFile( ).empty( ) ? 0 : 1;
		udBinaryData data;
		data.add( ( char ) 80 );
		data.add( num );
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( num == 1 ) { // отправляем файл
			udFileTransfer objFileTransfer;
			udInt err = 0;
			string log = "";
			objFileTransfer.SetFileName( filepick->GetFile( ) );
			objFileTransfer.SetFilePath( filepick->GetFolder( ) + L"\\" + filepick->GetFile( ) );
			err = objFileTransfer.Send( sock );
			log += udFileTransfer::ErrorExplain( err ) + "\n";
			g_pApp->GetLog( )->Write( log );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	void CHDocTemplate::ExtraDataRecieve( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		__log->Write( "CHDocTemplate::ExtraDataRecieve, enter\n" );
		char code = 0;
		sock->ReadByte( &code );
		if ( code > 0 ) {
			string log;
			udUInt cnt = 0;
			udPWndFilePick filepick = m_wndForm->GetInfo( ctrlidText )->filepick;
			wstring tmp_dir = filepick->GetFolder( );
			udInt err = 0;
			udFileTransfer objFileTransfer;
			// очищаем временную папку от левых файлов
			DirectoryClear( tmp_dir );
			objFileTransfer.SetTmpDir( tmp_dir );
			err = objFileTransfer.Recv( sock );
			log = udFileTransfer::ErrorExplain( err ) + "\n";
			__log->Write( log );
			if ( err == udFileTransfer::FileTransferErrorOk ) {
				filepick->SetFile( obj->get( "text" )->toWString( ) );
				filepick->Render( );
				filepick->Flush( );
			}
		} else {
			__log->Write( "файл шаблона отсутствует на сервере\n" );
		}
		__log->Write( "CHDocTemplate::ExtraDataRecieve, exit\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataRecieve

	void CHDocTemplate::ProcContractTemplate( udPWndModeForm formTour, udPBase pClient, udPBase pTpl, udPBase pClDoc, const wstring& szAmount, udPArrBase pClients, udPBase pApp1/*udPBase pInfo, udPBase pApp1, udPBase pApp2*/ ) {
		assert( formTour );
		assert( pTpl );
		//////////////////////////////////////////////////////////////////////////////////////////
		wstring folder = g_pApp->GetUniqueTmpFolder( ), text = L"", szHead = L"";
		string out = "", log = "";
		udPExtList extlst = NULL;
		udPBase pTour = formTour->GetObj( ), obj = NULL;
		udPDwAscBase items = NULL;
		udPLog __log = g_pApp->GetLog( );
		udPSocket sock = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "CHDocTemplate::ProcContractTemplate 1\n" );
		udArrBase arrService;
		udPHService hService = ( udPHService ) g_pApp->GetModel( )->GetHandler( "service" );
		if ( hService ) {
			map< udUInt, udUInt > ids;
			log = "\nservices:\n";
			items = formTour->GetInfo( udHTour::ctrlidService )->frmlst->GetItems( );
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				ids[ i->second->get( "service" )->toUInt( ) ] = 1;
				i->second->dump( log );
				log += "\n";
			}
			__log->Write( log );
			hService->GetServicesById( ids, arrService );
			log = "\nservices2: полученные услуги\n";
			for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
				( *i )->dump( log );
				log += "\n";
			}
			__log->Write( log );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		// генерируем xml
		datetime dcur, dt1 = pTour->get( "date1" )->toTime( ), dt2 = pTour->get( "date2" )->toTime( );
		time_t days = 0, nights = 0, t1 = dt1.get( ), t2 = dt2.get( );
		dcur.load( );
		//nights = ( time_t ) ceil( ( double ) ( ( double ) t2 - ( double ) t1 ) / ( double ) 86400 );
		nights = ( t2 - t1 ) / 86400;
		days = nights + 1;
		__log->Write(
			"date1=" + dt1.toString( "%Y-%m-%d %H-%M-%S" ) + ", "
			"date2=" + dt2.toString( "%Y-%m-%d %H-%M-%S" ) + ", "
			"t1=" + toString( t1 ) + ", "
			"t2=" + toString( t2 ) + ", "
			"diff=" + toString( t2 - t1 ) + ", "
			"nights=" + toString( nights ) + ", "
			"days=" + toString( days ) + "\n"
		);
		szHead = L"<?xml version=\"1.0\"?><?xml-stylesheet type=\"text/xsl\" href=\"" + pTpl->get( "text" )->toWString( ) + L"\"?><tpldata>";
		text += L"<current_date date=\"" + dcur.toWString( "%d/%m/%Y" ) + L"\"/>";
		if ( pApp1 ) {
			text += L"<info>"
				L"<days1><![CDATA[" + pApp1->get( "days1" )->toWString( ) + L"]]></days1>"
				L"<days2><![CDATA[" + pApp1->get( "days2" )->toWString( ) + L"]]></days2>"
				L"<days3><![CDATA[" + pApp1->get( "days3" )->toWString( ) + L"]]></days3>"
				L"</info>";
		}
		text += L"<tour "
			L"days=\"" + toWString( days ) + L"\" "
			L"nights=\"" + toWString( nights ) + L"\" "
			L"date_begin=\"" + dt1.toWString( "%d/%m/%Y" ) + L"\" "
			L"date_end=\"" + dt2.toWString( "%d/%m/%Y" ) + L"\" "
			L"><name><![CDATA[" + pTour->get( "name" )->toWString( ) + L"]]></name>"
			L"<amount><![CDATA[" + szAmount + L"]]></amount>"
			L"</tour>";
		text += L"<company><name><![CDATA[" + formTour->GetInfo( udHTour::ctrlidSeller )->extlst->extlst->GetObj( )->get( "name" )->toWString( ) + L"]]></name></company>";
		text += L"<user><fio><![CDATA[" + formTour->GetInfo( udHTour::ctrlidManager )->extlst->extlst->GetObj( )->get( "fio" )->toWString( ) + L"]]></fio></user>";
		if ( pClient ) { // если распечатка группового договора, то клиент не турист не выбирается
			text += L"<tourist>"
				L"<fio><![CDATA[" + pClient->get( "fio" )->toWString( ) + L"]]></fio>"
				L"<email><![CDATA[" + pClient->get( "lemail" )->toWString( ) + L"]]></email>"
				L"<phone><![CDATA[" + pClient->get( "lphone" )->toWString( ) + L"]]></phone>"
				L"<addr><![CDATA[" + pClient->get( "laddr" )->toWString( ) + L"]]></addr>"
				L"</tourist>";
		}
		extlst = formTour->GetInfo( udHTour::ctrlidEmployer )->extlst->extlst;
		obj = extlst->GetObj( );
		text += L"<employer>"
			L"<fio><![CDATA[" + ( obj->isset( "fio" ) ? obj->get( "fio" )->toWString( ) : L"" ) + L"]]></fio>"
			L"<name><![CDATA[" + ( obj->isset( "name" ) ? obj->get( "name" )->toWString( ) : L"" ) + L"]]></name>"
			L"<email><![CDATA[" + obj->get( "lemail" )->toWString( ) + L"]]></email>"
			L"<phone><![CDATA[" + obj->get( "lphone" )->toWString( ) + L"]]></phone>"
			L"<addr><![CDATA[" + obj->get( "laddr" )->toWString( ) + L"]]></addr>"
			L"</employer>";
		if ( pClDoc ) {
			text += L"<cldoc>"
				L"<type><![CDATA[" + pClDoc->get( "type_" )->toWString( ) + L"]]></type>"
				L"<series><![CDATA[" + pClDoc->get( "series" )->toWString( ) + L"]]></series>"
				L"<number><![CDATA[" + pClDoc->get( "number" )->toWString( ) + L"]]></number>"
				L"<issuedate><![CDATA[" + pClDoc->get( "issuedate" )->toWString( ) + L"]]></issuedate>"
				L"<issuedby><![CDATA[" + pClDoc->get( "issuedby" )->toWString( ) + L"]]></issuedby>"
				L"<birthplace><![CDATA[" + pClDoc->get( "birthplace" )->toWString( ) + L"]]></birthplace>"
				L"<enddate><![CDATA[" + pClDoc->get( "enddate" )->toWString( ) + L"]]></enddate>"
				L"<fio><![CDATA[" + pClDoc->get( "fio" )->toWString( ) + L"]]></fio>"
				L"</cldoc>";
		}
		text += L"<services>";
		udPBase service = NULL;
		wstring name = L"", type_name = L"";
		for( udArrBase::iterator i = arrService.begin( ); i != arrService.end( ); ++i ) {
			service = *i;
			name = service->get( "name_" )->toWString( );
			type_name = service->get( "type_" )->toWString( );
			text += wstring( L"<service live=\"" ) + ( type_name == L"Гостиница" ? L"1" : L"0" ) + L"\" " +
				L"trans=\"" + ( type_name == L"Внутренний трансфер" ? L"1" : L"0" ) + L"\">"
				L"<name><![CDATA[" + name + L"]]></name></service>";
		}
		ClearArrBase( arrService );
		text += L"</services>";
		//////////////////////////////////////////////////////////////////////////////////////////
		map< udUInt, udUInt > idsCity;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		text += L"<route>";
		items = formTour->GetInfo( udHTour::ctrlidRoute )->frmlst->GetItems( );
		for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
			id = i->second->get( "city" )->toUInt( );
			idsCity[ id ] = id;
			text += L"<city><name><![CDATA[" + i->second->get( "name" )->toWString( ) + L"]]></name></city>";
		}
		text += L"</route>";
		udPArrBase pArrCities = LoadCitiesById( idsCity );
		if ( pArrCities ) {
			__log->Write( "pArrCities->size()=" + toString( pArrCities->size( ) ) + "\n" );
			__log->Write( "items->size()=" + toString( items->size( ) ) + "\n" );
			//////////////////////////////////////////////////////////////////////////////////////
			udPBase city = NULL;
			//////////////////////////////////////////////////////////////////////////////////////
			text += L"<route2>";
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				id = i->second->get( "city" )->toUInt( );
				__log->Write( "id=" + toString( id ) + "\n" );
				for( udArrBase::iterator j = pArrCities->begin( ); j != pArrCities->end( ); ++j ) {
					city = *j;
					__log->Write( "citi->id=" + city->get( "id" )->toString( ) + "\n" );
					if ( id == city->get( "id" )->toUInt( ) ) {
						text += L"<city>"
							L"<name><![CDATA[" + city->get( "name" )->toWString( ) + L"]]></name>"
							L"<country><![CDATA[" + city->get( "country_" )->toWString( ) + L"]]></country>"
							L"</city>";
						break;
					}
				}
			}
			text += L"</route2>";
			//////////////////////////////////////////////////////////////////////////////////////
			udUInt iLastCountry = 0, iCurCountry = 0;;
			text += L"<route3>";
			for( udDwAscBase::iterator i = items->begin( ); i != items->end( ); ++i ) {
				id = i->second->get( "city" )->toUInt( );
				for( udArrBase::iterator j = pArrCities->begin( ); j != pArrCities->end( ); ++j ) {
					city = *j;
					if ( id == city->get( "id" )->toUInt( ) ) {
						iCurCountry = city->get( "country" )->toUInt( );
						if ( ( iLastCountry == 0 ) || ( iCurCountry != iLastCountry ) ) {
							text += L"<country><![CDATA[" + city->get( "country_" )->toWString( ) + L"]]></country>";
						}
						iLastCountry = iCurCountry;
						break;
					}
				}
			}
			text += L"</route3>";
			//////////////////////////////////////////////////////////////////////////////////////
			for( udArrBase::iterator i = pArrCities->begin( ); i != pArrCities->end( ); ++i ) {
				udDELETE( *i );
			}
			//////////////////////////////////////////////////////////////////////////////////////
		}
		udDELETE( pArrCities );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( pTpl->get( "type" )->toInt( ) == udProjectDocTemplate::tplContract2 ) {
			if ( pApp1 ) {
				text += L"<app1>"
					L"<pay1><![CDATA[" + pApp1->get( "pay1" )->toWString( ) + L"]]></pay1>"
					L"<pay1date><![CDATA[" + pApp1->get( "pay1date" )->toWString( ) + L"]]></pay1date>"
					L"<pay2><![CDATA[" + pApp1->get( "pay2" )->toWString( ) + L"]]></pay2>"
					L"<pay2date><![CDATA[" + pApp1->get( "pay2date" )->toWString( ) + L"]]></pay2date>";
				//////////////////////////////////////////////////////////////////////////////////////////
				udPArrBase pArr = NULL;
				udPBase pObj = NULL;
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "occupancy" )->get( );
				text += L"<occupancy_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<occupancy>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"<category><![CDATA[" + pObj->get( "category" )->toWString( ) + L"]]></category>"
						L"<amount><![CDATA[" + pObj->get( "amount" )->toWString( ) + L"]]></amount>"
						L"<feed><![CDATA[" + pObj->get( "feed" )->toWString( ) + L"]]></feed>"
						L"</occupancy>";
				}
				text += L"</occupancy_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "program" )->get( );
				text += L"<program_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<program>"
						L"<text><![CDATA[" + pObj->get( "text" )->toWString( ) + L"]]></text>"
						L"</program>";
				}
				text += L"</program_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "transfer" )->get( );
				text += L"<transfer_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<transfer>"
						L"<fio><![CDATA[" + pObj->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<trans><![CDATA[" + pObj->get( "trans" )->toWString( ) + L"]]></trans>"
						L"<level><![CDATA[" + pObj->get( "level" )->toWString( ) + L"]]></level>"
						L"<date1><![CDATA[" + pObj->get( "date1" )->toWString( ) + L"]]></date1>"
						L"<date2><![CDATA[" + pObj->get( "date2" )->toWString( ) + L"]]></date2>"
						L"</transfer>";
				}
				text += L"</transfer_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "cover" )->get( );
				text += L"<cover_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<cover>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"</cover>";
				}
				text += L"</cover_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "visa" )->get( );
				text += L"<visa_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<visa>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"</visa>";
				}
				text += L"</visa_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "service" )->get( );
				text += L"<service_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<service>"
						L"<name><![CDATA[" + pObj->get( "name" )->toWString( ) + L"]]></name>"
						L"<char><![CDATA[" + pObj->get( "char" )->toWString( ) + L"]]></char>"
						L"</service>";
				}
				text += L"</service_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				pArr = ( udPArrBase ) pApp1->get( "transfer2" )->get( );
				text += L"<transfer2_list>";
				for( udArrBase::iterator i = pArr->begin( ); i != pArr->end( ); ++i ) {
					pObj = *i;
					text += L"<transfer2>"
						L"<route><![CDATA[" + pObj->get( "route" )->toWString( ) + L"]]></route>"
						L"<type><![CDATA[" + pObj->get( "type" )->toWString( ) + L"]]></type>"
						L"</transfer2>";
				}
				text += L"</transfer2_list>";
				//////////////////////////////////////////////////////////////////////////////////////////
				text += L"</app1>";
				//////////////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( pClients ) {
			text += L"<clients>";
			if ( pTour->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
				udPBase client = NULL;
				udUInt iEmployerId = pTour->get( "employer" )->toUInt( );
				for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
					client = *i;
					text += L"<client";
					if ( iEmployerId == client->get( "id" )->toUInt( ) ) {
						text += L" employer=\"1\"";
					}
					text += L">";
					text +=	L"<fio><![CDATA[" + client->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<fiolatin><![CDATA[" + client->get( "fiolatin" )->toWString( ) + L"]]></fiolatin>"
						L"<born><![CDATA[" + client->get( "born" )->toWString( ) + L"]]></born>"
						L"<sex><![CDATA[" + client->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<lemail><![CDATA[" + client->get( "lemail" )->toWString( ) + L"]]></lemail>"
						L"<lphone><![CDATA[" + client->get( "lphone" )->toWString( ) + L"]]></lphone>"
						L"<laddr><![CDATA[" + client->get( "laddr" )->toWString( ) + L"]]></laddr>"
						L"</client>";
				}
			} else {
				udPBase client = NULL;
				for( udArrBase::iterator i = pClients->begin( ); i != pClients->end( ); ++i ) {
					client = *i;
					text += L"<client>"
						L"<fio><![CDATA[" + client->get( "fio" )->toWString( ) + L"]]></fio>"
						L"<fiolatin><![CDATA[" + client->get( "fiolatin" )->toWString( ) + L"]]></fiolatin>"
						L"<born><![CDATA[" + client->get( "born" )->toWString( ) + L"]]></born>"
						L"<sex><![CDATA[" + client->get( "sex" )->toWString( ) + L"]]></sex>"
						L"<lemail><![CDATA[" + client->get( "lemail" )->toWString( ) + L"]]></lemail>"
						L"<lphone><![CDATA[" + client->get( "lphone" )->toWString( ) + L"]]></lphone>"
						L"<laddr><![CDATA[" + client->get( "laddr" )->toWString( ) + L"]]></laddr>"
						L"</client>";
				}
			}
			text += L"</clients>";
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock = BeginRequest( );
		if ( sock ) {
			udPBase pObjWrite = NULL;
			char code = 0;
			string log = "";
			udBinaryData data;
			data.add( string( "doctpl" ) );
			data.add( ( udInt ) opidGetResult );
			//////////////////////////////////////////////////////////////////////////////////////
			pObjWrite = GetProjectTour( );
			pObjWrite->apply( pTour, true, true );
			pObjWrite->dump( data );
			udDELETE( pObjWrite );
			//////////////////////////////////////////////////////////////////////////////////////
			pObjWrite = GetProjectDocTemplate( );
			pObjWrite->apply( pTpl, true, true );
			pObjWrite->dump( data );
			udDELETE( pObjWrite );
			//////////////////////////////////////////////////////////////////////////////////////
			data.add( szHead );
			data.add( text );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) {
				//////////////////////////////////////////////////////////////////////////////////
				udType szret( wstring( L"" ) );
				szret.read( sock );
				szHead += szret.toWString( );
				//////////////////////////////////////////////////////////////////////////////////
			}
			//////////////////////////////////////////////////////////////////////////////////////
			// закрываем соединение
			sock->Shutdown( );
			sock->Close( );
			udDELETE( sock );
		}
		text = szHead + text + L"</tpldata>";
		out = ConvertString( text, CP_UTF8 );
		WriteToFile( folder + L"\\out.xml", out );
		out.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// загружаем xsl
		sock = BeginRequest( );
		if ( sock ) {
			char code = 0;
			string log = "";
			udBinaryData data;
			data.add( string( "doctpl" ) );
			data.add( ( udInt ) opidGetFile );
			data.add( pTpl->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->ReadByte( &code );
			if ( code > 0 ) {
				udInt errCode = 0;
				udFileTransfer objFileTransfer;
				// очищаем временную папку от левых файлов
				objFileTransfer.SetTmpDir( folder );
				errCode = objFileTransfer.Recv( sock );
				log = udFileTransfer::ErrorExplain( errCode ) + "\n";
				__log->Write( log );
			}
			//////////////////////////////////////////////////////////////////////////////////////
			// закрываем соединение
			sock->Shutdown( );
			sock->Close( );
			udDELETE( sock );
		}
		//*/
		//////////////////////////////////////////////////////////////////////////////////////////
		ExtOpenFile( folder + L"\\out.xml" );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcContractTemplate

} // namespace UdSDK
