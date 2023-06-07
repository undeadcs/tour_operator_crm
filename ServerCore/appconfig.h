
#ifndef APPCONFIG_H
#define APPCONFIG_H

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
