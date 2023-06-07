
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_SUMM_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_SUMM_H

namespace UdSDK {

	class CWndSummHandler {

	public:
		CWndSummHandler( ) { }

		virtual ~CWndSummHandler( ) { }

		// вызывается в конце конструктора
		virtual void	AfterConstruct( udPWndSumm wndSumm ) = 0;
		// вызывается при нажатии ссылки "зачислить"
		virtual void	OnSummAdd( udPWndSumm wndSumm ) = 0;
		// вызывается при нажатии ссылки "снять"
		virtual void	OnSummBack( udPWndSumm wndSumm ) = 0;
		// вызывается при нажатии ссылки "конвертировать"
		virtual void	OnSummConvert( udPWndSumm wndSumm ) = 0;
		// вызывается в Create после создания всех окон
		virtual void	AfterCreate( udPWndSumm wndSumm ) = 0;

	};

	class CWndSumm : public udWndGraphic {
		// указатели на статики, для каждой строки
		typedef map< udDWord, udPWndStatic >	dwa_wrows;		// карта надписей
		typedef map< udUInt, udPWndStatic >		uwa_headers;	// карта надписей групп

		dwa_wrows			m_mapRows;		// статики каждой строки
		udDwAscBase			m_ascRows;		// данные по каждой строке
		udPWndSummHandler	m_pHandler;		// обработчик сообщений от окна сумм
		udPWndLink			m_lnkAdd,		// ссылка "зачислить"
							m_lnkBack,		// ссылка "снять"
							m_lnkConvert;	// ссылка "конвертировать"
		bool				m_bGrouping;	// группировать ли записи
		string				m_szGroupAttr,	// атрибут группировки (по UInt значению)
							m_szGroupTitle;	// атрибут заголовка группы (wstring значение)
		uwa_headers			m_ascGroups;	// заголовки групп

	public:
		CWndSumm( udPWndSummHandler handler = NULL );

		virtual ~CWndSumm( );

		udInt	AddRow( udArrBase& arr );
		udInt	ShowRow( udDWord index );
		void	ShowRows( );
		void	HideRows( );
		void	ClearRows( );
		udUInt	GetRowNum( ) const;

		const udDwAscBase&	GetRows( ) const { return m_ascRows; }

		virtual void	Render( );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetGrouping( bool bGrouping ) { m_bGrouping = bGrouping; }
		void	SetGroupAttr( const string& attr ) { m_szGroupAttr = attr; }
		void	SetGroupTitleAttr( const string& attr ) { m_szGroupTitle = attr; }
		void	SetHandler( udPWndSummHandler handler ) { m_pHandler = handler; }

		udPWndLink	GetLnkAdd( )		const { return m_lnkAdd;		}
		udPWndLink	GetLnkBack( )		const { return m_lnkBack;		}
		udPWndLink	GetLnkConvert( )	const { return m_lnkConvert;	}

	}; // class CWndSumm

	class CHSummAdd : public udFormHandler {
	public:
		enum {
			ctrlidSumm,			// выбор суммы
			ctrlidCurrency,		// выбор валюты
			ctrlidType,			// выбор типа (чер,бел)
			ctrlidHow,			// выбор способа оплаты (наличные,карта,безнал)
			ctrlidComment,		// комментарий
			ctrlidPartner,		// партнер
		};

	private:

	public:
		CHSummAdd( );

		virtual ~CHSummAdd( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectFinanceSummAdd; }

	}; // class CHSummAdd

	udPBase GetProjectFinanceSummBack( );

	class CHSummBack : public udFormHandler {
	public:
		enum {
			ctrlidSumm,			// выбор суммы
			ctrlidCurrency,		// выбор валюты
			ctrlidType,			// выбор типа (чер,бел)
			ctrlidHow,			// выбор способа оплаты (наличные,карта,безнал)
			ctrlidComment,		// комментарий
			ctrlidPartner,		// партнер
		};

	private:

	public:
		CHSummBack( );

		virtual ~CHSummBack( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectFinanceSummBack; }

	}; // class CHSummBack

	class CHSummConvert : public udFormHandler {
	public:
		enum {
			ctrlidSumm,			// сумма
			ctrlidCurrency1,	// валюта "из"
			ctrlidCurrency2,	// валюта "в"
			ctrlidType1,		// тип "из"
			ctrlidType2,		// тип "в"
			ctrlidComment,		// комментарий
			ctrlidPartner,		// партнер
		};

	private:

	public:
		CHSummConvert( );

