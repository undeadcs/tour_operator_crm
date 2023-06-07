
#include "includes.h"

namespace UdSDK {

	void DirectoryListing( const wstring& name, pfnListingForEachFile fnForEachFile, pfnListingForEachFolder fnListingForEachFolder ) {
		if ( name.empty( ) ) {
			return;
		}
		wstring path = name + L"\\*";
		WIN32_FIND_DATA data;
		ZeroMemory( &data, sizeof( WIN32_FIND_DATA ) );
		HANDLE handle = FindFirstFile( path.data( ), &data );
		if ( handle != INVALID_HANDLE_VALUE ) {
			wstring filename = data.cFileName;
			WriteUdLibLog( toString( filename ) + "\n" );
			if ( filename != L"." && filename != L".." ) {
				if ( fnForEachFile ) {
					filename = name + L"\\" + filename;
					fnForEachFile( filename );
				}
			}
			while( FindNextFile( handle, &data ) != 0 ) {
				filename = data.cFileName;
				WriteUdLibLog( toString( filename ) + "\n" );
				if ( filename != L"." && filename != L".." ) {
					filename = name + L"\\" + filename;
					if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
						if ( fnListingForEachFolder ) {
							fnListingForEachFolder( filename );
						}
					} else {
						if ( fnForEachFile ) {
							fnForEachFile( filename );
						}
					}
				}
			}
			FindClose( handle );
		} else {
			char tmp[ 256 ];
			sprintf_s( tmp, 256, "DirectoryListing, FindFirstFile error code=%lu\n", GetLastError( ) );
			WriteUdLibLog( tmp );
		}
	} // void DirectoryListing

	bool DirectoryClear( const wstring& name ) {
		if ( name.empty( ) ) {
			return false;
		}
		wstring path = name + L"\\*";
		WIN32_FIND_DATA data;
		ZeroMemory( &data, sizeof( WIN32_FIND_DATA ) );
		WriteUdLibLog( "DirectoryClear, FindFirstFile, '" + toString( path ) + "'\n" );
		HANDLE handle = FindFirstFile( path.data( ), &data );
		if ( handle != INVALID_HANDLE_VALUE ) {
			wstring filename = data.cFileName;
			if ( filename != L"." && filename != L".." ) {
				filename = name + L"\\" + filename;
				WriteUdLibLog( toString( filename ) + "\n" );
				if ( DeleteFile( path.data( ) ) == 0 ) {
					char tmp[ 256 ];
					sprintf_s( tmp, 256, "DirectoryClear, DeleteFile error code=%lu\n", GetLastError( ) );
					WriteUdLibLog( tmp );
					FindClose( handle );
					return false;
				}
			}
			while( FindNextFile( handle, &data ) != 0 ) {
				filename = data.cFileName;
				if ( filename != L"." && filename != L".." ) {
					filename = name + L"\\" + filename;
					WriteUdLibLog( toString( filename ) + "\n" );
					if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
					} else {
						if ( data.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) {
							SetFileAttributes( filename.data( ), FILE_ATTRIBUTE_NORMAL );
						}
						if ( DeleteFile( filename.data( ) ) == 0 ) {
							char tmp[ 256 ];
							sprintf_s( tmp, 256, "DirectoryClear, DeleteFile error code=%lu\n", GetLastError( ) );
							WriteUdLibLog( tmp );
							break;
						}
					}
				}
			}
			FindClose( handle );
		} else {
			char tmp[ 256 ];
			sprintf_s( tmp, 256, "DirectoryClear, FindFirstFile error code=%lu\n", GetLastError( ) );
			WriteUdLibLog( tmp );
		}
		return true;
	} // bool DirectoryClear

	void DirectoryCopyFiles( const wstring& src, const wstring& dst ) {
		WriteUdLibLog( "DirectoryCopyFiles enter\n" );
		if ( src.empty( ) ) {
			return;
		}
		WriteUdLibLog( "src='" + toString( src ) + "', dst='" + toString( dst ) + "'\n" );
		wstring path = src + L"\\*";
		WIN32_FIND_DATA data;
		ZeroMemory( &data, sizeof( WIN32_FIND_DATA ) );
		HANDLE handle = FindFirstFile( path.data( ), &data );
		if ( handle != INVALID_HANDLE_VALUE ) {
			wstring filename = data.cFileName, filename_dst;
			WriteUdLibLog( toString( filename ) + "\n" );
			if ( filename != L"." && filename != L".." ) {
				filename_dst = dst + L"\\" + filename;
				filename = src + L"\\" + filename;
				WriteUdLibLog( "src='" + toString( filename ) + "', dst='" + toString( filename_dst ) + "'\n" );
				if ( CopyFile( filename.data( ), filename_dst.data( ), FALSE ) == 0 ) {
					char tmp[ 256 ];
					sprintf_s( tmp, 256, "DirectoryCopyFiles, CopyFile error code=%lu\n", GetLastError( ) );
					WriteUdLibLog( tmp );
					FindClose( handle );
					WriteUdLibLog( "DirectoryCopyFiles exit\n" );
					return;
				}
			}
			while( FindNextFile( handle, &data ) != 0 ) {
				filename = data.cFileName;
				WriteUdLibLog( toString( filename ) + "\n" );
				if ( filename != L"." && filename != L".." ) {
					filename_dst = dst + L"\\" + filename;
					filename = src + L"\\" + filename;
					if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
					} else {
						WriteUdLibLog( "src='" + toString( filename ) + "', dst='" + toString( filename_dst ) + "'\n" );
						if ( CopyFile( filename.data( ), filename_dst.data( ), FALSE ) == 0 ) {
							char tmp[ 256 ];
							sprintf_s( tmp, 256, "DirectoryCopyFiles, CopyFile error code=%lu\n", GetLastError( ) );
							WriteUdLibLog( tmp );
							FindClose( handle );
							WriteUdLibLog( "DirectoryCopyFiles exit\n" );
							return;
						}
					}
				}
			}
			FindClose( handle );
		} else {
			char tmp[ 256 ];
			sprintf_s( tmp, 256, "DirectoryCopyFiles, FindFirstFile error code=%lu\n", GetLastError( ) );
			WriteUdLibLog( tmp );
		}
		WriteUdLibLog( "DirectoryCopyFiles exit\n" );
	} // void DirectoryCopyFiles

	udInt WriteToFile( const wstring& szFilePath, const string& szContent ) {
		FILE *pFile = NULL;
		if ( _wfopen_s( &pFile, szFilePath.data( ), L"wb" ) == 0 ) {
			fwrite( szContent.data( ), 1, szContent.size( ), pFile );
			fclose( pFile );
			return 1;
		}
		return 0;
	} // udInt WriteToFile

	string Join( string szGlue, const udArrString& arrString ) {
		string szRet( "" );
		if ( !arrString.empty( ) ) {
			udArrString::const_iterator	i = arrString.begin( ), j = arrString.end( );
			--j;

			while( i != j ) {
				szRet += *i;
				szRet += szGlue;
				++i;
			}

			szRet += *i;
		}

		return szRet;
	} // string Join

	wstring Join( wstring szGlue, const udArrWString& arrString ) {
		wstring szRet( L"" );
		if ( !arrString.empty( ) ) {
			udArrWString::const_iterator i = arrString.begin( ), j = arrString.end( );
			--j;

			while( i != j ) {
				szRet += *i;
				szRet += szGlue;
				++i;
			}

			szRet += *i;
		}

		return szRet;
	} // string Join

} // namespace UdSDK