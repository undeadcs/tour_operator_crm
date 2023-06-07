/*
	Tourist application
	Клиентское приложение
	Указание об оплате за тур
*/
#ifndef TOURIST_CLIENT_TOUR_PAY_ORDER_H
#define TOURIST_CLIENT_TOUR_PAY_ORDER_H

namespace UdSDK {

	udPBase	GetProjectTourPayOrderList( );

	class CHTourPayOrder : public udListHandlerBtnForm, public udListHandler {

	public:
		CHTourPayOrder( );
		CHTourPayOrder( const CHTourPayOrder& objCopy );

		virtual ~CHTourPayOrder( );

		CHTourPayOrder& operator =( const CHTourPayOrder& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelTourPayOrder : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTourPayOrder( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_tour );
				add( m_col_state );
				add( m_col_employer );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_tour;
			Gtk::TreeModelColumn< Glib::ustring > m_col_state;
			Gtk::TreeModelColumn< Glib::ustring > m_col_employer;
		};

	protected:
		ColModelTourPayOrder *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltTour,
						m_lblFltState;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtListPick	m_inpFltTour;
		udWgtSelect		m_inpFltState;

		enum {
			fltTour
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTourPayOrder

	class CHTourPayOrderModeless : public udListHandlerModeless, public CHTourPayOrder {

	public:
		CHTourPayOrderModeless( );
		CHTourPayOrderModeless( const CHTourPayOrderModeless& objCopy );

		virtual ~CHTourPayOrderModeless( );

		CHTourPayOrderModeless& operator =( const CHTourPayOrderModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHTourPayOrderModeless

	class CHTourPayOrderPick : public udListHandlerPick, public CHTourPayOrder {

	public:
		CHTourPayOrderPick( );
		CHTourPayOrderPick( const CHTourPayOrderPick& objCopy );

		virtual ~CHTourPayOrderPick( );

		CHTourPayOrderPick& operator =( const CHTourPayOrderPick& objCopy );

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

	}; // class CHTourPayOrderPick

	class CHTourPayOrderForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblTour,
						m_lblSumm,
						m_lblState;
		Gtk::Table		m_tblInput;
		udWgtListPick	m_inpTour;
		udWgtSelect		m_inpState;

		// если атрибут задан, то контрол скрывается, а поле выставляется этим значением
		udInt	m_iState;	// >=0
		udUInt	m_iTour;	// > 0

		Gtk::Button	m_btnOpenPersAcc;

	public:
		enum {
			ctrlidTour,
			ctrlidSumm
		};

	public:
		CHTourPayOrderForm( );
		CHTourPayOrderForm( const CHTourPayOrderForm& objCopy );

		virtual ~CHTourPayOrderForm( );

		CHTourPayOrderForm& operator =( const CHTourPayOrderForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

		void	SetStateValue( udInt iState ) { m_iState = iState; }
		void	SetTourValue( udUInt iTour ) { m_iTour = iTour; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// определение списка сущностей формы, с использованием формы при редактировании
	public:
		class ColModelFormListSumm : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelFormListSumm( ) {
				add( m_col_summ );
				add( m_col_currency );
				add( m_col_type );
				add( m_col_object );
			} // ColModelFormListSumm

			Gtk::TreeModelColumn< Glib::ustring > m_col_summ;
			Gtk::TreeModelColumn< Glib::ustring > m_col_currency;
			Gtk::TreeModelColumn< Glib::ustring > m_col_type;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		}; // class ColModelFormListSumm

	protected:
		ColModelFormListSumm* m_pModelFormListSumm;
		Glib::RefPtr< Gtk::ListStore > m_objSummModel;
		udWgtFormList m_inpSumm;
		Gtk::Button	*m_btnSummAdd,
					*m_btnSummBack;

		void	OnFormListSumm_Add( );
		void	OnFormListSumm_Back( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		void	OnButtonOpenPersAcc( );

	}; // class CHTourPayOrderForm

	class CHTourPayOrderFormModal : public CHTourPayOrderForm {

	public:
		CHTourPayOrderFormModal( );
		CHTourPayOrderFormModal( const CHTourPayOrderFormModal& objCopy );

		virtual ~CHTourPayOrderFormModal( );

		CHTourPayOrderFormModal& operator =( const CHTourPayOrderFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTourPayOrderFormModal

	class CHTourPayOrderFormModeless : public CHTourPayOrderForm {

	public:
		CHTourPayOrderFormModeless( );
		CHTourPayOrderFormModeless( const CHTourPayOrderFormModeless& objCopy );

		virtual ~CHTourPayOrderFormModeless( );

		CHTourPayOrderFormModeless& operator =( const CHTourPayOrderFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHTourPayOrderFormModal

} // namespace UdSDK

#endif
