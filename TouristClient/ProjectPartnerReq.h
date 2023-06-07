
#ifndef PROJECTPARTNERREQ_H
#define PROJECTPARTNERREQ_H

namespace UdSDK {
	
	class CHPartnerReq : public udStateHandlerInfo {
	public:
		enum {
			ctrlidPartner,
			ctrlidComment,
			ctrlidText
		};

		enum {
			fltPartner,
			fltDate1,
			fltDate2,
			fltManager,
			fltOrder,

			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHPartnerReq( );
		CHPartnerReq( const CHPartnerReq& objCopy );

		virtual ~CHPartnerReq( );

		CHPartnerReq& operator =( const CHPartnerReq& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	LoadAfterSetObject( );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	RequestList( );

	}; // class CHPartnerReq

} // namespace UdSDK

#endif
