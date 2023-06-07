/*
	Tourist application
	Клиентское приложение
	Туры
*/
#ifndef TOURIST_CLIENT_TOUR_H
#define TOURIST_CLIENT_TOUR_H

namespace UdSDK {

	udPBase	GetProjectTourList( );
	udPBase	GetProjectServiceForPrint( );

	udPArrBase LoadClients( const map< udUInt, udUInt >& ids );

	class CHTour : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		CHTour( );
		CHTour( const CHTour& objCopy );

		virtual ~CHTour( );

		CHTour& operator =( const CHTour& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTour( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_name );
				add( m_col_direct );
				add( m_col_date1 );
				add( m_col_status );
				add( m_col_employer );
				add( m_col_manager );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring > m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_direct;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_status;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_employer;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_manager;
		};

	protected:
		ColModelTour *m_pColModel;

		void	OnColClickId( );
		void	OnColClickCreated( );
		void	OnColClickName( );
		void	OnColClickDirect( );
		void	OnColClickDate( );
		void	OnColClickStatus( );
		void	OnColClickEmployer( );
		void	OnColClickManager( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltDirect,
						m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltStatus,
						m_lblFltEmployer,
						m_lblFltOrder,
						m_lblFltTplTour,
						m_lblFltUser,
						m_lblFltClient;
		udWgtInput		m_inpFltName;
		udWgtListPick	m_inpFltDirect,
						m_inpFltUser,
						m_inpFltOrder,
						m_inpFltTplTour,
						m_inpFltClient;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udPWgtEmployerPick
						m_inpFltEmployer;
		udWgtSelect		m_inpFltStatus;

		enum {
			fltDirect,
			fltUser,
			fltOrder,
			fltTplTour,
			fltEmployer,
			fltClient
		};

	}; // class CHTour

	class CHTourModeless : public udListHandlerModeless, public CHTour {
		Gtk::Button	m_btnPayOrder;

	public:
		CHTourModeless( );
		CHTourModeless( const CHTourModeless& objCopy );

		virtual ~CHTourModeless( );

		CHTourModeless& operator =( const CHTourModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTourModeless

	class CHTourPick : public udListHandlerPick, public CHTour {

	public:
		CHTourPick( );
		CHTourPick( const CHTourPick& objCopy );

		virtual ~CHTourPick( );

		CHTourPick& operator =( const CHTourPick& objCopy );

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

	}; // class CHTourPick

	class CHTourForm : public udFormHandler, public udWgtEmployerDocPick::Handler, public udWgtListPick::Handler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblDate1,
						m_lblDate2,
						m_lblCompany,
						m_lblStatus,
						m_lblEmployer,
						m_lblUser,
						m_lblDirect,
						m_lblComment,
						m_lblState,
						m_lblClient,
						m_lblApp,
						m_lblRoute,
						m_lblTour,
						m_lblService,
						m_lblDocument;
		udWgtInput		m_inpName,
						m_inpComment;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		udPWgtEmployerPick
						m_inpEmployer;
		udWgtListPick	m_inpUser,
						m_inpCompany,
						m_inpDirect,
						m_inpState;
		udWgtEmployerDocPick
						m_inpDocument;
		udWgtSelect		m_inpStatus;
		Gtk::Table		m_tblInput;

		Gtk::Notebook	m_wndTab;

		udPHTourAppForm		m_wndApp;
		udPHTourFinanceForm	m_wndFinance;
		udInt				m_iFinancePage;

		Gtk::Button		m_btnDocPrint,
						m_btnAddToGroup,
						m_btnMakeCopy;

	public:
		enum {
			ctrlidUser,
			ctrlidCompany,
			ctrlidEmployer,
			ctrlidDocument,
			ctrlidDirect,
			ctrlidState,
			ctrlidClient,
			ctrlidApp,
			ctrlidRoute,
			ctrlidService,
			ctrlidTour,
			ctrlidApp1,
			ctrlidFinance
		};

		enum {
			opidSummAdd = opidGet + 1,	// операция добавления суммы
			opidSummBack,				// снятие
			opidSummConvert,			// конвертация
			opidSummLoad,				// загрузка сумм
			opidCalculateFinal,			// вычисление итоговой цены
			opidAddToGroup,				// добавление в группу
			opidGetPersAcc,				// получение лицевого счета заказчик-компания для тура
			opidGetServicePrice			// получение услуг с их ценами
		};

	public:
		CHTourForm( );
		CHTourForm( const CHTourForm& objCopy );

		virtual ~CHTourForm( );

		CHTourForm& operator =( const CHTourForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		virtual bool	BeforeCreatePickHandler( udPWgtEmployerDocPick wgt, udPHEmployerDocPick handler );
		virtual udPHClDocFormModal GetEditHandler( udPWgtEmployerDocPick wgt );

		virtual void	AfterPick( udPWgtListPick wgt );

		void	CalculateFinal( udArrBase& arrSumm );
		void	LoadFromTplTour( udPHTplTourForm pTplTour );
		void	LoadFromTour( udPHTourForm pTour );

		void	GetServicesPrices( udArrBase& arrService );

		udPHTourFinanceForm	GetFinanceForm( )	const { return m_wndFinance;	}

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
		void	OnFormListClient_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );
		void	OnFormListClient_Add( );
		void	OnFormListClient_Del( );
		void	OnFormListClient_Replace( );
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
	// определение списка сущностей формы, с использованием списка выбора
	public:
		class ColModelFormListTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListTour( ) {
				add( m_col_name );
				add( m_col_object );
			} // ColModelFormListTour

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListTour

	protected:
		ColModelFormListTour* m_pModelFormListTour;
		Glib::RefPtr< Gtk::ListStore > m_objTourModel;
		udWgtFormList m_inpTour;
		Gtk::Button	*m_btnTourAdd,
					*m_btnTourDel,
					*m_btnTourReplace,
					*m_btnTourFromCopy;

		void	OnFormListTour_Select( );
		void	OnFormListTour_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );
		void	OnFormListTour_Add( );
		void	OnFormListTour_Del( );
		void	OnFormListTour_Replace( );
		void	OnFormListTour_FromCopy( );
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
				add( m_col_ready );
				add( m_col_object );
			} // ColModelFormListService

			Gtk::TreeModelColumn< Glib::ustring > m_col_service;
			Gtk::TreeModelColumn< Glib::ustring > m_col_num;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring > m_col_ready;
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
		void	OnFormListService_Activate( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );
		void	OnFormListService_Add( );
		void	OnFormListService_Edit( );
		void	OnFormListService_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	protected:
		void	OnButtonDocPrint( );
		void	OnButtonAddToGroup( );
		void	OnButtonMakeCopy( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTourForm

	class CHTourFormModal : public CHTourForm {

	public:
		CHTourFormModal( );
		CHTourFormModal( const CHTourFormModal& objCopy );

		virtual ~CHTourFormModal( );

		CHTourFormModal& operator =( const CHTourFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTourFormModal

	class CHTourFormModeless : public CHTourForm {

	public:
		CHTourFormModeless( );
		CHTourFormModeless( const CHTourFormModeless& objCopy );

		virtual ~CHTourFormModeless( );

		CHTourFormModeless& operator =( const CHTourFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTourFormModal

} // namespace UdSDK

#endif
