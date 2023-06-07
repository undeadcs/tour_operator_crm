
#ifndef PROJECTCURRENCY_H
#define PROJECTCURRENCY_H

namespace UdSDK {

	class CHCurrency : public udStateHandlerInfo {
	public:
		enum {
			ctrlidName,
			ctrlidCode
		};

		enum {
			fltName,
			fltCode,

			LIST_CONTROL_ID_LAST = fltCode
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHCurrency( );
		CHCurrency( const CHCurrency& objCopy );

		virtual ~CHCurrency( );

		CHCurrency& operator =( const CHCurrency& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual void	ClearFilter( );

	}; // class CHCurrency

} // namespace UdSDK

#endif
