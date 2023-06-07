/**
	Tourist application
	Серверное приложение

	указание на оплату тура
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_TOUR_PAY_ORDER_H
#define TOURIST_APPLICATION_TOURIST_SERVER_TOUR_PAY_ORDER_H

namespace UdSDK {

	udPBase	GetProjectTourPayOrder2( );

	class CHTourPayOrder : public udStateHandlerSimple {

	public:
		CHTourPayOrder( );
		CHTourPayOrder( const CHTourPayOrder& objCopy );

		virtual ~CHTourPayOrder( );

		CHTourPayOrder& operator =( const CHTourPayOrder& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual string	GetWhereCondition( );

	}; // class CHTourPayOrder

} // namespace UdSDK

#endif
