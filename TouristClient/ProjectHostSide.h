/**
	Tourist application
	Клиентская часть

	принимающая сторона
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_HOST_SIDE_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_HOST_SIDE_H

namespace UdSDK {

	class CHHostSide : public udStateHandlerSimpleOneName {

	public:
		CHHostSide( );
		CHHostSide( const CHHostSide& objCopy );

		virtual ~CHHostSide( );

		CHHostSide& operator =( const CHHostSide& objCopy );

	}; // class CHCountry

} // namespace UdSDK

#endif