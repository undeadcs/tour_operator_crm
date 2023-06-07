
#ifndef FORWARD_H
#define FORWARD_H

// базовые типы SDK
namespace UdSDK {

	struct SessionData {
		string		id,			// ID сессии
					ip;			// IP клиента
		datetime	created;	// дата+время создания
		udPBase		user;		// пользователь
	};

	typedef class CStateHandlerSimpleOneName	udStateHandlerSimpleOneName, *udPStateHandlerSimpleOneName, **udPPStateHandlerSimpleOneName;

	// обработчики состояния
	typedef class CHMainPage		udHMainPage, *udPHMainPage, **udPPHMainPage;
	typedef class CHRequest			udHRequest, *udPHRequest, **udPPHRequest;
	typedef class CHOrder			udHOrder, *udPHOrder, **udPPHOrder;
	typedef class CHTour			udHTour, *udPHTour, **udPPHTour;
	typedef class CHTplTour			udHTplTour, *udPHTplTour, **udPPHTplTour;
	typedef class CHGroup			udHGroup, *udPHGroup, **udPPHGroup;
	typedef class CHInfoPage		udHInfoPage, *udPHInfoPage, **udPPHInfoPage;
	typedef class CHCountry			udHCountry, *udPHCountry, **udPPHCountry;
	typedef class CHTService		udHTService, *udPHTService, **udPPHTService;
	typedef class CHSOrder			udHSOrder, *udPHSOrder, **udPPHSOrder;
	typedef class CHRRequest		udHRRequest, *udPHRRequest, **udPPHRRequest;
	typedef class CHSTour			udHSTour, *udPHSTour, **udPPHSTour;
	typedef class CHTRequest		udHTRequest, *udPHTRequest, **udPPHTRequest;
	typedef class CHMUService		udHMUService, *udPHMUService, **udPPHMUService;
	typedef class CHDirect			udHDirect, *udPHDirect, **udPPHDirect;
	typedef class CHNameService		udHNameService, *udPHNameService, **udPPHNameService;
	typedef class CHDocType			udHDocType, *udPHDocType, **udPPHDocType;
	typedef class CHCurrency		udHCurrency, *udPHCurrency, **udPPHCurrency;
	typedef class CHUser			udHUser, *udPHUser, **udPPHUser;
	typedef class CHLog				udHLog, *udPHLog, **udPPHLog;
	typedef class CHCity			udHCity, *udPHCity, **udPPHCity;
	typedef class CHCurrencyPair	udHCurrencyPair, *udPHCurrencyPair, **udPPHCurrencyPair;
	typedef class CHCompany			udHCompany, *udPHCompany, **udPPHCompany;
	typedef class CHClDoc			udHClDoc, *udPClDoc, **udPPClDoc;
	typedef class CHPartner			udHPartner, *udPHPartner, **udPPHPartner;
	typedef class CHPartnerReq		udHPartnerReq, *udPHPartnerReq, **udPPPartnerReq;
	typedef class CHPartnerAnswr	udHPartnerAnswr, *udPHPartnerAnswr, **udPPHPartnerAnswr;
	typedef class CHService			udHService, *udPHService, **udPPHService;
	typedef class CHClient			udHClient, *udPHClient, **udPPHClient;
	typedef class CHSRequest		udHSRequest, *udPHSRequest, **udPPHSRequest;
	typedef class CHClientUr		udHClientUr, *udPHClientUr, **udPPHClientUr;
	typedef class CHPersAcc			udHPersAcc, *udPHPersAcc, **udPPHPersAcc;
	typedef class CHReport			udHReport, *udPHReport, **udPPHReport;
	typedef class CHObjectLock2		udHObjectLock2, *udPHObjectLock2, **udPPHObjectLock2;
	typedef class CHDocTemplate		udHDocTemplate, *udPHDocTemplate, **udPPHDocTemplate;
	typedef class CHTourPayOrder	udHTourPayOrder, *udPHTourPayOrder, **udPPHTourPayOrder;
	typedef class CHDocTemplatePrinted		udHDocTemplatePrinted, *udPHDocTemplatePrinted, **udPPHDocTemplatePrinted;
	typedef class CHTransferPoint	udHTransferPoint, *udPHTransferPoint, **udPPHTransferPoint;
	typedef class CHRoute			udHRoute, *udPHRoute, **udPPHRoute;
	typedef class CHHostSide		udHHostSide, *udPHHostSide, **udPPHHostSide;
	typedef class CHTouristList		udHTouristList, *udPHTouristList, **udPPHTouristList;
	typedef class CHGroupPeriod		udHGroupPeriod, *udPHGroupPeriod, **udPPHGroupPeriod;
	typedef class CHReportLanding	udHReportLanding, *udPHReportLanding, **udPPHReportLanding;
	typedef class CHReportFilling	udHReportFilling, *udPHReportFilling, **udPPHReportFilling;
	typedef class CHReportGroupPay	udHReportGroupPay, *udPHReportGroupPay, **udPPHReportGroupPay;
	typedef class CHReportHotelPay	udHReportHotelPay, *udPHReportHotelPay, **udPPHReportHotelPay;
	typedef class CHReportDayPay	udHReportDayPay, *udPHReportDayPay, **udPPHReportDayPay;

} // namespace UdSDK

#endif
