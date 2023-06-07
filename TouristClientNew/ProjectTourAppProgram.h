/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, Экскурсионная программа
*/
#ifndef TOURIST_CLIENT_TOUR_APP_PROGRAM_H
#define TOURIST_CLIENT_TOUR_APP_PROGRAM_H

namespace UdSDK {

	udPBase GetProjectTourAppProgramList( );

	class CHTourAppProgramForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppProgramForm( );
		CHTourAppProgramForm( const CHTourAppProgramForm& objCopy );

		virtual ~CHTourAppProgramForm( );

		CHTourAppProgramForm& operator =( const CHTourAppProgramForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppProgramForm

	class CHTourAppProgramFormModal : public CHTourAppProgramForm {

	public:
		CHTourAppProgramFormModal( );
		CHTourAppProgramFormModal( const CHTourAppProgramFormModal& objCopy );

		virtual ~CHTourAppProgramFormModal( );

		CHTourAppProgramFormModal& operator =( const CHTourAppProgramFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppProgramFormModal

} // namespace UdSDK

#endif
