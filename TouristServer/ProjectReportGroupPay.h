/**
	Tourist application
	Серверное приложение
	Отчет "Оплата группы"
*/
#ifndef TOURIST_SERVER_REPORT_GROUP_PAY_H
#define TOURIST_SERVER_REPORT_GROUP_PAY_H

namespace UdSDK {

	class CHReportGroupPay : public udStateHandler {
		udPMorph	m_pObjFilter;

	public:
		CHReportGroupPay( );
		CHReportGroupPay( const CHReportGroupPay& objCopy );

		virtual ~CHReportGroupPay( );

		CHReportGroupPay& operator =( const CHReportGroupPay& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

	}; // class CHReport

} // namespace UdSDK

#endif