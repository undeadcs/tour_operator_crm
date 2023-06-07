/**
	Tourist application
	Набор классов объектов

	запрос партнера
*/
#ifndef TOURIST_APPLICATION_OBJECTS_PARTNERREQ_H
#define TOURIST_APPLICATION_OBJECTS_PARTNERREQ_H

namespace UdSDK {

	class CProjectPartnerReq : public udBase {

	public:
		CProjectPartnerReq( );

		virtual ~CProjectPartnerReq( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectPartnerReq

} // namespace UdSDK

#endif
