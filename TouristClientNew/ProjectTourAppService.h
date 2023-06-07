/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, другие услуги
*/
#ifndef TOURIST_CLIENT_TOUR_APP_SERVICE_H
#define TOURIST_CLIENT_TOUR_APP_SERVICE_H

namespace UdSDK {

	udPBase GetProjectTourAppServiceList( );

	class CHTourAppServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblChar;
		udWgtInput		m_inpName,
						m_inpChar;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppServiceForm( );
		CHTourAppServiceForm( const CHTourAppServiceForm& objCopy );

		virtual ~CHTourAppServiceForm( );

		CHTourAppServiceForm& operator =( const CHTourAppServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppServiceForm

	class CHTourAppServiceFormModal : public CHTourAppServiceForm {

	public:
		CHTourAppServiceFormModal( );
		CHTourAppServiceFormModal( const CHTourAppServiceFormModal& objCopy );

		virtual ~CHTourAppServiceFormModal( );

		CHTourAppServiceFormModal& operator =( const CHTourAppServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppServiceFormModal

} // namespace UdSDK

#endif
