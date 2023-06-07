/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, трансфер
*/
#ifndef TOURIST_CLIENT_TOUR_APP_TRANSFER_H
#define TOURIST_CLIENT_TOUR_APP_TRANSFER_H

namespace UdSDK {

	udPBase GetProjectTourAppTransferList( );

	class CHTourAppTransferForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblRoute,
						m_lblType;
		udWgtInput		m_inpRoute,
						m_inpType;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppTransferForm( );
		CHTourAppTransferForm( const CHTourAppTransferForm& objCopy );

		virtual ~CHTourAppTransferForm( );

		CHTourAppTransferForm& operator =( const CHTourAppTransferForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppTransferForm

	class CHTourAppTransferFormModal : public CHTourAppTransferForm {

	public:
		CHTourAppTransferFormModal( );
		CHTourAppTransferFormModal( const CHTourAppTransferFormModal& objCopy );

		virtual ~CHTourAppTransferFormModal( );

		CHTourAppTransferFormModal& operator =( const CHTourAppTransferFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppTransferFormModal

} // namespace UdSDK

#endif
