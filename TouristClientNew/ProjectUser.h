/*
	Tourist application
	Клиентское приложение
	Пользователи
*/
#ifndef TOURIST_CLIENT_USER_H
#define TOURIST_CLIENT_USER_H

namespace UdSDK {

	udPBase	GetProjectUserList( );

	class CHUser : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHUser( );
		CHUser( const CHUser& objCopy );

		virtual ~CHUser( );

		CHUser& operator =( const CHUser& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelUser : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelUser( ) {
				add( m_col_id );
				add( m_col_login );
				add( m_col_email );
				add( m_col_fio );
				add( m_col_rank );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_login;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_email;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_rank;
		};

	protected:
		ColModelUser *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltLogin,
						m_lblFltEmail,
						m_lblFltFio,
						m_lblFltRank;
		udWgtInput		m_inpFltLogin,
						m_inpFltEmail,
						m_inpFltFio;
		udWgtSelect		m_inpFltRank;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHUser

	class CHUserModeless : public udListHandlerModeless, public CHUser {

	public:
		CHUserModeless( );
		CHUserModeless( const CHUserModeless& objCopy );

		virtual ~CHUserModeless( );

		CHUserModeless& operator =( const CHUserModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHUserModeless

	class CHUserPick : public udListHandlerPick, public CHUser {

	public:
		CHUserPick( );
		CHUserPick( const CHUserPick& objCopy );

		virtual ~CHUserPick( );

		CHUserPick& operator =( const CHUserPick& objCopy );

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

	}; // class CHUserPick

	class CHUserForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblLogin,
						m_lblPassword,
						m_lblEmail,
						m_lblFio,
						m_lblRank,
						m_lblSeries;
		udWgtInput		m_inpLogin,		// Логин
						m_inpPassword,	// Пароль
						m_inpEmail,		// E-mail
						m_inpFio,		// ФИО
						m_inpSeries;	// Серия
		udWgtSelect		m_inpRank;
		Gtk::Table		m_tblInput;

	public:
		CHUserForm( );
		CHUserForm( const CHUserForm& objCopy );

		virtual ~CHUserForm( );

		CHUserForm& operator =( const CHUserForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHUserForm

	class CHUserFormModal : public CHUserForm {

	public:
		CHUserFormModal( );
		CHUserFormModal( const CHUserFormModal& objCopy );

		virtual ~CHUserFormModal( );

		CHUserFormModal& operator =( const CHUserFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHUserFormModal

	class CHUserFormModeless : public CHUserForm {

	public:
		CHUserFormModeless( );
		CHUserFormModeless( const CHUserFormModeless& objCopy );

		virtual ~CHUserFormModeless( );

		CHUserFormModeless& operator =( const CHUserFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHUserFormModal

} // namespace UdSDK

#endif
