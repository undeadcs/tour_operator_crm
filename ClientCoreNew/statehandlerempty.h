/*
	Tourist application
	Библиотека ядра клиентского приложения
	Обработчик состояния для кастомной обработки
*/
#ifndef TOURIST_CLIENT_CORE_STATE_HANDLER_EMPTY_H
#define TOURIST_CLIENT_CORE_STATE_HANDLER_EMPTY_H

namespace UdSDK {

	class CStateHandlerEmpty : public udStateHandler {
	protected:
		udPWndHandler	m_wndEmpty;

	public:
		CStateHandlerEmpty( );
		CStateHandlerEmpty( const CStateHandlerEmpty& objCopy );

		virtual ~CStateHandlerEmpty( );

		CStateHandlerEmpty& operator =( const CStateHandlerEmpty& objCopy );

		virtual void	Init( );
		virtual void	Load( );
		virtual void	Show( );
		virtual void	Hide( );
		virtual void	Clear( );

	}; // class CStateHandlerEmpty

} // namespace UdSDK

#endif
