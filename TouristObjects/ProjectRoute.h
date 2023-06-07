/**
	Tourist application
	Набор классов объектов

	маршруты
*/
#ifndef TOURIST_APPLICATION_OBJECTS_ROUTE_H
#define TOURIST_APPLICATION_OBJECTS_ROUTE_H

namespace UdSDK {

	class CProjectRoute : public udBase {

	public:
		CProjectRoute( );

		virtual ~CProjectRoute( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectRoute

	class CProjectTourRoute : public udBase {

	public:
		CProjectTourRoute( );

		virtual ~CProjectTourRoute( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectTourRoute

	class CProjectTplTourRoute : public udBase {

	public:
		CProjectTplTourRoute( );

		virtual ~CProjectTplTourRoute( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectTplTourRoute

	class CProjectOrderRoute : public udBase {

	public:
		CProjectOrderRoute( );

		virtual ~CProjectOrderRoute( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectOrderRoute

} // namespace UdSDK

#endif
