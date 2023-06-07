/*
	Tourist application
	Клиентское приложение
	Приложение к туру
*/
#ifndef TOURIST_CLIENT_TOUR_APP_H
#define TOURIST_CLIENT_TOUR_APP_H

namespace UdSDK {

	class CFDETourApp : public udFDEObject {

	public:
		CFDETourApp( );
		CFDETourApp( const CFDETourApp& objCopy );

		virtual ~CFDETourApp( );

		CFDETourApp& operator =( const CFDETourApp& objCopy );

		virtual void	Load( udPSocket sock );
		virtual void	Clear( );
		virtual void	Send( udPSocket sock );
		virtual void	Make( );
		virtual void	Make( bool bDelete );

		void	AddToArray( const string& szName, udPBase obj );
		void	LoadArrayFrom( const string& szName, udPArrBase arrObject );

	private:
		static pfnGetObject	GetHandlerByName( const string& szName );

	}; // class CFDETourApp

	class CHTourAppForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblPrice,
						m_lblPay1,
						m_lblPay2,
						m_lblDate1,
						m_lblDate2,
						m_lblDays1,
						m_lblDays2,
						m_lblDays3,
						m_lblProduct,
						m_lblOccupancy,
						m_lblProgram,
						m_lblTransferService,
						m_lblCover,
						m_lblVisa,
						m_lblService,
						m_lblTransfer;
		udWgtInput		m_inpPrice,
						m_inpPay1,
						m_inpPay2,
						m_inpDays1,
						m_inpDays2,
						m_inpDays3,
						m_inpProduct;
		udWgtDatePick	m_inpDate1,
						m_inpDate2;
		Gtk::Table		m_tblInput;

		CFDETourApp* m_pFde;

	public:
		enum {
			ctrlidOccupancy,	// средства размещения
			ctrlidProgram,		// экскурсионная программа
			ctrlidTransferService,		// услуги по перевозке
			ctrlidCover,		// страхование
			ctrlidVisa,			// визовое оформление
			ctrlidService,		// другие услуги
			ctrlidTransfer		// трансфер
		};

	public:
		CHTourAppForm( CFDETourApp* pFde = NULL );
		CHTourAppForm( const CHTourAppForm& objCopy );

		virtual ~CHTourAppForm( );

		CHTourAppForm& operator =( const CHTourAppForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		virtual void	OnButtonOk( ) { }
		virtual void	OnButtonCancel( ) { }

		friend class CHTourForm;
		friend class CHTourFormModal;
		friend class CHTourFormModeless;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListOccupancy : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListOccupancy( ) {
				add( m_col_name );
				add( m_col_category );
				add( m_col_amount );
				add( m_col_feed );
				add( m_col_object );
			} // ColModelFormListOccupancy

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< Glib::ustring > m_col_category;
			Gtk::TreeModelColumn< Glib::ustring > m_col_amount;
			Gtk::TreeModelColumn< Glib::ustring > m_col_feed;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListOccupancy

	protected:
		ColModelFormListOccupancy* m_pModelFormListOccupancy;
		Glib::RefPtr< Gtk::ListStore > m_objOccupancyModel;
		udWgtFormList m_inpOccupancy;
		Gtk::Button	*m_btnOccupancyAdd,
					*m_btnOccupancyDel,
					*m_btnOccupancyEdit;

		void	OnFormListOccupancy_Select( );
		void	OnFormListOccupancy_Add( );
		void	OnFormListOccupancy_Edit( );
		void	OnFormListOccupancy_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListProgram : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListProgram( ) {
				add( m_col_text );
				add( m_col_object );
			} // ColModelFormListProgram

			Gtk::TreeModelColumn< Glib::ustring > m_col_text;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListProgram

	protected:
		ColModelFormListProgram* m_pModelFormListProgram;
		Glib::RefPtr< Gtk::ListStore > m_objProgramModel;
		udWgtFormList m_inpProgram;
		Gtk::Button	*m_btnProgramAdd,
					*m_btnProgramDel,
					*m_btnProgramEdit;

		void	OnFormListProgram_Select( );
		void	OnFormListProgram_Add( );
		void	OnFormListProgram_Edit( );
		void	OnFormListProgram_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListTransferService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListTransferService( ) {
				add( m_col_fio );
				add( m_col_trans );
				add( m_col_level );
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_object );
			} // ColModelFormListTransferService

			Gtk::TreeModelColumn< Glib::ustring > m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring > m_col_trans;
			Gtk::TreeModelColumn< Glib::ustring > m_col_level;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring > m_col_date2;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListTransferService

	protected:
		ColModelFormListTransferService* m_pModelFormListTransferService;
		Glib::RefPtr< Gtk::ListStore > m_objTransferServiceModel;
		udWgtFormList m_inpTransferService;
		Gtk::Button	*m_btnTransferServiceAdd,
					*m_btnTransferServiceDel,
					*m_btnTransferServiceEdit;

		void	OnFormListTransferService_Select( );
		void	OnFormListTransferService_Add( );
		void	OnFormListTransferService_Edit( );
		void	OnFormListTransferService_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListCover : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListCover( ) {
				add( m_col_type );
				add( m_col_name );
				add( m_col_object );
			} // ColModelFormListCover

			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListCover

	protected:
		ColModelFormListCover* m_pModelFormListCover;
		Glib::RefPtr< Gtk::ListStore > m_objCoverModel;
		udWgtFormList m_inpCover;
		Gtk::Button	*m_btnCoverAdd,
					*m_btnCoverDel,
					*m_btnCoverEdit;

		void	OnFormListCover_Select( );
		void	OnFormListCover_Add( );
		void	OnFormListCover_Edit( );
		void	OnFormListCover_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListVisa : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListVisa( ) {
				add( m_col_type );
				add( m_col_object );
			} // ColModelFormListVisa

			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListVisa

	protected:
		ColModelFormListVisa* m_pModelFormListVisa;
		Glib::RefPtr< Gtk::ListStore > m_objVisaModel;
		udWgtFormList m_inpVisa;
		Gtk::Button	*m_btnVisaAdd,
					*m_btnVisaDel,
					*m_btnVisaEdit;

		void	OnFormListVisa_Select( );
		void	OnFormListVisa_Add( );
		void	OnFormListVisa_Edit( );
		void	OnFormListVisa_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListService : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListService( ) {
				add( m_col_name );
				add( m_col_char );
				add( m_col_object );
			} // ColModelFormListService

			Gtk::TreeModelColumn< Glib::ustring > m_col_name;
			Gtk::TreeModelColumn< Glib::ustring > m_col_char;
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
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListTransfer : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListTransfer( ) {
				add( m_col_route );
				add( m_col_type );
				add( m_col_object );
			} // ColModelFormListTransfer

			Gtk::TreeModelColumn< Glib::ustring > m_col_route;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListTransfer

	protected:
		ColModelFormListTransfer* m_pModelFormListTransfer;
		Glib::RefPtr< Gtk::ListStore > m_objTransferModel;
		udWgtFormList m_inpTransfer;
		Gtk::Button	*m_btnTransferAdd,
					*m_btnTransferDel,
					*m_btnTransferEdit;

		void	OnFormListTransfer_Select( );
		void	OnFormListTransfer_Add( );
		void	OnFormListTransfer_Edit( );
		void	OnFormListTransfer_Del( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTourAppForm

} // namespace UdSDK

#endif
