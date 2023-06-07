
#ifndef PROJECTLOG_H
#define PROJECTLOG_H

namespace UdSDK {

	udPBase	GetProjectLog2( );

	class CHLog : public udStateHandlerSimple {

	public:
		CHLog( );
		CHLog( const CHLog& objCopy );

		virtual ~CHLog( );

		CHLog& operator =( const CHLog& objCopy );

		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHLog

} // namespace UdSDK

#endif
