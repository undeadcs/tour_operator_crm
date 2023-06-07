/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, страхование
*/
#ifndef TOURIST_CLIENT_TOUR_APP_COVER_H
#define TOURIST_CLIENT_TOUR_APP_COVER_H

namespace UdSDK {

	udPBase GetProjectTourAppCoverList( );

	class CHTourAppCoverForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblType,
						m_lblName;
		udWgtInput		m_inpType,
						m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppCoverForm( );
		CHTourAppCoverForm( const CHTourAppCoverForm& objCopy );

		virtual ~CHTourAppCoverForm( );

		CHTourAppCoverForm& operator =( const CHTourAppCoverForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppCoverForm

	class CHTourAppCoverFormModal : public CHTourAppCoverForm {

	public:
		CHTourAppCoverFormModal( );
		CHTourAppCoverFormModal( const CHTourAppCoverFormModal& objCopy );

		virtual ~CHTourAppCoverFormModal( );

		CHTourAppCoverFormModal& operator =( const CHTourAppCoverFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppCoverFormModal

} // namespace UdSDK

#endif
