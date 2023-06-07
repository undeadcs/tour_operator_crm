
#ifndef PROJECTPARTNER_H
#define PROJECTPARTNER_H

namespace UdSDK {

	class CHPartner : public udStateHandlerInfo {
	public:
		enum {
			ctrlidName,
			ctrlidDfio,
			ctrlidCfio,
			ctrlidEmail,
			ctrlidPhone,
			ctrlidFax,
			ctrlidComment
		};

		enum {
			fltName,
			fltCfio,
			fltEmail,
			fltPhone,

			LIST_CONTROL_ID_LAST = fltPhone
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHPartner( );
		CHPartner( const CHPartner& objCopy );

		virtual ~CHPartner( );

		CHPartner& operator =( const CHPartner& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );

	}; // class CHPartner

} // namespace UdSDK

#endif
