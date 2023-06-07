/**
	Tourist application
	Набор классов объектов

	принимающая сторона
*/
#ifndef TOURIST_APPLICATION_OBJECTS_HOST_SIDE_H
#define TOURIST_APPLICATION_OBJECTS_HOST_SIDE_H

namespace UdSDK {

	class CProjectHostSide : public udBase {

	public:
		CProjectHostSide( );

		virtual ~CProjectHostSide( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectHostSide

} // namespace UdSDK

#endif
