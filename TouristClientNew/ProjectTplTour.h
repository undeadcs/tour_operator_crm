/*
	Tourist application
	Клиентское приложение
	Шаблоны туров
*/
#ifndef TOURIST_CLIENT_TPLTOUR_H
#define TOURIST_CLIENT_TPLTOUR_H

namespace UdSDK {

	udPBase	GetProjectTplTourList( );

	class CHTplTour : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHTplTour( );
		CHTplTour( const CHTplTour& objCopy );

		virtual ~CHTplTour( );

		CHTplTour& operator =( const CHTplTour& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTplTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTplTour( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_direct );
				add( m_col_cost );
				add( m_col_price );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_direct;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_cost;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price;
		};

	protected:
		ColModelTplTour *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltDirect,
						m_lblFltTour;
		udWgtInput		m_inpFltName;
		udWgtListPick	m_inpFltDirect,
						m_inpFltTour;

		enum {
			fltDirect,
			fltTour
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTplTour

	class CHTplTourModeless : public udListHandlerModeless, public CHTplTour {

	public:
		CHTplTourModeless( );
		CHTplTourModeless( const CHTplTourModeless& objCopy );

		virtual ~CHTplTourModeless( );

		CHTplTourModeless& operator =( const CHTplTourModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTplTourModeless

	class CHTplTourPick : public udListHandlerPick, public CHTplTour {

	public:
		CHTplTourPick( );
		CHTplTourPick( const CHTplTourPick& objCopy );

		virtual ~CHTplTourPick( );

		CHTplTourPick& operator =( const CHTplTourPick& objCopy );

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

	}; // class CHTplTourPick

	class CHTplTourForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblDaynum,
						m_lblDirect,
						m_lblRoute,
						m_lblService,
						m_lblApp;
		udWgtInput		m_inpName,
						m_inpDaynum;
		udWgtListPick	m_inpDirect;
		Gtk::Table		m_tblInput;

		Gtk::Button		m_btnCreateTour;

	public:
		enum {
			ctrlidDirect,
			ctrlidRoute,
			ctrlidService,
			ctrlidApp
		};

		friend udHTourForm;

	public:
		CHTplTourForm( );
		CHTplTourForm( const CHTplTourForm& objCopy );

		virtual ~CHTplTourForm( );

		CHTplTourForm& operator =( const CHTplTourForm& objCopy );

		void	OnButtonCreateTour( );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListService( ) {
				add( m_col_service );
				add( m_col_num );
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_object );
			} // ColModelFormListService

			Gtk::TreeModelColumn< Glib::ustring > m_col_service;
			Gtk::TreeModelColumn< Glib::ustring > m_col_num;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
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
	// определение списка сущностей формы, с использованием списка выбора
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
					*m_btnRouteReplace;

		void	OnFormListRoute_Select( );
		void	OnFormListRoute_Add( );
		void	OnFormListRoute_Del( );
		void	OnFormListRoute_Replace( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTplTourForm

	class CHTplTourFormModal : public CHTplTourForm {

	public:
		CHTplTourFormModal( );
		CHTplTourFormModal( const CHTplTourFormModal& objCopy );

		virtual ~CHTplTourFormModal( );

		CHTplTourFormModal& operator =( const CHTplTourFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTplTourFormModal

	class CHTplTourFormModeless : public CHTplTourForm {

	public:
		CHTplTourFormModeless( );
		CHTplTourFormModeless( const CHTplTourFormModeless& objCopy );

		virtual ~CHTplTourFormModeless( );

		CHTplTourFormModeless& operator =( const CHTplTourFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTplTourFormModal

} // namespace UdSDK

#endif
