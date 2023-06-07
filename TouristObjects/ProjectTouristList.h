/**
	Tourist application
	Набор классов объектов

	список туристов
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TOURIST_LIST_H
#define TOURIST_APPLICATION_OBJECTS_TOURIST_LIST_H

namespace UdSDK {

	class CProjectTouristList : public udBase {

	public:
		CProjectTouristList( );

		virtual ~CProjectTouristList( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTouristList

	class CProjectTouristListLink : public udBase {

	public:
		CProjectTouristListLink( );

		virtual ~CProjectTouristListLink( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTouristListLink

} // namespace UdSDK

#endif
