
#ifndef PROJECTRREQUEST_H
#define PROJECTRREQUEST_H

namespace UdSDK {

	class CHRRequest : public udStateHandlerSimpleOneName {

	public:
		CHRRequest( );
		CHRRequest( const CHRRequest& objCopy );

		virtual ~CHRRequest( );

		CHRRequest& operator =( const CHRRequest& objCopy );

	}; // class CHRRequest

} // namespace UdSDK

#endif
