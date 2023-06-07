
#ifndef PROJECTCLDOC_H
#define PROJECTCLDOC_H

namespace UdSDK {

	udPBase GetProjectClDoc2( );

	class CHClDoc : public udStateHandlerSimple {

	public:
		CHClDoc( );
		CHClDoc( const CHClDoc& objCopy );

		virtual ~CHClDoc( );

		CHClDoc& operator =( const CHClDoc& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual void	AfterLoadList( udArrBase& arr );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHClDoc

} // namespace UdSDK

#endif
