/*
	Tourist application
	Клиентское приложение
	Тип услуги
*/
#ifndef TOURIST_CLIENT_TSERVICE_H
#define TOURIST_CLIENT_TSERVICE_H

namespace UdSDK {

	udPBase	GetProjectTServiceList( );

	class CHTService : public udListHandlerBtnForm, public udListHandler {

	public:
		CHTService( );
		CHTService( const CHTService& objCopy );

		virtual ~CHTService( );

		CHTService& operator =( const CHTService& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTService( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelTService *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTService

	class CHTServiceModeless : public udListHandlerModeless, public CHTService {

	public:
		CHTServiceModeless( );
		CHTServiceModeless( const CHTServiceModeless& objCopy );

		virtual ~CHTServiceModeless( );

		CHTServiceModeless& operator =( const CHTServiceModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTServiceModeless

	class CHTServicePick : public udListHandlerPick, public CHTService {

	public:
		CHTServicePick( );
		CHTServicePick( const CHTServicePick& objCopy );

		virtual ~CHTServicePick( );

		CHTServicePick& operator =( const CHTServicePick& objCopy );

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

	}; // class CHTServicePick

	class CHTServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;

	public:
		CHTServiceForm( );
		CHTServiceForm( const CHTServiceForm& objCopy );

		virtual ~CHTServiceForm( );

		CHTServiceForm& operator =( const CHTServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHTServiceForm

	class CHTServiceFormModal : public CHTServiceForm {

	public:
		CHTServiceFormModal( );
		CHTServiceFormModal( const CHTServiceFormModal& objCopy );

		virtual ~CHTServiceFormModal( );

		CHTServiceFormModal& operator =( const CHTServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTServiceFormModal

	class CHTServiceFormModeless : public CHTServiceForm {

	public:
		CHTServiceFormModeless( );
		CHTServiceFormModeless( const CHTServiceFormModeless& objCopy );

		virtual ~CHTServiceFormModeless( );

		CHTServiceFormModeless& operator =( const CHTServiceFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTServiceFormModal

} // namespace UdSDK

#endif
