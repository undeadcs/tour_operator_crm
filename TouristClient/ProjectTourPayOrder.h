/**
	Tourist application
	Клиентское приложение

	указание на оплату по туру
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_TOUR_PAY_ORDER_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_TOUR_PAY_ORDER_H

namespace UdSDK {

	class CHTourPayOrder : public udStateHandlerInfo, public udWndSummHandler {
	public:
		enum {
			ctrlidTour,
			ctrlidState,
			ctrlidSumm,
			ctrlidLabelEmployer,
			ctrlidLabelCompany,
		};

		enum {
			fltDate1,
			fltDate2,
			fltTour,

			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndSumm		m_wndSumm;
		udPWndLink		m_lnkEmployer,	// ссылка на заказчика
						m_lnkCompany,	// ссылка на компанию
						m_lnkPersAcc;	// ссылка на лицевой счет
		udPBase			m_pTour,		// тур
						m_pEmployer,	// заказчик
						m_pCompany,		// компания
						m_pPersAcc;		// лицевой счет

	public:
		CHTourPayOrder( );
		CHTourPayOrder( const CHTourPayOrder& objCopy );

		virtual ~CHTourPayOrder( );

		CHTourPayOrder& operator =( const CHTourPayOrder& objCopy );

		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );
		//virtual void	OnSelectRow( udLong row );
		//virtual udInt	ProcForm( bool modal = false );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );
		virtual void	RequestList( );
		virtual void	LinkClick( udPWndLink lnk );
		virtual void	SwitchToForm( );
		// обработчик сообщений окна сумм
		virtual void	AfterConstruct( udPWndSumm wndSumm );
		virtual void	OnSummAdd( udPWndSumm wndSumm );
		virtual void	OnSummBack( udPWndSumm wndSumm );
		virtual void	OnSummConvert( udPWndSumm wndSumm );
		virtual void	AfterCreate( udPWndSumm wndSumm );

		udPWndSumm	GetWndSumm( ) const { return m_wndSumm; }

	}; // class CHTourPayOrder

} // namespace UdSDK

#endif
