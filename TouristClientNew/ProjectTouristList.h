/*
	Tourist application
	Клиентское приложение
	Список туристов
*/
#ifndef TOURIST_CLIENT_TOURIST_LIST_H
#define TOURIST_CLIENT_TOURIST_LIST_H

namespace UdSDK {

	udPBase	GetProjectTouristListList( );

	class CHTouristList : public udListHandlerBtnForm, public udListHandler {

	public:
		CHTouristList( );
		CHTouristList( const CHTouristList& objCopy );

		virtual ~CHTouristList( );

		CHTouristList& operator =( const CHTouristList& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTouristList : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTouristList( ) {
				add( m_col_id );
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_leader );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_leader;
		};

	protected:
		ColModelTouristList *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltClient;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtListPick	m_inpFltClient;

		enum {
			fltClient
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTouristList

	class CHTouristListModeless : public udListHandlerModeless, public CHTouristList {

	public:
		CHTouristListModeless( );
		CHTouristListModeless( const CHTouristListModeless& objCopy );

		virtual ~CHTouristListModeless( );

		CHTouristListModeless& operator =( const CHTouristListModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTouristListModeless

	class CHTouristListPick : public udListHandlerPick, public CHTouristList {

	public:
		CHTouristListPick( );
		CHTouristListPick( const CHTouristListPick& objCopy );

		virtual ~CHTouristListPick( );

		CHTouristListPick& operator =( const CHTouristListPick& objCopy );

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

	}; // class CHTouristListPick

	class CHTouristListForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblDate1,
						m_lblDate2,
						m_lblExit,
						m_lblEnter,
						m_lblHost,
						m_lblLeader,
						m_lblRoute,
						m_lblClient;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udWgtListPick	m_inpExit,
						m_inpEnter,
						m_inpHost,
						m_inpRoute;
		udWgtTouristListLeaderPick m_inpLeader;
		Gtk::Table		m_tblInput;

		Gtk::Button		m_btnPrint;

	public:
		enum {
			ctrlidExit,
			ctrlidEnter,
			ctrlidHost,
			ctrlidRoute,
			ctrlidLeader,
			ctrlidClient
		};

	public:
		CHTouristListForm( );
		CHTouristListForm( const CHTouristListForm& objCopy );

		virtual ~CHTouristListForm( );

		CHTouristListForm& operator =( const CHTouristListForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		virtual void	OnButtonPrint( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием списка выбора
	public:
		class ColModelFormListClient : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListClient( ) {
				add( m_col_fio );
				add( m_col_object );
			} // ColModelFormListClient

			Gtk::TreeModelColumn< Glib::ustring > m_col_fio;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListClient

	protected:
		ColModelFormListClient* m_pModelFormListClient;
		Glib::RefPtr< Gtk::ListStore > m_objClientModel;
		udWgtFormList m_inpClient;
		Gtk::Button	*m_btnClientAdd,
					*m_btnClientDel,
					*m_btnClientReplace;

		void	OnFormListClient_Select( );
		void	OnFormListClient_Add( );
		void	OnFormListClient_Del( );
		void	OnFormListClient_Replace( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTouristListForm

	class CHTouristListFormModal : public CHTouristListForm {

	public:
		CHTouristListFormModal( );
		CHTouristListFormModal( const CHTouristListFormModal& objCopy );

		virtual ~CHTouristListFormModal( );

		CHTouristListFormModal& operator =( const CHTouristListFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTouristListFormModal

	class CHTouristListFormModeless : public CHTouristListForm {

	public:
		CHTouristListFormModeless( );
		CHTouristListFormModeless( const CHTouristListFormModeless& objCopy );

		virtual ~CHTouristListFormModeless( );

		CHTouristListFormModeless& operator =( const CHTouristListFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTouristListFormModal

} // namespace UdSDK

#endif
