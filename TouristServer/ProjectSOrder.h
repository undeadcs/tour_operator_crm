
#ifndef PROJECTSORDER_H
#define PROJECTSORDER_H

namespace UdSDK {

	class CHSOrder : public udStateHandlerSimpleOneName {

	public:
		CHSOrder( );
		CHSOrder( const CHSOrder& objCopy );

		virtual ~CHSOrder( );

		CHSOrder& operator =( const CHSOrder& objCopy );

	}; // class CHSOrder

} // namespace UdSDK

#endif
