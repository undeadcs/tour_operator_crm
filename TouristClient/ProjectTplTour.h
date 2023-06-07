
#ifndef PROJECTTPLTOUR_H
#define PROJECTTPLTOUR_H

namespace UdSDK {

	void TplTourAssignTplRoute( udPBase tpl );
	void TplTourAssignTplService( udPBase tpl );

	class CHTplTour : public udStateHandlerSimple {
	public:
		enum {
			ctrlidName,
			ctrlidRoute,
			ctrlidService,
			ctrlidCost,
			ctrlidPrice,
			ctrlidDirect,
			ctrlidDaynum,
			ctrlidDoc,
			ctrlidApp
		};

		enum {
			fltName,
			fltDirect,
			fltTour,

			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndLink	m_lnkTour,	// ссылка создания тура из шаблона
					m_lnkPrint;	// ссылка распечатать

	public:
		CHTplTour( );
		CHTplTour( const CHTplTour& objCopy );

		virtual ~CHTplTour( );

		CHTplTour& operator =( const CHTplTour& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	Init( );
		virtual void	Clear( );
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );

	}; // class CHTplTour





	class CHTplTourService : public udFormHandler {
	public:
		enum {
			ctrlidService,
			ctrlidDate1,
			ctrlidDate2,
			ctrlidNum,
		};

	private:

	public:
		CHTplTourService( );

		virtual ~CHTplTourService( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual void	LoadAfterSetObject( );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectTplTourService; }

	}; // class CHTplTourService

} // namespace UdSDK

#endif
