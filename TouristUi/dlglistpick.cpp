
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	LRESULT MakeDialog( HWND hWndParent, DLGPROC dlgProc, LPARAM lParam ) {
		HGLOBAL hgbl;
		LPDLGTEMPLATE lpdt;
		LPWORD lpw;

		hgbl = GlobalAlloc( GMEM_ZEROINIT, 1024 );
		if ( !hgbl ) {
			return -1;
		}

		char tmp[ 256 ];
		sprintf_s( tmp, 256, "адрес блока памяти: %p\n", hgbl );
		WriteUdUiLog( tmp );

		lpdt = ( LPDLGTEMPLATE ) GlobalLock( hgbl );
		lpdt->style = WS_POPUP | WS_SYSMENU | WS_CAPTION;
		lpdt->cdit = 0;
		lpdt->x  = 0;
		lpdt->y  = 0;
		lpdt->cx = 0;
		lpdt->cy = 0;
		lpw = ( LPWORD )( lpdt + 1 );
		*lpw++ = 0; // без меню
		*lpw++ = 0; // класс диалогового окна по умолчанию
		*lpw++ = 0; // нет заголовка
	 
		GlobalUnlock( hgbl );
		LRESULT ret = DialogBoxIndirectParam( g_pApp->GetHInstance( ), ( LPDLGTEMPLATE ) hgbl, hWndParent, dlgProc, lParam );
		GlobalFree( hgbl );
		return ret;
	} // LRESULT MakeDialog

	static BOOL CALLBACK dlgListPickProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		static map< HWND, udPWndModeList > mapList;

		switch( uMsg ) {
			case WM_INITDIALOG: {
				udPListHandler handler = ( udPListHandler ) lParam;
				handler->CreateList( hWnd, true );
				handler->LoadList( );
				handler->GetWndList( )->Show( );
				mapList[ hWnd ] = handler->GetWndList( );
				return TRUE;
			} break;

			case WM_COMMAND: {
				switch( LOWORD( wParam ) ) {
					case IDOK: {
						// если через сообщение пересылать указатель на handler, то почему-то теряется
						// у него объект списка - мистика
						udPWndModeList wndList = ( udPWndModeList ) lParam;
						if ( !wndList ) { // была нажата клавиша Enter
							wndList = mapList[ hWnd ];
						}
						assert( wndList );
						udLong index = wndList->GetList( )->GetSelected( );
						if ( index > -1 ) {
							udPBase obj = wndList->GetHandler( )->GetItems( )->at( index );
							if ( obj ) {
								EndDialog( hWnd, ( INT_PTR ) obj );
							} else {
								mb::err( hWnd, L"произошла ошибка при выборе объекта" );
							}
						} else {
							mb::err( hWnd, L"выберите элемент из списка" );
						}
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
	} // BOOL dlgListPickProc

	udPMorph DialogListPick( HWND hWndParent, udPListHandler handler ) {
		udPMorph ret = NULL;
		LRESULT res = MakeDialog( hWndParent, ( DLGPROC ) dlgListPickProc, ( LPARAM ) handler );
		if ( res > 0 ) {
			udPBase obj = ( udPBase ) res;
			ret = obj->clone( );
		}
		return ret;
	} // udPMorph DialogListPick

	struct DialogFormData {
		udPFormHandler handler;
		udPBase obj;
	};

	static BOOL CALLBACK dlgForm( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
		static map< HWND, udPWndModeForm > mapForm;

		switch( uMsg ) {
			case WM_INITDIALOG: {
				DialogFormData *data = ( DialogFormData* ) lParam;
				udPFormHandler handler = data->handler;
				if ( handler->CreateForm( hWnd, true ) != udFormHandler::FormHandlerErrorOk ) {
					mb::err( L"Ошибка при создании формы" );
					EndDialog( hWnd, 0 );
					return TRUE;
				}
				udPWndModeForm wndForm = handler->GetWndForm( );
				int h = wndForm->get( "height" )->toInt( ) + GetSystemMetrics( SM_CYCAPTION );
				wndForm->get( "height" )->ini( h );
				wndForm->Show( );
				wndForm->SetObj( data->obj );
				handler->LoadAfterSetObject( );
				SetWindowPos( hWnd, NULL, 0, 0,
					wndForm->get( "width" )->toInt( ),
					wndForm->get( "height" )->toInt( ),
					SWP_NOMOVE
				);
				CenterOneWindowAboveParent( hWnd );
				mapForm[ hWnd ] = wndForm;
				return TRUE;
			} break;

			case WM_COMMAND: {
				switch( LOWORD( wParam ) ) {
					case IDOK: {
						udPWndModeForm form = ( udPWndModeForm ) lParam;
						if ( !form ) {
							form = mapForm[ hWnd ];
						}
						assert( form );
						udPFormHandler handler = form->GetHandler( );
						assert( handler );
						if ( handler->ProcForm( ) == udFormHandler::FormHandlerErrorOk ) {
							udPBase obj = form->GetObj( );
							EndDialog( hWnd, ( INT_PTR ) obj );
						}
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
	} // BOOL dlgForm

	udPMorph DialogForm( HWND hWndParent, udPFormHandler handler, udPBase obj ) {
		DialogFormData *data = new DialogFormData;
		data->handler = handler;
		data->obj = obj;
		udPMorph ret = NULL;
		LRESULT res = MakeDialog( hWndParent, ( DLGPROC ) dlgForm, ( LPARAM ) data );
		if ( res > 0 ) {
			udPBase obj = ( udPBase ) res;
			ret = obj->clone( );
		}
		udDELETE( data );
		return ret;
	} // udPMorph DialogForm

} // namespace UdSDK
