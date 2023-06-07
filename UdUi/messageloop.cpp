
#include "includes.h"

namespace UdSDK {

	void MessageLoop( udPHMsgLoop pHandler ) {
		int code = CHMsgLoop::MsgLoopQuit;
		MSG objMsg;

		ZeroMemory( &objMsg, sizeof( MSG ) );

		while( !pHandler->IsFinish( ) ) {
			code = pHandler->GetMessage( &objMsg );
			if ( code == CHMsgLoop::MsgLoopQuit ) {
				pHandler->OnQuit( );
				break;
			} else if ( code == CHMsgLoop::MsgLoopError ) {
				pHandler->OnError( );
				break;
			} else if ( code == CHMsgLoop::MsgLoopMessage ) {
				pHandler->ProcMessage( &objMsg );
			}
		}
	} // void MessageLoop

	CHMsgLoop::CHMsgLoop( ) {
	}

	CHMsgLoop::~CHMsgLoop( ) {
	}

} // namespace UdSDK