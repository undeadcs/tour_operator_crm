/**
	Tourist application
	Набор классов объектов

	группа
*/
#ifndef TOURIST_APPLICATION_OBJECTS_GROUP_H
#define TOURIST_APPLICATION_OBJECTS_GROUP_H

namespace UdSDK {

	class CProjectGroup : public udBase {

	public:
		CProjectGroup( );

		virtual ~CProjectGroup( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGroup

	class CProjectGroupTplTour : public udBase {

	public:
		CProjectGroupTplTour( );

		virtual ~CProjectGroupTplTour( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGroupTplTour

	class CProjectGroupTour : public udBase {

	public:
		CProjectGroupTour( );

		virtual ~CProjectGroupTour( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGroupTour

	class CProjectGroupPeriod : public udBase {

	public:
		CProjectGroupPeriod( );

		virtual ~CProjectGroupPeriod( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGroupPeriod

} // namespace UdSDK

#endif
