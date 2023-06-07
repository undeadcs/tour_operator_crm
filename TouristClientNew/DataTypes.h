/*
	Tourist application
	Клиентское приложение
	Типы данных
*/
#ifndef TOURIST_CLIENT_DATA_TYPES_H
#define TOURIST_CLIENT_DATA_TYPES_H

#define forward_class( name )	typedef class C##name			ud##name##, *udP##name##, **udPP##name##;
#define forward_handler( name ) forward_class(H##name##)
#define forward_form( name ) forward_class(H##name##Form) forward_class(H##name##FormModal) forward_class(H##name##FormModeless)
#define forward_get_list_handler_pick( name ) udPListHandlerPick GetH##name##ListPick( );
#define forward_get_form_handler( name ) udPFormHandler GetH##name##Form( );
#define forward_handler_full( name ) forward_handler( name ) forward_handler( name##Modeless ) forward_handler( name##Pick ) forward_form( name ) forward_get_form_handler( name )

namespace UdSDK {

	typedef class CAppInitProject	udAppInitProject, *udPAppInitProject, **udPPAppInitProject;
	typedef class CHMain			udHMain, *udPHMain, **udPPHMain;

	forward_handler( Reference );
	forward_handler( Report );

	forward_handler_full( Order );
	forward_handler_full( Request );
	forward_handler_full( Tour );
	forward_handler_full( TplTour );
	forward_handler_full( Group );
	forward_handler_full( User );
	forward_handler_full( Company );
	forward_handler_full( Partner );
	forward_handler_full( Service );
	forward_handler_full( Client );
	forward_handler_full( Country );
	forward_handler_full( TService );
	forward_handler_full( SOrder );
	forward_handler_full( RRequest );
	forward_handler_full( STour );
	forward_handler_full( TRequest );
	forward_handler_full( Direct );
	forward_handler_full( NameService );
	forward_handler_full( DocType );
	forward_handler_full( SRequest );
	forward_handler_full( MUService );
	forward_handler_full( Route );
	forward_handler_full( TransferPoint );
	forward_handler_full( HostSide );
	forward_handler_full( ClientUr );
	forward_handler_full( City );
	forward_handler_full( Currency );
	forward_handler_full( CurrencyPair );
	forward_handler_full( TouristList );
	forward_handler_full( ClDoc );
	forward_handler_full( PartnerReq );
	forward_handler_full( PartnerAnswr );
	forward_handler_full( DocTemplate );
	forward_handler_full( DocTemplatePrinted );
	forward_handler_full( PersAcc );
	forward_handler_full( TourPayOrder );

	forward_get_list_handler_pick( Country );
	forward_get_list_handler_pick( Currency );
	forward_get_list_handler_pick( SRequest );
	forward_get_list_handler_pick( TRequest );
	forward_get_list_handler_pick( Direct );
	forward_get_list_handler_pick( User );
	forward_get_list_handler_pick( RRequest );
	forward_get_list_handler_pick( Client );
	forward_get_list_handler_pick( DocType );
	forward_get_list_handler_pick( Partner );
	forward_get_list_handler_pick( HostSide );
	forward_get_list_handler_pick( MUService );
	forward_get_list_handler_pick( NameService );
	forward_get_list_handler_pick( Route );
	forward_get_list_handler_pick( SOrder );
	forward_get_list_handler_pick( STour );
	forward_get_list_handler_pick( TransferPoint );
	forward_get_list_handler_pick( TService );
	forward_get_list_handler_pick( City );
	forward_get_list_handler_pick( PartnerAnswr );
	forward_get_list_handler_pick( PartnerReq );
	forward_get_list_handler_pick( CurrencyPair );
	forward_get_list_handler_pick( Request );
	forward_get_list_handler_pick( ClientUr );
	forward_get_list_handler_pick( ClDoc );
	forward_get_list_handler_pick( Order );
	forward_get_list_handler_pick( Service );
	forward_get_list_handler_pick( TplTour );
	forward_get_list_handler_pick( Tour );
	forward_get_list_handler_pick( Company );
	forward_get_list_handler_pick( DocTemplate );
	forward_get_list_handler_pick( DocTemplatePrinted );
	forward_get_list_handler_pick( TouristList );
	forward_get_list_handler_pick( PersAcc );
	forward_get_list_handler_pick( TourPayOrder );
	forward_get_list_handler_pick( Group );

	forward_class( HOrderRouteForm );
	forward_class( HOrderRouteFormModal );
	forward_class( HOrderServiceForm );
	forward_class( HOrderServiceFormModal );

	forward_handler( EmployerPick );
	forward_get_list_handler_pick( Employer );
	forward_class( WgtEmployerPick );
	forward_class( FDEEmployer );
	forward_class( WgtEmployerDocPick );
	forward_get_list_handler_pick( EmployerDoc );

