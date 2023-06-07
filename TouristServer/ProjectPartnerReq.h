
#ifndef PROJECTPARTNERREQ_H
#define PROJECTPARTNERREQ_H

namespace UdSDK {

	udPBase	GetProjectPartnerReq2( );
	
	class CHPartnerReq : public udStateHandlerSimple {

	public:
		CHPartnerReq( );
		CHPartnerReq( const CHPartnerReq& objCopy );

		virtual ~CHPartnerReq( );

		CHPartnerReq& operator =( const CHPartnerReq& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );

	}; // class CHPartnerReq

} // namespace UdSDK

#endif
