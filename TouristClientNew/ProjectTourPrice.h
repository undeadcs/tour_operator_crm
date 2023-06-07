/*
	Tourist application
	Клиентское приложение
	Цена за тур
*/
#ifndef TOURIST_CLIENT_TOUR_PRICE_H
#define TOURIST_CLIENT_TOUR_PRICE_H

namespace UdSDK {

	udPBase	GetProjectTourPriceList( );

	class CHTourPriceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblAmount;
		udWgtInput		m_inpAmount;
		Gtk::Table		m_tblInput;


	public:
		CHTourPriceForm( );
		CHTourPriceForm( const CHTourPriceForm& objCopy );

		virtual ~CHTourPriceForm( );

		CHTourPriceForm& operator =( const CHTourPriceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourPriceForm

	class CHTourPriceFormModal : public CHTourPriceForm {

	public:
		CHTourPriceFormModal( );
		CHTourPriceFormModal( const CHTourPriceFormModal& objCopy );

		virtual ~CHTourPriceFormModal( );

		CHTourPriceFormModal& operator =( const CHTourPriceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourPriceFormModal

} // namespace UdSDK

#endif
