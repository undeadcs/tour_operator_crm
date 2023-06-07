
#ifndef PROJECTREQUEST_H
#define PROJECTREQUEST_H

namespace UdSDK {

	class CHRequest : public udStateHandlerSimple {
	public:
		enum {
			ctrlidType,
			ctrlidStatus,
			ctrlidState,
			ctrlidFio,
			ctrlidContact,
			ctrlidDirect,
			ctrlidManager,
			ctrlidResult,
			ctrlidComment,
			ctrlidRemind,
			ctrlidRemindc
		};

		enum {
			fltDate1,
			fltDate2,
			fltType,
			fltFio,
			fltDirect,
			fltManager,
			fltOrder,

			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndLink	m_lnkOrder,	// ссылка создания заявки на основе обращения
					m_lnkPrint;	// ссылка распечатки
		
	public:
		CHRequest( );
		CHRequest( const CHRequest& objCopy );

		virtual ~CHRequest( );

		CHRequest& operator =( const CHRequest& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual udInt	ToolbarAction( string index );
		virtual void	ClickToolbar( string index );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	LoadAfterSetObject( );
		virtual void	SwitchToForm( );
		virtual void	RequestList( );

	}; // class CHRequest

} // namespace UdSDK

#endif
