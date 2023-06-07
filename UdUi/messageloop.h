/**
	UndeadCS SDK
	Библиотека интерфейса пользователя
	
	цикл сообщений
*/
#ifndef UNDEADCS_SDK_USER_INTERFACE_MESSAGE_LOOP_H
#define UNDEADCS_SDK_USER_INTERFACE_MESSAGE_LOOP_H

namespace UdSDK {

	void	MessageLoop( udPHMsgLoop pHandler );

	class CHMsgLoop {
	public:
		enum {
			MsgLoopQuit,	// получено сообщение на выход
			MsgLoopError,	// получена ошибка
			MsgLoopMessage,	// получено сообщение
		};

	public:
		CHMsgLoop( );

		virtual ~CHMsgLoop( );

		virtual bool	IsFinish( ) = 0;
		virtual int		GetMessage( LPMSG pMsg ) = 0;
		virtual void	OnQuit( ) = 0;
		virtual void	OnError( ) = 0;
		virtual void	ProcMessage( LPMSG pMsg ) = 0;

	}; // class CHMsgLoop

} // namespace UdSDK

#endif
