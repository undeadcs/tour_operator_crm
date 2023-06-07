/*
	Tourist application
	Клиентское приложение
	Единица измерения услуги
*/
#ifndef TOURIST_CLIENT_MUSERVICE_H
#define TOURIST_CLIENT_MUSERVICE_H

namespace UdSDK {

	udPBase	GetProjectMUServiceList( );

	class CHMUService : public udListHandlerBtnForm, public udListHandler {

	public:
		CHMUService( );
		CHMUService( const CHMUService& objCopy );

		virtual ~CHMUService( );

		CHMUService& operator =( const CHMUService& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelMUService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelMUService( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelMUService *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHMUService

	class CHMUServiceModeless : public udListHandlerModeless, public CHMUService {

	public:
		CHMUServiceModeless( );
		CHMUServiceModeless( const CHMUServiceModeless& objCopy );

		virtual ~CHMUServiceModeless( );

		CHMUServiceModeless& operator =( const CHMUServiceModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHMUServiceModeless

	class CHMUServicePick : public udListHandlerPick, public CHMUService {

	public:
		CHMUServicePick( );
		CHMUServicePick( const CHMUServicePick& objCopy );

		virtual ~CHMUServicePick( );

		CHMUServicePick& operator =( const CHMUServicePick& objCopy );

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

	}; // class CHMUServicePick

	class CHMUServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;		// наименование
		Gtk::Table		m_tblInput;


	public:
		CHMUServiceForm( );
		CHMUServiceForm( const CHMUServiceForm& objCopy );

		virtual ~CHMUServiceForm( );

		CHMUServiceForm& operator =( const CHMUServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHMUServiceForm

	class CHMUServiceFormModal : public CHMUServiceForm {

	public:
		CHMUServiceFormModal( );
		CHMUServiceFormModal( const CHMUServiceFormModal& objCopy );

		virtual ~CHMUServiceFormModal( );

		CHMUServiceFormModal& operator =( const CHMUServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHMUServiceFormModal

	class CHMUServiceFormModeless : public CHMUServiceForm {

	public:
		CHMUServiceFormModeless( );
		CHMUServiceFormModeless( const CHMUServiceFormModeless& objCopy );

		virtual ~CHMUServiceFormModeless( );

		CHMUServiceFormModeless& operator =( const CHMUServiceFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHMUServiceFormModal

} // namespace UdSDK

#endif
