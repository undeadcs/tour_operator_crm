/*
	Tourist application
	Клиентское приложение
	Обращения
*/
#ifndef TOURIST_CLIENT_REQUEST_H
#define TOURIST_CLIENT_REQUEST_H

namespace UdSDK {

	udPBase	GetProjectRequestList( );

	class CHRequest : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHRequest( );
		CHRequest( const CHRequest& objCopy );

		virtual ~CHRequest( );

		CHRequest& operator =( const CHRequest& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelRequest : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelRequest( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_type );
				add( m_col_fio );
				add( m_col_direct );
				add( m_col_manager );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_type;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_direct;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_manager;
		};

	protected:
		ColModelRequest *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltType,
						m_lblFltFio,
						m_lblFltDirect,
						m_lblFltUser,
						m_lblFltOrder;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtInput		m_inpFltFio;
		udWgtListPick	m_inpFltType,
						m_inpFltDirect,
						m_inpFltUser,
						m_inpFltOrder;

		enum {
			fltType,
			fltDirect,
			fltUser,
			fltOrder
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHRequest

	class CHRequestModeless : public udListHandlerModeless, public CHRequest {

	public:
		CHRequestModeless( );
		CHRequestModeless( const CHRequestModeless& objCopy );

		virtual ~CHRequestModeless( );

		CHRequestModeless& operator =( const CHRequestModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHRequestModeless

	class CHRequestPick : public udListHandlerPick, public CHRequest {

	public:
		CHRequestPick( );
		CHRequestPick( const CHRequestPick& objCopy );

		virtual ~CHRequestPick( );

		CHRequestPick& operator =( const CHRequestPick& objCopy );

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

	}; // class CHRequestPick

	class CHRequestForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblFio,
						m_lblComment,
						m_lblState,
						m_lblType,
						m_lblDirect,
						m_lblManager,
						m_lblResult,
						m_lblStatus;
		udWgtInput		m_inpFio,
						m_inpComment;
		Gtk::Table		m_tblInput;
		udWgtListPick	m_inpState,
						m_inpType,
						m_inpDirect,
						m_inpManager,
						m_inpResult;
		udWgtSelect		m_inpStatus;

	public:
		enum {
			ctrlidState,
			ctrlidType,
			ctrlidDirect,
			ctrlidManager,
			ctrlidResult
		};

	public:
		CHRequestForm( );
		CHRequestForm( const CHRequestForm& objCopy );

		virtual ~CHRequestForm( );

		CHRequestForm& operator =( const CHRequestForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHRequestForm

	class CHRequestFormModal : public CHRequestForm {

	public:
		CHRequestFormModal( );
		CHRequestFormModal( const CHRequestFormModal& objCopy );

		virtual ~CHRequestFormModal( );

		CHRequestFormModal& operator =( const CHRequestFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHRequestFormModal

	class CHRequestFormModeless : public CHRequestForm {

	public:
		CHRequestFormModeless( );
		CHRequestFormModeless( const CHRequestFormModeless& objCopy );

		virtual ~CHRequestFormModeless( );

		CHRequestFormModeless& operator =( const CHRequestFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHRequestFormModal

} // namespace UdSDK

#endif
