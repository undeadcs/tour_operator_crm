
#ifndef FORWARD_H
#define FORWARD_H

// базовые типы SDK
namespace UdSDK {

#define defineHandler( type, name )	udP##type##Handler GetH##name##type##( ) { return new udH##name##; }
#define defineListHandler( name )	defineHandler( List, name );
#define defineFormHandler( name )	defineHandler( Form, name );
#define _def_handler( name )	defineListHandler( name ) defineFormHandler( name )
#define defineForwardHandler( name )	udPListHandler GetH##name##List( ); udPFormHandler GetH##name##Form( );
#define forwardHandlerCallback( type, name )	udP##type##Handler GetH##name##type##( );
#define forwardListHandlerCallback( name )	forwardHandlerCallback( List, name )
#define forwardFormHandlerCallback( name )	forwardHandlerCallback( Form, name )
#define typedefHandler( name )	(typedef class CH##name udH##name##, *udPH##name##, **udPP##name##);
#define forwardHandler( name )	typedefHandler( name ) forwardListHandler( name ) forwardFormHandler( name )
#define forwardHandlerOnlyList( name )	typedefHandler( name ) forwardListHandler( name )
#define forwardHandlerOnlyForm( name )	typedefHandler( name ) forwardFormHandler( name )

	typedef class CStateHandlerSimpleOneName	udStateHandlerSimpleOneName, *udPStateHandlerSimpleOneName, **udPPStateHandlerSimpleOneName;
	typedef class CStateHandlerInfo				udStateHandlerInfo, *udPStateHandlerInfo, **udPPStateHandlerInfo;

	//typedefHandler( MainPage );

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
	typedef class CHOrderRoute		udHOrderRoute, *udPHOrderRoute, **udPPHOrderRoute;
	typedef class CHOrderService	udHOrderService, *udPHOrderService, **udPPHOrderService;
	typedef class CHTourService		udHTourService, *udPHTourService, **udPPHTourService;
	typedef class CHServicePeriod	udHServicePeriod, *udPHServicePeriod, **udPPHServicePeriod;
	typedef class CHServicePrice	udHServicePrice, *udPHServicePrice, **udPPHServicePrice;
	typedef class CHGroupPeriod		udHGroupPeriod, *udPHGroupPeriod, **udPPHGroupPeriod;
	typedef class CHGroupTour		udHGroupTour, *udPHGroupTour, **udPPHGroupTour;
	typedef class CHTplTourService	udHTplTourService, *udPHTplTourService, **udPPHTplTourService;
	typedef class CWndSumm			udWndSumm, *udPWndSumm, **udPPWndSumm;
	typedef class CWndSummHandler	udWndSummHandler, *udPWndSummHandler, **udPPWndSummHandler;
	typedef class CHSummAdd			udHSummAdd, *udPSummAdd, **udPPSummAdd;
	typedef class CHSummBack		udHSummBack, *udPHSummBack, **udPPHSummBack;
	typedef class CHSummConvert		udHSummConvert, *udPHSummConvert, **udPPHSummConvert;
	typedef class CHDocTemplate		udHDocTemplate, *udPHDocTemplate, **udPPHDocTemplate;
	typedef class CWndOperations	udWndOperations, *udPWndOperations, **udPPWndOperations;
	typedef class CHTourPayOrder	udHTourPayOrder, *udPHTourPayOrder, **udPPHTourPayOrder;
	typedef class CHTourPrice		udHTourPrice, *udPHTourPrice, **udPPHTourPrice;
	//typedef class CHGroupContractF	udHGroupContractF, *udPGroupContractF, **udPPGroupContractF;
	typedef class CHGroupContractApp1	udHGroupContractApp1, *udPGroupContractApp1, **udPPGroupContractApp1;
	typedef class CHGCTouristInfo	udHGCTouristInfo, *udPHGCTouristInfo, **udPPHGCTouristInfo;
	typedef class CHGCOccupancy		udHGCOccupancy, *udPHGCOccupancy, **udPPHGCOccupancy;
	typedef class CHGCProgram		udHGCProgram, *udPHGCProgram, **udPPHGCProgram;
	typedef class CHGCTransfer		udHGCTransfer, *udPHGCTransfer, **udPPHGCTransfer;
	typedef class CHGCCover			udHGCCover, *udPHGCCover, **udPPHGCCover;
	typedef class CHGCVisa			udHGCVisa, *udPHGCVisa, **udPPHGCVisa;
	typedef class CHGCService		udHGCService, *udPHGCService, **udPPHGCService;
	//typedef class CHGroupContractApp2F	udHGroupContractApp2F, *udPGroupContractApp2F, **udPPGroupContractApp2F;
	typedef class CHGCTransfer2		udHGCTransfer2, *udPHGCTransfer2, **udPPHGCTransfer2;
	typedef class CHDocTemplatePrinted	udHDocTemplatePrinted, *udPHDocTemplatePrinted, **udPPHDocTemplatePrinted;
	typedef class CHTransferPoint	udHTransferPoint, *udPHTransferPoint, **udPPHTransferPoint;
	typedef class CHRoute			udHRoute, *udPHRoute, **udPPHRoute;
	typedef class CHHostSide		udHHostSide, *udPHHostSide, **udPPHHostSide;
	typedef class CHTouristList		udHTouristList, *udPHTouristList, **udPPHTouristList;
	typedef class CHTouristListLink	udHTouristListLink, *udPHTouristListLink, **udPPHTouristListLink;

