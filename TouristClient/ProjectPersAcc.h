
#ifndef PROJECTPERSACC_H
#define PROJECTPERSACC_H

namespace UdSDK {

	class CHPersAcc : public udStateHandlerSimple, public CWndSummHandler {
	public:
		enum {
			opidSummAdd = opidGet + 1,	// операция добавления суммы
			opidSummBack,				// снятие
			opidSummConvert,			// конвертация
		};

		enum {
			fltCompany,	// компания
			fltPartner,	// партнер
			fltClient,	// клиент
		};

	private:
		udPWndSumm			m_wndSumm;
		udPWndOperations	m_wndOp;

	public:
		CHPersAcc( );
		CHPersAcc( const CHPersAcc& objCopy );

		virtual ~CHPersAcc( );

		CHPersAcc& operator =( const CHPersAcc& objCopy );

		virtual void	Clear( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	OnSelectRow( udLong row );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );

		void	LoadAccSumm( udPSocket sock );
		void	LoadAccOp( udPSocket sock );

		virtual void	OnSummAdd( udPWndSumm wndSumm );
		virtual void	OnSummBack( udPWndSumm wndSumm );
		virtual void	OnSummConvert( udPWndSumm wndSumm );
		virtual void	AfterConstruct( udPWndSumm wndSumm ) { }
		virtual void	AfterCreate( udPWndSumm wndSumm ) { }
/*
		virtual bool	ValidateObj( udPBase obj );
		virtual void	CreateWindowModeList( udPWndModel wnd );
		virtual void	CreateWindowModeForm( udPWndModel wnd );
*/
	}; // class CHPersAcc

} // namespace UdSDK

#endif