	typedef enum {
		emptypeFiz,	// физ. лицо
		emptypeUr	// юр. лицо
	} employerType;

	forward_handler( ServicePeriod );
	forward_handler( ServicePeriodPick );
	forward_class( HServicePeriodForm );
	forward_class( HServicePeriodFormModal );
	forward_get_list_handler_pick( ServicePeriod );
	forward_class( WgtServicePeriodPick );
	forward_class( DAListServicePeriod );

	forward_class( HServicePriceForm );
	forward_class( HServicePriceFormModal );

	forward_class( HTplTourServiceForm );
	forward_class( HTplTourServiceFormModal );

	forward_class( WgtServicePick );

	forward_handler( GroupPeriod );
	forward_handler( GroupPeriodPick );
	forward_class( HGroupPeriodForm );
	forward_class( HGroupPeriodFormModal );
	forward_get_list_handler_pick( GroupPeriod );
	forward_class( WgtGroupPeriodPick );
	forward_class( DAListGroupPeriod );

	forward_handler( GroupPeriodLoad );
	forward_handler( GroupPeriodLoadPick );
	forward_get_list_handler_pick( GroupPeriodLoad );

	forward_class( HGroupTourForm );
	forward_class( HGroupTourFormModal );

	forward_class( HTourServiceForm );
	forward_class( HTourServiceFormModal );

	forward_class( HTourAppForm );
	forward_class( FDETourApp );

	forward_class( HTourAppOccupancyForm );
	forward_class( HTourAppOccupancyFormModal );
	forward_class( HTourAppProgramForm );
	forward_class( HTourAppProgramFormModal );
	forward_class( HTourAppTransferServiceForm );
	forward_class( HTourAppTransferServiceFormModal );
	forward_class( HTourAppCoverForm );
	forward_class( HTourAppCoverFormModal );
	forward_class( HTourAppVisaForm );
	forward_class( HTourAppVisaFormModal );
	forward_class( HTourAppServiceForm );
	forward_class( HTourAppServiceFormModal );
	forward_class( HTourAppTransferForm );
	forward_class( HTourAppTransferFormModal );

	forward_handler( Log );
	forward_handler( LogModeless );

	forward_class( HTourPriceForm );
	forward_class( HTourPriceFormModal );

	forward_class( DAListTourist );
	forward_handler( Tourist );
	forward_handler( TouristPick );

	forward_handler( EmployerDoc );
	forward_handler( EmployerDocPick );

	forward_get_list_handler_pick( TouristListLeader );
	forward_class( WgtTouristListLeaderPick );
	forward_class( DAListTouristListLeader );
	forward_handler( TouristListLeader );
	forward_handler( TouristListLeaderPick );

	forward_class( FDEPersAccObject );

	forward_class( HFinanceSummAddForm );
	forward_class( HFinanceSummAddFormModal );
	forward_class( HFinanceSummBackForm );
	forward_class( HFinanceSummBackFormModal );
	forward_class( HFinanceSummConvertForm );
	forward_class( HFinanceSummConvertFormModal );

	forward_class( HTourFinanceForm );
	forward_class( FDETourFinance );

	forward_class( HFinanceSummAddTourServiceForm );
	forward_class( HFinanceSummAddTourServiceFormModal );
	forward_class( HFinanceSummBackTourServiceForm );
	forward_class( HFinanceSummBackTourServiceFormModal );
	forward_class( HFinanceSummConvertTourServiceForm );
	forward_class( HFinanceSummConvertTourServiceFormModal );

	forward_class( HTourPayOrderFinanceSummAddFormModal );
	forward_class( HTourPayOrderFinanceSummBackFormModal );
	forward_class( HTourPayOrderFinanceSummConvertFormModal );

	forward_class( FDETourPayOperation );

	forward_class( DALocal );

	forward_class( DAReportLanding );
	forward_handler( ReportLanding );
	forward_handler( ReportLandingModeless )

	forward_handler( ReportFilling );
	forward_handler( ReportFillingModeless );

	forward_class( DAReportGroupPay );
	forward_handler( ReportGroupPay );
	forward_handler( ReportGroupPayModeless );

	forward_class( DAReportDayPay );
	forward_handler( ReportDayPay );
	forward_handler( ReportDayPayModeless );

	forward_class( WgtGroupPeriodLoadPick );
	forward_class( DAReportHotelPay );
	forward_handler( ReportHotelPay );
	forward_handler( ReportHotelPayModeless );

	forward_class( WgtClDocFromForm );
	forward_class( DAListClDocFromForm );
	forward_handler( ClDocFromFormPick );
	forward_get_list_handler_pick( ClDocFromForm );

} // namespace UdSDK

#endif
