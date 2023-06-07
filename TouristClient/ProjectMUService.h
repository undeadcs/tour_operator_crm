
#ifndef PROJECTMUSERVICE_H
#define PROJECTMUSERVICE_H

namespace UdSDK {

	class CHMUService : public udStateHandlerSimpleOneName {

	public:
		CHMUService( );
		CHMUService( const CHMUService& objCopy );

		virtual ~CHMUService( );

		CHMUService& operator =( const CHMUService& objCopy );

	}; // class CHMUService

} // namespace UdSDK

#endif
