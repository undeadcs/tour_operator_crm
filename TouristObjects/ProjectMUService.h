/**
	Tourist application
	Набор классов объектов

	единица измерения услуги
*/
#ifndef TOURIST_APPLICATION_OBJECTS_MUSERVICE_H
#define TOURIST_APPLICATION_OBJECTS_MUSERVICE_H

namespace UdSDK {

	class CProjectMUService : public udBase {

	public:
		CProjectMUService( );

		virtual ~CProjectMUService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectMUService

} // namespace UdSDK

#endif
