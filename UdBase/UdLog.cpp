
#include "includes.h"

namespace UdSDK {

	CUdLog::CUdLog( ) {
		m_szPath	= "";
		m_pFile		= NULL;
	}

	CUdLog::CUdLog( const CUdLog& objCopy ) {
	}

	CUdLog::~CUdLog( ) {
		m_szPath.clear( );

		if ( m_pFile ) {
			fclose( m_pFile );
			m_pFile = NULL;
		}
	}

	CUdLog::LogError CUdLog::Open( const string& szPath, const char* mode ) {
		if ( szPath.empty( ) ) {
			return CUdLog::UDERROR_INVALID_ARG;
		}
#ifdef MSVC
		m_pFile = _fsopen( szPath.c_str( ), mode, _SH_DENYNO );
		/*if ( fopen_s( &m_pFile, szPath.c_str( ), mode ) ) {
			return CUdLog::UDERROR_CANT_OPEN_LOG;
		}*/
#else
		m_pFile = fopen( szPath.c_str( ), mode );
#endif
		if ( m_pFile ) {
			m_szPath = szPath;
			return CUdLog::UDERROR_OK;
		}

		return CUdLog::UDERROR_CANT_OPEN_LOG;
	} // LogError Open

	CUdLog::LogError CUdLog::Close( ) {
		if ( m_pFile ) {
			fclose( m_pFile );
			m_pFile = NULL;
			m_szPath.clear( );
			return CUdLog::UDERROR_OK;
		}

		return CUdLog::UDERROR_NOT_OPENED;
	} // LogErorr Close

	CUdLog::LogError CUdLog::Write( const string& szText, udDWord dwShowTime ) {
		if ( szText.empty( ) ) {
			return CUdLog::UDERROR_INVALID_ARG;
		}
		if ( !m_pFile ) {
			return CUdLog::UDERROR_NOT_OPENED;
		}

		if ( dwShowTime ) {
			time_t		t;
			time( &t );
#ifdef MSVC
			struct tm	objTime;
			localtime_s( &objTime, &t );
#else
			struct tm* pTime = NULL;
			pTime = localtime( &t );
#endif
			char buf[ 30 ];
			strftime( buf, 30, "[%Y.%m.%d %H:%M:%S] ", &objTime );
			fwrite( buf, 1, strlen( buf ), m_pFile );
		}

		size_t	iSize	= szText.size( ),
				iResult	= fwrite( szText.c_str( ), 1, iSize, m_pFile );
		if ( iResult != iSize ) {
			return CUdLog::UDERROR_WRITE_LOG_FAILED;
		}

		fflush( m_pFile );

		return CUdLog::UDERROR_OK;
	} // LogError Write

} // namespace UdSDK

