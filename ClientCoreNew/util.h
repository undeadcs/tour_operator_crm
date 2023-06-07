/**
	Tourist application
	Библиотека ядра клиентского приложения
	вспомогательные функции
*/
#ifndef TOURIST_CLIENT_CORE_UTIL_H
#define TOURIST_CLIENT_CORE_UTIL_H

namespace UdSDK {

	void	ExtOpenFile( const wstring& path, bool open = true, bool maximized = true );
	void	ExtOpenFolder( const wstring& path, bool maximized = true );

} // namespace UdSDK

#endif
