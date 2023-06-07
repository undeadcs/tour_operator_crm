
#include "includes.h"

namespace UdSDK {

	static udPStdFile __log = NULL;

	udInt OpenUdBaseLog( string path, const char* mode ) {
		//if ( !__log && ( fopen_s( &__log, path.data( ), mode ) == 0 ) ) {
		if ( !__log && ( __log = _fsopen( path.data( ), mode, _SH_DENYNO ) ) ) {
			return 1;
		}
		return 0;
	} // OpenUdBaseLog

	udInt CloseUdBaseLog( ) {
		if ( __log ) {
			fclose( __log );
			return 1;
		}
		return 0;
	} // void CloseUdBaseLog

	udInt WriteUdBaseLog( string log ) {
		if ( __log ) {
			size_t	iSize	= log.size( ),
					iResult	= fwrite( log.data( ), 1, iSize, __log );
			if ( iResult != iSize ) {
				return 0;
			}

			fflush( __log );
		}
		return 0;
	} // void WriteUdBaseLog

} // namespace UdSDK