	//_def_handler( HMainPage );
	defineForwardHandler( Request );
	defineForwardHandler( Order );
	defineForwardHandler( Tour );
	defineForwardHandler( TplTour );
	defineForwardHandler( Group );
	//_def_handler( HInfoPage );
	defineForwardHandler( Country );
	defineForwardHandler( TService );
	defineForwardHandler( SOrder );
	defineForwardHandler( RRequest );
	defineForwardHandler( STour );
	defineForwardHandler( TRequest );
	defineForwardHandler( MUService );
	defineForwardHandler( Direct );
	defineForwardHandler( NameService );
	defineForwardHandler( DocType );
	defineForwardHandler( Currency );
	defineForwardHandler( User );
	defineForwardHandler( Log );
	defineForwardHandler( CurrencyPair );
	defineForwardHandler( Company );
	defineForwardHandler( ClDoc );
	defineForwardHandler( Partner );
	defineForwardHandler( PartnerReq );
	defineForwardHandler( PartnerAnswr );
	defineForwardHandler( Service );
	defineForwardHandler( Client );
	defineForwardHandler( SRequest );
	defineForwardHandler( ClientUr );
	defineForwardHandler( PersAcc );
	defineForwardHandler( DocTemplate );
	defineForwardHandler( TourPayOrder );
	defineForwardHandler( City );
	forwardFormHandlerCallback( OrderRoute );
	forwardFormHandlerCallback( OrderService );
	forwardFormHandlerCallback( TourService );
	defineForwardHandler( ServicePeriod );
	forwardFormHandlerCallback( ServicePrice );
	defineForwardHandler( GroupPeriod );
	forwardFormHandlerCallback( GroupTour );
	forwardFormHandlerCallback( TplTourService );
	forwardFormHandlerCallback( SummAdd );
	forwardFormHandlerCallback( SummBack );
	forwardFormHandlerCallback( SummConvert );
	forwardFormHandlerCallback( TourPrice );
	//forwardFormHandlerCallback( GroupContractF );
	forwardFormHandlerCallback( GroupContractApp1 );
	forwardFormHandlerCallback( GCTouristInfo );
	forwardFormHandlerCallback( GCOccupancy );
	forwardFormHandlerCallback( GCProgram );
	forwardFormHandlerCallback( GCTransfer );
	forwardFormHandlerCallback( GCCover );
	forwardFormHandlerCallback( GCVisa );
	forwardFormHandlerCallback( GCService );
	//forwardFormHandlerCallback( GroupContractApp2F );
	forwardFormHandlerCallback( GCTransfer2 );
	defineForwardHandler( DocTemplatePrinted );
	defineForwardHandler( TransferPoint );
	defineForwardHandler( Route );
	defineForwardHandler( HostSide );
	defineForwardHandler( TouristList );
	forwardFormHandlerCallback( TouristListLink );

} // namespace UdSDK

#endif
