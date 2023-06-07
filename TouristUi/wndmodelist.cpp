
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndModeList::CWndModeList( udPListHandler handler ) {
		m_wndList	= new udWndInstanceList;
		m_pHandler	= handler;
		m_wndPager	= new udWndPager;
		m_btnOk		= new udWndButton;
		m_btnCancel	= new udWndButton;
		m_btnFilter	= new udWndButton;
		m_btnReset	= new udWndButton;
		m_bIsModal	= false;
		m_bShowFilter	= false;
		m_pObjFilter	= new udMorph;
	}

	CWndModeList::~CWndModeList( ) {
		udDELETE( m_wndList );
		udDELETE( m_wndPager );
		udDELETE( m_btnOk );
		udDELETE( m_btnCancel );
		udDELETE( m_btnFilter );
		udDELETE( m_btnReset );
		if ( !m_arrLink.empty( ) ) {
			for( udLAscWndLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
				delete i->second;
			}
			m_arrLink.clear( );
		}
		if ( !m_arrControl.empty( ) ) {
			ModeFormControlInfo *info = NULL;
			for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
				delete i->second;
			}
			m_arrControl.clear( );
		}
	}

	void CWndModeList::AddLink( udPWndLink lnk ) {
		assert( lnk );
		udLong index = m_arrLink.size( );
		m_arrLink[ index ] = lnk;
	} // void AddLink

	void CWndModeList::AddInput( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udDWord addstyle ) {
		if ( m_arrControl.find( iIndex ) == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlInput;
			ModeFormInputInfo *input = new ModeFormInputInfo;
			input->font = font;
			input->addstyle = addstyle;
			info->input = input;
			m_arrControl[ iIndex ] = info;
		}
	} // void AddInput

	void CWndModeList::AddText( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font ) {
		if ( m_arrControl.find( iIndex ) == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlText;
			ModeFormTextInfo *textarea = new ModeFormTextInfo;
			textarea->font = font;
			info->textarea = textarea;
			m_arrControl[ iIndex ] = info;
		}
	} // void AddText

	void CWndModeList::AddDate( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udInt bString ) {
		if ( m_arrControl.find( iIndex ) == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlDate;
			ModeFormDateInfo *date = new ModeFormDateInfo;
			date->font = font;
			date->bString = bString;
			info->date = date;
			m_arrControl[ iIndex ] = info;
		}
	} // void AddDate

	void CWndModeList::AddDatetime( udInt iIndex, string attr, ModeFormControlBox* box ) {
		if ( m_arrControl.find( iIndex ) == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlDatetime;
			m_arrControl[ iIndex ] = info;
		}
	} // void AddDatetime

	void CWndModeList::AddSelect( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, const udArrWString& arr ) {
		if ( m_arrControl.find( iIndex ) == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlSelect;
			ModeFormSelectInfo *select = new ModeFormSelectInfo;
			select->font = font;
			if ( !arr.empty( ) ) {
				for( udArrWString::const_iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					select->arr.push_back( *i );
				}
			}
			info->select = select;
			m_arrControl[ iIndex ] = info;
		}
	} // void AddSelect

	udPExtList CWndModeList::AddExtlist(
		udInt iIndex, string attr,
		ModeFormControlBox* box, const string& attrpick,
		//const string& index, const wstring& title,
		///const string& table, pfnGetObject fnGetObject,
		//bool auto_load
		const wstring& title,
		pfnGetListHandler fnGetListHandler,
		udPMorph opt,
		pfnExtLstAfterPick fnAfterPick
	) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlExtlist;
			ModeFormExtlistInfo *extlstinfo = new ModeFormExtlistInfo;
			udPExtList extlst = new udExtList;
			extlst->get( "title" )->ini( title );
			extlst->SetHandler( fnGetListHandler );
			//extlst->SetIndex( index );					// индекс контрола
			extlst->SetAttr( attrpick );				// атрибут сущности, по значению которого выбирать другие
			extlst->SetOpt( opt );
			extlst->SetAfterPick( fnAfterPick );
			extlstinfo->extlst = extlst;				// контрол
			//extlstinfo->attr = attr;				// атрибут сущности
			//extlstinfo->fnGetObject = fnGetObject;		// калбэк создания другой сущности
			//extlstinfo->table = table;					// таблица других сущностей
			//extlstinfo->auto_load = auto_load;			// автозагрузка объекта, иначе объект спрашивается у обработчика
			//extlstinfo->fnGetListHandler = fnGetListHandler;
			info->extlst = extlstinfo;					// информация о контроле
			m_arrControl[ iIndex ] = info;
			return extlst;
		}
		return i->second->extlst->extlst;
	} // udPExtList AddExtlist

	udPWndFormList CWndModeList::AddFormlist( udInt iIndex, string attr, ModeFormControlBox* box ) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlFormlist;
			udPWndFormList frmlst = new udWndFormList;
			info->frmlst = frmlst;
			m_arrControl[ iIndex ] = info;
			return info->frmlst;
		}
		return i->second->frmlst;
	} // udPFormList

	ModeFormControlInfo* CWndModeList::GetInfo( udInt iIndex ) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i != m_arrControl.end( ) ) {
			return i->second;
		}
		return NULL;
	} // ModeFormControlInfo* GetInfo

	void CWndModeList::FillInput( ModeFormControlInfo* info ) {
		wstring title = m_pObjFilter->get( info->attr )->toWString( );
		SetWindowText( info->input->hwnd, title.data( ) );
	} // void FillInput

	void CWndModeList::FillText( ModeFormControlInfo* info ) {
		wstring title = m_pObjFilter->get( info->attr )->toWString( );
		SetWindowText( info->textarea->hwnd, title.data( ) );
	} // void FillText

	void CWndModeList::FillExtlst( ModeFormControlInfo* info ) {
	} // void FillExtlst

	void CWndModeList::FillSelect( ModeFormControlInfo* info ) {
		udUInt val = m_pObjFilter->get( info->attr )->toUInt( ), toSet = 0;
		if ( val > 0 ) {
			toSet = val - 1;
		}
		SendMessage( info->select->hwnd, CB_SETCURSEL, ( WPARAM ) toSet, 0 );
	} // void FillSelect

	void CWndModeList::FillDate( ModeFormControlInfo* info ) {
		PSYSTEMTIME	time = NULL;
		if ( info->date->bString == TRUE ) {
			string	date = m_pObjFilter->get( info->attr )->toString( ),
					tmp;
			tmp += date.data( )[ 0 ];
			tmp += date.data( )[ 1 ];
			tmp += date.data( )[ 2 ];
			tmp += date.data( )[ 3 ];
			int year = toInt( tmp );
			tmp.clear( );
			tmp += date.data( )[ 5 ];
			tmp += date.data( )[ 6 ];
			int month = toInt( tmp );
			tmp.clear( );
			tmp += date.data( )[ 8 ];
			tmp += date.data( )[ 9 ];
			int day = toInt( tmp );
			time = new SYSTEMTIME;
			time->wYear = year;
			time->wMonth = month;
			time->wDayOfWeek = 0; // хз нахуя эта фигня
			time->wDay = day;
			time->wHour = 0;
			time->wMinute = 0;
			time->wSecond = 0;
			time->wMilliseconds = 0;
		} else {
			datetime ddate = m_pObjFilter->get( info->attr )->toTime( );
			if ( ddate.get( ) == 0 ) {
				//ddate.load( );
			}
			time_t date = ddate.get( );
			struct tm cur;
			if ( !localtime_s( &cur, &date ) ) {
				time = new SYSTEMTIME;
				time->wYear = cur.tm_year + 1900;
				time->wMonth = cur.tm_mon + 1;
				time->wDayOfWeek = 0; // хз нахуя эта фигня
				time->wDay = cur.tm_mday;
				time->wHour = cur.tm_hour;
				time->wMinute = cur.tm_min;
				time->wSecond = cur.tm_sec;
				time->wMilliseconds = 0;
			} else {
				WriteUdUiLog( "CWndModeList::FillDate, localtime_s failed\n" );
			}
		}
		if ( time ) {
			DateTime_SetSystemtime( info->date->hwnd, GDT_VALID, time );
		}
		udDELETE( time );
	} // void FillDate

	void CWndModeList::FillFormlist( ModeFormControlInfo* info ) {
	} // void FillFormlist

	void CWndModeList::MakeInput( udPGdi gdi, ModeFormControlInfo* info ) {
		// высоту окна контрола (EDIT) выставляем как высоту линии (14.48=>15)
		// ширину выставляем того что указали в коробке, но позиционируем так, что
		// оно будет находиться от (x+padding-left,y+padding-top), а рамку
		// рисуем вниз от контрола на расстояние отступов снизу-справа
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
		// создаем контрол
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;
		info->input->hwnd = CreateWindowEx( 0, L"EDIT", L"", WS_STANDARD_EDIT | WS_TABSTOP | info->input->addstyle,
			x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
		// настраиваем его
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;
		ZeroMemory( &objLogFont, sizeof( LOGFONT ) );

		udGdi gdi2( m_hWnd );
		info->input->font->GetLogFontW( &gdi2, &objLogFont );
		//info->input->font->GetLogFontW( gdi, &objLogFont );

		hFont = CreateFontIndirect( &objLogFont );
		SendMessage( info->input->hwnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
		info->input->hfont = hFont;
	} // void MakeInput

	static LRESULT CALLBACK udEditMultiline(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		UINT_PTR uIdSubclass, DWORD_PTR dwRefData
	) {
		switch( uMsg ) {
			case WM_KEYDOWN:
				if ( wParam == VK_ESCAPE ) {
					::SendMessage( GetParent( hWnd ), WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), 0 );
					return TRUE;
				}
				break;
		}
		return DefSubclassProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT udEditMultiline

	void CWndModeList::MakeText( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
		// создаем контрол
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;
		info->textarea->hwnd = CreateWindowEx( 0, L"EDIT", L"",
			//WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | WS_TABSTOP,
			WS_STANDARD_EDIT_MULTILINE | WS_TABSTOP,
			x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
		SetWindowSubclass( info->textarea->hwnd, udEditMultiline, 0, 0 );
		// настраиваем его
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		info->textarea->font->GetLogFontW( gdi, &objLogFont );
		hFont = CreateFontIndirect( &objLogFont );
		SendMessage( info->textarea->hwnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
		info->textarea->hfont = hFont;
	} // void MakeText

	void CWndModeList::MakeExtlist( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udPExtList extlst = info->extlst->extlst;
		extlst->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_TABSTOP ) );
		extlst->get( "x" )->ini( info->box->x );
		extlst->get( "y" )->ini( info->box->y );
		extlst->get( "width" )->ini( info->box->w );
		extlst->get( "height" )->ini( info->box->h );
		if ( m_bIsModal ) {
			extlst->SetDialogParent( m_hWndParent );
		} else {
			extlst->SetDialogParent( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ) );
		}
		extlst->Create( m_hWnd, ( HMENU ) info->id );
	} // void MakeExtlist

	void CWndModeList::MakeSelect( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
		// создаем контрол
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = 500;//box->h;
		info->select->hwnd = CreateWindowEx( 0, L"COMBOBOX", L"", WS_STANDARD_SELECT | WS_TABSTOP,// | CBS_OWNERDRAWFIXED,
			x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
		// настраиваем его
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		udGdi gdi2( m_hWnd );
		info->select->font->GetLogFontW( &gdi2, &objLogFont );
		hFont = CreateFontIndirect( &objLogFont );
		SendMessage( info->select->hwnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
		info->select->hfont = hFont;

		for( udArrWString::iterator i = info->select->arr.begin( ); i != info->select->arr.end( ); ++i ) {
			SendMessage( info->select->hwnd, CB_ADDSTRING, 0, ( LPARAM ) ( *i ).data( ) );
		}

		ComboBox_SetMinVisible( info->select->hwnd, 10 );
	} // void MakeSelect

	void CWndModeList::MakeDate( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
		// создаем контрол
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;
		info->date->hwnd = CreateWindowEx( 0, DATETIMEPICK_CLASS, L"", WS_CHILD | WS_TABSTOP,
			x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
		// настраиваем его
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		udGdi gdi2( m_hWnd );
		info->date->font->GetLogFontW( &gdi2, &objLogFont );
		hFont = CreateFontIndirect( &objLogFont );
		info->date->hfont = hFont;
		DateTime_SetMonthCalFont( info->date->hwnd, hFont, TRUE );
		udWChar frmt[ 256 ];
		swprintf_s( frmt, 256, L"dd'.'MM'.'yyyy" );
		DateTime_SetFormat( info->date->hwnd, frmt );
	} // void MakeDate

	void CWndModeList::MakeFormlist( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udPWndFormList frmlst = info->frmlst;
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		/*x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );*/
		// создаем контрол
		x = box->x;// + box->border + box->padL;
		y = box->y;// + box->border + box->padT;
		w = box->w;
		h = box->h;
		// создаем контрол
		frmlst->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_TABSTOP ) );
		frmlst->get( "x" )->ini( x );
		frmlst->get( "y" )->ini( y );
		frmlst->get( "width" )->ini( w );
		frmlst->get( "height" )->ini( h );
		frmlst->Create( m_hWnd, ( HMENU ) info->id );
	} // void MakeFormlist

	void CWndModeList::FillControls( ) {
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			switch( info->type ) {
				case ctrlInput: this->FillInput( info ); break;
				case ctrlText: this->FillText( info ); break;
				case ctrlExtlist: this->FillExtlst( info ); break;
				case ctrlSelect: this->FillSelect( info ); break;
				case ctrlDate: this->FillDate( info ); break;
				case ctrlFormlist: this->FillFormlist( info ); break;
				default: break;
			}
		}
	} // void FillControls

	void CWndModeList::ShowControls( ) {
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			switch( info->type ) {
				case ctrlInput: ShowWindow( info->input->hwnd, SW_SHOW ) ; break;
				case ctrlText: ShowWindow( info->textarea->hwnd, SW_SHOW ); break;
				case ctrlExtlist: info->extlst->extlst->Show( ); break;
				case ctrlSelect: ShowWindow( info->select->hwnd, SW_SHOW ); break;
				case ctrlDate: ShowWindow( info->date->hwnd, SW_SHOW ); break;
				case ctrlFormlist: info->frmlst->Show( ); break;
				default: break;
			}
		}
	} // void ShowControls

	void CWndModeList::RenderControls( ) {
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			switch( info->type ) {
				case ctrlInput: this->RenderInput( info ); break;
				case ctrlText: this->RenderText( info ); break;
				case ctrlExtlist: this->RenderExtlist( info ); break;
				case ctrlSelect: this->RenderSelect( info ); break;
				case ctrlDate: this->RenderDate( info ); break;
				case ctrlFormlist: this->RenderFormlist( info ); break;
				default: break;
			}
		}
	} // void RenderControls

	void CWndModeList::DestroyControls( ) {
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			//WriteUdUiLog( "control_type=" + toString( info->type ) + ", control_attr=" + info->attr + "\n" );
			switch( info->type ) {
				case ctrlInput: DestroyWindow( info->input->hwnd ); break;
				case ctrlText: DestroyWindow( info->textarea->hwnd ); break;
				case ctrlExtlist: info->extlst->extlst->Destroy( ); break;
				case ctrlSelect: DestroyWindow( info->select->hwnd ); break;
				case ctrlDate: info->frmlst->Destroy( ); break;
				default: break;
			}
		}
	} // void DestroyControls

	void CWndModeList::LoadControlData( ) {
		//WriteUdUiLog( "CWndModeList::LoadControlData, enter\n" );
		//WriteUdUiLog( "control_num=" + toString( m_arrControl.size( ) ) + "\n" );
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			//WriteUdUiLog( "control_type=" + toString( info->type ) + ", control_attr=" + info->attr + "\n" );
			switch( info->type ) {
				case ctrlInput: this->LoadInput( info ); break;
				case ctrlText: this->LoadText( info ); break;
				case ctrlExtlist: this->LoadExtlst( info ); break;
				case ctrlSelect: this->LoadSelect( info ); break;
				case ctrlDate: this->LoadDate( info ); break;
				default: break;
			}
		}
		//string log = "\nобъект после подгрузки данных:\n";
		//m_pObjFilter->dump( log );
		//WriteUdUiLog( log + "\n" );
		//WriteUdUiLog( "CWndModeList::LoadControlData, exit\n" );
	} // void LoadControlData

	void CWndModeList::LoadInput( ModeFormControlInfo* info ) {
		udWChar tmp[ 256 ];
		GetWindowText( info->input->hwnd, tmp, 256 );
		m_pObjFilter->get( info->attr )->ini( wstring( tmp ) );
	} // void LoadInput

	void CWndModeList::LoadText( ModeFormControlInfo* info ) {
		udWChar tmp[ 1024 ];
		GetWindowText( info->textarea->hwnd, tmp, 1024 );
		m_pObjFilter->get( info->attr )->ini( wstring( tmp ) );
	} // void LoadText

	void CWndModeList::LoadExtlst( ModeFormControlInfo *info ) {
		udPBase obj = info->extlst->extlst->GetObj( );
		if ( obj ) {
			m_pObjFilter->get( info->attr )->ini( obj->get( "id" )->toUInt( ) );
		}
	} // void LoadExtlst

	void CWndModeList::LoadSelect( ModeFormControlInfo* info ) {
		udUInt val = SendMessage( info->select->hwnd, CB_GETCURSEL, 0, 0 );
		m_pObjFilter->get( info->attr )->ini( val + 1 );
	} // void LoadSelect

	void CWndModeList::LoadDate( ModeFormControlInfo* info ) {
		udWChar date[ 256 ];
		GetWindowText( info->date->hwnd, date, 256 );
		wstring date2;
		// дата [xx-xx-xxxx]
		// год
		date2 += date[ 6 ];
		date2 += date[ 7 ];
		date2 += date[ 8 ];
		date2 += date[ 9 ];
		date2 += L"-";
		// месяц
		date2 += date[ 3 ];
		date2 += date[ 4 ];
		date2 += L"-";
		// день
		date2 += date[ 0 ];
		date2 += date[ 1 ];
		//
		if ( info->date->bString == TRUE ) {
			m_pObjFilter->get( info->attr )->ini( date2 );
		} else {
			m_pObjFilter->get( info->attr )->ini( udDatetime( date2 ) );
		}
	} // void LoadDate

	void CWndModeList::LoadFormlist( ModeFormControlInfo* info ) {
	} // void LoadFormlist

	LRESULT CWndModeList::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_BUTTONCLICK:
				return this->msgButtonClick( lParam );
				break;

			case WM_LINKCLICK:
				return this->msgLinkClick( lParam );
				break;

			case WM_PAGER_SELECT:
				return this->msgPagerSelect( lParam );
				break;

			case WM_LIST_ROW_SELECT:
				return this->msgListRowSelect( lParam );
				break;

			case WM_LIST_HEADER_CLICK:
				return this->msgListHeaderClick( lParam );
				break;
		}
		if ( m_bIsModal && ( uMsg == WM_COMMAND ) ) {
			::PostMessage( m_hWndParent, WM_COMMAND, wParam, lParam );
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	bool CWndModeList::Create( HWND hParent, HMENU hMenu ) {
		//WriteUdUiLog( "CWndModeList::Create, enter\n" );
		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			WriteUdUiLog( "CWndModeList::Create, failed\n" );
			return false;
		}

		if ( m_bIsModal ) {
			m_btnOk->get( "title" )->ini( wstring( L"Ок" ) );
			m_btnOk->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE | WS_TABSTOP ) );
			m_btnOk->get( "x" )->ini( ( udInt ) 0 );
			m_btnOk->get( "y" )->ini( ( udInt ) 0 );
			m_btnOk->get( "width" )->ini( ( udInt ) 103 );
			m_btnOk->get( "height" )->ini( ( udInt ) 37 );
			m_btnOk->SetFontSize( 14 );
			m_btnOk->SetPaddingLeft( 40 );
			m_btnOk->SetPaddingTop( 10 );
			m_btnOk->SetRectNormal( new Rect( 1746, 108, 103, 37 ) );
			m_btnOk->SetTargetWnd( m_hWnd );
			m_btnOk->Create( m_hWnd, ( HMENU ) IDOK );
			//
			m_btnCancel->get( "title" )->ini( wstring( L"Отмена" ) );
			m_btnCancel->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE | WS_TABSTOP ) );
			m_btnCancel->get( "x" )->ini( ( udInt ) 0 );
			m_btnCancel->get( "y" )->ini( ( udInt ) 0 );
			m_btnCancel->get( "width" )->ini( ( udInt ) 103 );
			m_btnCancel->get( "height" )->ini( ( udInt ) 37 );
			m_btnCancel->SetFontSize( 14 );
			m_btnCancel->SetPaddingLeft( 40 );
			m_btnCancel->SetPaddingTop( 10 );
			m_btnCancel->SetRectNormal( new Rect( 1746, 161, 103, 37 ) );
			m_btnCancel->SetTargetWnd( m_hWnd );
			m_btnCancel->Create( m_hWnd, ( HMENU ) IDCANCEL );
		}
		if ( m_bShowFilter ) {
			m_btnFilter->get( "width" )->ini( ( udInt ) 152 );
			m_btnFilter->get( "height" )->ini( ( udInt ) 34 );
			m_btnReset->get( "width" )->ini( ( udInt ) 152 );
			m_btnReset->get( "height" )->ini( ( udInt ) 34 );
		}
		//WriteUdUiLog( "CWndModeList::Create, exit\n" );
		return true;
	} // bool Create

	void CWndModeList::MakeControls( ) {
		//WriteUdUiLog( "CWndModeList::MakeControls, enter\n" );
		HMENU		id	= ( HMENU ) 1;
		udPWndLink	lnk	= NULL;

		//WriteUdUiLog( "CWndModeList::MakeControls, creating links\n" );
		for( udLAscWndLink::iterator i = m_arrLink.begin( ); i != m_arrLink.end( ); ++i ) {
			lnk = i->second;
			lnk->SetTargetWnd( m_hWnd );
			lnk->Create( m_hWnd, id++ );
		}
		//WriteUdUiLog( "CWndModeList::MakeControls, links created\n" );

		if ( m_bShowFilter ) {
			udPGdi gdi = this->GetGdi( );
			ModeFormControlInfo *info = NULL;
			//WriteUdUiLog( "CWndModeList::MakeControls, creating controls\n" );
			for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
				info = i->second;
				switch( info->type ) {
					case ctrlInput: this->MakeInput( gdi, info ); break;
					case ctrlText: this->MakeText( gdi, info ); break;
					case ctrlExtlist: this->MakeExtlist( gdi, info ); break;
					case ctrlSelect: this->MakeSelect( gdi, info ); break;
					case ctrlDate: this->MakeDate( gdi, info ); break;
					case ctrlFormlist: this->MakeFormlist( gdi, info ); break;
					default: break;
				}
			}
			//WriteUdUiLog( "CWndModeList::MakeControls, controls created\n" );

			if ( !m_btnFilter->isset( "title" ) || m_btnFilter->get( "title" )->toWString( ).empty( ) ) {
				m_btnFilter->get( "title" )->ini( wstring( L"Фильтровать" ) );
			}
			m_btnFilter->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE | WS_TABSTOP ) );
			//m_btnFilter->get( "x" )->ini( ( udInt ) 0 );
			//m_btnFilter->get( "y" )->ini( ( udInt ) 0 );
			m_btnFilter->get( "width" )->ini( ( udInt ) 152 );
			m_btnFilter->get( "height" )->ini( ( udInt ) 34 );
			m_btnFilter->SetFontSize( 14 );
			m_btnFilter->SetPaddingLeft( 10 );
			m_btnFilter->SetPaddingTop( 10 );
			m_btnFilter->SetRectNormal( new Rect( 253, 159, 152, 34 ) );
			m_btnFilter->SetTargetWnd( m_hWnd );
			m_btnFilter->Create( m_hWnd, ( HMENU ) 998 );

			if ( !m_btnReset->isset( "title" ) || m_btnReset->get( "title" )->toWString( ).empty( ) ) {
				m_btnReset->get( "title" )->ini( wstring( L"Сброс" ) );
			}
			m_btnReset->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE | WS_TABSTOP ) );
			//m_btnReset->get( "x" )->ini( ( udInt ) 0 );
			//m_btnReset->get( "y" )->ini( ( udInt ) 0 );
			m_btnReset->get( "width" )->ini( ( udInt ) 152 );
			m_btnReset->get( "height" )->ini( ( udInt ) 34 );
			m_btnReset->SetFontSize( 14 );
			m_btnReset->SetPaddingLeft( 10 );
			m_btnReset->SetPaddingTop( 10 );
			m_btnReset->SetRectNormal( new Rect( 253, 159, 152, 34 ) );
			m_btnReset->SetTargetWnd( m_hWnd );
			m_btnReset->Create( m_hWnd, ( HMENU ) 999 );
		}

		//WriteUdUiLog( "CWndModeList::MakeControls, creating list\n" );
		m_wndList->Create( m_hWnd, id++ );
		//WriteUdUiLog( "CWndModeList::MakeControls, list created\n" );
		//WriteUdUiLog( "CWndModeList::MakeControls, creating pager\n" );
		m_wndPager->Create( m_hWnd, id++ );
		//WriteUdUiLog( "CWndModeList::MakeControls, pager created\n" );
		//WriteUdUiLog( "CWndModeList::MakeControls, exit\n" );
	} // void MakeControls

	LRESULT CWndModeList::msgListHeaderClick( LPARAM lParam ) {
		assert( m_pHandler );
		udPWndButton btn = ( udPWndButton ) lParam;
		m_pHandler->OnHeaderClick( btn->get( "name" )->toString( ), btn->get( "attr" )->toString( ) );
		return TRUE;
	} // LRESULT msgListHeaderClick

	LRESULT CWndModeList::msgListRowSelect( LPARAM lParam ) {
		assert( m_pHandler );
		m_pHandler->OnSelectRow( ( udLong ) lParam );
		return TRUE;
	} // LRESULT msgListRowSelect

	LRESULT CWndModeList::msgPagerSelect( LPARAM lParam ) {
		assert( m_pHandler );
		m_pHandler->OnPagerSelect( ( udUInt ) lParam );
		return TRUE;
	} // LRESULT msgPagerSelect

	LRESULT CWndModeList::msgLinkClick( LPARAM lParam ) {
		assert( m_pHandler );
		udPWndLink lnk = ( udPWndLink ) lParam;
		assert( lnk );
		m_pHandler->OnLinkClick( lnk );
		return TRUE;
	} // LRESULT msgLinkClick

	LRESULT CWndModeList::msgButtonClick( LPARAM lParam ) {
		assert( m_pHandler );
		udPWndButton btn = ( udPWndButton ) lParam;
		assert( btn );
		if ( btn == m_btnOk ) {
			m_pHandler->ListOk( );
		} else if ( btn == m_btnCancel ) {
			m_pHandler->ListCancel( );
		} else if ( btn == m_btnFilter ) {
			m_pHandler->OnFilter( );
		} else if ( btn == m_btnReset ) {
			m_pHandler->OnFilterReset( );
		} else {
			m_pHandler->OnButtonClick( btn );
		}
		return 1;
	} // LRESULT msgButtonClick

	void CWndModeList::Render( ) {
		udPGdi gdi = this->GetGdi( );
		gdi->Clear( Color( 255, 255, 255 ) );
		udInt	w	= this->get( "width" )->toInt( ),
				h	= this->get( "height" )->toInt( );
		if ( m_bIsModal ) {
			Pen	penBorder( Color( 180, 180, 180 ), 2 );
			gdi->DrawLine( &penBorder, Point( 1, 1 ), Point( w - 1, 1 ) );
			gdi->DrawLine( &penBorder, Point( 1, 1 ), Point( 1, h - 1 ) );
			gdi->DrawLine( &penBorder, Point( w - 1, 1 ), Point( w - 1, h - 1 ) );
			gdi->DrawLine( &penBorder, Point( 1, h - 1 ), Point( w - 1, h - 1 ) );
		} else {
			udPBitmap	imgSprite	= g_pApp->GetBitmap( "sprite.png" );
			udRenderer::RenderBackground(
				gdi, imgSprite,
				Rect( 0, 0, 10, h ),
				Rect( 556, 283, 10, 101 ),
				udRenderer::UDSDK_REPEAT_Y
			);
		}
		//this->RenderControls( );
	} // void Render

	void CWndModeList::SetButtonOkPosition( udInt x, udInt y ) {
		assert( m_btnOk );
		m_btnOk->get( "x" )->ini( x );
		m_btnOk->get( "y" )->ini( y );
	} // void SetButtonOkPosition

	void CWndModeList::SetButtonCancelPosition( udInt x, udInt y ) {
		assert( m_btnCancel );
		m_btnCancel->get( "x" )->ini( x );
		m_btnCancel->get( "y" )->ini( y );
	} // void SetButtonCancelPosition

	void CWndModeList::SetButtonFilterPosition( udInt x, udInt y ) {
		assert( m_btnFilter );
		m_btnFilter->get( "x" )->ini( x );
		m_btnFilter->get( "y" )->ini( y );
	} // void SetButtonFilterPosition

	void CWndModeList::SetButtonResetPosition( udInt x, udInt y ) {
		assert( m_btnReset );
		m_btnReset->get( "x" )->ini( x );
		m_btnReset->get( "y" )->ini( y );
	} // void SetButtonResetPosition

	void CWndModeList::RenderInput( ModeFormControlInfo* info ) {
		udPGdi gdi = this->GetGdi( );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
	} // void RenderInput

	void CWndModeList::RenderText( ModeFormControlInfo* info ) {
		udPGdi gdi = this->GetGdi( );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
	} // void RenderText

	void CWndModeList::RenderExtlist( ModeFormControlInfo* info ) {
	} // void RenderExtlist

	void CWndModeList::RenderSelect( ModeFormControlInfo* info ) {
		udPGdi gdi = this->GetGdi( );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
	} // void RenderSelect

	void CWndModeList::RenderDate( ModeFormControlInfo* info ) {
		udPGdi gdi = this->GetGdi( );
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
		// рисуем рамку
		x = box->x;
		y = box->y;
		w = box->w + box->padL + box->padR;
		h = box->h + box->padT + box->padB;
		Pen penBorder( Color( box->color ), ( float ) box->border );
		gdi->DrawRectangle( &penBorder, x, y, w, h );
	} // void RenderDate

	void CWndModeList::RenderFormlist( ModeFormControlInfo* info ) {
	} // void RenderFormlist

	void CWndModeList::ClearFilter( ) {
		ModeFormControlInfo *info = NULL;
		wstring empty = L"";
		datetime ddate;
		time_t date = ddate.get( );
		struct tm cur;
		LPSYSTEMTIME time;
		if ( !localtime_s( &cur, &date ) ) {
			time = new SYSTEMTIME;
			time->wYear = cur.tm_year + 1900;
			time->wMonth = cur.tm_mon + 1;
			time->wDayOfWeek = 0; // хз нахуя эта фигня
			time->wDay = cur.tm_mday;
			time->wHour = cur.tm_hour;
			time->wMinute = cur.tm_min;
			time->wSecond = cur.tm_sec;
			time->wMilliseconds = 0;
		}

		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			if ( info->type == ctrlInput ) {
				SetWindowText( info->input->hwnd, empty.data( ) );
			} else if ( info->type == ctrlText ) {
				SetWindowText( info->textarea->hwnd, empty.data( ) );
			} else if ( info->type == ctrlDate || info->type == ctrlDatetime ) {
				DateTime_SetSystemtime( info->date->hwnd, GDT_VALID, time );
			} else if ( info->type == ctrlSelect ) {
				SendMessage( info->select->hwnd, CB_SETCURSEL, ( WPARAM ) 0, 0 );
			} else if ( info->type == ctrlExtlist ) {
				info->extlst->extlst->ClearSelection( );
			} else if ( info->type == ctrlFormlist ) {
				info->frmlst->Clear( );
			}
		}
		delete time;
	} // void ClearFilter

} // namespace UdSDK
