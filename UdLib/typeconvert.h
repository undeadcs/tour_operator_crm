/**
	UndeadCS SDK
	Базовая библиотека

	набор функций для получения значений одного типа в другом
*/
#ifndef UDNEADCS_SDK_LIBRARY_TYPECONVERT_H
#define UDNEADCS_SDK_LIBRARY_TYPECONVERT_H

namespace UdSDK {

	wstring	ConvertString( const string& szString, UINT iCodePage = CP_ACP );
	string	ConvertString( const wstring& szString, UINT iCodePage = CP_ACP );

	// udInt
	udInt		toInt( udInt v );
	udFloat		toFloat( udInt v );
	udDouble	toDouble( udInt v );
	udUInt		toUInt( udInt v );
	udWord		toWord( udInt v );
	udDWord		toDWord( udInt v );
	string		toString( udInt v, const char* szFormat = "%d" );
	wstring		toWString( udInt v, const wchar_t* szFormat = L"%d" );
	udLongLong	toLLong( udInt v );
	udLong		toLong( udInt v );
	udQWord		toQWord( udInt v );

	// udUInt
	udInt		toInt( udUInt v );
	udFloat		toFloat( udUInt v );
	udDouble	toDouble( udUInt v );
	udUInt		toUInt( udUInt v );
	udWord		toWord( udUInt v );
	udDWord		toDWord( udUInt v );
	string		toString( udUInt v, const char* szFormat = "%u" );
	wstring		toWString( udUInt v, const wchar_t* szFormat = L"%u" );
	udLongLong	toLLong( udUInt v );
	udLong		toLong( udUInt v );
	udQWord		toQWord( udUInt v );

	// udLong
	udInt		toInt( udLong v );
	udFloat		toFloat( udLong v );
	udDouble	toDouble( udLong v );
	udUInt		toUInt( udLong v );
	udWord		toWord( udLong v );
	udDWord		toDWord( udLong v );
	string		toString( udLong v, const char* szFormat = "%ld" );
	wstring		toWString( udLong v, const wchar_t* szFormat = L"%ld" );
	udLongLong	toLLong( udLong v );
	udLong		toLong( udLong v );
	udQWord		toQWord( udLong v );

	// udDWord
	udInt		toInt( udDWord v );
	udFloat		toFloat( udDWord v );
	udDouble	toDouble( udDWord v );
	udUInt		toUInt( udDWord v );
	udWord		toWord( udDWord v );
	udDWord		toDWord( udDWord v );
	string		toString( udDWord v, const char* szFormat = "%lu" );
	wstring		toWString( udDWord v, const wchar_t* szFormat = L"%lu" );
	udLongLong	toLLong( udDWord v );
	udLong		toLong( udDWord v );
	udQWord		toQWord( udDWord v );

	// udLongLong
	udInt		toInt( udLongLong v );
	udFloat		toFloat( udLongLong v );
	udDouble	toDouble( udLongLong v );
	udUInt		toUInt( udLongLong v );
	udWord		toWord( udLongLong v );
	udDWord		toDWord( udLongLong v );
	string		toString( udLongLong v, const char* szFormat = "%lld" );
	wstring		toWString( udLongLong v, const wchar_t* szFormat = L"%lld" );
	udLongLong	toLLong( udLongLong v );
	udLong		toLong( udLongLong v );
	udQWord		toQWord( udLongLong v );

	// udQWord
	udInt		toInt( udQWord v );
	udFloat		toFloat( udQWord v );
	udDouble	toDouble( udQWord v );
	udUInt		toUInt( udQWord v );
	udWord		toWord( udQWord v );
	udDWord		toDWord( udQWord v );
	string		toString( udQWord v, const char* szFormat = "%llu" );
	wstring		toWString( udQWord v, const wchar_t* szFormat = L"%llu" );
	udLongLong	toLLong( udQWord v );
	udLong		toLong( udQWord v );
	udQWord		toQWord( udQWord v );

	// udFloat
	udInt		toInt( udFloat v );
	udFloat		toFloat( udFloat v );
	udDouble	toDouble( udFloat v );
	udUInt		toUInt( udFloat v );
	udWord		toWord( udFloat v );
	udDWord		toDWord( udFloat v );
	string		toString( udFloat v, const char* szFormat = "%f" );
	wstring		toWString( udFloat v, const wchar_t* szFormat = L"%f" );
	udLongLong	toLLong( udFloat v );
	udLong		toLong( udFloat v );
	udQWord		toQWord( udFloat v );

	// udDouble
	udInt		toInt( udDouble v );
	udFloat		toFloat( udDouble v );
	udDouble	toDouble( udDouble v );
	udUInt		toUInt( udDouble v );
	udWord		toWord( udDouble v );
	udDWord		toDWord( udDouble v );
	string		toString( udDouble v, const char* szFormat = "%f" );
	wstring		toWString( udDouble v, const wchar_t* szFormat = L"%f" );
	udLongLong	toLLong( udDouble v );
	udLong		toLong( udDouble v );
	udQWord		toQWord( udDouble v );

	// string
	udInt		toInt( const string& v );
	udFloat		toFloat( const string& v );
	udDouble	toDouble( const string& v );
	udUInt		toUInt( const string& v );
	udWord		toWord( const string& v );
	udDWord		toDWord( const string& v );
	string		toString( const string& v );
	wstring		toWString( const string& v );
	udLongLong	toLLong( const string& v );
	udLong		toLong( const string& v );
	udQWord		toQWord( const string& v );

	// wstring
	udInt		toInt( const wstring& v );
	udFloat		toFloat( const wstring& v );
	udDouble	toDouble( const wstring& v );
	udUInt		toUInt( const wstring& v );
	udWord		toWord( const wstring& v );
	udDWord		toDWord( const wstring& v );
	string		toString( const wstring& v );
	wstring		toWString( const wstring& v );
	udLongLong	toLLong( const wstring& v );
	udLong		toLong( const wstring& v );
	udQWord		toQWord( const wstring& v );

} // namespace UdSDK

#endif
