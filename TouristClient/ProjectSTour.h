
#ifndef PROJECTSTOUR_H
#define PROJECTSTOUR_H

namespace UdSDK {

	class CHSTour : public udStateHandlerSimpleOneName {

	public:
		CHSTour( );
		CHSTour( const CHSTour& objCopy );

		virtual ~CHSTour( );

		CHSTour& operator =( const CHSTour& objCopy );

	}; // class CHSTour

} // namespace UdSDK

#endif
