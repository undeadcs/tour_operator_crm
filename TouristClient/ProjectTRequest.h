
#ifndef PROJECTTREQUEST_H
#define PROJECTTREQUEST_H

namespace UdSDK {

	class CHTRequest : public udStateHandlerSimpleOneName {

	public:
		CHTRequest( );
		CHTRequest( const CHTRequest& objCopy );

		virtual ~CHTRequest( );

		CHTRequest& operator =( const CHTRequest& objCopy );

	}; // class CHTRequest

} // namespace UdSDK

#endif
