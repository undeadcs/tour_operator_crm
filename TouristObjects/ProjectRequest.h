/**
	Tourist application
	Набор классов объектов

	обращение
*/
#ifndef TOURIST_APPLICATION_OBJECTS_REQUEST_H
#define TOURIST_APPLICATION_OBJECTS_REQUEST_H

namespace UdSDK {

	class CProjectRequest : public udBase {
	public:
		enum {
			REQUEST_STATUS_NONE,
			REQUEST_STATUS_OPENED,
			REQUEST_STATUS_CLOSED,
			REQUEST_STATUS_REMIND
		};

	public:
		CProjectRequest( );

		virtual ~CProjectRequest( );

		virtual void		GetConfig( udBaseConfig& objConfig );

	}; // class CProjectRequest

} // namespace UdSDK

#endif
