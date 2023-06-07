/*
	Tourist application
	Клиентское приложение
	Маршрут
*/
#ifndef TOURIST_CLIENT_ROUTE_H
#define TOURIST_CLIENT_ROUTE_H

namespace UdSDK {

	udPBase	GetProjectRouteList( );

	class CHRoute : public udListHandlerBtnForm, public udListHandler {

	public:
		CHRoute( );
		CHRoute( const CHRoute& objCopy );

		virtual ~CHRoute( );

		CHRoute& operator =( const CHRoute& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelRoute : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelRoute( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelRoute *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHRoute

	class CHRouteModeless : public udListHandlerModeless, public CHRoute {

	public:
		CHRouteModeless( );
		CHRouteModeless( const CHRouteModeless& objCopy );

		virtual ~CHRouteModeless( );

		CHRouteModeless& operator =( const CHRouteModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHRouteModeless

	class CHRoutePick : public udListHandlerPick, public CHRoute {

	public:
		CHRoutePick( );
		CHRoutePick( const CHRoutePick& objCopy );

		virtual ~CHRoutePick( );

		CHRoutePick& operator =( const CHRoutePick& objCopy );

		virtual void	Init( );
		virtual void	OnButtonForm( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual void BeforeCreate( ) {
			Init( );
			Load( );
		} // void BeforeCreate

		//virtual udPWndHandler	GetWindowPick( ) const { return m_wndListWrap; }
		virtual udPWndHandler	GetWindow( ) const { return m_wndListWrap; }

	}; // class CHRoutePick

	class CHRouteForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblNameLatin;
		udWgtInput		m_inpName,
						m_inpNameLatin;
		Gtk::Table		m_tblInput;


	public:
		CHRouteForm( );
		CHRouteForm( const CHRouteForm& objCopy );

		virtual ~CHRouteForm( );

		CHRouteForm& operator =( const CHRouteForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHRouteForm

	class CHRouteFormModal : public CHRouteForm {

	public:
		CHRouteFormModal( );
		CHRouteFormModal( const CHRouteFormModal& objCopy );

		virtual ~CHRouteFormModal( );

		CHRouteFormModal& operator =( const CHRouteFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHRouteFormModal

	class CHRouteFormModeless : public CHRouteForm {

	public:
		CHRouteFormModeless( );
		CHRouteFormModeless( const CHRouteFormModeless& objCopy );

		virtual ~CHRouteFormModeless( );

		CHRouteFormModeless& operator =( const CHRouteFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHRouteFormModal

} // namespace UdSDK

#endif
