/**
	Tourist application
	Клиентская часть

	пункт перехода
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_TRANSFER_POINT_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_TRANSFER_POINT_H

namespace UdSDK {

	class CHTransferPoint : public udStateHandlerSimpleOneName {

	public:
		CHTransferPoint( );
		CHTransferPoint( const CHTransferPoint& objCopy );

		virtual ~CHTransferPoint( );

		CHTransferPoint& operator =( const CHTransferPoint& objCopy );

	}; // class CHCountry

} // namespace UdSDK

#endif