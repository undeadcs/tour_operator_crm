/*
	Tourist application
	Клиентское приложение
	Услуги
*/
#ifndef TOURIST_CLIENT_SERVICE_H
#define TOURIST_CLIENT_SERVICE_H

namespace UdSDK {

	udPBase	GetProjectServiceList( );

	class CWgtServicePick : public udWgtListPick {

	public:
		CWgtServicePick( );
		CWgtServicePick( const CWgtServicePick& objCopy );

		virtual ~CWgtServicePick( );

		CWgtServicePick& operator =( const CWgtServicePick& objCopy );

		virtual void	OnButtonClick( );

	}; // class CWgtServicePeriodPick

	class CHService : public udListHandlerBtnForm, public udListHandler {
	protected:
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		enum {
			opidGet2 = opidGet + 1,	// получение услуг по ID
		};

	public:
		CHService( );
		CHService( const CHService& objCopy );

		virtual ~CHService( );

		CHService& operator =( const CHService& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelService( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_type );
				add( m_col_period );
				add( m_col_partner );
				add( m_col_price );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_type;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_period;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_partner;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price;
		};

	protected:
		ColModelService *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltName1,
						m_lblFltType,
						m_lblFltPartner;
		udWgtInput		m_inpFltName;
		udWgtListPick	m_inpFltName1,
						m_inpFltType,
						m_inpFltPartner;

		enum {
			fltName,
			fltType,
			fltPartner
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		static void GetServicesById( const map< udUInt, udUInt >& ids, udArrBase& arrResult );

	}; // class CHService

	class CHServiceModeless : public udListHandlerModeless, public CHService {

	public:
		CHServiceModeless( );
		CHServiceModeless( const CHServiceModeless& objCopy );

		virtual ~CHServiceModeless( );

		CHServiceModeless& operator =( const CHServiceModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHServiceModeless

	class CHServicePick : public udListHandlerPick, public CHService {

	public:
		CHServicePick( );
		CHServicePick( const CHServicePick& objCopy );

		virtual ~CHServicePick( );

		CHServicePick& operator =( const CHServicePick& objCopy );

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

	}; // class CHServicePick

	class CHServiceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblType,
						m_lblCountry,
						m_lblCity,
						m_lblPartner,
						m_lblMunit,
						m_lblPeriod,
						m_lblPrice;
		udWgtListPick	m_inpName,
						m_inpType,
						m_inpCountry,
						m_inpCity,
						m_inpPartner,
						m_inpMunit;
		Gtk::Table		m_tblInput;

	public:
		enum {
			ctrlidName,
			ctrlidType,
			ctrlidCountry,
			ctrlidCity,
			ctrlidPartner,
			ctrlidMunit,
			ctrlidPeriod,
			ctrlidPrice
		};

	public:
		CHServiceForm( );
		CHServiceForm( const CHServiceForm& objCopy );

		virtual ~CHServiceForm( );

		CHServiceForm& operator =( const CHServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListPeriod : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListPeriod( ) {
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_object );
			} // ColModelFormListPeriod

			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListPeriod

	protected:
		ColModelFormListPeriod* m_pModelFormListPeriod;
		Glib::RefPtr< Gtk::ListStore > m_objPeriodModel;
		udWgtFormList m_inpPeriod;
		Gtk::Button	*m_btnPeriodAdd,
					*m_btnPeriodDel,
					*m_btnPeriodEdit;
		map< Glib::ustring, udPBase > m_mapObjectToRowPeriod;

		void	OnFormListPeriod_Select( );
		void	OnFormListPeriod_Add( );
		void	OnFormListPeriod_Edit( );
		void	OnFormListPeriod_Del( );

		udUInt	GetPeriodLastIndex( );

	public:
		void	SavePeriod( udPBase obj );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListPrice : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListPrice( ) {
				add( m_col_date1 );
				add( m_col_date2 );
				//add( m_col_period );
				add( m_col_num );
				add( m_col_amount );
				add( m_col_currency );
				add( m_col_amount_recommend );
				add( m_col_currency_recommend );
				add( m_col_object );
			} // ColModelFormListPrice

			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
			//Gtk::TreeModelColumn< Glib::ustring > m_col_period;
			Gtk::TreeModelColumn< Glib::ustring > m_col_num;
			Gtk::TreeModelColumn< Glib::ustring > m_col_amount;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_amount_recommend;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency_recommend;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListPrice

	protected:
		ColModelFormListPrice* m_pModelFormListPrice;
		Glib::RefPtr< Gtk::ListStore > m_objPriceModel;
		udWgtFormList m_inpPrice;
		Gtk::Button	*m_btnPriceAdd,
					*m_btnPriceDel,
					*m_btnPriceEdit;
		map< Glib::ustring, udPBase > m_mapObjectToRowPrice;

		void	OnFormListPrice_Select( );
		void	OnFormListPrice_Add( );
		void	OnFormListPrice_Edit( );
		void	OnFormListPrice_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHServiceForm

	class CHServiceFormModal : public CHServiceForm {

	public:
		CHServiceFormModal( );
		CHServiceFormModal( const CHServiceFormModal& objCopy );

		virtual ~CHServiceFormModal( );

		CHServiceFormModal& operator =( const CHServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHServiceFormModal

	class CHServiceFormModeless : public CHServiceForm {

	public:
		CHServiceFormModeless( );
		CHServiceFormModeless( const CHServiceFormModeless& objCopy );

		virtual ~CHServiceFormModeless( );

		CHServiceFormModeless& operator =( const CHServiceFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHServiceFormModal

} // namespace UdSDK

#endif
