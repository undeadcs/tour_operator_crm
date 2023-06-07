/**
	Tourist application
	Библиотека ядра клиентского приложения
	Инициализатор приложения
*/
#ifndef TOURIST_CLIENT_CORE_APP_INIT_H
#define TOURIST_CLIENT_CORE_APP_INIT_H

namespace UdSDK {

	/**
	 *	Инициализатор приложения
	 */
	class CAppInit {

	public:
		CAppInit( );
		CAppInit( const CAppInit& objCopy );

		virtual ~CAppInit( );

		CAppInit& operator =( const CAppInit& objCopy );

		virtual void	InitToolbar( ) = 0;
		virtual void	InitMenu( ) = 0;
		virtual void	InitModel( ) = 0;

	}; // class CAppInit

} // namespace UdSDK

#endif
