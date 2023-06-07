
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndModeForm::CWndModeForm( udPFormHandler handler ) {
		m_pHandler		= handler;
		m_pObject		= NULL;
		m_btnOk			= new udWndButton;
		m_btnCancel		= new udWndButton;
		m_fnGetObject	= NULL;
		m_bIsModal		= false;
		m_bIsWatch		= false;
	}

	CWndModeForm::~CWndModeForm( ) {
		udDELETE ( m_pObject );
		udDELETE ( m_btnOk );
		udDELETE ( m_btnCancel );
		if ( !m_arrControl.empty( ) ) {
			ModeFormControlInfo *info = NULL;
			for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
				delete i->second;
			}
			m_arrControl.clear( );
		}
	}

	LRESULT CWndModeForm::MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		switch( uMsg ) {
			case WM_BUTTONCLICK:
				return this->msgButtonClick( lParam );
				break;
		}
		if ( m_bIsModal && ( uMsg == WM_COMMAND ) ) {
			::PostMessage( m_hWndParent, WM_COMMAND, wParam, lParam );
		}
		return udWndGraphic::MessageProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT MessageProc

	LRESULT CWndModeForm::msgButtonClick( LPARAM lParam ) {
		assert( m_pHandler );
		udPWndButton btn = ( udPWndButton ) lParam;
		assert( btn );
		if ( btn == m_btnOk ) {
			if ( !m_bIsWatch ) {
				if ( m_bIsModal ) {
					::PostMessage( m_hWndParent, WM_COMMAND, MAKEWPARAM( IDOK, 0 ), ( LPARAM ) this );
				} else {
					m_pHandler->FormOk( );
				}
			}
		} else {
			if ( m_bIsModal ) {
				::PostMessage( m_hWndParent, WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), ( LPARAM ) this );
			} else {
				m_pHandler->FormCancel( );
			}
		}
		return 1;
	} // LRESULT msgButtonClick

	LRESULT CWndModeForm::msgDrawItem( WPARAM wParam, LPARAM lParam ) {
		if ( wParam ) {
			LPDRAWITEMSTRUCT di = ( LPDRAWITEMSTRUCT ) lParam;
			if ( di->CtlType == ODT_COMBOBOX ) {
				this->DrawSelect( di );
			}
		}
		return TRUE;
	} // LRESULT msgDrawItem

	void CWndModeForm::DrawSelect( LPDRAWITEMSTRUCT di ) {
		ModeFormControlInfo *info = m_arrControl[ di->CtlID - 1000 ];
		udPGdi gdi = new udGdi( di->hDC );

		delete gdi;
	} // void DrawSelect

	void CWndModeForm::AddInput( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udDWord addstyle ) {
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

	void CWndModeForm::AddText( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font ) {
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

	void CWndModeForm::AddDate( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udInt bString ) {
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

	void CWndModeForm::AddDatetime( udInt iIndex, string attr, ModeFormControlBox* box ) {
		if ( m_arrControl.find( iIndex ) == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlDatetime;
			m_arrControl[ iIndex ] = info;
		}
	} // void AddDatetime

	void CWndModeForm::AddSelect( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, const udArrWString& arr ) {
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

	udPExtList CWndModeForm::AddExtlist(
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

	udPWndFormList CWndModeForm::AddFormlist( udInt iIndex, string attr, ModeFormControlBox* box, const wstring& folder ) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlFormlist;
			udPWndFormList frmlst = new udWndFormList;
			frmlst->SetHandler( m_pHandler );
			frmlst->SetFolder( folder );
			info->frmlst = frmlst;
			m_arrControl[ iIndex ] = info;
			return info->frmlst;
		}
		return i->second->frmlst;
	} // udPFormList

	udPWndFilePick CWndModeForm::AddFilePick( udInt iIndex, const string& attr, ModeFormControlBox* box, const wstring& title, const wstring& folder ) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = attr;
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlFilepick;
			udPWndFilePick filepick = new udWndFilePick;
			filepick->get( "title" )->ini( title );
			filepick->SetFolder( folder );
			info->filepick = filepick;
			m_arrControl[ iIndex ] = info;
			return info->filepick;
		}
		return NULL;
	} // udPWndFilePick AddFilePick

	udPWndStatic CWndModeForm::AddStatic( udInt iIndex, ModeFormControlBox* box, const wstring& title, udUInt size, udDWord color ) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i == m_arrControl.end( ) ) {
			ModeFormControlInfo *info = new ModeFormControlInfo;
			info->attr = "";
			info->id = 1000 + iIndex;
			info->box = box;
			info->type = ctrlStatic;
			udPWndStatic staticw = new udWndStatic( size, 0 );
			staticw->get( "title" )->ini( title );
			info->staticw = staticw;
			m_arrControl[ iIndex ] = info;
			return info->staticw;
		}
		return NULL;
	} // udPWndStatic AddStatic

	void CWndModeForm::Render( ) {
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
	} // void Render

	bool CWndModeForm::Create( HWND hParent, HMENU hMenu ) {
		if ( !udWndGraphic::Create( hParent, hMenu ) ) {
			return false;
		}
		if ( m_fnGetObject ) {
			m_pObject = m_fnGetObject( );
		}

		return true;
	} // bool Create

	void CWndModeForm::SetObj( udPBase obj ) {
		udDELETE( m_pObject );
		if ( obj ) {
			m_pObject = obj;
		} else if ( m_fnGetObject ) {
			m_pObject = m_fnGetObject( );
		}
		this->FillControls( );
	} // void SetObj

	void CWndModeForm::SetButtonOkPosition( udInt x, udInt y ) {
		assert( m_btnOk );
		m_btnOk->get( "x" )->ini( x );
		m_btnOk->get( "y" )->ini( y );
		m_btnOk->Move( );
	} // void SetButtonOkPosition

	void CWndModeForm::SetButtonCancelPosition( udInt x, udInt y ) {
		assert( m_btnCancel );
		m_btnCancel->get( "x" )->ini( x );
		m_btnCancel->get( "y" )->ini( y );
		m_btnCancel->Move( );
	} // void SetButtonCancelPosition

	void CWndModeForm::MakeControls( ) {
		udPGdi gdi = this->GetGdi( );
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			switch( info->type ) {
				case ctrlInput: this->MakeInput( gdi, info ); break;
				case ctrlText: this->MakeText( gdi, info ); break;
				case ctrlExtlist: this->MakeExtlist( gdi, info ); break;
				case ctrlSelect: this->MakeSelect( gdi, info ); break;
				case ctrlDate: this->MakeDate( gdi, info ); break;
				case ctrlFormlist: this->MakeFormlist( gdi, info ); break;
				case ctrlFilepick: this->MakeFilePick( gdi, info ); break;
				default: break;
			}
		}
		udInt x = 0, y = 0, w = 0, h = 0, m = 8;
		udDWord style = 0;
		wstring title = L"";

		x = 32;
		y = this->get( "height" )->toInt( ) - 42;
		w = 103;
		h = 37;
		title = L"Ок";
		style = WS_CHILD | WS_TABSTOP;
		m_btnOk->get( "title" )->ini( title );
		m_btnOk->get( "style" )->ini( style );
		if ( !m_btnOk->isset( "x" ) ) {
			m_btnOk->get( "x" )->ini( x );
		}
		if ( !m_btnOk->isset( "y" ) ) {
			m_btnOk->get( "y" )->ini( y );
		}
		m_btnOk->get( "width" )->ini( w );
		m_btnOk->get( "height" )->ini( h );
		m_btnOk->SetFontSize( 14 );
		m_btnOk->SetPaddingLeft( 40 );
		m_btnOk->SetPaddingTop( 10 );
		m_btnOk->SetRectNormal( new Rect( 1746, 108, w, h ) );
		m_btnOk->SetTargetWnd( m_hWnd );
		m_btnOk->Create( m_hWnd, ( HMENU ) 1001 );//( HMENU ) IDOK );

		x += w + m;
		title = L"Отмена";
		m_btnCancel->get( "title" )->ini( title );
		m_btnCancel->get( "style" )->ini( style );
		if ( !m_btnCancel->isset( "x" ) ) {
			m_btnCancel->get( "x" )->ini( x );
		}
		if ( !m_btnCancel->isset( "y" ) ) {
			m_btnCancel->get( "y" )->ini( y );
		}
		m_btnCancel->get( "width" )->ini( w );
		m_btnCancel->get( "height" )->ini( h );
		m_btnCancel->SetFontSize( 14 );
		m_btnCancel->SetPaddingLeft( 40 );
		m_btnCancel->SetPaddingTop( 10 );
		m_btnCancel->SetRectNormal( new Rect( 1746, 161, w, h ) );
		m_btnCancel->SetTargetWnd( m_hWnd );
		m_btnCancel->Create( m_hWnd, ( HMENU ) 1002 );//( HMENU ) IDCANCEL );
	} // void MakeControls

	void CWndModeForm::FillControls( ) {
		assert( m_pObject );
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
				case ctrlFilepick: this->FillFilePick( info ); break;
				default: break;
			}
		}
	} // void FillControls

	void CWndModeForm::ShowControls( ) {
		assert( m_fnGetObject );
		if ( !m_pObject ) {
			m_pObject = m_fnGetObject( );
		}
		//assert( m_pObject );
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
				case ctrlFilepick: info->filepick->Show( ); break;
				default: break;
			}
		}
	} // void ShowControls

	void CWndModeForm::Clear( ) {
		assert( m_fnGetObject );
		WriteUdUiLog( "CWndModeForm::Clear enter\n" );
		// пересоздаем объект
		if ( m_pObject ) {
			WriteUdUiLog( "CWndModeForm::Clear, m_pObject delete\n" );
			delete m_pObject;
			m_pObject = NULL;
		}
		m_pObject = m_fnGetObject( );
		// очищаем контролы (особенно списки сущностей)
		ModeFormControlInfo *info = NULL;
		WriteUdUiLog( "CWndModeForm::Clear, m_arrControl iterating\n" );
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			if ( info->type == ctrlExtlist ) {
				info->extlst->extlst->ClearSelection( );
			} else if ( info->type == ctrlFormlist ) {
				info->frmlst->Clear( );
			} else if ( info->type == ctrlFilepick ) {
				info->filepick->Clear( );
			}
		}
		WriteUdUiLog( "CWndModeForm::Clear exit\n" );
	} // void Clear

	void CWndModeForm::MakeInput( udPGdi gdi, ModeFormControlInfo* info ) {
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

		// область вывода текста
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;

		// создаем шрифт для текста
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		udGdi gdi2( m_hWnd );
		Status status = info->input->font->GetLogFontW( &gdi2, &objLogFont );
		if ( status != Ok ) {
			WriteUdUiLog( "CWndModeForm::MakeInput, failed getting font, status=" + toString( status ) + "\n" );
		}
		hFont = CreateFontIndirect( &objLogFont );
		info->input->hfont = hFont;

		if ( !m_bIsWatch ) {
			// создаем контрол
			info->input->hwnd = CreateWindowEx( 0, L"EDIT", L"", WS_STANDARD_EDIT | WS_TABSTOP | info->input->addstyle,
				x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
		}
		SendMessage( info->input->hwnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
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
			/*case WM_COMMAND:
				if ( LOWORD( wParam ) == VK_ESCAPE ) {
					//::PostMessage( m_hWndParent, WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), 0 );
					::SendMessage( GetParent( hWnd ), WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ), 0 );
					return TRUE;
				}
				break;*/
		}
		return DefSubclassProc( hWnd, uMsg, wParam, lParam );
	} // LRESULT udEditMultiline

	void CWndModeForm::MakeText( udPGdi gdi, ModeFormControlInfo* info ) {
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

		// область текста
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;

		// шрифт текста
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		udGdi gdi2( m_hWnd );
		Status status = info->textarea->font->GetLogFontW( &gdi2, &objLogFont );
		if ( status != Ok ) {
			WriteUdUiLog( "CWndModeForm::MakeText, failed getting font, status=" + toString( status ) + "\n" );
		}
		hFont = CreateFontIndirect( &objLogFont );
		info->textarea->hfont = hFont;

		if ( !m_bIsWatch ) {
			// создаем контрол
			info->textarea->hwnd = CreateWindowEx( 0, L"EDIT", L"",
				WS_STANDARD_EDIT_MULTILINE | WS_TABSTOP,
				x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
			SetWindowSubclass( info->textarea->hwnd, udEditMultiline, 0, 0 );
			SendMessage( info->textarea->hwnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );
		}
	} // void MakeText

	void CWndModeForm::MakeExtlist( udPGdi gdi, ModeFormControlInfo* info ) {
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

	void CWndModeForm::MakeSelect( udPGdi gdi, ModeFormControlInfo* info ) {
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

		// область текста
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;

		// шрифт текста
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		udGdi gdi2( m_hWnd );
		Status status = info->select->font->GetLogFontW( &gdi2, &objLogFont );
		if ( status != Ok ) {
			WriteUdUiLog( "CWndModeForm::MakeSelect, failed getting font, status=" + toString( status ) + "\n" );
		}

		hFont = CreateFontIndirect( &objLogFont );
		info->select->hfont = hFont;

		if ( !m_bIsWatch ) {
			// создаем контрол
			h = 500;
			info->select->hwnd = CreateWindowEx( 0, L"COMBOBOX", L"", WS_STANDARD_SELECT | WS_TABSTOP,// | CBS_OWNERDRAWFIXED,
				x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
			SendMessage( info->select->hwnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );

			for( udArrWString::iterator i = info->select->arr.begin( ); i != info->select->arr.end( ); ++i ) {
				SendMessage( info->select->hwnd, CB_ADDSTRING, 0, ( LPARAM ) ( *i ).data( ) );
			}

			ComboBox_SetMinVisible( info->select->hwnd, 10 );
		}
	} // void MakeSelect

	void CWndModeForm::MakeDate( udPGdi gdi, ModeFormControlInfo* info ) {
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

		// расположение
		x = box->x + box->border + box->padL;
		y = box->y + box->border + box->padT;
		w = box->w;
		h = box->h;

		// шрифт
		HFONT		hFont	= NULL;
		LOGFONT		objLogFont;

		udGdi gdi2( m_hWnd );
		Status status = info->date->font->GetLogFontW( &gdi2, &objLogFont );
		if ( status != Ok ) {
			WriteUdUiLog( "CWndModeForm::MakeSelect, failed getting font, status=" + toString( status ) + "\n" );
		}

		hFont = CreateFontIndirect( &objLogFont );
		info->date->hfont = hFont;

		if ( !m_bIsWatch ) {
			// создаем контрол
			info->date->hwnd = CreateWindowEx( 0, DATETIMEPICK_CLASS, L"", WS_CHILD | WS_TABSTOP,
				x, y, w, h, m_hWnd, ( HMENU ) info->id, g_pApp->GetHInstance( ), NULL );
			// настраиваем его
			
			DateTime_SetMonthCalFont( info->date->hwnd, hFont, TRUE );
			udWChar frmt[ 256 ];
			swprintf_s( frmt, 256, L"dd'.'MM'.'yyyy" );
			DateTime_SetFormat( info->date->hwnd, frmt );
		}
	} // void MakeDate

	void CWndModeForm::MakeFormlist( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udPWndFormList frmlst = info->frmlst;
		udInt x = 0, y = 0, w = 0, h = 0;
		ModeFormControlBox* box = info->box;
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

	void CWndModeForm::MakeFilePick( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udPWndFilePick filepick = info->filepick;
		filepick->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_TABSTOP ) );
		filepick->get( "x" )->ini( info->box->x );
		filepick->get( "y" )->ini( info->box->y );
		filepick->get( "width" )->ini( info->box->w );
		filepick->get( "height" )->ini( info->box->h );
		if ( m_bIsModal ) {
			filepick->SetDialogParent( m_hWndParent );
		} else {
			filepick->SetDialogParent( g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ) );
		}
		filepick->Create( m_hWnd, ( HMENU ) info->id );
	} // void MakeFilePick

	void CWndModeForm::MakeStatic( udPGdi gdi, ModeFormControlInfo* info ) {
		assert( m_hWnd );
		assert( info );
		assert( info->box );
		udPWndStatic staticw = info->staticw;
		staticw->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_TABSTOP ) );
		staticw->get( "x" )->ini( info->box->x );
		staticw->get( "y" )->ini( info->box->y );
		staticw->get( "width" )->ini( info->box->w );
		staticw->get( "height" )->ini( info->box->h );
		staticw->Create( m_hWnd, ( HMENU ) info->id );
	} // void MakeStatic

	void CWndModeForm::FillInput( ModeFormControlInfo* info ) {
		wstring title = m_pObject->get( info->attr )->toWString( );
		if ( m_bIsWatch ) {
			udPGdi gdi = this->GetGdi( );
			ModeFormControlBox* box = info->box;
			int x = box->x + box->border + box->padL,
				y = box->y + box->border + box->padT,
				w = box->w,
				h = box->h;
			SolidBrush	brhWhite( Color( 255, 255, 255 ) ),
						brhLabel( Color( 0, 0, 0 ) );
			// рисуем текст
			gdi->FillRectangle( &brhWhite, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ) );
			gdi->DrawString(
				title.data( ), title.size( ),
				info->input->font, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ),
				NULL, &brhLabel
			);
		} else {
			SetWindowText( info->input->hwnd, title.data( ) );
		}
	} // void FillInput

	void CWndModeForm::FillText( ModeFormControlInfo* info ) {
		wstring title = m_pObject->get( info->attr )->toWString( );
		if ( m_bIsWatch ) {
			udPGdi gdi = this->GetGdi( );
			ModeFormControlBox* box = info->box;
			int x = box->x + box->border + box->padL,
				y = box->y + box->border + box->padT,
				w = box->w,
				h = box->h;
			SolidBrush	brhWhite( Color( 255, 255, 255 ) ),
						brhLabel( Color( 0, 0, 0 ) );
			// рисуем текст
			gdi->FillRectangle( &brhWhite, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ) );
			gdi->DrawString(
				title.data( ), title.size( ),
				info->textarea->font, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ),
				NULL, &brhLabel
			);
		} else {
			SetWindowText( info->textarea->hwnd, title.data( ) );
		}
	} // void FillText

	void CWndModeForm::FillExtlst( ModeFormControlInfo* info ) {
		assert( m_pObject );
		assert( info->extlst );
		if ( m_bIsWatch ) {
			info->extlst->extlst->SetReadOnly( true );
		}
		/*udPBase obj = NULL;
		if ( info->extlst->auto_load ) {
			string scheme = g_pApp->GetConfig( )->get( "scheme" )->toString( );
			if ( scheme == "restricted" ) {
				udPHandler		handler = g_pApp->GetHandler( );
				pfnGetObject	fnGetObject = info->extlst->fnGetObject;
				udHandlerOption	option;
				string			szWhere	= "", table = info->extlst->table;
				udBaseConfig	config;
				udArrBase		arr;
				udArrError		err;

				obj = fnGetObject( );
				obj->GetConfig( config );
				delete obj;
				obj = NULL;
				szWhere = config.GetAttrIndex( "id" ) + "=" + m_pObject->get( info->attr )->toString( );
				option.SetTable( table );
				option.SetClass( fnGetObject );
				option.SetWhere( szWhere );
				handler->GetUdObject( arr, option, &err );
				if ( !arr.empty( ) ) {
					obj = arr[ 0 ];
					arr.clear( );
				}
				if ( !err.empty( ) ) {
					udPLog log = g_pApp->GetLog( );
					for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
						log->Write( i->GetText( ) + "\n" );
					}
					err.clear( );
				}
			} else if ( scheme == "center" ) {
				return;
			}
		} else {
			//obj = m_pStHandler->modftmExtlstLoad( this, info->extlst->extlst );
		}
		info->extlst->extlst->SetObj( obj );*/
	} // void FillExtlst

	void CWndModeForm::FillSelect( ModeFormControlInfo* info ) {
		udUInt val = m_pObject->get( info->attr )->toUInt( ), toSet = 0;
		if ( val > 0 ) {
			toSet = val - 1;
		}
		if ( m_bIsWatch ) {
			udUInt cnt = 0;
			wstring title;
			for( udArrWString::iterator i = info->select->arr.begin( ); i != info->select->arr.end( ); ++i ) {
				if ( cnt == toSet ) {
					title = *i;
					break;
				}
				++cnt;
			}
			udPGdi gdi = this->GetGdi( );
			ModeFormControlBox* box = info->box;
			int x = box->x + box->border + box->padL,
				y = box->y + box->border + box->padT,
				w = box->w,
				h = box->h;
			SolidBrush	brhWhite( Color( 255, 255, 255 ) ),
						brhLabel( Color( 0, 0, 0 ) );
			// рисуем текст
			gdi->FillRectangle( &brhWhite, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ) );
			gdi->DrawString(
				title.data( ), title.size( ),
				info->select->font, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ),
				NULL, &brhLabel
			);
		} else {
			SendMessage( info->select->hwnd, CB_SETCURSEL, ( WPARAM ) toSet, 0 );
		}
	} // void FillSelect

	void CWndModeForm::FillDate( ModeFormControlInfo* info ) {
		if ( m_bIsWatch ) {
			wstring title;

			if ( info->date->bString ) {
				title = toWString( m_pObject->get( info->attr )->toWString( ) );
			} else {
				title = toWString( m_pObject->get( info->attr )->toWString( ) );
				title = title.substr( 0, 10 );
			}
			
			udPGdi gdi = this->GetGdi( );
			ModeFormControlBox* box = info->box;
			int x = box->x + box->border + box->padL,
				y = box->y + box->border + box->padT,
				w = box->w,
				h = box->h;
			SolidBrush	brhWhite( Color( 255, 255, 255 ) ),
						brhLabel( Color( 0, 0, 0 ) );
			// рисуем текст
			gdi->FillRectangle( &brhWhite, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ) );
			gdi->DrawString(
				title.data( ), title.size( ),
				info->date->font, RectF( ( float ) x, ( float ) y, ( float ) w, ( float ) h ),
				NULL, &brhLabel
			);
		} else {
			PSYSTEMTIME	time = NULL;
			if ( info->date->bString ) {
				string	date = m_pObject->get( info->attr )->toString( ),
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
				datetime ddate = m_pObject->get( info->attr )->toTime( );
				if ( ddate.get( ) == 0 ) {
					ddate.load( );
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
				}
			}
			if ( time ) {
				DateTime_SetSystemtime( info->date->hwnd, GDT_VALID, time );
			}
			udDELETE( time );
		}
	} // void FillDate

	void CWndModeForm::FillFormlist( ModeFormControlInfo* info ) {
		//assert( m_pStHandler );
		//m_pStHandler->modfrmFormListLoad( info->frmlst );
	} // void FillFormlist

	void CWndModeForm::FillFilePick( ModeFormControlInfo* info ) {
	} // void FillFilePick

	ModeFormControlInfo* CWndModeForm::GetInfo( udInt iIndex ) {
		_int_asc_control::iterator i = m_arrControl.find( iIndex );
		if ( i != m_arrControl.end( ) ) {
			return i->second;
		}
		return NULL;
	} // ModeFormControlInfo* GetInfo

	void CWndModeForm::LoadControlData( ) {
		assert( m_pObject );
		ModeFormControlInfo *info = NULL;
		for( _int_asc_control::iterator i = m_arrControl.begin( ); i != m_arrControl.end( ); ++i ) {
			info = i->second;
			switch( info->type ) {
				case ctrlInput: this->LoadInput( info ); break;
				case ctrlText: this->LoadText( info ); break;
				case ctrlExtlist: this->LoadExtlst( info ); break;
				case ctrlSelect: this->LoadSelect( info ); break;
				case ctrlDate: this->LoadDate( info ); break;
				case ctrlFormlist: this->LoadFormlist( info ); break;
				case ctrlFilepick: this->LoadFilePick( info ); break;
				default: break;
			}
		}
	} // void LoadControlData

	void CWndModeForm::LoadInput( ModeFormControlInfo* info ) {
		udWChar tmp[ 256 ];
		GetWindowText( info->input->hwnd, tmp, 256 );
		m_pObject->get( info->attr )->ini( wstring( tmp ) );
	} // void LoadInput

	void CWndModeForm::LoadText( ModeFormControlInfo* info ) {
		udWChar tmp[ 1024 ];
		GetWindowText( info->textarea->hwnd, tmp, 1024 );
		m_pObject->get( info->attr )->ini( wstring( tmp ) );
	} // void LoadText

	void CWndModeForm::LoadExtlst( ModeFormControlInfo *info ) {
		assert( m_pObject );
		udPBase obj = info->extlst->extlst->GetObj( );
		if ( obj ) {
			m_pObject->get( info->attr )->ini( obj->get( "id" )->toUInt( ) );
		}
	} // void LoadExtlst

	void CWndModeForm::LoadSelect( ModeFormControlInfo* info ) {
		udUInt val = SendMessage( info->select->hwnd, CB_GETCURSEL, 0, 0 );
		m_pObject->get( info->attr )->ini( val + 1 );
	} // void LoadSelect

	void CWndModeForm::LoadDate( ModeFormControlInfo* info ) {
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
			m_pObject->get( info->attr )->ini( date2 );
		} else {
			m_pObject->get( info->attr )->ini( udDatetime( date2 ) );
		}
	} // void LoadDate

	void CWndModeForm::LoadFormlist( ModeFormControlInfo* info ) {
	} // void LoadFormlist

	void CWndModeForm::LoadFilePick( ModeFormControlInfo* info ) {
		m_pObject->get( info->attr )->ini( info->filepick->GetFile( ) );
	} // void LoadFilePick

	void CWndModeForm::SetIsWatch( bool bIsWatch ) {
		// TODO: сделать смену режима
		// при смене с просмотра на редактирование: затереть надписи, создать элементы
		// при смене с редактирования на просмотр: скрыть элементы, нарисовать надписи
		m_bIsWatch = bIsWatch;
	} // void SetIsWatch

} // namespace UdSDK