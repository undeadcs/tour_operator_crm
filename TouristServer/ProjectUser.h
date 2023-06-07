
#ifndef PROJECTUSER_H
#define PROJECTUSER_H

namespace UdSDK {

	udPBase	GetProjectUser2( );

	class CHUser : public udStateHandlerSimple {

	public:
		CHUser( );
		CHUser( const CHUser& objCopy );

		virtual ~CHUser( );

		CHUser& operator =( const CHUser& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual udInt	UpdObj( udPBase obj );
		//virtual void	AfterLoadList( udArrBase& arr );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHUser

} // namespace UdSDK

#endif
