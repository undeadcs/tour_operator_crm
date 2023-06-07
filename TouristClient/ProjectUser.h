
#ifndef PROJECTUSER_H
#define PROJECTUSER_H

namespace UdSDK {

	class CHUser : public udStateHandlerSimple {
	public:
		enum {
			ctrlidLogin,
			ctrlidPassword,
			ctrlidEmail,
			ctrlidFio,
			ctrlidRank,
			ctrlidSeries
		};

		enum {
			fltLogin,
			fltEmail,
			fltFio,
			fltRank,

			LIST_CONTROL_ID_LAST = fltRank
		};

	private:
		udPWndLink	m_lnkPrint;	// ссылка распечатать

	public:
		CHUser( );
		CHUser( const CHUser& objCopy );

		virtual ~CHUser( );

		CHUser& operator =( const CHUser& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	ClearFilter( );
		virtual void	AfterLoadFilterData( );

	}; // class CHUser

} // namespace UdSDK

#endif
