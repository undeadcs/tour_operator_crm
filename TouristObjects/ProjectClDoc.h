/**
	Tourist application
	Набор классов объектов

	документ клиента
*/
#ifndef TOURIST_APPLICATION_OBJECTS_CLDOC_H
#define TOURIST_APPLICATION_OBJECTS_CLDOC_H

namespace UdSDK {

	class CProjectClDoc : public udBase {

	public:
		CProjectClDoc( );

		virtual ~CProjectClDoc( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectClDoc

} // namespace UdSDK

#endif