		virtual ~CHSummConvert( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );
		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectFinanceSummConvert; }

	}; // class CHSummConvert



	///////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef class CHWndSummTourPrice	udHWndSummTourPrice, *udPHWndSummTourPrice, **udPPHWndSummTourPrice;
	typedef class CHWndSummTour			udHWndSummTour, *udPHWndSummTour, **udPPHWndSummTour;
	typedef class CHWndSummTourCost		udHWndSummTourCost, *udPHWndSummTourCost, **udPPHWndSummTourCost;
	typedef class CHWndSummService		udHWndSummService, *udPHWndSummService, **udPPHWndSummService;

	/**
		обработчик окна сумм итоговой цены тура
	*/
	class CHWndSummTourPrice : public udWndSummHandler {
		udPBase	m_pTour;

	public:
		CHWndSummTourPrice( udPBase pTour );

		virtual ~CHWndSummTourPrice( );

		virtual void	AfterConstruct( udPWndSumm wndSumm );
		virtual void	OnSummAdd( udPWndSumm wndSumm );
		virtual void	OnSummBack( udPWndSumm wndSumm );
		virtual void	OnSummConvert( udPWndSumm wndSumm );
		virtual void	AfterCreate( udPWndSumm wndSumm );

	}; // class CHWndSummTourPrice

	/**
		обработчик окна сумм оплата тура
	*/
	class CHWndSummTour : public udWndSummHandler {
		udPBase	m_pTour;

	public:
		CHWndSummTour( udPBase pTour );

		virtual ~CHWndSummTour( );

		virtual void	AfterConstruct( udPWndSumm wndSumm );
		virtual void	OnSummAdd( udPWndSumm wndSumm );
		virtual void	OnSummBack( udPWndSumm wndSumm );
		virtual void	OnSummConvert( udPWndSumm wndSumm );
		virtual void	AfterCreate( udPWndSumm wndSumm );

	}; // class CHWndSummTour

	/**
		обработчик окна сумм себестоимости
	*/
	class CHWndSummTourCost : public udWndSummHandler {
		udPBase			m_pTour;
		udPWndStatic	m_lblWarning;

	public:
		CHWndSummTourCost( udPBase pTour );

		virtual ~CHWndSummTourCost( );

		virtual void	AfterConstruct( udPWndSumm wndSumm );
		virtual void	OnSummAdd( udPWndSumm wndSumm );
		virtual void	OnSummBack( udPWndSumm wndSumm );
		virtual void	OnSummConvert( udPWndSumm wndSumm );
		virtual void	AfterCreate( udPWndSumm wndSumm );

	}; // class CHWndSummTourCost

	/**
		обработчик окна сумм оплата услуг
	*/
	class CHWndSummService : public udWndSummHandler {
		udPBase	m_pTour;

	public:
		CHWndSummService( udPBase pTour );

		virtual ~CHWndSummService( );

		virtual void	AfterConstruct( udPWndSumm wndSumm );
		virtual void	OnSummAdd( udPWndSumm wndSumm );
		virtual void	OnSummBack( udPWndSumm wndSumm );
		virtual void	OnSummConvert( udPWndSumm wndSumm );
		virtual void	AfterCreate( udPWndSumm wndSumm );

	}; // class CHWndSummTourCost

	class CWndLinkTourPayOrder : public udWndGraphic {
		udPWndLink	m_lnkPayOrder;
		udPBase		m_pTour;

	public:
		CWndLinkTourPayOrder( udPBase pTour );

		virtual ~CWndLinkTourPayOrder( );

		virtual void	Render( );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	}; // class CWndLinkTourPayOrder

	/**
		диалоговое окно финансов
	*/
	class CDlgFinance : public udWndGraphic {
		udPWndStatic		m_lblTourPrice,	// итоговая цена
							m_lblSummTour,	// оплата тура
							m_lblTourCost,	// себестоимость
							m_lblService;	// оплата услуг
		udPWndSummHandler	m_hTourPrice,	// итоговая цена
							m_hSummTour,	// оплата тура
							m_hTourCost,	// себестоимость
							m_hService;		// оплата услуг
		udPWndSumm			m_wndTourPrice,	// итоговая цена
							m_wndSummTour,	// оплата тура
							m_wndTourCost,	// себестоимость
							m_wndService;	// оплата услуг
		udPBase				m_pTour;		// указатель на тур

		CWndLinkTourPayOrder	*m_wndLnkPayOrder;	// окно со ссылкой на оплату

	public:
		CDlgFinance( udPBase pTour );

		~CDlgFinance( );

		void	LoadData( );

		virtual void	Render( );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );

		static void	start( HWND hWndParent, udPBase pTour );

	}; /// class CWndSumm2


} // namespace UdSDK

#endif
