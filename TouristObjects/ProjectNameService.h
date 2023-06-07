/**
	Tourist application
	Набор классов объектов

	название услуги
*/
#ifndef TOURIST_APPLICATION_OBJECTS_NAMESERVICE_H
#define TOURIST_APPLICATION_OBJECTS_NAMESERVICE_H

namespace UdSDK {

	class CProjectNameService : public udBase {

	public:
		CProjectNameService( );

		virtual ~CProjectNameService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectNameService

} // namespace UdSDK

#endif
