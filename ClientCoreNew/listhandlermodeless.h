/*
	Tourist application
	Библиотека ядра клиентского приложения
	не модальный обработчик списка

	необходимо учесть, что Init данного класс не вызывает Init ListHandler'а, чтобы не было
	повторных вызовов для дочерних
*/
#ifndef TOURIST_CLIENT_CORE_LISTHANDLER_MODELESS_H
#define TOURIST_CLIENT_CORE_LISTHANDLER_MODELESS_H

namespace UdSDK {

	class CListHandlerModeless {
	protected:
		Gtk::Button	m_btnMain;

	public:
		CListHandlerModeless( );
		CListHandlerModeless( const CListHandlerModeless& objCopy );

		virtual ~CListHandlerModeless( );

		CListHandlerModeless& operator =( const CListHandlerModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonMain( );

		virtual string			GetModelessName( ) const = 0;
		virtual udPWndHandler	GetModelessWindow( ) const = 0;

	}; // class CListHandlerModeless

} // namespace UdSDK

#endif
