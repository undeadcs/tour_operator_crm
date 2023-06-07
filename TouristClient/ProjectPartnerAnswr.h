
#ifndef PROJECTPARTNERANSWR_H
#define PROJECTPARTNERANSWR_H

namespace UdSDK {

	class CHPartnerAnswr : public udStateHandlerInfo {
	public:
		enum {
			ctrlidPartner,
			ctrlidText
		};

		enum {
			fltPartner,
			fltDate1,
			fltDate2,

			LIST_CONTROL_ID_LAST = fltDate2
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHPartnerAnswr( );
		CHPartnerAnswr( const CHPartnerAnswr& objCopy );

		virtual ~CHPartnerAnswr( );

		CHPartnerAnswr& operator =( const CHPartnerAnswr& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );

	}; // class CHPartnerAnswr

} // namespace UdSDK

#endif
