
#ifndef PROJECTTPLTOUR_H
#define PROJECTTPLTOUR_H

namespace UdSDK {

	udPBase	GetProjectTplTour2( );

	class CHTplTour : public udStateHandlerSimple {

	public:
		CHTplTour( );
		CHTplTour( const CHTplTour& objCopy );

		virtual ~CHTplTour( );

		CHTplTour& operator =( const CHTplTour& objCopy );

		virtual void	Init( );
		//virtual void	AfterLoadList( udArrBase& arr );
		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

		void	DeleteLinkedObjects( udPBase obj );

	}; // class CHTplTour

} // namespace UdSDK

#endif
