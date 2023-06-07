/*
	Tourist application
	Клиентское приложение
	Клиенты юр. лица
*/
#ifndef TOURIST_CLIENT_CLIENTUR_H
#define TOURIST_CLIENT_CLIENTUR_H

namespace UdSDK {

	udPBase	GetProjectClientUrList( );

	class CHClientUr : public udListHandlerBtnForm, public udListHandler {

	public:
		CHClientUr( );
		CHClientUr( const CHClientUr& objCopy );

		virtual ~CHClientUr( );

		CHClientUr& operator =( const CHClientUr& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelClientUr : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelClientUr( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_cfio );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_cfio;
		};

	protected:
		ColModelClientUr *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltCfio;
		udWgtInput		m_inpFltName,
						m_inpFltCfio;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHClientUr

	class CHClientUrModeless : public udListHandlerModeless, public CHClientUr {
		Gtk::Button	m_btnFiz;

	public:
		CHClientUrModeless( );
		CHClientUrModeless( const CHClientUrModeless& objCopy );

		virtual ~CHClientUrModeless( );

		CHClientUrModeless& operator =( const CHClientUrModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHClientUrModeless

	class CHClientUrPick : public udListHandlerPick, public CHClientUr {

	public:
		CHClientUrPick( );
		CHClientUrPick( const CHClientUrPick& objCopy );

		virtual ~CHClientUrPick( );

		CHClientUrPick& operator =( const CHClientUrPick& objCopy );

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

	}; // class CHClientUrPick

	class CHClientUrForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblDfio,
						m_lblCfio,
						m_lblEmail,
						m_lblPhone,
						m_lblFax,
						m_lblAddr,
						m_lblComment;
		udWgtInput		m_inpName,		// Наименование
						m_inpDfio,		// ФИО руководителя
						m_inpCfio,		// ФИО контактного лица
						m_inpEmail,		// E-mail
						m_inpPhone,		// Телефон
						m_inpFax,		// Факс
						m_inpAddr,		// Адрес
						m_inpComment;	// Комментарий
		Gtk::Table		m_tblInput;


	public:
		CHClientUrForm( );
		CHClientUrForm( const CHClientUrForm& objCopy );

		virtual ~CHClientUrForm( );

		CHClientUrForm& operator =( const CHClientUrForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHClientUrForm

	class CHClientUrFormModal : public CHClientUrForm {

	public:
		CHClientUrFormModal( );
		CHClientUrFormModal( const CHClientUrFormModal& objCopy );

		virtual ~CHClientUrFormModal( );

		CHClientUrFormModal& operator =( const CHClientUrFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHClientUrFormModal

	class CHClientUrFormModeless : public CHClientUrForm {

	public:
		CHClientUrFormModeless( );
		CHClientUrFormModeless( const CHClientUrFormModeless& objCopy );

		virtual ~CHClientUrFormModeless( );

		CHClientUrFormModeless& operator =( const CHClientUrFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHClientUrFormModal

} // namespace UdSDK

#endif
