/**
	Tourist application
	Набор классов объектов

	лицевой счет
*/
#ifndef TOURIST_APPLICATION_OBJECTS_PERSACC_H
#define TOURIST_APPLICATION_OBJECTS_PERSACC_H

namespace UdSDK {

	class CProjectPersAcc : public udBase {
	public:
		typedef enum accType {
			PERS_ACC_CLIENT_COMPANY,		// клиент(физ.лицо)-компания
			PERS_ACC_CLIENTUR_COMPANY,		// клиент(юр.лицо)-компания
			PERS_ACC_COMPANY_PARTNER		// компания-партнер
		} accType;

	public:
		CProjectPersAcc( );

		virtual ~CProjectPersAcc( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectPersAcc

} // namespace UdSDK

#endif
