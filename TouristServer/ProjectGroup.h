
#ifndef PROJECTGROUP_H
#define PROJECTGROUP_H

namespace UdSDK {

	udPBase	GetProjectGroup2( );

	class CHGroup : public udStateHandlerSimple {
	public:
		enum {
			opidGetTouristList = opidGet + 1, // загрузка списка туристов по периоду
		};

	public:
		CHGroup( );
		CHGroup( const CHGroup& objCopy );

		virtual ~CHGroup( );

		CHGroup& operator =( const CHGroup& objCopy );

		virtual void	Init( );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		//virtual void	AfterLoadList( udArrBase& arr );
		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );
		virtual void	SchemeCenter( udInt opId, udPSocket sock );

		void	DeleteLinkedObjects( udPBase obj );

		void	OpGetTouristList( udPSocket sock );

	}; // class CHGroup

} // namespace UdSDK

#endif
