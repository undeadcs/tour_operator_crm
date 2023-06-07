#ifndef PROJECTORDER_H
#define PROJECTORDER_H

namespace UdSDK {

	bool	OrderSpecLoadEmployer( udPBase main, udPBase& obj, udPWndModeForm form, udPSocket sock );
	void	OrderAfterPickEmployer( udPBase obj, udPListHandler handler );

	class CHOrder : public udStateHandlerSimple {
	public:
		enum {
			ctrlidManager,
			ctrlidStatus,
			ctrlidState,
			ctrlidComment,
			ctrlidApp,
			ctrlidAmount,
			ctrlidRoute,
			ctrlidEmployer,
			ctrlidDate1,
			ctrlidDate2,
			ctrlidService,
			ctrlidDirect,
		};

		enum {
			fltDate1,
			fltDate2,
			fltManager,
			fltStatus,
			fltAmount1,
			fltAmount2,
			fltEmployer,
			fltDirect,
			fltRequest,
			fltTour,
			// последний id
			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndLink	m_lnkTour,			// ссылка на форму создания тура из заявки
					m_lnkPartnerReq,	// ссылка на форму создания запроса партнеру	
					m_lnkPrint;			// ссылка рспечатки формы

	public:
		CHOrder( );
		CHOrder( const CHOrder& objCopy );

		virtual ~CHOrder( );

		CHOrder& operator =( const CHOrder& objCopy );

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
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );
		virtual void	AfterLoadFilterData( );
		virtual void	LoadAfterSetObject( );
		virtual void	SwitchToForm( );
		virtual void	RequestList( );

	}; // class CHOrder




	class CHOrderService : public udFormHandler {
	public:
		enum {
			ctrlidDate1,
			ctrlidDate2,
			ctrlidType,
			ctrlidCity,
			ctrlidComment
		};

	public:
		CHOrderService( );

		virtual ~CHOrderService( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual void	LoadAfterSetObject( );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectOrderService; }

	}; // class CHOrderService

} // namespace UdSDK

#endif
