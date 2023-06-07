/*
	Tourist application
	Клиентское приложение
	Услуги в заявке
*/
#ifndef TOURIST_CLIENT_ORDER_SERVICE_H
#define TOURIST_CLIENT_ORDER_SERVICE_H

namespace UdSDK {

	udPBase	GetProjectOrderServiceList( );

	class CHOrderServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblComment,
						m_lblDate1,
						m_lblDate2,
						m_lblType,
						m_lblCity;
		udWgtInput		m_inpComment;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udWgtListPick	m_inpType,
						m_inpCity;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidType,
			ctrlidCity
		};

	public:
		CHOrderServiceForm( );
		CHOrderServiceForm( const CHOrderServiceForm& objCopy );

		virtual ~CHOrderServiceForm( );

		CHOrderServiceForm& operator =( const CHOrderServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHOrderServiceForm

	class CHOrderServiceFormModal : public CHOrderServiceForm {

	public:
		CHOrderServiceFormModal( );
		CHOrderServiceFormModal( const CHOrderServiceFormModal& objCopy );

		virtual ~CHOrderServiceFormModal( );

		CHOrderServiceFormModal& operator =( const CHOrderServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHOrderServiceFormModal

} // namespace UdSDK

#endif
