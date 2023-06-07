/**
	Tourist application
	Клиентское приложение
	Маршрут заявки
*/
#ifndef TOURIST_CLIENT_ORDER_ROUTE_H
#define TOURIST_CLIENT_ORDER_ROUTE_H

namespace UdSDK {

	udPBase	GetProjectOrderRouteList( );

	class CHOrderRouteForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHOrderRouteForm( );
		CHOrderRouteForm( const CHOrderRouteForm& objCopy );

		virtual ~CHOrderRouteForm( );

		CHOrderRouteForm& operator =( const CHOrderRouteForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHOrderRouteForm

	class CHOrderRouteFormModal : public CHOrderRouteForm {

	public:
		CHOrderRouteFormModal( );
		CHOrderRouteFormModal( const CHOrderRouteFormModal& objCopy );

		virtual ~CHOrderRouteFormModal( );

		CHOrderRouteFormModal& operator =( const CHOrderRouteFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

		void	LoadFrom( udPBase from );

	}; // class CHOrderRouteFormModal

} // namespace UdSDK

#endif
