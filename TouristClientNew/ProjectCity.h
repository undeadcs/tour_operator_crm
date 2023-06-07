/*
	Tourist application
	Клиентское приложение
	Город
*/
#ifndef TOURIST_CLIENT_CITY_H
#define TOURIST_CLIENT_CITY_H

namespace UdSDK {

	udPBase	GetProjectCityList( );

	class CHCity : public udListHandlerBtnForm, public udListHandler {
	public:
		enum {
			opidGet2 = opidGet + 1,	// выборка городов по набору ID
		};

	public:
		CHCity( );
		CHCity( const CHCity& objCopy );

		virtual ~CHCity( );

		CHCity& operator =( const CHCity& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelCity : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelCity( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_country );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_country;
		};

	protected:
		ColModelCity *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		enum {
			fltCountry
		};

		Gtk::Label		m_lblFltName,
						m_lblFltCountry;
		udWgtInput		m_inpFltName;
		udWgtListPick	m_inpFltCountry;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		static udPArrBase	LoadCitiesById( const map< udUInt, udUInt >& ids );

	}; // class CHCity

	class CHCityModeless : public udListHandlerModeless, public CHCity {

	public:
		CHCityModeless( );
		CHCityModeless( const CHCityModeless& objCopy );

		virtual ~CHCityModeless( );

		CHCityModeless& operator =( const CHCityModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHCityModeless

	class CHCityPick : public udListHandlerPick, public CHCity {

	public:
		CHCityPick( );
		CHCityPick( const CHCityPick& objCopy );

		virtual ~CHCityPick( );

		CHCityPick& operator =( const CHCityPick& objCopy );

		virtual void	Init( );
		virtual void	OnButtonForm( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual void BeforeCreate( ) {
			Init( );
			Load( );
		} // void BeforeCreate

		virtual udPWndHandler	GetWindow( ) const { return m_wndListWrap; }

	}; // class CHCityPick

	class CHCityForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblCountry;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;
		udWgtListPick	m_inpCountry;

	public:
		enum {
			ctrlidCountry
		};

	public:
		CHCityForm( );
		CHCityForm( const CHCityForm& objCopy );

		virtual ~CHCityForm( );

		CHCityForm& operator =( const CHCityForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHCityForm

	class CHCityFormModal : public CHCityForm {

	public:
		CHCityFormModal( );
		CHCityFormModal( const CHCityFormModal& objCopy );

		virtual ~CHCityFormModal( );

		CHCityFormModal& operator =( const CHCityFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCityFormModal

	class CHCityFormModeless : public CHCityForm {

	public:
		CHCityFormModeless( );
		CHCityFormModeless( const CHCityFormModeless& objCopy );

		virtual ~CHCityFormModeless( );

		CHCityFormModeless& operator =( const CHCityFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCityFormModal

} // namespace UdSDK

#endif
