/**
	Tourist application
	Серверная часть
	Отчет "Оплата за день"
*/
#ifndef TOURIST_SERVER_REPORT_DAY_PAY_H
#define TOURIST_SERVER_REPORT_DAY_PAY_H

namespace UdSDK {

	class CHReportDayPay : public udStateHandler {
		udPMorph	m_pObjFilter;

	public:
		CHReportDayPay( );
		CHReportDayPay( const CHReportDayPay& objCopy );

		virtual ~CHReportDayPay( );

		CHReportDayPay& operator =( const CHReportDayPay& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

	}; // class CHReport

} // namespace UdSDK

#endif
