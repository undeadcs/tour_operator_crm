/**
	Tourist application
	Клиентское приложение
	Операции об оплате тура
*/
#ifndef TOURIST_CLIENT_TOUR_PAY_OPERATION_H
#define TOURIST_CLIENT_TOUR_PAY_OPERATION_H

namespace UdSDK {

	udPBase GetProjectTourPayOperationList( );

	class CFDETourPayOperation : public CFDEList {

	public:
		CFDETourPayOperation( );
		CFDETourPayOperation( const CFDETourPayOperation& objCopy );

		virtual ~CFDETourPayOperation( );

		CFDETourPayOperation& operator =( const CFDETourPayOperation& objCopy );

		virtual void	Send( udPSocket sock );

	}; // class CFDETourPayOperation

} // namespace UdSDK

#endif
