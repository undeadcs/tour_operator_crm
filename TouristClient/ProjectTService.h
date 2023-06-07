
#ifndef PROJECTTSERVICE_H
#define PROJECTTSERVICE_H

namespace UdSDK {

	class CHTService : public udStateHandlerSimpleOneName {

	public:
		CHTService( );
		CHTService( const CHTService& objCopy );

		virtual ~CHTService( );

		CHTService& operator =( const CHTService& objCopy );

	}; // class CHTService

} // namespace UdSDK

#endif
