/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, услуги по перевозке
*/
#ifndef TOURIST_CLIENT_TOUR_APP_TRANSFER_SERVICE_H
#define TOURIST_CLIENT_TOUR_APP_TRANSFER_SERVICE_H

namespace UdSDK {

	udPBase GetProjectTourAppTransferServiceList( );

	class CHTourAppTransferServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblFio,
						m_lblTrans,
						m_lblLevel,
						m_lblDate1,
						m_lblDate2;
		udWgtInput		m_inpFio,
						m_inpTrans,
						m_inpLevel;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppTransferServiceForm( );
		CHTourAppTransferServiceForm( const CHTourAppTransferServiceForm& objCopy );

		virtual ~CHTourAppTransferServiceForm( );

		CHTourAppTransferServiceForm& operator =( const CHTourAppTransferServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppTransferServiceForm

	class CHTourAppTransferServiceFormModal : public CHTourAppTransferServiceForm {

	public:
		CHTourAppTransferServiceFormModal( );
		CHTourAppTransferServiceFormModal( const CHTourAppTransferServiceFormModal& objCopy );

		virtual ~CHTourAppTransferServiceFormModal( );

		CHTourAppTransferServiceFormModal& operator =( const CHTourAppTransferServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppTransferServiceFormModal

} // namespace UdSDK

#endif
