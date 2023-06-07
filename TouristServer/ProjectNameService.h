
#ifndef PROJECTNAMESERVICE_H
#define PROJECTNAMESERVICE_H

namespace UdSDK {

	class CHNameService : public udStateHandlerSimpleOneName {

	public:
		CHNameService( );
		CHNameService( const CHNameService& objCopy );

		virtual ~CHNameService( );

		CHNameService& operator =( const CHNameService& objCopy );

	}; // class CHNameService

} // namespace UdSDK

#endif
