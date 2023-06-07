/*
	Tourist application
	Клиентское приложение
	Партнеры
*/
#ifndef TOURIST_CLIENT_PARTNER_H
#define TOURIST_CLIENT_PARTNER_H

namespace UdSDK {

	udPBase	GetProjectPartnerList( );

	class CHPartner : public udListHandlerBtnForm, public udListHandler {

	public:
		CHPartner( );
		CHPartner( const CHPartner& objCopy );

		virtual ~CHPartner( );

		CHPartner& operator =( const CHPartner& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelPartner : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelPartner( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_cfio );
				add( m_col_email );
				add( m_col_phone );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_cfio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_email;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_phone;
		};

	protected:
		ColModelPartner *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltCfio,
						m_lblFltEmail,
						m_lblFltPhone;
		udWgtInput		m_inpFltName,
						m_inpFltCfio,
						m_inpFltEmail,
						m_inpFltPhone;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHPartner

	class CHPartnerModeless : public udListHandlerModeless, public CHPartner {
		Gtk::Button	m_btnRequest,
					m_btnAnswer;

	public:
		CHPartnerModeless( );
		CHPartnerModeless( const CHPartnerModeless& objCopy );

		virtual ~CHPartnerModeless( );

		CHPartnerModeless& operator =( const CHPartnerModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHPartnerModeless

	class CHPartnerPick : public udListHandlerPick, public CHPartner {

	public:
		CHPartnerPick( );
		CHPartnerPick( const CHPartnerPick& objCopy );

		virtual ~CHPartnerPick( );

		CHPartnerPick& operator =( const CHPartnerPick& objCopy );

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

	}; // class CHPartnerPick

	class CHPartnerForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,		// Наименование
						m_lblDfio,		// ФИО директора
						m_lblCfio,		// ФИО контактного лица
						m_lblEmail,		// E-mail
						m_lblPhone,		// Телефон
						m_lblFax,		// Факс
						m_lblComment;	// Комментарий
		udWgtInput		m_inpName,		// Наименование
						m_inpDfio,		// ФИО директора
						m_inpCfio,		// ФИО контактного лица
						m_inpEmail,		// E-mail
						m_inpPhone,		// Телефон
						m_inpFax,		// Факс
						m_inpComment;	// Комментарий
		Gtk::Table		m_tblInput;


	public:
		CHPartnerForm( );
		CHPartnerForm( const CHPartnerForm& objCopy );

		virtual ~CHPartnerForm( );

		CHPartnerForm& operator =( const CHPartnerForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHPartnerForm

	class CHPartnerFormModal : public CHPartnerForm {

	public:
		CHPartnerFormModal( );
		CHPartnerFormModal( const CHPartnerFormModal& objCopy );

		virtual ~CHPartnerFormModal( );

		CHPartnerFormModal& operator =( const CHPartnerFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPartnerFormModal

	class CHPartnerFormModeless : public CHPartnerForm {

	public:
		CHPartnerFormModeless( );
		CHPartnerFormModeless( const CHPartnerFormModeless& objCopy );

		virtual ~CHPartnerFormModeless( );

		CHPartnerFormModeless& operator =( const CHPartnerFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHPartnerFormModeless

} // namespace UdSDK

#endif
