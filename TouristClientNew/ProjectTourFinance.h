/*
	Tourist application
	Клиентское приложение
	Финансы тура
*/
#ifndef TOURIST_CLIENT_TOUR_FINANCE_H
#define TOURIST_CLIENT_TOUR_FINANCE_H

namespace UdSDK {

	udPBase GetProjectTourFinanceSummCostList( );
	udPBase GetProjectTourFinanceSummServiceList( );

	class CFDETourFinance : public udFDEObject {
		udPHTourFinanceForm m_pForm;

	public:
		CFDETourFinance( );
		CFDETourFinance( const CFDETourFinance& objCopy );

		virtual ~CFDETourFinance( );

		CFDETourFinance& operator =( const CFDETourFinance& objCopy );

		virtual void	Load( udPSocket sock );
		virtual void	Clear( );
		virtual void	Send( udPSocket sock );
		virtual void	Make( );
		virtual void	Make( bool bDelete );

		void	SetTourFinanceForm( udPHTourFinanceForm pForm ) { m_pForm = pForm; }

	}; // class CFDETourApp

	class CHTourFinanceForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblFinal,
						m_lblTour,
						m_lblCost,
						m_lblService,
						m_lblTourOperation;
		Gtk::Table		m_tblInput;

		udPHTourForm	m_pTour;

	public:
		enum {
			ctrlidFinal,			// итоговая цена за тур
			ctrlidTour,				// оплата за тур
			ctrlidCost,				// себестоимость
			ctrlidService,			// оплата услуг
			ctrlidTourOperation		// операции по оплате за тур
		};

	public:
		CHTourFinanceForm( );
		CHTourFinanceForm( const CHTourFinanceForm& objCopy );

		virtual ~CHTourFinanceForm( );

