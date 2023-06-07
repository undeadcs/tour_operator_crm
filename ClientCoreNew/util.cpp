
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void ExtOpenFile( const wstring& path, bool open, bool maximized ) {
		ShellExecute( NULL, ( open ? L"open" : L"edit" ), path.data( ), NULL, NULL, ( maximized ? SW_SHOWMAXIMIZED : SW_SHOW ) );
	} // void ExtOpenFile

	void ExtOpenFolder( const wstring& path, bool maximized ) {
		ShellExecute( NULL, L"explore", path.data( ), NULL, NULL, ( maximized ? SW_SHOWMAXIMIZED : SW_SHOW ) );
	} // void ExtOpenFolder

} // namespace UdSDK
