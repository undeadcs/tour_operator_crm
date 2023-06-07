#ifndef PROJECTORDER_H
#define PROJECTORDER_H

namespace UdSDK {

	udPBase OrderSpecAppendEmployer( udBinaryData& data, udPBase obj );
	udPBase	GetProjectOrder2( );

	class CHOrder : public udStateHandlerSimple {

	public:
		CHOrder( );
		CHOrder( const CHOrder& objCopy );

		virtual ~CHOrder( );

		CHOrder& operator =( const CHOrder& objCopy );

		virtual void	Init( );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		//virtual void	AfterLoadList( udArrBase& arr );
		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );

	}; // class CHOrder

} // namespace UdSDK

#endif
