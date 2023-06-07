/*
	Tourist application
	Клиентское приложение
	Валюта
*/
#ifndef TOURIST_CLIENT_CURRENCY_H
#define TOURIST_CLIENT_CURRENCY_H

namespace UdSDK {

	udPBase	GetProjectCurrencyList( );

	class CHCurrency : public udListHandlerBtnForm, public udListHandler {

	public:
		CHCurrency( );
		CHCurrency( const CHCurrency& objCopy );

		virtual ~CHCurrency( );

		CHCurrency& operator =( const CHCurrency& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelCurrency : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelCurrency( ) {
				add( m_col_id );
				add( m_col_name );
				add( m_col_code );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_code;
		};

	protected:
		ColModelCurrency *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName,
						m_lblFltCode;
		udWgtInput		m_inpFltName,
						m_inpFltCode;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHCurrency

	class CHCurrencyModeless : public udListHandlerModeless, public CHCurrency {

	public:
		CHCurrencyModeless( );
		CHCurrencyModeless( const CHCurrencyModeless& objCopy );

		virtual ~CHCurrencyModeless( );

		CHCurrencyModeless& operator =( const CHCurrencyModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHCurrencyModeless

	class CHCurrencyPick : public udListHandlerPick, public CHCurrency {

	public:
		CHCurrencyPick( );
		CHCurrencyPick( const CHCurrencyPick& objCopy );

		virtual ~CHCurrencyPick( );

		CHCurrencyPick& operator =( const CHCurrencyPick& objCopy );

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

	}; // class CHCurrencyPick

	class CHCurrencyForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName,
						m_lblCode;
		udWgtInput		m_inpName,		// наименование
						m_inpCode;		// код
		Gtk::Table		m_tblInput;


	public:
		CHCurrencyForm( );
		CHCurrencyForm( const CHCurrencyForm& objCopy );

		virtual ~CHCurrencyForm( );

		CHCurrencyForm& operator =( const CHCurrencyForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHCurrencyForm

	class CHCurrencyFormModal : public CHCurrencyForm {

	public:
		CHCurrencyFormModal( );
		CHCurrencyFormModal( const CHCurrencyFormModal& objCopy );

		virtual ~CHCurrencyFormModal( );

		CHCurrencyFormModal& operator =( const CHCurrencyFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCurrencyFormModal

	class CHCurrencyFormModeless : public CHCurrencyForm {

	public:
		CHCurrencyFormModeless( );
		CHCurrencyFormModeless( const CHCurrencyFormModeless& objCopy );

		virtual ~CHCurrencyFormModeless( );

		CHCurrencyFormModeless& operator =( const CHCurrencyFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHCurrencyFormModal

} // namespace UdSDK

#endif
