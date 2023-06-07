/*
	Tourist application
	Клиентское приложение
	Страны
*/
#ifndef TOURIST_CLIENT_COUNTRY_H
#define TOURIST_CLIENT_COUNTRY_H

namespace UdSDK {

	udPBase	GetProjectCountryList( );

	class CHCountry : public udListHandlerBtnForm, public udListHandler {

	public:
		CHCountry( );
		CHCountry( const CHCountry& objCopy );

		virtual ~CHCountry( );

		CHCountry& operator =( const CHCountry& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelCountry : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelCountry( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelCountry *m_pColModel;

		void	OnColClickId( );
		void	OnColClickName( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHCountry

	class CHCountryModeless : public udListHandlerModeless, public CHCountry {

	public:
		CHCountryModeless( );
		CHCountryModeless( const CHCountryModeless& objCopy );

		virtual ~CHCountryModeless( );

		CHCountryModeless& operator =( const CHCountryModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHCountryModeless

	class CHCountryPick : public udListHandlerPick, public CHCountry {

	public:
		CHCountryPick( );
		CHCountryPick( const CHCountryPick& objCopy );

		virtual ~CHCountryPick( );

		CHCountryPick& operator =( const CHCountryPick& objCopy );

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

	}; // class CHCountryPick

	class CHCountryForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;		// наименование
		Gtk::Table		m_tblInput;

	public:
		CHCountryForm( );
		CHCountryForm( const CHCountryForm& objCopy );

		virtual ~CHCountryForm( );

		CHCountryForm& operator =( const CHCountryForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHCountryForm

	class CHCountryFormModal : public CHCountryForm {

	public:
		CHCountryFormModal( );
		CHCountryFormModal( const CHCountryFormModal& objCopy );

		virtual ~CHCountryFormModal( );

		CHCountryFormModal& operator =( const CHCountryFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCountryFormModal

	class CHCountryFormModeless : public CHCountryForm {

	public:
		CHCountryFormModeless( );
		CHCountryFormModeless( const CHCountryFormModeless& objCopy );

		virtual ~CHCountryFormModeless( );

		CHCountryFormModeless& operator =( const CHCountryFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCountryFormModal

} // namespace UdSDK

#endif
