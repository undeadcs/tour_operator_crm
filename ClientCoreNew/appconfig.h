/**
	Tourist application
	Библиотека ядра клиентского приложения
	Конфиг приложения
*/
#ifndef TOURIST_CLIENT_CORE_APP_CONFIG_H
#define TOURIST_CLIENT_CORE_APP_CONFIG_H

namespace UdSDK {

	/**
	 *	Конфиг приложения
	 */
	class CAppConfig : public udMorph {

	public:
		CAppConfig( );
		CAppConfig( const CAppConfig& objCopy );

		virtual ~CAppConfig( );

		CAppConfig& operator =( const CAppConfig& objCopy );

	}; // class CAppConfig

} // namespace UdSDK

#endif
