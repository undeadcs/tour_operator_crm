/**
	Tourist application
	Набор классов объектов

	город
*/
#ifndef TOURIST_APPLICATION_OBJECTS_CITY_H
#define TOURIST_APPLICATION_OBJECTS_CITY_H

namespace UdSDK {

	class CProjectCity : public udBase {

	public:
		CProjectCity( );

		virtual ~CProjectCity( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectCity

} // namespace UdSDK

#endif
