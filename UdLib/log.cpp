
#include "includes.h"

namespace UdSDK {

	static udPStdFile __log = NULL;

	udInt OpenUdLibLog( string path, const char* mode ) {
		//if ( !__log && ( fopen_s( &__log, path.data( ), mode ) == 0 ) ) {
		if ( !__log && ( __log = _fsopen( path.data( ), mode, _SH_DENYNO ) ) ) {
			return 1;
		}
		return 0;
	} // OpenUdLibLog

	udInt CloseUdLibLog( ) {
		if ( __log ) {
			fclose( __log );
			return 1;
		}
		return 0;
	} // void CloseUdLibLog

	udInt WriteUdLibLog( string log ) {
		if ( __log ) {
			size_t	iSize	= log.size( ),
					iResult	= fwrite( log.data( ), 1, iSize, __log );
			if ( iResult != iSize ) {
				return 0;
			}

			fflush( __log );
		}
		return 0;
	} // void WriteUdLibLog

} // namespace UdSDK