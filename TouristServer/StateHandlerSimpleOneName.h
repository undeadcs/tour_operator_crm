
#ifndef TOURIST_SERVER_STATE_HANDLER_SIMPLE_ONE_NAME_H
#define TOURIST_SERVER_STATE_HANDLER_SIMPLE_ONE_NAME_H

namespace UdSDK {

	class CStateHandlerSimpleOneName : public udStateHandlerSimple {

	public:
		CStateHandlerSimpleOneName( );

		virtual ~CStateHandlerSimpleOneName( );

		virtual string	GetWhereCondition( );

	}; // class CStateHandlerSimpleOneName

} // namespace UdSDK

#endif
