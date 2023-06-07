
#ifndef PROJECTGROUP_H
#define PROJECTGROUP_H

namespace UdSDK {

	class CHGroup : public udStateHandlerSimple {
	public:
		enum {
			ctrlidName,
			ctrlidComment,
			ctrlidDirect,
			ctrlidApp,
			ctrlidTour,
			ctrlidPeriod,
			ctrlidTpltour,
		};

		enum {
			fltName,

			LIST_CONTROL_ID_LAST = fltName
		};

	private:
		udPWndLink	m_lnkPrint;	// ссылка распечатать

	public:
		CHGroup( );
		CHGroup( const CHGroup& objCopy );

		virtual ~CHGroup( );

		CHGroup& operator =( const CHGroup& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );

	}; // class CHGroup



	class CHGroupPeriod : public udListHandler, public udFormHandler {
	public:
		enum {
			ctrlidDate1,
			ctrlidMin,
			ctrlidMax,
		};

	private:

	public:
		CHGroupPeriod( );

		virtual ~CHGroupPeriod( );

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

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGroupPeriod; }

	}; // class CHGroupPeriod


	class CHGroupTour : public udFormHandler {
	public:
		enum {
			ctrlidPeriod,
			ctrlidTour,
		};

	private:

	public:
		CHGroupTour( );

		virtual ~CHGroupTour( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual void	LoadAfterSetObject( );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGroupTour; }

	}; // class CHGroupTour

} // namespace UdSDK

#endif
