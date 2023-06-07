/**
	UndeadCS SDK
	Библиотека интерфейса пользователя

	функции работы с окнами
*/
#ifndef UNDEADCS_SDK_USERINTERFACE_UTIL_H
#define UNDEADCS_SDK_USERINTERFACE_UTIL_H

namespace UdSDK {

	void	CenterOneWindowAboveParent( HWND hWnd );
	void	ExtOpenFile( const wstring& path, bool open = true, bool maximized = true );
	void	ExtOpenFolder( const wstring& path, bool maximized = true );

} // namespace UdSDK

#endif
