/**
	Tourist application
	Набор классов объектов

	приложение 1 к туру при распечатке группового договора
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TOUR_APPS_H
#define TOURIST_APPLICATION_OBJECTS_TOUR_APPS_H

namespace UdSDK {

	class CProjectGroupContractApp1 : public udBase {

	public:
		CProjectGroupContractApp1( );

		virtual ~CProjectGroupContractApp1( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGroupContractApp1

	class CProjectGCTouristInfo : public udBase {

	public:
		CProjectGCTouristInfo( );

		virtual ~CProjectGCTouristInfo( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCTouristInfo

	class CProjectGCOccupancy : public udBase {

	public:
		CProjectGCOccupancy( );

		virtual ~CProjectGCOccupancy( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCOccupancy

	class CProjectGCProgram : public udBase {

	public:
		CProjectGCProgram( );

		virtual ~CProjectGCProgram( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCProgram

	class CProjectGCTransfer : public udBase {

	public:
		CProjectGCTransfer( );

		virtual ~CProjectGCTransfer( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCTransfer

	class CProjectGCCover : public udBase {

	public:
		CProjectGCCover( );

		virtual ~CProjectGCCover( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCCover

	class CProjectGCVisa : public udBase {

	public:
		CProjectGCVisa( );

		virtual ~CProjectGCVisa( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCVisa

	class CProjectGCService : public udBase {

	public:
		CProjectGCService( );

		virtual ~CProjectGCService( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCService

	class CProjectGCTransfer2 : public udBase {

	public:
		CProjectGCTransfer2( );

		virtual ~CProjectGCTransfer2( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectGCTransfer2

} // namespace UdSDK

#endif
