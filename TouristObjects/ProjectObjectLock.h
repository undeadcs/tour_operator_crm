/**
	Tourist application
	Набор классов объектов

	блокировка
*/
#ifndef TOURIST_APPLICATION_OBJECTS_OBJECT_LOCK_H
#define TOURIST_APPLICATION_OBJECTS_OBJECT_LOCK_H

namespace UdSDK {

	class CProjectObjectLock : public udBase {

	public:
		// тип объекта
		enum {
			lockRequest,
			lockOrder,
			lockTour,
			lockTRequest,
			lockTService,
			lockDirect,
			lockService,
			lockNameService,
			lockMUService,
			lockCountry,
			lockRRequest,
			lockSTour,
			lockGroup,
			lockTplTour,
			lockCity,
			lockClient,
			lockUser,
			lockSOrder,
			lockLog,
			lockClDoc,
			lockDocType,
			lockCurrency,
			lockCurrencyPair,
			lockCompany,
			lockPartner,
			lockClientUr,
			lockPersAcc,
			lockPartnerReq,
			lockPartnerAnswr,
			lockAService,
			lockAttach,
			lockPeriodAct,
			lockPriceTableItem,
			lockSRequest,
			lockTourRoute,
			lockTplTourRoute,
			lockTplTourService,
			lockTourService,
			lockTourClient,
			lockTourTour,
			lockOrderRoute,
			lockOrderService,
			lockGroupTour,
			lockGroupPeriod,
			lockServicePeriod,
			lockGroupTplTour,
			lockObjectLink,
		};

	private:

	public:
		CProjectObjectLock( );

		virtual ~CProjectObjectLock( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectObjectLock

} // namespace UdSDK

#endif
