
#ifndef PROJECTTOUR_H
#define PROJECTTOUR_H

namespace UdSDK {

	void	TourAssignTplService( udPBase obj );
	void	TourAssignTplClient( udPBase obj );
	void	TourAssignTplTourLink( udPBase obj );
	void	TourAssignTplRoute( udPBase obj );
	bool	TourSpecLoadEmployer( udPBase main, udPBase& obj, udPWndModeForm form, udPSocket sock );
	void	TourAfterPickEmployer( udPBase obj, udPListHandler handler );
	void	TourAfterPickEmployer2( udPBase obj, udPListHandler handler );

	class CHTour : public udStateHandlerSimple {
	public:
		enum {
			ctrlidName,
			ctrlidDate1,
			ctrlidDate2,
			ctrlidRoute,
			ctrlidService,
			ctrlidCost,
			ctrlidPrice,
			ctrlidApp,
			ctrlidSeller,
			ctrlidState,
			ctrlidStatus,
			ctrlidEmployer,
			ctrlidDirect,
			ctrlidTourist,
			ctrlidManager,
			ctrlidComment,
			ctrlidTour,
		};

		enum {
			fltName,
			fltDirect,
			fltDate1,
			fltDate2,
			fltStatus,
			fltEmployer,
			fltOrder,
			fltTplTour,
			fltManager,

			LIST_CONTROL_ID_LAST
		};

		enum {
			opidSummAdd = opidGet + 1,	// операция добавления суммы
			opidSummBack,				// снятие
			opidSummConvert,			// конвертация
			opidSummLoad,				// загрузка сумм
		};

	private:
		udPWndLink	m_lnkFinance,		// финансы
					m_lnkDocTpl,		// распечатка шаблона документа
					m_lnkApp1,			// Доп. информация - аля Приложение 1 для распечатки гр.договор
					m_lnkPrint;			// распечатка формы
		udPBase		m_pApp1;			// приложение 1

	public:
		CHTour( );
		CHTour( const CHTour& objCopy );

		virtual ~CHTour( );

		CHTour& operator =( const CHTour& objCopy );

		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	Init( );
		virtual void	Clear( );
		virtual void	ClickToolbar( string index );
		virtual udInt	ToolbarAction( string index );
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );
		virtual void	LoadAfterSetObject( ); // вызывается в модальном режиме
		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual void	SwitchToForm( ); // вызывается в не модальном режиме
		virtual void	RequestList( );
		virtual void	AfterLoadFilterData( );

	private:
		void	MakeAppData( );
		void	ClearAppData( );

	}; // class CHTour



	class CHTourService : public udFormHandler {
	public:
		enum {
			ctrlidService,
			ctrlidDate1,
			ctrlidDate2,
			ctrlidNum,
			ctrlidReady,
		};

	private:

	public:
		CHTourService( );

		virtual ~CHTourService( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual void	LoadAfterSetObject( );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectTourService; }

	}; // class CHTourService


	class CHTourPrice : public udFormHandler {
	public:
		enum {
			ctrlidAmount,
		};

	public:
		CHTourPrice( );

		virtual ~CHTourPrice( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectTour; }

	}; // class CHTourPrice

/*
	udPBase GetProjectGroupContractF( );	// для формы

	class CHGroupContractF : public udFormHandler {
	public:
		enum {
			ctrlidDays1,	// За сколько дней до поездки предоставить документы (не позднее)
			ctrlidDays2,	// В течение скольки дней после бронирования подтвердить тур
			ctrlidDays3,	// В течение скольки дней после бронирования оплатить тур (не позднее)
		};

	public:
		CHGroupContractF( );

		virtual ~CHGroupContractF( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGroupContractF; }

	}; // class CHGroupContractF

	udPBase GetProjectGroupContractApp2F( );	// для формы

	class CHGroupContractApp2F : public udFormHandler {
	public:
		enum {
			ctrlidName,			// название туроператора
			ctrlidAddr,			// адрес
			ctrlidNumber,		// реестровый номер
			ctrlidFinType,		// Вид и размер финансового обеспечения
			ctrlidGarant,		// дата и срок действия договора страхования ответственности туроператора или банковской гарантии
			ctrlidOrgName,		// наименование организации, предоставившей финансовое обеспечение
			ctrlidGarantAddr,	// адрес гаранта
		};

	public:
		CHGroupContractApp2F( );

		virtual ~CHGroupContractApp2F( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGroupContractApp2F; }

	}; // class CHGroupContractApp2F
*/
} // namespace UdSDK

#endif
