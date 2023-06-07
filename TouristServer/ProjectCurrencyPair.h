
#ifndef PROJECTCURRENCYPAIR_H
#define PROJECTCURRENCYPAIR_H

namespace UdSDK {

	udPBase	GetProjectCurrencyPair2( );

	class CHCurrencyPair : public udStateHandlerSimple {

	public:
		CHCurrencyPair( );
		CHCurrencyPair( const CHCurrencyPair& objCopy );

		virtual ~CHCurrencyPair( );

		CHCurrencyPair& operator =( const CHCurrencyPair& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		//virtual void	AfterLoadList( udArrBase& arr );
		//virtual void	AppendDataEdt( udBinaryData& data, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHCurrencyPair

} // namespace UdSDK

#endif
