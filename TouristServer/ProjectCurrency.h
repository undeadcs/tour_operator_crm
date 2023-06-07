
#ifndef PROJECTCURRENCY_H
#define PROJECTCURRENCY_H

namespace UdSDK {

	class CHCurrency : public udStateHandlerSimple {

	public:
		CHCurrency( );
		CHCurrency( const CHCurrency& objCopy );

		virtual ~CHCurrency( );

		CHCurrency& operator =( const CHCurrency& objCopy );

		virtual string	GetWhereCondition( );

	}; // class CHCurrency

} // namespace UdSDK

#endif
