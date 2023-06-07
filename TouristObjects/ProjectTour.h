/**
	Tourist application
	Набор классов объектов

	тур
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TOUR_H
#define TOURIST_APPLICATION_OBJECTS_TOUR_H

namespace UdSDK {

	class CProjectTour : public udBase {
	public:
		enum {
			TOUR_STATUS_NONE,
			TOUR_STATUS_OPENED,
			TOUR_STATUS_CLOSED,
			TOUR_STATUS_REMIND
		};

		enum {
			TOUR_EMPLOYER_CLIENT,
			TOUR_EMPLOYER_CLIENTUR
		};

	public:
		CProjectTour( );

		virtual ~CProjectTour( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTour

	class CProjectTourService : public udBase {

	public:
		CProjectTourService( );

		virtual ~CProjectTourService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTTourService

	class CProjectTourClient : public udBase {

	public:
		CProjectTourClient( );

		virtual ~CProjectTourClient( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTourClient

	class CProjectTourTour : public udBase {

	public:
		CProjectTourTour( );

		virtual ~CProjectTourTour( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTourTour

	class CProjectTourPrice : public udBase {

	public:
		CProjectTourPrice( );

		virtual ~CProjectTourPrice( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTourPrice

} // namespace UdSDK

#endif
