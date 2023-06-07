/*
	Tourist application
	Библиотека ядра клиентского приложения
	Пользователь приложения
*/
#ifndef TOURIST_CLIENT_CORE_APP_USER_H
#define TOURIST_CLIENT_CORE_APP_USER_H

namespace UdSDK {

	class CAppUser : public udMorph {

	public:
		CAppUser( );
		CAppUser( const CAppUser& objCopy );

		virtual ~CAppUser( );

		CAppUser& operator =( const CAppUser& objCopy );

	}; // class CAppUser

} // namespace UdSDK

#endif
