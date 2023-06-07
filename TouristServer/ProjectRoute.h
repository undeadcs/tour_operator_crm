/**
	Tourist application
	Серверная часть

	маршрут
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_ROUTE_H
#define TOURIST_APPLICATION_TOURIST_SERVER_ROUTE_H

namespace UdSDK {

	class CHRoute : public udStateHandlerSimpleOneName {

	public:
		CHRoute( );
		CHRoute( const CHRoute& objCopy );

		virtual ~CHRoute( );

		CHRoute& operator =( const CHRoute& objCopy );

	}; // class CHCountry

} // namespace UdSDK

#endif