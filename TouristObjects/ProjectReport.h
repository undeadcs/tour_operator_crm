/**
	Tourist application
	Набор классов объектов

	отчеты
*/
#ifndef TOURIST_APPLICATION_OBJECTS_REPORT_H
#define TOURIST_APPLICATION_OBJECTS_REPORT_H

namespace UdSDK {

	class CProjectReportGroup : public udBase {

	public:
		CProjectReportGroup( );

		virtual ~CProjectReportGroup( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectReportGroup

	class CProjectReportLanding : public udBase {

	public:
		CProjectReportLanding( );

		virtual ~CProjectReportLanding( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectReportLanding

	class CProjectReportFilling : public udBase {

	public:
		CProjectReportFilling( );

		virtual ~CProjectReportFilling( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectReportFilling

	class CProjectReportGroupPay : public udBase {

	public:
		CProjectReportGroupPay( );

		virtual ~CProjectReportGroupPay( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectReportGroupPay

	class CProjectReportHotelPay : public udBase {

	public:
		CProjectReportHotelPay( );

		virtual ~CProjectReportHotelPay( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectReportHotelPay

	class CProjectReportDayPay : public udBase {

	public:
		CProjectReportDayPay( );

		virtual ~CProjectReportDayPay( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectReportDayPay

} // namespace UdSDK

#endif
