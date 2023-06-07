
#include "includes.h"

namespace UdSDK {

	wstring ConvertString( const string& szString, UINT iCodePage ) {
		wstring szRet( L"" );
		if ( szString.size( ) ) {
			udInt iRequired = MultiByteToWideChar( iCodePage, 0, szString.c_str( ), -1, NULL, 0 );
			if ( iRequired == 0 ) {
				WriteUdLibLog( "MultiByteToWideChar failed getting required length\n" );
			} else {
				udPWChar	pOut	= NULL;
				udInt		iLen	= iRequired + 1;

				pOut = new udWChar[ iRequired + 1 ];

				udInt iResult = MultiByteToWideChar( iCodePage, 0, szString.c_str( ), -1, pOut, iRequired + 1 );
				if ( iResult == 0 ) {
					WriteUdLibLog( "MultiByteToWideChar failed converting\n" );
					udDWord dwErr = GetLastError( );

					switch( dwErr ) {
						case ERROR_INSUFFICIENT_BUFFER:
							WriteUdLibLog( "ERROR_INSUFFICIENT_BUFFER\n" );
							break;

						case ERROR_INVALID_FLAGS:
							WriteUdLibLog( "ERROR_INVALID_FLAGS\n" );
							break;

						case ERROR_INVALID_PARAMETER:
							WriteUdLibLog( "ERROR_INVALID_PARAMETER\n" );
							break;

						case ERROR_NO_UNICODE_TRANSLATION:
							WriteUdLibLog( "ERROR_NO_UNICODE_TRANSLATION\n" );
							break;

						default: {
							udChar log[ 256 ];
							sprintf_s( log, 256, "error code: %u\n", dwErr );
							WriteUdLibLog( log );
						} break;
					}
				} else {
					szRet = pOut;
				}

				delete pOut;
			}
		}
		return szRet;
	} // wstring ConvertString

	string ConvertString( const wstring& szString, UINT iCodePage ) {
		string szRet( "" );
		if ( szString.size( ) ) {
			udInt iRequired = WideCharToMultiByte( iCodePage, 0, szString.c_str( ), -1, NULL, 0, NULL, NULL );
			if ( iRequired == 0 ) {
				WriteUdLibLog( "WideCharToMultiByte failed getting required length\n" );
			} else {
				udPChar	pOut	= NULL;
				udInt	iLen	= iRequired;

				pOut = new udChar[ iRequired ];

				udInt iResult = WideCharToMultiByte( iCodePage, 0, szString.c_str( ), -1, pOut, iRequired, NULL, NULL );
				if ( iResult == 0 ) {
					WriteUdLibLog( "MultiByteToWideChar failed converting\n" );
					udDWord dwErr = GetLastError( );
					udChar	log[ 1024 ];

					switch( dwErr ) {
						case ERROR_INSUFFICIENT_BUFFER:
							WriteUdLibLog( "ERROR_INSUFFICIENT_BUFFER\n" );
							break;

						case ERROR_INVALID_FLAGS:
							WriteUdLibLog( "ERROR_INVALID_FLAGS\n" );
							break;

						case ERROR_INVALID_PARAMETER:
							WriteUdLibLog( "ERROR_INVALID_PARAMETER\n" );
							break;

						case ERROR_NO_UNICODE_TRANSLATION:
							WriteUdLibLog( "ERROR_NO_UNICODE_TRANSLATION\n" );
							break;

						default:
							sprintf_s( log, 1024, "error code: %u\n", dwErr );
							WriteUdLibLog( log );
							break;
					}
				} else {
					szRet = pOut;
				}

				delete pOut;
			}
		}
		return szRet;
	} // string ConvertString

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udInt
	udInt toInt( udInt v ) {
		return v;
	} // udInt toInt

	udFloat toFloat( udInt v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udInt v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udInt v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udInt v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udInt v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udInt v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udInt v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udInt v ) {
		return ( udLongLong ) v;
	} // udLongLong toLLong

