/*
	Tourist application
	Клиентское приложение
	Приложение 1 к туру, средство размещения
*/
#ifndef TOURIST_CLIENT_TOUR_APP_OCCUPANCY_H
#define TOURIST_CLIENT_TOUR_APP_OCCUPANCY_H

namespace UdSDK {

	udPBase GetProjectTourAppOccupancyList( );

	class CHTourAppOccupancyForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblCategory,
						m_lblAmount,
						m_lblFeed;
		udWgtInput		m_inpName,
						m_inpCategory,
						m_inpAmount,
						m_inpFeed;
		Gtk::Table		m_tblInput;


	public:
		CHTourAppOccupancyForm( );
		CHTourAppOccupancyForm( const CHTourAppOccupancyForm& objCopy );

		virtual ~CHTourAppOccupancyForm( );

		CHTourAppOccupancyForm& operator =( const CHTourAppOccupancyForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTourAppOccupancyForm

	class CHTourAppOccupancyFormModal : public CHTourAppOccupancyForm {

	public:
		CHTourAppOccupancyFormModal( );
		CHTourAppOccupancyFormModal( const CHTourAppOccupancyFormModal& objCopy );

		virtual ~CHTourAppOccupancyFormModal( );

		CHTourAppOccupancyFormModal& operator =( const CHTourAppOccupancyFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHTourAppOccupancyFormModal

} // namespace UdSDK

#endif
