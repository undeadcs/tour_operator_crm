/*
	Tourist application
	Клиентское приложение
	Результат заявки
*/
#ifndef TOURIST_CLIENT_RREQUEST_H
#define TOURIST_CLIENT_RREQUEST_H

namespace UdSDK {

	udPBase	GetProjectRRequestList( );

	class CHRRequest : public udListHandlerBtnForm, public udListHandler {

	public:
		CHRRequest( );
		CHRRequest( const CHRRequest& objCopy );

		virtual ~CHRRequest( );

		CHRRequest& operator =( const CHRRequest& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelRRequest : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelRRequest( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelRRequest *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHRRequest

	class CHRRequestModeless : public udListHandlerModeless, public CHRRequest {

	public:
		CHRRequestModeless( );
		CHRRequestModeless( const CHRRequestModeless& objCopy );

		virtual ~CHRRequestModeless( );

		CHRRequestModeless& operator =( const CHRRequestModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHRRequestModeless

	class CHRRequestPick : public udListHandlerPick, public CHRRequest {

	public:
		CHRRequestPick( );
		CHRRequestPick( const CHRRequestPick& objCopy );

		virtual ~CHRRequestPick( );

		CHRRequestPick& operator =( const CHRRequestPick& objCopy );

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

	}; // class CHRRequestPick

	class CHRRequestForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHRRequestForm( );
		CHRRequestForm( const CHRRequestForm& objCopy );

		virtual ~CHRRequestForm( );

		CHRRequestForm& operator =( const CHRRequestForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHRRequestForm

	class CHRRequestFormModal : public CHRRequestForm {

	public:
		CHRRequestFormModal( );
		CHRRequestFormModal( const CHRRequestFormModal& objCopy );

		virtual ~CHRRequestFormModal( );

		CHRRequestFormModal& operator =( const CHRRequestFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHRRequestFormModal

	class CHRRequestFormModeless : public CHRRequestForm {

	public:
		CHRRequestFormModeless( );
		CHRRequestFormModeless( const CHRRequestFormModeless& objCopy );

		virtual ~CHRRequestFormModeless( );

		CHRRequestFormModeless& operator =( const CHRRequestFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHRRequestFormModal

} // namespace UdSDK

#endif
