/**
	Tourist application
	Набор классов объектов

	результат обращения
*/
#ifndef TOURIST_APPLICATION_OBJECTS_RREQUEST_H
#define TOURIST_APPLICATION_OBJECTS_RREQUEST_H

namespace UdSDK {

	class CProjectRRequest : public udBase {

	public:
		CProjectRRequest( );

		virtual ~CProjectRRequest( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectRRequest

} // namespace UdSDK

#endif
