/**
	Tourist application
	Набор классов объектов

	направление
*/
#ifndef TOURIST_APPLICATION_OBJECTS_DIRECT_H
#define TOURIST_APPLICATION_OBJECTS_DIRECT_H

namespace UdSDK {

	class CProjectDirect : public udBase {

	public:
		CProjectDirect( );

		virtual ~CProjectDirect( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectDirect

} // namespace UdSDK

#endif
