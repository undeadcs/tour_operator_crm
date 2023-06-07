
#ifndef PROJECTCOMPANY_H
#define PROJECTCOMPANY_H

namespace UdSDK {

	class CHCompany : public udStateHandlerInfo {
	public:
		enum {
			ctrlidName,
			ctrlidEmail,
			ctrlidInn,
			ctrlidUaddr,
			ctrlidPaddr
		};

		enum {
			fltName,
			fltEmail,

			LIST_CONTROL_ID_LAST = fltEmail
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHCompany( );
		CHCompany( const CHCompany& objCopy );

		virtual ~CHCompany( );

		CHCompany& operator =( const CHCompany& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual void	ClearFilter( );

	}; // class CHCompany

} // namespace UdSDK

#endif
