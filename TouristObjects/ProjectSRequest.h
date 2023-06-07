/**
	Tourist application
	Набор классов объектов

	состояние обращения
*/
#ifndef TOURIST_APPLICATION_OBJECTS_SREQUEST_H
#define TOURIST_APPLICATION_OBJECTS_SREQUEST_H

namespace UdSDK {

	class CProjectSRequest : public udBase {

	public:
		CProjectSRequest( );

		virtual ~CProjectSRequest( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectSRequest

} // namespace UdSDK

#endif
