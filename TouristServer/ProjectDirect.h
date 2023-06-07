
#ifndef PROJECTDIRECT_H
#define PROJECTDIRECT_H

namespace UdSDK {

	class CHDirect : public udStateHandlerSimpleOneName {

	public:
		CHDirect( );
		CHDirect( const CHDirect& objCopy );

		virtual ~CHDirect( );

		CHDirect& operator =( const CHDirect& objCopy );

	}; // class CHDirect

} // namespace UdSDK

#endif
