/*
	Tourist application
	Библиотека ядра клиентского приложения
	Окно обработчика состояния
*/
#ifndef TOURIST_CLIENT_CORE_WND_HANDLER_H
#define TOURIST_CLIENT_CORE_WND_HANDLER_H

namespace UdSDK {

	class CWndHandler : public Gtk::VBox {

	public:
		CWndHandler( );
		CWndHandler( const CWndHandler& objCopy );

		virtual ~CWndHandler( );

		CWndHandler& operator =( const CWndHandler& objCopy );

	}; // class CWndHandler

} // namespace UdSDK

#endif
