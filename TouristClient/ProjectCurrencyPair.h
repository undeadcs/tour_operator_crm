
#ifndef PROJECTCURRENCYPAIR_H
#define PROJECTCURRENCYPAIR_H

namespace UdSDK {

	class CHCurrencyPair : public udStateHandlerInfo {
	public:
		enum {
			ctrlidC1,
			ctrlidC2,
			ctrlidCb,
			ctrlidSet,
			ctrlidInternal
		};

		enum {
			fltDate1,
			fltDate2,
			fltCourse1,
			fltCourse2,
			fltCb1,
			fltCb2,

			LIST_CONTROL_ID_LAST = fltCb2
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHCurrencyPair( );
		CHCurrencyPair( const CHCurrencyPair& objCopy );

		virtual ~CHCurrencyPair( );

		CHCurrencyPair& operator =( const CHCurrencyPair& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	AfterLoadFilterData( );
		virtual void	RequestList( );
		
	}; // class CHCurrencyPair

} // namespace UdSDK

#endif
