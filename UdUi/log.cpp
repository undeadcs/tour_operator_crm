
#include "includes.h"

namespace UdSDK {

	static udPStdFile __log = NULL;

	udInt OpenUdUiLog( string path, const char* mode ) {
		//if ( !__log && ( fopen_s( &__log, path.data( ), mode ) == 0 ) ) {
		if ( !__log && ( __log = _fsopen( path.data( ), mode, _SH_DENYNO ) ) ) {
			return 1;
		}
		return 0;
	} // OpenUdUiLog

	udInt CloseUdUiLog( ) {
		if ( __log ) {
			fclose( __log );
			return 1;
		}
		return 0;
	} // void CloseUdUiLog

	udInt WriteUdUiLog( string log ) {
		if ( __log ) {
			size_t	iSize	= log.size( ),
					iResult	= fwrite( log.data( ), 1, iSize, __log );
			if ( iResult != iSize ) {
				return 0;
			}

			fflush( __log );
		}
		return 0;
	} // void WriteUdUiLog

	void UdUiLogMessage( UINT msg ) {
		string log;
		switch( msg ) {
			case WM_NCCREATE:
				log = "WM_NCCREATE\n";
				break;

			case WM_CREATE:
				log = "WM_CREATE\n";
				break;

			case WM_PAINT:
				log = "WM_PAINT\n";
				break;

			case WM_ERASEBKGND:
				log = "WM_ERASEBKGND\n";
				break;

			case WM_NCPAINT:
				log = "WM_NCPAINT\n";
				break;

			case WM_SYNCPAINT:
				log = "WM_SYNCPAINT\n";
				break;

			case WM_SETCURSOR:
				log = "WM_SETCURSOR\n";
				break;

			case WM_ACTIVATEAPP:
				log = "WM_ACTIVATEAPP\n";
				break;

			case WM_NCCALCSIZE:
				log = "WM_NCCALCSIZE\n";
				break;

			case WM_NCHITTEST:
				log = "WM_NCHITTEST\n";
				break;
				
			case WM_MOUSEMOVE:
				log = "WM_MOUSEMOVE\n";
				break;

			case WM_MOVE:
				log = "WM_MOVE\n";
				break;

			case WM_SIZE:
				log = "WM_SIZE\n";
				break;

			case WM_DESTROY:
				log = "WM_DESTROY\n";
				break;

			case WM_ACTIVATE:
				log = "WM_ACTIVATE\n";
				break;

			case WM_NCDESTROY:
				log = "WM_NCDESTROY\n";
				break;

			case WM_SHOWWINDOW:
				log = "WM_SHOWWINDOW\n";
				break;

			case WM_WINDOWPOSCHANGING:
				log = "WM_WINDOWPOSCHANGING\n";
				break;

			case WM_GETICON:
				log = "WM_GETICON\n";
				break;

			case WM_NCACTIVATE:
				log = "WM_NCACTIVATE\n";
				break;

			case WM_WINDOWPOSCHANGED:
				log = "WM_WINDOWPOSCHANGED\n";
				break;

			case WM_KILLFOCUS:
				log = "WM_KILLFOCUS\n";
				break;

			case WM_IME_SETCONTEXT:
				log = "WM_IME_SETCONTEXT\n";
				break;

			case WM_IME_NOTIFY:
				log = "WM_IME_NOTIFY\n";
				break;

			default: {
				udChar tmp[ 256 ];
				sprintf_s( tmp, 256, "%u, 0x%04x\n", msg, msg );
				log = tmp;
			} break;
		}
		WriteUdUiLog( log );
	} // void UdUiLogMessage

} // namespace UdSDK