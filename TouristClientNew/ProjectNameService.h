/*
	Tourist application
	Клиентское приложение
	Наименование услуги
*/
#ifndef TOURIST_CLIENT_NAME_SERVICE_H
#define TOURIST_CLIENT_NAME_SERVICE_H

namespace UdSDK {

	udPBase	GetProjectNameServiceList( );

	class CHNameService : public udListHandlerBtnForm, public udListHandler {

	public:
		CHNameService( );
		CHNameService( const CHNameService& objCopy );

		virtual ~CHNameService( );

		CHNameService& operator =( const CHNameService& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelNameService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelNameService( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelNameService *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHNameService

	class CHNameServiceModeless : public udListHandlerModeless, public CHNameService {

	public:
		CHNameServiceModeless( );
		CHNameServiceModeless( const CHNameServiceModeless& objCopy );

		virtual ~CHNameServiceModeless( );

		CHNameServiceModeless& operator =( const CHNameServiceModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHNameServiceModeless

	class CHNameServicePick : public udListHandlerPick, public CHNameService {

	public:
		CHNameServicePick( );
		CHNameServicePick( const CHNameServicePick& objCopy );

		virtual ~CHNameServicePick( );

		CHNameServicePick& operator =( const CHNameServicePick& objCopy );

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

	}; // class CHNameServicePick

	class CHNameServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHNameServiceForm( );
		CHNameServiceForm( const CHNameServiceForm& objCopy );

		virtual ~CHNameServiceForm( );

		CHNameServiceForm& operator =( const CHNameServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHNameServiceForm

	class CHNameServiceFormModal : public CHNameServiceForm {

	public:
		CHNameServiceFormModal( );
		CHNameServiceFormModal( const CHNameServiceFormModal& objCopy );

		virtual ~CHNameServiceFormModal( );

		CHNameServiceFormModal& operator =( const CHNameServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHNameServiceFormModal

	class CHNameServiceFormModeless : public CHNameServiceForm {

	public:
		CHNameServiceFormModeless( );
		CHNameServiceFormModeless( const CHNameServiceFormModeless& objCopy );

		virtual ~CHNameServiceFormModeless( );

		CHNameServiceFormModeless& operator =( const CHNameServiceFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHNameServiceFormModal

} // namespace UdSDK

#endif
