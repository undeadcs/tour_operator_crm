/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, визовое оформление
*/
#ifndef TOURIST_CLIENT_TOUR_APP_VISA_H
#define TOURIST_CLIENT_TOUR_APP_VISA_H

namespace UdSDK {

	udPBase GetProjectTourAppVisaList( );

	class CHTourAppVisaForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblType;
		udWgtInput		m_inpType;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppVisaForm( );
		CHTourAppVisaForm( const CHTourAppVisaForm& objCopy );

		virtual ~CHTourAppVisaForm( );

		CHTourAppVisaForm& operator =( const CHTourAppVisaForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppVisaForm

	class CHTourAppVisaFormModal : public CHTourAppVisaForm {

	public:
		CHTourAppVisaFormModal( );
		CHTourAppVisaFormModal( const CHTourAppVisaFormModal& objCopy );

		virtual ~CHTourAppVisaFormModal( );

		CHTourAppVisaFormModal& operator =( const CHTourAppVisaFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppVisaFormModal

} // namespace UdSDK

#endif
