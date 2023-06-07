/**

	Абстрактный класс инициализации приложения.
	Главное приложение будет вызывать необходимые функции класса, чтобы инициализировать необходимые данные.

*/

#ifndef APPINIT_H
#define APPINIT_H

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

		virtual void	LoadFonts( udPWndSplash pSplash ) = 0;
		virtual void	LoadImages( udPWndSplash pSplash ) = 0;
		virtual void	InitToolbar( udPWndSplash pSplash, udPWndToolbar pToolbar ) = 0;
		virtual void	InitMenu( udPWndSplash pSplash, udPWndMenu pMenu ) = 0;
		virtual void	InitSync( udPDatabaseSync sync ) = 0;
		virtual void	InitModel( udPWndSplash pSplash, udPModelWork pModel ) = 0;
		virtual udInt	Login( udPWndLogin wnd, wstring login, wstring password ) = 0;
		virtual udInt	Login( udPSocket socket ) = 0;

		virtual void	HandleSync( udPSocket sock, udAscBase& ascClient ) = 0;

		void	SetApp( udPApplication pApp ) { m_pApp = pApp; }

		udPApplication	GetApp( ) const { return m_pApp; }

	}; // class CAppInit

} // namespace UdSDK

#endif
