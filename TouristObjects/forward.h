/**
	Tourist application
	Набор классов объектов

	предварительное объявление
*/
#ifndef TOURIST_APPLICATION_OBJECTS_FORWARD_H
#define TOURIST_APPLICATION_OBJECTS_FORWARD_H

namespace UdSDK {

	// сущности
	typedef class CProjectRequest		udProjectRequest, *udPProjectRequest, **udPPProjectRequest;
	typedef class CProjectOrder			udProjectOrder, *udPProjectOrder, **udPPProjectOrder;
	typedef class CProjectTour			udProjectTour, *udPProjectTour, **udPPProjectTour;
	typedef class CProjectTRequest		udProjectTRequest, *udPProjectTRequest, **udPPProjectTRequest;
	typedef class CProjectTService		udProjectTService, *udPProjectTService, **udPPProjectTService;
	typedef class CProjectDirect		udProjectDirect, *udPProjectDirect, **udPPProjectDirect;
	typedef class CProjectService		udProjectService, *udPProjectService, **udPPProjectService;
	typedef class CProjectNameService	udProjectNameService, *udPProjectNameService, *udPPProjectNameService;
	typedef class CProjectMUService		udProjectMUService, *udPProjectMUService, **udPPMUService;
	typedef class CProjectCountry		udProjectCountry, *udPProjectCountry, **udPPProjectCountry;
	typedef class CProjectRRequest		udProjectRRequest, *udPProjectRRequest, **udPPProjectRRequest;
	typedef class CProjectSTour			udProjectSTour, *udPProjectSTour, **udPPProjectSTour;
	typedef class CProjectGroup			udProjectGroup, *udPProjectGroup, **udPPProjectGroup;
	typedef class CProjectTplTour		udProjectTplTour, *udPProjectTplTour, **udPPProjectTplTour;
	typedef class CProjectCity			udProjectCity, *udPProjectCity, **udPPProjectCity;
	typedef class CProjectClient		udProjectClient, *udPProjectClient, **udPPProjectClient;
	typedef class CProjectUser			udProjectUser, *udPProjectUser, **udPPProjectUser;
	typedef class CProjectSOrder		udProjectSOrder, *udPProjectSOrder, **udPPProjectSOrder;
	typedef class CProjectLog			udProjectLog, *udPProjectLog, **udPPProjectLog;
	typedef class CProjectClDoc			udProjectClDoc, *udPProjectClDoc, **udPPProjectClDoc;
	typedef class CProjectDocType		udProjectDocType, *udPProjectDocType, **udPPProjectDocType;
	typedef class CProjectCurrency		udProjectCurrency, *udPProjectCurrency, **udPPProjectCurrency;
	typedef class CProjectCurrencyPair	udProjectCurrencyPair, *udPProjectCurrencyPair, **udPPProjectCurrencyPair;
	typedef class CProjectCompany		udProjectCompany, *udPProjectCompany, *udPPProjectCompany;
	typedef class CProjectPartner		udProjectPartner, *udPProjectPartner, **udPPProjectPartner;
	typedef class CProjectClientUr		udProjectClientUr, *udPProjectClientUr, **udPPProjectClientUr;
	typedef class CProjectPersAcc		udProjectPersAcc, *udPProjectPersAcc, **udPPProjectPersAcc;
	typedef class CProjectPartnerReq	udProjectPartnerReq, *udPProjectPartnerReq, **udPPProjectPartnerReq;
	typedef class CProjectPartnerAnswr	udProjectPartnerAnswr, *udPProjectPartnerAnswr, **udPPProjectPartnerAnswr;
	typedef class CProjectAService		udProjectAService, *udPProjectAService, **udPPProjectAService;
	typedef class CProjectAttach		udProjectAttach, *udPProjectAttach, **udPPProjectAttach;
	typedef class CProjectPeriodAct		udProjectPeriodAct, *udPProjectPeriodAct, **udPPProjectPeriodAct;
	typedef class CProjectPriceTableItem	udProjectPriceTableItem, *udPProjectPriceTableItem, **udPPProjectPriceTableItem;
	typedef class CProjectSRequest		udProjectSRequest, *udPProjectSRequest, **udPPProjectSRequest;
	typedef class CProjectTourRoute		udProjectTourRoute, *udPProjectTourRoute, **udPPProjectTourRoute;
	typedef class CProjectTplTourRoute	udProjectTplTourRoute, *udPProjectTplTourRoute, **udPPProjectTplTourRoute;
	typedef class CProjectTplTourService	udProjectTplTourService, *udPProjectTplTourService, **udPPProjectTplTourService;
	typedef class CProjectTourService	udProjectTourService, *udPProjectTourService, **udPPProjectTourService;
	typedef class CProjectTourClient	udProjectTourClient, *udPProjectTourClient, **udPPProjectTourClient;
	typedef class CProjectTourTour		udProjectTourTour, *udPProjectTourTour, **udPPProjectTourTour;
	typedef class CProjectOrderRoute	udProjectOrderRoute, *udPProjectOrderRoute, **udPPProjectOrderRoute;
	typedef class CProjectOrderService	udProjectOrderService, *udPProjectOrderService, **udPPProjectOrderService;
	typedef class CProjectGroupTour		udProjectGroupTour, *udPProjectGroupTour, **udPPProjectGroupTour;
	typedef class CProjectGroupPeriod	udProjectGroupPeriod, *udPProjectGroupPeriod, **udPPProjectGroupPeriod;
	typedef class CProjectServicePeriod	udProjectServicePeriod, *udPProjectServicePeriod, **udPPProjectServicePeriod;
	typedef class CProjectGroupTplTour	udProjectGroupTplTour, *udPProjectGroupTplTour, **udPPProjectGroupTplTour;
	typedef class CProjectObjectLink	udProjectObjectLink, *udPProjectObjectLink, **udPPProjectObjectLink;
	typedef class CProjectReportGroup	udProjectReportGroup, *udPProjectReportGroup, **udPPProjectReportGroup;
	typedef class CProjectReportLanding	udProjectReportLanding, *udPProjectReportLanding, **udPPProjectReportLanding;
	typedef class CProjectReportFilling	udProjectReportFilling, *udPProjectReportFilling, **udPPProjectReportFilling;
	typedef class CProjectFinanceSumm		udProjectFinanceSumm, *udPProjectFinanceSumm, **udPPProjectFinanceSumm;
	typedef class CProjectFinanceOperation	udProjectFinanceOperation, *udPProjectFinanceOperation, **udPPProjectFinanceOperation;
	typedef class CProjectObjectLock		udProjectObjectLock, *udPProjectObjectLock, **udPPProjectObjectLock;
	typedef class CProjectFinanceSummAdd	udProjectFinanceSummAdd, *udPProjectFinanceSummAdd, **udPPProjectFinanceSummAdd;
	typedef class CProjectFinanceSummBack	udProjectFinanceSummBack, *udPProjectFinanceSummBack, **udPPProjectFinanceSummBack;
	typedef class CProjectFinanceSummConvert	udProjectFinanceSummConvert, *udPProjectFinanceSummConvert, **udPPProjectFinanceSummConvert;
	typedef class CProjectFinanceSummToPartner	udProjectFinanceSummToPartner, *udPProjectFinanceSummToPartner, **udPPProjectFinanceSummToPartner;
	typedef class CDlgFinanceHandler		udDlgFinanceHandler, *udPDlgFinanceHandler, **udPPDlgFinanceHandler;
	typedef class CDlgFinance				udDlgFinance, *udPDlgFinance, **udPPDlgFinance;
	typedef class CProjectDocTemplate		udProjectDocTemplate, *udPProjectDocTemplate, **udPPProjectDocTemplate;
	typedef class CProjectDocTemplatePrinted	udProjectDocTemplatePrinted, *udPProjectDocTemplatePrinted, **udPPProjectDocTemplatePrinted;
	typedef class CProjectTourPayOrder		udProjectTourPayOrder, *udPProjectTourPayOrder, **udPPProjectTourPayOrder;
	typedef class CProjectGroupContractApp1	udProjectGroupContractApp1, *udPProjectGroupContractApp1, **udPPProjectGroupContractApp1;
	typedef class CProjectGCTouristInfo		udProjectGCTouristInfo, *udPProjectGCTouristInfo, **udPPProjectGCTouristInfo;
	typedef class CProjectGCOccupancy		udProjectGCOccupancy, *udPProjectGCOccupancy, **udPPProjectGCOccupancy;
	typedef class CProjectGCProgram			udProjectGCProgram, *udPProjectGCProgram, **udPPProjectGCProgram;
	typedef class CProjectGCTransfer		udProjectGCTransfer, *udPProjectGCTransfer, **udPPProjectGCTransfer;
	typedef class CProjectGCCover			udProjectGCCover, *udPProjectGCCover, **udPPProjectGCCover;
	typedef class CProjectGCVisa			udProjectGCVisa, *udPProjectGCVisa, **udPPProjectGCVisa;
	typedef class CProjectGCService			udProjectGCService, *udPProjectGCService, **udPPProjectGCService;
	typedef class CProjectGCTransfer2		udProjectGCTransfer2, *udPProjectGCTransfer2, **udPPProjectGCTransfer2;
	typedef class CProjectTransferPoint		udProjectTransferPoint, *udPProjectTransferPoint, **udPPProjectTransferPoint;
	typedef class CProjectRoute				udProjectRoute, *udPProjectRoute, **udPPProjectRoute;
	typedef class CProjectHostSide			udProjectHostSide, *udPProjectHostSide, **udPPProjectHostSide;
	typedef class CProjectTouristList		udProjectTouristList, *udPProjectTouristList, **udPPProjectTouristList;
	typedef class CProjectTouristListLink	udProjectTouristListLink, *udPProjectTouristListLink, **udPPProjectTouristListLink;
	typedef class CProjectTourPrice			udProjectTourPrice, *udPProjectTourPrice, **udPPProjectTourPrice;
	typedef class CProjectReportGroupPay	udProjectReportGroupPay, *udPProjectReportGroupPay, **udPPProjectReportGroupPay;
	typedef class CProjectReportHotelPay	udProjectReportHotelPay, *udPProjectReportHotelPay, **udPPProjectReportHotelPay;
	typedef class CProjectReportDayPay		udProjectReportDayPay, *udPProjectReportDayPay, **udPPProjectReportDayPay;

