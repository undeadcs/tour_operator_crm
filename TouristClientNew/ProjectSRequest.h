/*
	Tourist application
	Клиентское приложение
	Состояние обращения
*/
#ifndef TOURIST_CLIENT_SREQUEST_H
#define TOURIST_CLIENT_SREQUEST_H

namespace UdSDK {

	udPBase	GetProjectSRequestList( );

	class CHSRequest : public udListHandlerBtnForm, public udListHandler {

	public:
		CHSRequest( );
		CHSRequest( const CHSRequest& objCopy );

		virtual ~CHSRequest( );

		CHSRequest& operator =( const CHSRequest& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelSRequest : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelSRequest( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelSRequest *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHSRequest

	class CHSRequestModeless : public udListHandlerModeless, public CHSRequest {

	public:
		CHSRequestModeless( );
		CHSRequestModeless( const CHSRequestModeless& objCopy );

		virtual ~CHSRequestModeless( );

		CHSRequestModeless& operator =( const CHSRequestModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHSRequestModeless

	class CHSRequestPick : public udListHandlerPick, public CHSRequest {

	public:
		CHSRequestPick( );
		CHSRequestPick( const CHSRequestPick& objCopy );

		virtual ~CHSRequestPick( );

		CHSRequestPick& operator =( const CHSRequestPick& objCopy );

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

	}; // class CHSRequestPick

	class CHSRequestForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHSRequestForm( );
		CHSRequestForm( const CHSRequestForm& objCopy );

		virtual ~CHSRequestForm( );

		CHSRequestForm& operator =( const CHSRequestForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHSRequestForm

	class CHSRequestFormModal : public CHSRequestForm {

	public:
		CHSRequestFormModal( );
		CHSRequestFormModal( const CHSRequestFormModal& objCopy );

		virtual ~CHSRequestFormModal( );

		CHSRequestFormModal& operator =( const CHSRequestFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHSRequestFormModal

	class CHSRequestFormModeless : public CHSRequestForm {

	public:
		CHSRequestFormModeless( );
		CHSRequestFormModeless( const CHSRequestFormModeless& objCopy );

		virtual ~CHSRequestFormModeless( );

		CHSRequestFormModeless& operator =( const CHSRequestFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHSRequestFormModal

} // namespace UdSDK

#endif
