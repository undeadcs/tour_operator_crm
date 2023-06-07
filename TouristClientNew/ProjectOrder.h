/*
	Tourist application
	Клиентское приложение
	Заявки
*/
#ifndef TOURIST_CLIENT_ORDER_H
#define TOURIST_CLIENT_ORDER_H

namespace UdSDK {

	udPBase	GetProjectOrderList( );

	class CHOrder : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHOrder( );
		CHOrder( const CHOrder& objCopy );

		virtual ~CHOrder( );

		CHOrder& operator =( const CHOrder& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelOrder : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelOrder( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_manager );
				add( m_col_status );
				add( m_col_amount );
				add( m_col_employer );
				add( m_col_direct );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_manager;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_status;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_amount;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_employer;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_direct;
		};

	protected:
		ColModelOrder *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltUser,
						m_lblFltAmount1,
						m_lblFltAmount2,
						m_lblFltEmployer,
						m_lblFltDirect,
						m_lblFltRequest,
						m_lblFltTour;

		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtListPick	m_inpFltUser,
						m_inpFltDirect,
						m_inpFltRequest,
						m_inpFltTour;
		udWgtInput		m_inpFltAmount1,
						m_inpFltAmount2;
		udPWgtEmployerPick
						m_inpFltEmployer;

		enum {
			fltUser,
			fltDirect,
			fltRequest,
			fltTour,
			fltEmployer
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHOrder

	class CHOrderModeless : public udListHandlerModeless, public CHOrder {

	public:
		CHOrderModeless( );
		CHOrderModeless( const CHOrderModeless& objCopy );

		virtual ~CHOrderModeless( );

		CHOrderModeless& operator =( const CHOrderModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHOrderModeless

	class CHOrderPick : public udListHandlerPick, public CHOrder {

	public:
		CHOrderPick( );
		CHOrderPick( const CHOrderPick& objCopy );

		virtual ~CHOrderPick( );

		CHOrderPick& operator =( const CHOrderPick& objCopy );

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

	}; // class CHOrderPick

	class CHOrderForm : public udFormHandler, public CWgtEmployerDocPick::Handler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblManager,
						m_lblStatus,
						m_lblState,
						m_lblAmount,
						m_lblEmployer,
						m_lblDate1,
						m_lblDate2,
						m_lblDirect,
						m_lblComment,
						m_lblRoute,
						m_lblService,
						m_lblApp,
						m_lblDocument;
		udWgtInput		m_inpAmount,
						m_inpComment;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udWgtListPick	m_inpManager,
						m_inpState,
						m_inpDirect;
		udWgtEmployerDocPick
						m_inpDocument;
		udPWgtEmployerPick
						m_inpEmployer;
		udWgtSelect		m_inpStatus;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidManager,
			ctrlidState,
			ctrlidEmployer,
			ctrlidDirect,
			ctrlidRoute,
			ctrlidService,
			ctrlidApp,
			ctrlidDocument
		};

	public:
		CHOrderForm( );
		CHOrderForm( const CHOrderForm& objCopy );

		virtual ~CHOrderForm( );

		CHOrderForm& operator =( const CHOrderForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		virtual bool	BeforeCreatePickHandler( udPWgtEmployerDocPick wgt, udPHEmployerDocPick handler );
		virtual udPHClDocFormModal GetEditHandler( udPWgtEmployerDocPick wgt );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListRoute : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListRoute( ) {
				add( m_col_name );
				add( m_col_object );
			} // ColModelFormListRoute

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListRoute

	protected:
		ColModelFormListRoute* m_pModelFormListRoute;
		Glib::RefPtr< Gtk::ListStore > m_objRouteModel;
		udWgtFormList m_inpRoute;
		Gtk::Button	*m_btnRouteAdd,
					*m_btnRouteDel,
					*m_btnRouteEdit;

		void	OnFormListRoute_Select( );
		void	OnFormListRoute_Add( );
		void	OnFormListRoute_Edit( );
		void	OnFormListRoute_Del( );
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListService( ) {
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_type );
				add( m_col_city );
				add( m_col_object );
			} // ColModelFormListService

			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_city;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListService

	protected:
		ColModelFormListService* m_pModelFormListService;
		Glib::RefPtr< Gtk::ListStore > m_objServiceModel;
		udWgtFormList m_inpService;
		Gtk::Button	*m_btnServiceAdd,
					*m_btnServiceDel,
					*m_btnServiceEdit;

		void	OnFormListService_Select( );
		void	OnFormListService_Add( );
		void	OnFormListService_Edit( );
		void	OnFormListService_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHOrderForm

	class CHOrderFormModal : public CHOrderForm {

	public:
		CHOrderFormModal( );
		CHOrderFormModal( const CHOrderFormModal& objCopy );

		virtual ~CHOrderFormModal( );

		CHOrderFormModal& operator =( const CHOrderFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHOrderFormModal

	class CHOrderFormModeless : public CHOrderForm {

	public:
		CHOrderFormModeless( );
		CHOrderFormModeless( const CHOrderFormModeless& objCopy );

		virtual ~CHOrderFormModeless( );

		CHOrderFormModeless& operator =( const CHOrderFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHOrderFormModal

} // namespace UdSDK

#endif
