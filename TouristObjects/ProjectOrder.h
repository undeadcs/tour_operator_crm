/**
	Tourist application
	Набор классов объектов

	заявка
*/
#ifndef TOURIST_APPLICATION_OBJECTS_ORDER_H
#define TOURIST_APPLICATION_OBJECTS_ORDER_H

namespace UdSDK {

	class CProjectOrder : public udBase {
	public:
		enum {
			ORDER_STATUS_NONE,
			ORDER_STATUS_OPENED,
			ORDER_STATUS_CLOSED,
			ORDER_STATUS_REMIND
		};

		enum {
			ORDER_EMPLOYER_CLIENT,
			ORDER_EMPLOYER_CLIENTUR
		};
		
	public:
		CProjectOrder( );

		virtual ~CProjectOrder( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectOrder

	class CProjectOrderService : public udBase {

	public:
		CProjectOrderService( );

		virtual ~CProjectOrderService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectOrderService

} // namespace UdSDK

#endif
