/*
*/
#ifndef PROJECTCITY_H
#define PROJECTCITY_H

namespace UdSDK {

	udPBase GetProjectCity2( );

	class CHCity : public udStateHandlerSimple {
	public:
		enum {
			opidGet2 = opidGet + 1,	// выборка городов по набору ID
		};

	public:
		CHCity( );
		CHCity( const CHCity& objCopy );

		virtual ~CHCity( );

		CHCity& operator =( const CHCity& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		//virtual void	AfterLoadList( udArrBase& arr );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );
		virtual void	SchemeCenter( udInt opId, udPSocket sock );

	}; // class CHCity

} // namespace UdSDK

#endif
