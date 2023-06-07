/*
	Tourist application
	Библиотека ядра клиентского приложения
	Источник данных
*/
#ifndef TOURIST_CLIENT_CORE_DATA_ACCESS_H
#define TOURIST_CLIENT_CORE_DATA_ACCESS_H

namespace UdSDK {

	class CDataAccess {

	public:
		CDataAccess( );
		CDataAccess( const CDataAccess& objCopy );

		virtual ~CDataAccess( );

		CDataAccess& operator =( const CDataAccess& objCopy );

		virtual void	Load( ) = 0;
		virtual void	Clear( ) = 0;

		static udInt	Read( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject );
		static udInt	Read( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject, udPBase tpl );
		static udInt	Write( udArrBase& arr, udBinaryData& data, pfnGetObject fnGetObject = NULL );
		static udPBase	LoadById( const string& handler_name, udUInt id, pfnGetObject fnGetObject );

	}; // class CDataAccess

} // namespace UdSDK

#endif
