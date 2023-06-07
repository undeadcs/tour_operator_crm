/**
	UndeadCS SDK
	Базовая библиоткеа

	вспомогательные функции
*/
#ifndef UNDEADCS_SDK_LIBRARY_UTIL_H
#define UNDEADCS_SDK_LIBRARY_UTIL_H

namespace UdSDK {

	typedef void	(*pfnListingForEachFile)( const wstring& filepath );
	typedef void	(*pfnListingForEachFolder)( const wstring& folderpath );

	void	DirectoryListing( const wstring& name, pfnListingForEachFile fnForEachFile = NULL, pfnListingForEachFolder fnListingForEachFolder = NULL );
	bool	DirectoryClear( const wstring& name );
	void	DirectoryCopyFiles( const wstring& src, const wstring& dst );
	udInt	WriteToFile( const wstring& szFilePath, const string& szContent );
	string	Join( string szGlue, const udArrString& arrString );
	wstring	Join( wstring szGlue, const udArrWString& arrString );

} // namespace UdSDK

#endif
