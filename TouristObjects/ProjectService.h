/**
	Tourist application
	Набор классов объектов

	услуга
*/
#ifndef TOURIST_APPLICATION_OBJECTS_SERVICE_H
#define TOURIST_APPLICATION_OBJECTS_SERVICE_H

namespace UdSDK {

	class CProjectService : public udBase {

	public:
		CProjectService( );

		virtual ~CProjectService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectService

	class CProjectServicePeriod : public udBase {

	public:
		CProjectServicePeriod( );

		virtual ~CProjectServicePeriod( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectServicePeriod

} // namespace UdSDK

#endif
