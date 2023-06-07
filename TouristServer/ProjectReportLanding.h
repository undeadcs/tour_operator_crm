/*
	Tourist application
	Серверное приложение
	Отчет "Посадочная ведомость"
*/
#ifndef TOURIST_SERVER_REPORT_LANDING_H
#define TOURIST_SERVER_REPORT_LANDING_H

namespace UdSDK {

	class CHReportLanding : public udStateHandler {
		udPMorph	m_pObjFilter;

	public:
		CHReportLanding( );
		CHReportLanding( const CHReportLanding& objCopy );

		virtual ~CHReportLanding( );

		CHReportLanding& operator =( const CHReportLanding& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

	}; // class CHReport

} // namespace UdSDK

#endif