	udLong toLong( udInt v ) {
		return ( udLong ) v;
	} // udLong toLong

	udQWord toQWord( udInt v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udUInt
	udInt toInt( udUInt v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udUInt v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udUInt v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udUInt v ) {
		return v;
	} // udUInt toUInt

	udWord toWord( udUInt v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udUInt v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udUInt v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udUInt v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udUInt v ) {
		return ( udLongLong ) v;
	} // udLongLong toLLong

	udLong toLong( udUInt v ) {
		return ( udLong ) v;
	} // udLong toLong

	udQWord toQWord( udUInt v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udLong
	udInt toInt( udLong v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udLong v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udLong v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udLong v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udLong v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udLong v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udLong v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udLong v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udLong v ) {
		return ( udLongLong ) v;
	} // udLongLong toLLong

	udLong toLong( udLong v ) {
		return v;
	} // udLong toLong

	udQWord toQWord( udLong v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udDWord
	udInt toInt( udDWord v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udDWord v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udDWord v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udDWord v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udDWord v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udDWord v ) {
		return v;
	} // udDWord toDWord

	string toString( udDWord v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udDWord v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udDWord v ) {
		return ( udLongLong ) v;
	} // udLongLong toLLong

	udLong toLong( udDWord v ) {
		return ( udLong )v;
	} // udLong toLong

	udQWord toQWord( udDWord v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udLongLong
	udInt toInt( udLongLong v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udLongLong v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udLongLong v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udLongLong v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udLongLong v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udLongLong v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udLongLong v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udLongLong v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udLongLong v ) {
		return v;
	} // udLongLong toLLong

	udLong toLong( udLongLong v ) {
		return ( udLong )v;
	} // udLong toLong

	udQWord toQWord( udLongLong v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udQWord
	udInt toInt( udQWord v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udQWord v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udQWord v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udQWord v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udQWord v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udQWord v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udQWord v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udQWord v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udQWord v ) {
		return ( udQWord ) v;
	} // udLongLong toLLong

	udLong toLong( udQWord v ) {
		return ( udLong )v;
	} // udLong toLong

	udQWord toQWord( udQWord v ) {
		return v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udFloat
	udInt toInt( udFloat v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udFloat v ) {
		return v;
	} // udFloat toFloat

	udDouble toDouble( udFloat v ) {
		return ( udDouble ) v;
	} // udDouble toDouble

	udUInt toUInt( udFloat v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udFloat v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udFloat v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udFloat v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udFloat v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udFloat v ) {
		return ( udQWord ) v;
	} // udLongLong toLLong

	udLong toLong( udFloat v ) {
		return ( udLong )v;
	} // udLong toLong

	udQWord toQWord( udFloat v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// udDouble
	udInt toInt( udDouble v ) {
		return ( udInt ) v;
	} // udInt toInt

	udFloat toFloat( udDouble v ) {
		return ( udFloat ) v;
	} // udFloat toFloat

	udDouble toDouble( udDouble v ) {
		return v;
	} // udDouble toDouble

	udUInt toUInt( udDouble v ) {
		return ( udUInt ) v;
	} // udUInt toUInt

	udWord toWord( udDouble v ) {
		return ( udWord ) v;
	} // udWord toWord

	udDWord toDWord( udDouble v ) {
		return ( udDWord ) v;
	} // udDWord toDWord

	string toString( udDouble v, const char* szFormat ) {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, szFormat, v );
		return string( tmp );
	} // string toString

	wstring toWString( udDouble v, const wchar_t* szFormat ) {
		udWChar tmp[ 128 ];
		swprintf_s( tmp, 128, szFormat, v );
		return wstring( tmp );
	} // wstring toWString

	udLongLong toLLong( udDouble v ) {
		return ( udQWord ) v;
	} // udLongLong toLLong

	udLong toLong( udDouble v ) {
		return ( udLong )v;
	} // udLong toLong

	udQWord toQWord( udDouble v ) {
		return ( udQWord ) v;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// string
	udInt toInt( const string& v ) {
		udInt ret = 0;
		sscanf_s( v.data( ), "%d", &ret );
		return ret;
	} // udInt toInt

	udFloat toFloat( const string& v ) {
		udFloat ret = 0;
		sscanf_s( v.data( ), "%f", &ret );
		return ret;
	} // udFloat toFloat

	udDouble toDouble( const string& v ) {
		udDouble ret = 0;
		sscanf_s( v.data( ), "%lf", &ret );
		return ret;
	} // udDouble toDouble

	udUInt toUInt( const string& v ) {
		udUInt ret = 0;
		sscanf_s( v.data( ), "%u", &ret );
		return ret;
	} // udUInt toUInt

	udWord toWord( const string& v ) {
		udWord ret = 0;
		sscanf_s( v.data( ), "%hd", &ret );
		return ret;
	} // udWord toWord

	udDWord toDWord( const string& v ) {
		udDWord ret = 0;
		sscanf_s( v.data( ), "%lu", &ret );
		return ret;
	} // udDWord toDWord

	string toString( const string& v ) {
		return v;
	} // string toString

	wstring toWString( const string& v ) {
		return ConvertString( v );
		/*size_t len = v.size( );
		udPWChar tmp = new udWChar[ len ];
		swprintf_s( tmp, len, L"%s", v.data( ) );
		wstring ret( tmp );
		delete tmp;
		return ret;*/
	} // wstring toWString

	udLongLong toLLong( const string& v ) {
		udLongLong ret = 0;
		sscanf_s( v.data( ), "%I64d", &ret );
		return ret;
	} // udLongLong toLLong

	udLong toLong( const string& v ) {
		udLong ret = 0;
		sscanf_s( v.data( ), "%ld", &ret );
		return ret;
	} // udLong toLong

	udQWord toQWord( const string& v ) {
		udQWord ret = 0;
		sscanf_s( v.data( ), "%I64u", &ret );
		return ret;
	} // udQWord toQWord

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// wstring
	udInt toInt( const wstring& v ) {
		udInt ret = 0;
		swscanf_s( v.data( ), L"%d", &ret );
		return ret;
	} // udInt toInt

	udFloat toFloat( const wstring& v ) {
		udFloat ret = 0;
		swscanf_s( v.data( ), L"%f", &ret );
		return ret;
	} // udFloat toFloat

	udDouble toDouble( const wstring& v ) {
		udDouble ret = 0;
		swscanf_s( v.data( ), L"%lf", &ret );
		return ret;
	} // udDouble toDouble

	udUInt toUInt( const wstring& v ) {
		udUInt ret = 0;
		swscanf_s( v.data( ), L"%u", &ret );
		return ret;
	} // udUInt toUInt

	udWord toWord( const wstring& v ) {
		udWord ret = 0;
		swscanf_s( v.data( ), L"%hd", &ret );
		return ret;
	} // udWord toWord

	udDWord toDWord( const wstring& v ) {
		udDWord ret = 0;
		swscanf_s( v.data( ), L"%lu", &ret );
		return ret;
	} // udDWord toDWord

	string toString( const wstring& v ) {
		return ConvertString( v );
	} // string toString

	wstring toWString( const wstring& v ) {
		return v;
	} // wstring toWString

	udLongLong toLLong( const wstring& v ) {
		udLongLong ret = 0;
		swscanf_s( v.data( ), L"%I64d", &ret );
		return ret;
	} // udLongLong toLLong

	udLong toLong( const wstring& v ) {
		udLong ret = 0;
		swscanf_s( v.data( ), L"%ld", &ret );
		return ret;
	} // udLong toLong

	udQWord toQWord( const wstring& v ) {
		udQWord ret = 0;
		swscanf_s( v.data( ), L"%I64u", &ret );
		return ret;
	} // udQWord toQWord

} // namespace UdSDK
