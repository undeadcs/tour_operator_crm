/**
	Tourist application
	Набор классов объектов

	шаблон тура
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TPLTOUR_H
#define TOURIST_APPLICATION_OBJECTS_TPLTOUR_H

namespace UdSDK {

	class CProjectTplTour : public udBase {

	public:
		CProjectTplTour( );

		virtual ~CProjectTplTour( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTplTour

	class CProjectTplTourService : public udBase {

	public:
		CProjectTplTourService( );

		virtual ~CProjectTplTourService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTplTourService

} // namespace UdSDK

#endif
