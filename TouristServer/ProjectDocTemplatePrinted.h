/**
	Tourist application
	Серверная часть

	распечатка шаблона документа
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_DOC_TEMPLATE_PRINTED_H
#define TOURIST_APPLICATION_TOURIST_SERVER_DOC_TEMPLATE_PRINTED_H

namespace UdSDK {

	udPBase	GetProjectDocTemplatePrinted2( );

	class CHDocTemplatePrinted : public udStateHandlerSimple {

	public:
		CHDocTemplatePrinted( );
		CHDocTemplatePrinted( const CHDocTemplatePrinted& objCopy );

		virtual ~CHDocTemplatePrinted( );

		CHDocTemplatePrinted& operator =( const CHDocTemplatePrinted& objCopy );

		virtual void	Init( );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHCountry

} // namespace UdSDK

#endif