		CHTourFinanceForm& operator =( const CHTourFinanceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		virtual void	OnButtonOk( ) { }
		virtual void	OnButtonCancel( ) { }

		void	SetTourForm( udPHTourForm pTour ) { m_pTour = pTour; }

		void	OnPayOrder( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListFinal : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListFinal( ) {
				add( m_col_summ );
				add( m_col_currency );
				//add( m_col_type );
				add( m_col_object );
			} // ColModelFormListFinal

			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			//Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListFinal

	protected:
		ColModelFormListFinal* m_pModelFormListFinal;
		Glib::RefPtr< Gtk::ListStore > m_objFinalModel;
		udWgtFormList m_inpFinal;
		Gtk::Button	*m_btnFinalAdd,
					*m_btnFinalBack,
					*m_btnFinalConvert,
					*m_btnFinalCalculate;

		void	OnFormListFinal_Add( );
		void	OnFormListFinal_Back( );
		void	OnFormListFinal_Convert( );
		void	OnFormListFinal_Calculate( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListTour( ) {
				add( m_col_summ );
				add( m_col_currency );
				add( m_col_type );
				add( m_col_object );
			} // ColModelFormListTour

			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListTour

	protected:
		ColModelFormListTour* m_pModelFormListTour;
		Glib::RefPtr< Gtk::ListStore > m_objTourModel;
		udWgtFormList m_inpTour;
		Gtk::Button	*m_btnTourPayOrder,
					*m_btnTourAdd,
					*m_btnTourBack,
					*m_btnTourConvert;

		void	OnFormListTour_Add( );
		void	OnFormListTour_Back( );
		void	OnFormListTour_Convert( );
		void	OnFormListTour_PayOrder( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListCost : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListCost( ) {
				add( m_col_summ );
				add( m_col_currency );
				add( m_col_type );
				add( m_col_partner );
				add( m_col_object );
			} // ColModelFormListCost

			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_partner;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListCost

	protected:
		ColModelFormListCost* m_pModelFormListCost;
		Glib::RefPtr< Gtk::ListStore > m_objCostModel;
		udWgtFormList m_inpCost;
		Gtk::Button	*m_btnCostAdd,
					*m_btnCostBack,
					*m_btnCostConvert;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListService( ) {
				add( m_col_summ );
				add( m_col_currency );
				add( m_col_type );
				add( m_col_partner );
				add( m_col_object );
			} // ColModelFormListService

			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_partner;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListService

	protected:
		ColModelFormListService* m_pModelFormListService;
		Glib::RefPtr< Gtk::ListStore > m_objServiceModel;
		udWgtFormList m_inpService;
		Gtk::Button	*m_btnServiceAdd,
					*m_btnServiceBack,
					*m_btnServiceConvert;

		void	OnFormListService_Add( );
		void	OnFormListService_Back( );
		void	OnFormListService_Convert( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListOperation : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListOperation( ) {
				add( m_col_created );
				add( m_col_type );
				add( m_col_summ );
				add( m_col_currency );
				add( m_col_summ_type );
				add( m_col_object );
			} // ColModelFormListOperation

			Gtk::TreeModelColumn< Glib::ustring > m_col_created;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_summ_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListOperation

	protected:
		ColModelFormListOperation* m_pModelFormListOperation;
		Glib::RefPtr< Gtk::ListStore > m_objOperationModel;
		udWgtFormList m_inpOperation;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
		void	ReloadServiceSumm( );

	}; // class CHTourFinanceForm

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// сумма, которая содержит связь с партнером

	/**
	 * Сумма на зачисление
	 */
	class CHFinanceSummAddTourServiceForm : public CHFinanceSummAddForm {
	protected:
		Gtk::Label		m_lblPartner;
		udWgtListPick	m_inpPartner;

	public:
		enum {
			ctrlidPartner = ctrlidCurrency + 1
		};

	public:
		CHFinanceSummAddTourServiceForm( );
		CHFinanceSummAddTourServiceForm( const CHFinanceSummAddTourServiceForm& objCopy );

		virtual ~CHFinanceSummAddTourServiceForm( );

		CHFinanceSummAddTourServiceForm& operator =( const CHFinanceSummAddTourServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHFinanceSummAddForm

	class CHFinanceSummAddTourServiceFormModal : public CHFinanceSummAddTourServiceForm {

	public:
		CHFinanceSummAddTourServiceFormModal( );
		CHFinanceSummAddTourServiceFormModal( const CHFinanceSummAddTourServiceFormModal& objCopy );

		virtual ~CHFinanceSummAddTourServiceFormModal( );

		CHFinanceSummAddTourServiceFormModal& operator =( const CHFinanceSummAddTourServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHFinanceSummAddFormModal

	/**
	 * Сумма на снятие
	 */
	class CHFinanceSummBackTourServiceForm : public CHFinanceSummBackForm {
	protected:
		Gtk::Label		m_lblPartner;
		udWgtListPick	m_inpPartner;

	public:
		enum {
			ctrlidPartner = ctrlidCurrency + 1
		};

	public:
		CHFinanceSummBackTourServiceForm( );
		CHFinanceSummBackTourServiceForm( const CHFinanceSummBackTourServiceForm& objCopy );

		virtual ~CHFinanceSummBackTourServiceForm( );

		CHFinanceSummBackTourServiceForm& operator =( const CHFinanceSummBackTourServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHFinanceSummBackForm

	class CHFinanceSummBackTourServiceFormModal : public CHFinanceSummBackTourServiceForm {

	public:
		CHFinanceSummBackTourServiceFormModal( );
		CHFinanceSummBackTourServiceFormModal( const CHFinanceSummBackTourServiceFormModal& objCopy );

		virtual ~CHFinanceSummBackTourServiceFormModal( );

		CHFinanceSummBackTourServiceFormModal& operator =( const CHFinanceSummBackTourServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHFinanceSummBackFormModal

	/**
	 * Сумма на конвертацию
	 */
	class CHFinanceSummConvertTourServiceForm : public CHFinanceSummConvertForm {
	protected:
		Gtk::Label		m_lblPartner;
		udWgtListPick	m_inpPartner;

	public:
		enum {
			ctrlidPartner = ctrlidCurrency2 + 1,
		};

	public:
		CHFinanceSummConvertTourServiceForm( );
		CHFinanceSummConvertTourServiceForm( const CHFinanceSummConvertTourServiceForm& objCopy );

		virtual ~CHFinanceSummConvertTourServiceForm( );

		CHFinanceSummConvertTourServiceForm& operator =( const CHFinanceSummConvertTourServiceForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHFinanceSummConvertForm

	class CHFinanceSummConvertTourServiceFormModal : public CHFinanceSummConvertTourServiceForm {

	public:
		CHFinanceSummConvertTourServiceFormModal( );
		CHFinanceSummConvertTourServiceFormModal( const CHFinanceSummConvertTourServiceFormModal& objCopy );

		virtual ~CHFinanceSummConvertTourServiceFormModal( );

		CHFinanceSummConvertTourServiceFormModal& operator =( const CHFinanceSummConvertTourServiceFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHFinanceSummConvertFormModal

} // namespace UdSDK

#endif
