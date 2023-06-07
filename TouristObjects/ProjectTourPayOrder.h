/**
	Tourist application
	Библиотека классов сущностей

	указание на оплату по туру
*/
#ifndef TOURIST_APPLICATION_TOURIST_OBJECTS_TOUR_PAY_ORDER_H
#define TOURIST_APPLICATION_TOURIST_OBJECTS_TOUR_PAY_ORDER_H

namespace UdSDK {

	class CProjectTourPayOrder : public udBase {
	public:
		enum {
			stOpened,	// открыто
			stClosed,	// закрыто
		};

	private:

	public:
		CProjectTourPayOrder( );

		virtual ~CProjectTourPayOrder( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectTourPayOrder

} // namespace UdSDK

#endif
