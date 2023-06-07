
#ifndef PROJECTSERVICE_H
#define PROJECTSERVICE_H

namespace UdSDK {

	udPBase GetProjectService2( );

	class CHService : public udStateHandlerSimple {
	public:
		enum {
			opidGet2 = opidGet + 1,	// получение услуг по ID
		};

	private:

	public:
		CHService( );
		CHService( const CHService& objCopy );

		virtual ~CHService( );

		CHService& operator =( const CHService& objCopy );

		virtual void	SchemeCenter( udInt opId, udPSocket sock );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual udInt	SyncAdd( udPSocket sock, udPBase obj );
		virtual udInt	SyncUpd( udPSocket sock, udPBase obj );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );

	}; // class CHCountry

} // namespace UdSDK

#endif
