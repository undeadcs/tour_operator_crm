
#ifndef PROJECT_INFOHANDLER_H
#define PROJECT_INFOHANDLER_H

namespace UdSDK {

	class CHInfoPage : public udStateHandler {

	public:
		CHInfoPage( );
		CHInfoPage( const CHInfoPage& objCopy );

		virtual ~CHInfoPage( );

		CHInfoPage& operator =( const CHInfoPage& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

	}; // class CHInfoPage

} // namespace UdSDK

#endif
