
#ifndef PROJECTPARTNERANSWR_H
#define PROJECTPARTNERANSWR_H

namespace UdSDK {

	udPBase	GetProjectPartnerAnswr2( );

	class CHPartnerAnswr : public udStateHandlerSimple {

	public:
		CHPartnerAnswr( );
		CHPartnerAnswr( const CHPartnerAnswr& objCopy );

		virtual ~CHPartnerAnswr( );

		CHPartnerAnswr& operator =( const CHPartnerAnswr& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHPartnerAnswr

} // namespace UdSDK

#endif
