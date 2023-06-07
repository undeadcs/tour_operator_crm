/*
	Tourist application
	Клиентское приложение
	Состояния тура
*/
#ifndef TOURIST_CLIENT_STOUR_H
#define TOURIST_CLIENT_STOUR_H

namespace UdSDK {

	udPBase	GetProjectSTourList( );

	class CHSTour : public udListHandlerBtnForm, public udListHandler {

	public:
		CHSTour( );
		CHSTour( const CHSTour& objCopy );

		virtual ~CHSTour( );

		CHSTour& operator =( const CHSTour& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelSTour : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelSTour( ) {
				add( m_col_id );
				add( m_col_name );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
		};

	protected:
		ColModelSTour *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltName;
		udWgtInput		m_inpFltName;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHSTour

	class CHSTourModeless : public udListHandlerModeless, public CHSTour {

	public:
		CHSTourModeless( );
		CHSTourModeless( const CHSTourModeless& objCopy );

		virtual ~CHSTourModeless( );

		CHSTourModeless& operator =( const CHSTourModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }
		virtual void	OnButtonForm( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHSTourModeless

	class CHSTourPick : public udListHandlerPick, public CHSTour {

	public:
		CHSTourPick( );
		CHSTourPick( const CHSTourPick& objCopy );

		virtual ~CHSTourPick( );

		CHSTourPick& operator =( const CHSTourPick& objCopy );

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

	}; // class CHSTourPick

	class CHSTourForm : public udFormHandler {
	protected:
		Gtk::HButtonBox	m_boxButton;
		Gtk::Label		m_lblHeader,
						m_lblName;
		udWgtInput		m_inpName;
		Gtk::Table		m_tblInput;


	public:
		CHSTourForm( );
		CHSTourForm( const CHSTourForm& objCopy );

		virtual ~CHSTourForm( );

		CHSTourForm& operator =( const CHSTourForm& objCopy );

		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual udInt	Validate( );
		virtual void	Send( );

	}; // class CHSTourForm

	class CHSTourFormModal : public CHSTourForm {

	public:
		CHSTourFormModal( );
		CHSTourFormModal( const CHSTourFormModal& objCopy );

		virtual ~CHSTourFormModal( );

		CHSTourFormModal& operator =( const CHSTourFormModal& objCopy );

		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHSTourFormModal

	class CHSTourFormModeless : public CHSTourForm {

	public:
		CHSTourFormModeless( );
		CHSTourFormModeless( const CHSTourFormModeless& objCopy );

		virtual ~CHSTourFormModeless( );

		CHSTourFormModeless& operator =( const CHSTourFormModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );

	}; // class CHSTourFormModal

} // namespace UdSDK

#endif
