
#ifndef PROJECTSERVICE_H
#define PROJECTSERVICE_H

namespace UdSDK {

	void ServiceAfterLoadPeriodList( udArrBase& arr );

	class CHService : public udStateHandlerInfo {
	public:
		enum {
			ctrlidName,
			ctrlidType,
			ctrlidCountry,
			ctrlidCity,
			ctrlidPeriod,
			ctrlidPricetable,
			ctrlidPartner,
			ctrlidMunit,
		};

		enum {
			fltName,
			fltName1,
			fltType,
			fltPartner,

			LIST_CONTROL_ID_LAST = fltPartner
		};

		enum {
			opidGet2 = opidGet + 1,	// получение услуг по ID
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHService( );
		CHService( const CHService& objCopy );

		virtual ~CHService( );

		CHService& operator =( const CHService& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual void	ClearFilter( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	LoadExtraData( udPSocket sock, udPBase obj, udPWndModeForm wndForm );
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );

		void	GetServicesById( map< udUInt, udUInt > ids, udArrBase& arrResult );

	}; // class CHCountry



	class CHServicePeriod : public udListHandler, public udFormHandler {
		enum {
			ctrlidDate1,
			ctrlidDate2,
		};

	public:
		CHServicePeriod( );

		virtual ~CHServicePeriod( );

		virtual void	LoadList( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	ListOk( );
		virtual udInt	ListCancel( );
		virtual void	OnLinkClick( udPWndLink lnk );
		virtual void	OnSelectRow( udLong row );
		virtual void	OnHeaderClick( const string& name, const string& attr );
		virtual void	OnPagerSelect( udUInt page );
		virtual void	OnButtonClick( udPWndButton btn );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectServicePeriod; }

	}; // class CHServicePeriod

} // namespace UdSDK

#endif
