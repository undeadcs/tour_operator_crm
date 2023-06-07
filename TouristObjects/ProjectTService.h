/**
	Tourist application
	Набор классов объектов

	тип услуги
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TSERVICE_H
#define TOURIST_APPLICATION_OBJECTS_TSERVICE_H

namespace UdSDK {

	class CProjectTService : public udBase {

	public:
		CProjectTService( );

		virtual ~CProjectTService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTService

} // namespace UdSDK

#endif
