/**
	Tourist application
	Серверное приложение
	Отчет "Оплата гостиниц"
*/
#ifndef TOURIST_SERVER_REPORT_HOTEL_PAY_H
#define TOURIST_SERVER_REPORT_HOTEL_PAY_H

namespace UdSDK {

	class CHReportHotelPay : public udStateHandler {
		udPMorph	m_pObjFilter;

	public:
		CHReportHotelPay( );
		CHReportHotelPay( const CHReportHotelPay& objCopy );

		virtual ~CHReportHotelPay( );

		CHReportHotelPay& operator =( const CHReportHotelPay& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

	}; // class CHReport

} // namespace UdSDK

#endif