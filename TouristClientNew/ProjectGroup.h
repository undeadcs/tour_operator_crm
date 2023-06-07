/*
	Tourist application
	Клиентское приложение
	Группы
*/
#ifndef TOURIST_CLIENT_GROUP_H
#define TOURIST_CLIENT_GROUP_H

namespace UdSDK {

	udPBase	GetProjectGroupList( );

	class CHGroup : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHGroup( );
		CHGroup( const CHGroup& objCopy );

		virtual ~CHGroup( );

		CHGroup& operator =( const CHGroup& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelGroup : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelGroup( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_placenum );
				add( m_col_lperiod );
				add( m_col_tpltour );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_placenum;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_lperiod;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_tpltour;
		};

	protected:
		ColModelGroup *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHGroup

	class CHGroupModeless : public udListHandlerModeless, public CHGroup {

	public:
		CHGroupModeless( );
		CHGroupModeless( const CHGroupModeless& objCopy );

		virtual ~CHGroupModeless( );

		CHGroupModeless& operator =( const CHGroupModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHGroupModeless

	class CHGroupPick : public udListHandlerPick, public CHGroup {

	public:
		CHGroupPick( );
		CHGroupPick( const CHGroupPick& objCopy );

		virtual ~CHGroupPick( );

		CHGroupPick& operator =( const CHGroupPick& objCopy );

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

	}; // class CHGroupPick

	class CHGroupForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblDirect,
						m_lblComment,
						m_lblApp,
						m_lblTour,
						m_lblPeriod,
						m_lblTplTour;
		udWgtInput		m_inpName,
						m_inpComment;
		udWgtListPick	m_inpDirect;
		Gtk::Table		m_tblInput;

		Gtk::Button		m_btnExport;

	public:
		enum {
			ctrlidDirect,
			ctrlidApp,
			ctrlidTour,
			ctrlidPeriod,
			ctrlidTplTour
		};

		enum {
			opidGetTouristList = opidGet + 1, // загрузка списка туристов по периоду
		};

	public:
		CHGroupForm( );
		CHGroupForm( const CHGroupForm& objCopy );

		virtual ~CHGroupForm( );

		CHGroupForm& operator =( const CHGroupForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		void	OnButtonExportClick( );

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
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListTour( ) {
				add( m_col_name );
				add( m_col_date1 );
				add( m_col_object );
			} // ColModelFormListTour

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListTour

	protected:
		ColModelFormListTour* m_pModelFormListTour;
		Glib::RefPtr< Gtk::ListStore > m_objTourModel;
		udWgtFormList m_inpTour;
		Gtk::Button	*m_btnTourAdd,
					*m_btnTourDel,
					*m_btnTourEdit;

		void	OnFormListTour_Select( );
		void	OnFormListTour_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );
		void	OnFormListTour_Add( );
		void	OnFormListTour_Edit( );
		void	OnFormListTour_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListPeriod : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListPeriod( ) {
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_min );
				add( m_col_max );
				add( m_col_client );
				add( m_col_object );
			} // ColModelFormListPeriod

			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring > m_col_min;
			Gtk::TreeModelColumn< Glib::ustring > m_col_max;
			Gtk::TreeModelColumn< Glib::ustring > m_col_client;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListPeriod

	protected:
		ColModelFormListPeriod* m_pModelFormListPeriod;
		Glib::RefPtr< Gtk::ListStore > m_objPeriodModel;
		udWgtFormList m_inpPeriod;
		Gtk::Button	*m_btnPeriodAdd,
					*m_btnPeriodDel,
					*m_btnPeriodEdit;

		void	OnFormListPeriod_Select( );
		void	OnFormListPeriod_Add( );
		void	OnFormListPeriod_Edit( );
		void	OnFormListPeriod_Del( );

		udUInt	GetPeriodLastIndex( );

	public:
		void	SavePeriod( udPHGroupPeriodForm pForm );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием списка выбора
	public:
		class ColModelFormListTplTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListTplTour( ) {
				add( m_col_name );
				add( m_col_object );
			} // ColModelFormListTplTour

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListTplTour

	protected:
		ColModelFormListTplTour* m_pModelFormListTplTour;
		Glib::RefPtr< Gtk::ListStore > m_objTplTourModel;
		udWgtFormList m_inpTplTour;
		Gtk::Button	*m_btnTplTourAdd,
					*m_btnTplTourDel,
					*m_btnTplTourReplace;

		void	OnFormListTplTour_Select( );
		void	OnFormListTplTour_Add( );
		void	OnFormListTplTour_Del( );
		void	OnFormListTplTour_Replace( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHGroupForm

	class CHGroupFormModal : public CHGroupForm {

	public:
		CHGroupFormModal( );
		CHGroupFormModal( const CHGroupFormModal& objCopy );

		virtual ~CHGroupFormModal( );

		CHGroupFormModal& operator =( const CHGroupFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHGroupFormModal

	class CHGroupFormModeless : public CHGroupForm {

	public:
		CHGroupFormModeless( );
		CHGroupFormModeless( const CHGroupFormModeless& objCopy );

		virtual ~CHGroupFormModeless( );

		CHGroupFormModeless& operator =( const CHGroupFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHGroupFormModal

} // namespace UdSDK

#endif
