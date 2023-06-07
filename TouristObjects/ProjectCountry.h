/**
	Tourist application
	Набор классов объектов

	страна
*/
#ifndef TOURIST_APPLICATION_OBJECTS_COUNTRY_H
#define TOURIST_APPLICATION_OBJECTS_COUNTRY_H

namespace UdSDK {

	class CProjectCountry : public udBase {

	public:
		CProjectCountry( );

		virtual ~CProjectCountry( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectCountry

} // namespace UdSDK

#endif
