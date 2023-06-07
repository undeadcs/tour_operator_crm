/*
	Tourist application
	Клиентское приложение
	Цена услуги
*/
#ifndef TOURIST_CLIENT_SERVICE_PRICE_H
#define TOURIST_CLIENT_SERVICE_PRICE_H

namespace UdSDK {

	udPBase GetProjectServicePriceList( );

	class CHServicePriceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblPeriod,
						m_lblNum,
						m_lblAmount,
						m_lblCurrency,
						m_lblAmountRecommend,
						m_lblCurrencyRecommend;
		udWgtInput		m_inpNum,
						m_inpAmount,
						m_inpAmountRecommend;
		udWgtServicePeriodPick
						m_inpPeriod;
		udWgtListPick	m_inpCurrency,
						m_inpCurrencyRecommend;
		Gtk::Table		m_tblInput;

		udPHServiceForm	m_pService;

	public:
		enum {
			ctrlidPeriod,
			ctrlidCurrency,
			ctrlidCurrencyRecommend
		};

	public:
		CHServicePriceForm( udPHServiceForm pService = NULL );
		CHServicePriceForm( const CHServicePriceForm& objCopy );

		virtual ~CHServicePriceForm( );

		CHServicePriceForm& operator =( const CHServicePriceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		void	SetService( udPHServiceForm pService ) { m_pService = pService; }

	}; // class CHServicePriceForm

	class CHServicePriceFormModal : public CHServicePriceForm {

	public:
		CHServicePriceFormModal( udPHServiceForm pService = NULL );
		CHServicePriceFormModal( const CHServicePriceFormModal& objCopy );

		virtual ~CHServicePriceFormModal( );

		CHServicePriceFormModal& operator =( const CHServicePriceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHServicePriceFormModal

} // namespace UdSDK

#endif
