/*
	Tourist application
	Клиентское приложение
	Тур в группе
*/
#ifndef TOURIST_CLIENT_GROUP_TOUR_H
#define TOURIST_CLIENT_GROUP_TOUR_H

namespace UdSDK {

	udPBase GetProjectGroupTourList( );

	class CHGroupTourForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblTour,
						m_lblPeriod;
		udWgtListPick	m_inpTour;
		udWgtGroupPeriodPick
						m_inpPeriod;
		Gtk::Table		m_tblInput;

		udPHGroupForm	m_hGroup;

	public:
		enum {
			ctrlidTour,
			ctrlidPeriod
		};

	public:
		CHGroupTourForm( udPHGroupForm hGroup = NULL );
		CHGroupTourForm( const CHGroupTourForm& objCopy );

		virtual ~CHGroupTourForm( );

		CHGroupTourForm& operator =( const CHGroupTourForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		void	SetGroup( udPHGroupForm hGroup ) { m_hGroup = hGroup; }

	}; // class CHGroupTourForm

	class CHGroupTourFormModal : public CHGroupTourForm {

	public:
		CHGroupTourFormModal( udPHGroupForm hGroup = NULL );
		CHGroupTourFormModal( const CHGroupTourFormModal& objCopy );

		virtual ~CHGroupTourFormModal( );

		CHGroupTourFormModal& operator =( const CHGroupTourFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHGroupTourFormModal

} // namespace UdSDK

#endif
