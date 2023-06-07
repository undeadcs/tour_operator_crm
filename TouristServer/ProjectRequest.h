
#ifndef PROJECTREQUEST_H
#define PROJECTREQUEST_H

namespace UdSDK {

	udPBase GetProjectRequest2( );

	class CHRequest : public udStateHandlerSimple {

	public:
		CHRequest( );
		CHRequest( const CHRequest& objCopy );

		virtual ~CHRequest( );

		CHRequest& operator =( const CHRequest& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHRequest

} // namespace UdSDK

#endif
