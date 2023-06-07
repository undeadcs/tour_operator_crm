/**
	Tourist application
	Набор классов объектов

	тип обращения
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TREQUEST_H
#define TOURIST_APPLICATION_OBJECTS_TREQUEST_H

namespace UdSDK {

	class CProjectTRequest : public udBase {

	public:
		CProjectTRequest( );

		virtual ~CProjectTRequest( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTRequest

} // namespace UdSDK

#endif
