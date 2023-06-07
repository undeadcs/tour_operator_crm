/**
	Tourist application
	Библиоткеа сервера

	объект инициализации
*/
#ifndef TOURIST_APPLICATION_SERVER_CORE_APPINIT_H
#define TOURIST_APPLICATION_SERVER_CORE_APPINIT_H

namespace UdSDK {

	/**
	 *	Класс инициализации приложения
	 */
	class CAppInit : public udMorph {
		udPApplication	m_pApp;

	public:
		CAppInit( );
		CAppInit( const CAppInit& objCopy );

		virtual ~CAppInit( );

		CAppInit& operator =( const CAppInit& objCopy );

		virtual void	LoadFonts( ) = 0;
		virtual void	LoadImages( ) = 0;
		virtual void	InitSync( udPDatabaseSync sync ) = 0;
		virtual void	InitModel( udPModelWork pModel ) = 0;
		virtual udInt	Login( udPSocket socket ) = 0;

		virtual void	HandleSync( udPSocket sock, udAscBase& ascClient, udAscBase& ascServer ) = 0;

		void	SetApp( udPApplication pApp ) { m_pApp = pApp; }

		udPApplication	GetApp( ) const { return m_pApp; }

	}; // class CAppInit

} // namespace UdSDK

#endif
