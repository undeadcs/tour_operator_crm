
#ifndef PROJECT_SREQUEST_H
#define PROJECT_SREQUEST_H

namespace UdSDK {

	class CHSRequest : public udStateHandlerSimpleOneName {

	public:
		CHSRequest( );
		CHSRequest( const CHSRequest& objCopy );

		virtual ~CHSRequest( );

		CHSRequest& operator =( const CHSRequest& objCopy );

	}; // class CHSRequest

} // namespace UdSDK

#endif
