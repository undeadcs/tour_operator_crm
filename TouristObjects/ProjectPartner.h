/**
	Tourist application
	Набор классов объектов

	партнер
*/
#ifndef TOURIST_APPLICATION_OBJECTS_PARTNER_H
#define TOURIST_APPLICATION_OBJECTS_PARTNER_H

namespace UdSDK {

	class CProjectPartner : public udBase {

	public:
		CProjectPartner( );

		virtual ~CProjectPartner( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectPartner

} // namespace UdSDK

#endif
