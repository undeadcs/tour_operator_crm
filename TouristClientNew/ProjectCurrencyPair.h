/*
	Tourist application
	Клиентское приложение
	Валютная пара
*/
#ifndef TOURIST_CLIENT_CURRENCY_PAIR_H
#define TOURIST_CLIENT_CURRENCY_PAIR_H

namespace UdSDK {

	udPBase	GetProjectCurrencyPairList( );

	class CHCurrencyPair : public udListHandlerBtnForm, public udListHandler {

	public:
		CHCurrencyPair( );
		CHCurrencyPair( const CHCurrencyPair& objCopy );

		virtual ~CHCurrencyPair( );

		CHCurrencyPair& operator =( const CHCurrencyPair& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelCurrencyPair : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelCurrencyPair( ) {
				add( m_col_id );
				add( m_col_pair );
				add( m_col_course );
				add( m_col_set );
				add( m_col_internal );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_pair;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_course;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_set;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_internal;
		};

	protected:
		ColModelCurrencyPair *m_pColModel;
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltCourse1,
						m_lblFltCourse2,
						m_lblFltCb1,
						m_lblFltCb2;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtInput		m_inpFltCourse1,
						m_inpFltCourse2,
						m_inpFltCb1,
						m_inpFltCb2;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHCurrencyPair

	class CHCurrencyPairModeless : public udListHandlerModeless, public CHCurrencyPair {

	public:
		CHCurrencyPairModeless( );
		CHCurrencyPairModeless( const CHCurrencyPairModeless& objCopy );

		virtual ~CHCurrencyPairModeless( );

		CHCurrencyPairModeless& operator =( const CHCurrencyPairModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHCurrencyPairModeless

	class CHCurrencyPairPick : public udListHandlerPick, public CHCurrencyPair {

	public:
		CHCurrencyPairPick( );
		CHCurrencyPairPick( const CHCurrencyPairPick& objCopy );

		virtual ~CHCurrencyPairPick( );

		CHCurrencyPairPick& operator =( const CHCurrencyPairPick& objCopy );

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

	}; // class CHCurrencyPairPick

	class CHCurrencyPairForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblCurrency1,
						m_lblCurrency2,
						m_lblCourse,
						m_lblInternal,
						m_lblSet;
		udWgtInput		m_inpCourse,
						m_inpInternal;
		udWgtDatePick	m_inpSet;
		Gtk::Table		m_tblInput;

		udWgtListPick	m_inpCurrency1,
						m_inpCurrency2;

	public:
		enum {
			ctrlidCurrency1,
			ctrlidCurrency2
		};

	public:
		CHCurrencyPairForm( );
		CHCurrencyPairForm( const CHCurrencyPairForm& objCopy );

		virtual ~CHCurrencyPairForm( );

		CHCurrencyPairForm& operator =( const CHCurrencyPairForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHCurrencyPairForm

	class CHCurrencyPairFormModal : public CHCurrencyPairForm {

	public:
		CHCurrencyPairFormModal( );
		CHCurrencyPairFormModal( const CHCurrencyPairFormModal& objCopy );

		virtual ~CHCurrencyPairFormModal( );

		CHCurrencyPairFormModal& operator =( const CHCurrencyPairFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCurrencyPairFormModal

	class CHCurrencyPairFormModeless : public CHCurrencyPairForm {

	public:
		CHCurrencyPairFormModeless( );
		CHCurrencyPairFormModeless( const CHCurrencyPairFormModeless& objCopy );

		virtual ~CHCurrencyPairFormModeless( );

		CHCurrencyPairFormModeless& operator =( const CHCurrencyPairFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCurrencyPairFormModal

} // namespace UdSDK

#endif
