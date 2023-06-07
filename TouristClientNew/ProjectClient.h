/*
	Tourist application
	Клиентское приложение
	Клиенты
*/
#ifndef TOURIST_CLIENT_CLIENT_H
#define TOURIST_CLIENT_CLIENT_H

namespace UdSDK {

	udPBase	GetProjectClientList( );

	class CHClient : public udListHandlerBtnForm, public udListHandler {
	public:
		enum {
			opidGet2 = opidGet + 1,	// получение списка клиентов, по заданным ID
			opidGet3,				// получение клиентов по ID (без фильтров и пейджера)
		};

	public:
		CHClient( );
		CHClient( const CHClient& objCopy );

		virtual ~CHClient( );

		CHClient& operator =( const CHClient& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelClient : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelClient( ) {
				add( m_col_id );
				add( m_col_fio );
				add( m_col_born );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_born;
		};

	protected:
		ColModelClient *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltFio,
						m_lblFltDate1,
						m_lblFltDate2;
		udWgtInput		m_inpFltFio;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHClient

	class CHClientModeless : public udListHandlerModeless, public CHClient {
		Gtk::Button	m_btnDoc,
					m_btnUr;

	public:
		CHClientModeless( );
		CHClientModeless( const CHClientModeless& objCopy );

		virtual ~CHClientModeless( );

		CHClientModeless& operator =( const CHClientModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHClientModeless

	class CHClientPick : public udListHandlerPick, public CHClient {

	public:
		CHClientPick( );
		CHClientPick( const CHClientPick& objCopy );

		virtual ~CHClientPick( );

		CHClientPick& operator =( const CHClientPick& objCopy );

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

	}; // class CHClientPick

	class CHClientForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblFio,
						m_lblFioLatin,
						m_lblSex,
						m_lblEmail,
						m_lblPhone,
						m_lblAddr,
						m_lblBorn,
						m_lblDoc,
						m_lblApp,
						m_lblBornPlace,
						m_lblBornPlaceLatin,
						m_lblDefaultDoc;
		udWgtInput		m_inpFio,
						m_inpFioLatin,
						m_inpEmail,
						m_inpPhone,
						m_inpAddr,
						m_inpBornPlace,
						m_inpBornPlaceLatin;
		udWgtDatePick	m_inpBorn;
		udWgtSelect		m_inpSex;
		udWgtClDocFromForm	m_inpDefaultDoc;
		Gtk::Table		m_tblInput;

		Glib::RefPtr< Gtk::ListStore >	m_objSexModel;

	public:
		enum {
			ctrlidDefaultDoc,
			ctrlidDoc,
			ctrlidApp
		};

	public:
		CHClientForm( );
		CHClientForm( const CHClientForm& objCopy );

		virtual ~CHClientForm( );

		CHClientForm& operator =( const CHClientForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListDoc : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListDoc( ) {
				add( m_col_type );
				add( m_col_series );
				add( m_col_number );
				add( m_col_object );
			} // ColModelFormListDoc

			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_series;
			Gtk::TreeModelColumn< Glib::ustring > m_col_number;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListDoc

	protected:
		ColModelFormListDoc* m_pModelFormListDoc;
		Glib::RefPtr< Gtk::ListStore > m_objDocModel;
		udWgtFormList m_inpDoc;
		Gtk::Button	*m_btnDocAdd,
					*m_btnDocDel,
					*m_btnDocEdit;

		void	OnFormListDoc_Select( );
		void	OnFormListDoc_Add( );
		void	OnFormListDoc_Edit( );
		void	OnFormListDoc_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием
	public:
		class ColModelFormListApp : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListApp( ) {
				add( m_col_name );
				add( m_col_object );
			} // ColModelFormListApp

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListApp

	protected:
		ColModelFormListApp* m_pModelFormListApp;
		Glib::RefPtr< Gtk::ListStore > m_objAppModel;
		udWgtFormList m_inpApp;
		Gtk::Button	*m_btnAppAdd,
					*m_btnAppDel,
					*m_btnAppEdit,
					*m_btnAppReplace,
					*m_btnFolder;
		wstring m_szFolderApp;

		void	OnFormListApp_Select( );
		void	OnFormListApp_Add( );
		void	OnFormListApp_Edit( );
		void	OnFormListApp_Del( );
		void	OnFormListApp_Replace( );
		void	OnFormListApp_Folder( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHClientForm

	class CHClientFormModal : public CHClientForm {

	public:
		CHClientFormModal( );
		CHClientFormModal( const CHClientFormModal& objCopy );

		virtual ~CHClientFormModal( );

		CHClientFormModal& operator =( const CHClientFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHClientFormModal

	class CHClientFormModeless : public CHClientForm {

	public:
		CHClientFormModeless( );
		CHClientFormModeless( const CHClientFormModeless& objCopy );

		virtual ~CHClientFormModeless( );

		CHClientFormModeless& operator =( const CHClientFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHClientFormModal

} // namespace UdSDK

#endif
