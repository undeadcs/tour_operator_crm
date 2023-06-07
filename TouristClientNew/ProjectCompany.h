/*
	Tourist application
	Клиентское приложение
	Компании
*/
#ifndef TOURIST_CLIENT_COMPANY_H
#define TOURIST_CLIENT_COMPANY_H

namespace UdSDK {

	udPBase	GetProjectCompanyList( );

	class CHCompany : public udListHandlerBtnForm, public udListHandler {

	public:
		CHCompany( );
		CHCompany( const CHCompany& objCopy );

		virtual ~CHCompany( );

		CHCompany& operator =( const CHCompany& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelCompany : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelCompany( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_email );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_email;
		};

	protected:
		ColModelCompany *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltEmail;
		udWgtInput		m_inpFltName,
						m_inpFltEmail;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHCompany

	class CHCompanyModeless : public udListHandlerModeless, public CHCompany {

	public:
		CHCompanyModeless( );
		CHCompanyModeless( const CHCompanyModeless& objCopy );

		virtual ~CHCompanyModeless( );

		CHCompanyModeless& operator =( const CHCompanyModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHCompanyModeless

	class CHCompanyPick : public udListHandlerPick, public CHCompany {

	public:
		CHCompanyPick( );
		CHCompanyPick( const CHCompanyPick& objCopy );

		virtual ~CHCompanyPick( );

		CHCompanyPick& operator =( const CHCompanyPick& objCopy );

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

	}; // class CHCompanyPick

	class CHCompanyForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblEmail,
						m_lblInn,
						m_lblUaddr,
						m_lblPaddr;
		udWgtInput		m_inpName,		// Наименование
						m_inpEmail,		// E-mail
						m_inpInn,		// ИНН
						m_inpUaddr,		// Юридический адрес
						m_inpPaddr;		// Почтовый адрес
		Gtk::Table		m_tblInput;


	public:
		CHCompanyForm( );
		CHCompanyForm( const CHCompanyForm& objCopy );

		virtual ~CHCompanyForm( );

		CHCompanyForm& operator =( const CHCompanyForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHCompanyForm

	class CHCompanyFormModal : public CHCompanyForm {

	public:
		CHCompanyFormModal( );
		CHCompanyFormModal( const CHCompanyFormModal& objCopy );

		virtual ~CHCompanyFormModal( );

		CHCompanyFormModal& operator =( const CHCompanyFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCompanyFormModal

	class CHCompanyFormModeless : public CHCompanyForm {

	public:
		CHCompanyFormModeless( );
		CHCompanyFormModeless( const CHCompanyFormModeless& objCopy );

		virtual ~CHCompanyFormModeless( );

		CHCompanyFormModeless& operator =( const CHCompanyFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCompanyFormModal

} // namespace UdSDK

#endif