	// функции получения объектов
	udPBase GetProjectTour( );
	udPBase GetProjectDirect( );
	udPBase GetProjectTRequest( );
	udPBase GetProjectTService( );
	udPBase GetProjectService( );
	udPBase GetProjectRequest( );
	udPBase GetProjectOrder( );
	udPBase	GetProjectNameService( );
	udPBase GetProjectMUService( );
	udPBase GetProjectCountry( );
	udPBase GetProjectRRequest( );
	udPBase GetProjectSTour( );
	udPBase GetProjectGroup( );
	udPBase GetProjectTplTour( );
	udPBase GetProjectCity( );
	udPBase GetProjectClient( );
	udPBase GetProjectUser( );
	udPBase GetProjectSOrder( );
	udPBase GetProjectLog( );
	udPBase GetProjectClDoc( );
	udPBase GetProjectDocType( );
	udPBase GetProjectCurrency( );
	udPBase GetProjectCurrencyPair( );
	udPBase GetProjectCompany( );
	udPBase GetProjectPartner( );
	udPBase GetProjectClientUr( );
	udPBase GetProjectPersAcc( );
	udPBase GetProjectPartnerReq( );
	udPBase GetProjectPartnerAnswr( );
	udPBase GetProjectAttach( );
	udPBase GetProjectPriceTableItem( );
	udPBase GetProjectSRequest( );
	udPBase GetProjectTourRoute( );
	udPBase GetProjectTplTourRoute( );
	udPBase GetProjectTplTourService( );
	udPBase GetProjectTourService( );
	udPBase GetProjectTourClient( );
	udPBase GetProjectTourTour( );
	udPBase GetProjectOrderRoute( );
	udPBase GetProjectOrderService( );
	udPBase GetProjectGroupTour( );
	udPBase GetProjectGroupPeriod( );
	udPBase GetProjectServicePeriod( );
	udPBase	GetProjectGroupTplTour( );
	udPBase	GetProjectReportGroup( );
	udPBase	GetProjectReportLanding( );
	udPBase	GetProjectReportFilling( );
	udPBase	GetProjectObjectLink( );
	udPBase	GetProjectObjectLock( );
	udPBase	GetProjectFinanceSumm( );
	udPBase	GetProjectFinanceOperation( );
	udPBase	GetProjectFinanceSummAdd( );
	udPBase	GetProjectFinanceSummBack( );
	udPBase	GetProjectFinanceSummConvert( );
	udPBase	GetProjectDocTemplate( );
	udPBase	GetProjectDocTemplatePrinted( );
	udPBase	GetProjectFinanceSummToPartner( );
	udPBase	GetProjectTourPayOrder( );
	udPBase	GetProjectGroupContractApp1( );
	udPBase	GetProjectGCTouristInfo( );
	udPBase	GetProjectGCOccupancy( );
	udPBase	GetProjectGCProgram( );
	udPBase	GetProjectGCTransfer( );
	udPBase	GetProjectGCCover( );
	udPBase	GetProjectGCVisa( );
	udPBase	GetProjectGCService( );
	udPBase	GetProjectGCTransfer2( );
	udPBase	GetProjectTransferPoint( );
	udPBase	GetProjectRoute( );
	udPBase	GetProjectHostSide( );
	udPBase	GetProjectTouristList( );
	udPBase	GetProjectTouristListLink( );
	udPBase	GetProjectTourPrice( );
	udPBase	GetProjectReportGroupPay( );
	udPBase	GetProjectReportHotelPay( );
	udPBase	GetProjectReportDayPay( );

} // namespace UdSDK

#endif
