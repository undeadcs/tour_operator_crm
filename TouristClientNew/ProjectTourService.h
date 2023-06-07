/*
	Tourist application
	Клиентское приложение
	Услуга тура
*/
#ifndef TOURIST_CLIENT_TOUR_SERVICE_H
#define TOURIST_CLIENT_TOUR_SERVICE_H

namespace UdSDK {

	udPBase	GetProjectTourServiceList( );

	class CHTourServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblService,
						m_lblNum,
						m_lblDate1,
						m_lblDate2,
						m_lblReady;
		udWgtInput		m_inpNum;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udWgtServicePick
						m_inpService;
		udWgtSelect		m_inpReady;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidService
		};

	public:
		CHTourServiceForm( );
		CHTourServiceForm( const CHTourServiceForm& objCopy );

		virtual ~CHTourServiceForm( );

		CHTourServiceForm& operator =( const CHTourServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourServiceForm

	class CHTourServiceFormModal : public CHTourServiceForm {

	public:
		CHTourServiceFormModal( );
		CHTourServiceFormModal( const CHTourServiceFormModal& objCopy );

		virtual ~CHTourServiceFormModal( );

		CHTourServiceFormModal& operator =( const CHTourServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourServiceFormModal

} // namespace UdSDK

#endif
