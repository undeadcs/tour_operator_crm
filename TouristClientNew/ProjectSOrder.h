/*
	Tourist application
	Клиентское приложение
	Состояние заявки
*/
#ifndef TOURIST_CLIENT_SORDER_H
#define TOURIST_CLIENT_SORDER_H

namespace UdSDK {

	udPBase	GetProjectSOrderList( );

	class CHSOrder : public udListHandlerBtnForm, public udListHandler {

	public:
		CHSOrder( );
		CHSOrder( const CHSOrder& objCopy );

		virtual ~CHSOrder( );

		CHSOrder& operator =( const CHSOrder& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelSOrder : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelSOrder( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelSOrder *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHSOrder

	class CHSOrderModeless : public udListHandlerModeless, public CHSOrder {

	public:
		CHSOrderModeless( );
		CHSOrderModeless( const CHSOrderModeless& objCopy );

		virtual ~CHSOrderModeless( );

		CHSOrderModeless& operator =( const CHSOrderModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHSOrderModeless

	class CHSOrderPick : public udListHandlerPick, public CHSOrder {

	public:
		CHSOrderPick( );
		CHSOrderPick( const CHSOrderPick& objCopy );

		virtual ~CHSOrderPick( );

		CHSOrderPick& operator =( const CHSOrderPick& objCopy );

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

	}; // class CHSOrderPick

	class CHSOrderForm : public udFormHandler {
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;

	public:
		CHSOrderForm( );
		CHSOrderForm( const CHSOrderForm& objCopy );

		virtual ~CHSOrderForm( );

		CHSOrderForm& operator =( const CHSOrderForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHSOrderForm

	class CHSOrderFormModal : public CHSOrderForm {

	public:
		CHSOrderFormModal( );
		CHSOrderFormModal( const CHSOrderFormModal& objCopy );

		virtual ~CHSOrderFormModal( );

		CHSOrderFormModal& operator =( const CHSOrderFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHSOrderFormModal

	class CHSOrderFormModeless : public CHSOrderForm {

	public:
		CHSOrderFormModeless( );
		CHSOrderFormModeless( const CHSOrderFormModeless& objCopy );

		virtual ~CHSOrderFormModeless( );

		CHSOrderFormModeless& operator =( const CHSOrderFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHSOrderFormModal

} // namespace UdSDK

#endif
