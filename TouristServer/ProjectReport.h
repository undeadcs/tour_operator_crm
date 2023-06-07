/**
	Tourist application
	Серверная часть

	отчеты
*/
#ifndef TOURIST_APPLICATION_SERVER_REPORTS_H
#define TOURIST_APPLICATION_SERVER_REPORTS_H

namespace UdSDK {

	class CHReport : public udStateHandler {
	public:
		enum {
			opidLanding,
			opidFilling,
		};

	private:
		udPMorph	m_pObjFilter;

	public:
		CHReport( );
		CHReport( const CHReport& objCopy );

		virtual ~CHReport( );

		CHReport& operator =( const CHReport& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

		void	BuildQuery( string& query, udInt opid );

	}; // class CHReport

} // namespace UdSDK

#endif
