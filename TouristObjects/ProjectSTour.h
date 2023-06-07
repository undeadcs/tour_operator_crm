/**
	Tourist application
	Набор классов объектов

	состояние тура
*/
#ifndef TOURIST_APPLICATION_OBJECTS_STOUR_H
#define TOURIST_APPLICATION_OBJECTS_STOUR_H

namespace UdSDK {

	class CProjectSTour : public udBase {

	public:
		CProjectSTour( );

		virtual ~CProjectSTour( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectSTour

} // namespace UdSDK

#endif
