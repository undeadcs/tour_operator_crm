/**
	Tourist application
	Набор классов объектов

	состояние заявки
*/
#ifndef TOURIST_APPLICATION_OBJECTS_SORDER_H
#define TOURIST_APPLICATION_OBJECTS_SORDER_H

namespace UdSDK {

	class CProjectSOrder : public udBase {

	public:
		CProjectSOrder( );

		virtual ~CProjectSOrder( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectSOrder

} // namespace UdSDK

#endif
