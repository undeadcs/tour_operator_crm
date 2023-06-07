/*
	Tourist application
	Клиентское приложение
	Услуга шаблона тура
*/
#ifndef TOURIST_CLIENT_TPL_TOUR_SERVICE_H
#define TOURIST_CLIENT_TPL_TOUR_SERVICE_H

namespace UdSDK {

	udPBase	GetProjectTplTourServiceList( );

	class CHTplTourServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblService,
						m_lblNum,
						m_lblDate1,
						m_lblDate2;
		udWgtInput		m_inpNum;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udWgtServicePick
						m_inpService;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidService
		};

	public:
		CHTplTourServiceForm( );
		CHTplTourServiceForm( const CHTplTourServiceForm& objCopy );

		virtual ~CHTplTourServiceForm( );

		CHTplTourServiceForm& operator =( const CHTplTourServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTplTourServiceForm

	class CHTplTourServiceFormModal : public CHTplTourServiceForm {

	public:
		CHTplTourServiceFormModal( );
		CHTplTourServiceFormModal( const CHTplTourServiceFormModal& objCopy );

		virtual ~CHTplTourServiceFormModal( );

		CHTplTourServiceFormModal& operator =( const CHTplTourServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTplTourServiceFormModal

} // namespace UdSDK

#endif
